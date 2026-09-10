//------------------------------------------------------------------------------
//
// File Name:	GraphicsSystem.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		System for handling graphics assets and interfaces.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "GraphicsSystem.h"

#include GRAPHICS_INCLUDE

#if defined(GraphicsDebugCallback) && !defined(glDebugMessageCallback)
#error Unable to use debug callback, glDebugMessageCallback is not defined!
#endif

#include "BaseSystem.h"
#include "PlatformSystem.h"
#include "PerformanceSystem.h"

#include "Material.h"
#include "Shader.h"
#include "Texture.h"
#include "Sampler.h"
#include "SpriteSource.h"
#include "Mesh.h"
#include "Armature.h"
#include "CameraSource.h"
#include "LightSource.h"
#include "ModelData.h"
#include "AnimationSequence.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

#define PERFORM_ALL(action)\
action(Armature)\
action(Material)\
action(Mesh)\
action(Shader)\
action(Sampler)\
action(Texture)\

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    //--------------------------------------------------------------------------
    // Class Implementation:
    //--------------------------------------------------------------------------

#pragma region Class Implementation

    class GraphicsSystem_ SYSTEM_FINAL : public BaseSystem
    {
        friend GraphicsSystem;

        SYSTEM_COMMON(GraphicsSystem_);

        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    private:
        GraphicsSystem_(void);

        // All systems need a virtual destructor to have their destructor called 
        ~GraphicsSystem_(void) override;

        // Public Static Functions:
    public:

    public:

        // Public Event Handlers
    public:

        // Private Constants, Enums, and Structs:
    private:

        // Private Static Functions:
    private:
        //
        static void ResizeViewport(const IVec2& windowSize);

        //
        static void DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

        // Private Functions:
    private:
        /// <summary>
        /// Initializes the system.
        /// </summary>
        /// <returns>True if initialization succeeded, false otherwise.</returns>
        bool Init(void) override;

        /// <summary>
        /// Updates the system.
        /// </summary>
        void Update(void) override;

        /// <summary>
        /// Renders the system.
        /// </summary>
        void Render(void) override;

        /// <summary>
        /// Exits and shuts down the system.
        /// </summary>
        void Exit(void) override;

        /// <summary>
        /// Gets the priority of the system for a given engine loop.
        /// </summary>
        /// <param name="loop">The engine loop to check.</param>
        /// <returns>An int indicating how early (relative to other systems) to run this system in the given loop.</returns>
        int LoopPriority(EngineLoop loop) const override;

        void GenerateDebugTexture();

        void GenerateBlankTexture();

        void UpdateCamera();

        // Private Static Variables:
    private:
        static GraphicsSystem_* instance;

        // Private Variables:
    private:
        Container<Armature>* containerArmature_;
        Container<Material>* containerMaterial_;
        Container<Mesh>* containerMesh_;
        Container<Shader>* containerShader_;
        Container<Sampler>* containerSampler_;
        Container<Texture>* containerTexture_;
        unordered_set_t<string_t, StringHash, std::equal_to<>> extensions_;
        Vec4 clearColor_;
        CallbackID resizeCallback_;
        const Mesh* simpleMesh_;
        Material* simpleMatRect_, * simpleMatCircle_;
        CameraSource* camera_;
        ResourceID viewportMatrices_;
        ResourceID bufferDt_;
        bool dirtyView_;
    };

#pragma endregion Class Implementation

    //--------------------------------------------------------------------------
    // Public Constants:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Public Static Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Public Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Private Constants:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Private Static Variables:
    //--------------------------------------------------------------------------

    GraphicsSystem_* GraphicsSystem_::instance = nullptr;

#define inst GraphicsSystem_::instance

    //--------------------------------------------------------------------------
    // Private Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Constructors/Destructors:
    //--------------------------------------------------------------------------

#pragma region Constructors

#define CTOR_INITIALIZE(obj)\
    , container##obj##_(new Container<obj>())

    GraphicsSystem_::GraphicsSystem_(void)
        : BaseSystem("GraphicsSystem", LoopFlags::HasUpdate | LoopFlags::HasRender)
        PERFORM_ALL(CTOR_INITIALIZE)
        , extensions_()
        , clearColor_(0.1f, 0.1f, 0.1f, 1.0f)
        , resizeCallback_()
        , simpleMesh_(nullptr)
        , simpleMatRect_(nullptr)
        , simpleMatCircle_(nullptr)
        , camera_(nullptr)
        , viewportMatrices_(ResourceUnbind)
        , bufferDt_(ResourceUnbind)
        , dirtyView_()
    {
        // Raise an assert if this system has already been created.
        assert(instance == nullptr);

        // Store this system's instance for use by static functions.
        instance = this;
    }

    //--------------------------------------------------------------------------

#define DTOR_DELETE(obj)\
    delete container##obj##_;\

    GraphicsSystem_::~GraphicsSystem_(void)
    {
        PERFORM_ALL(DTOR_DELETE);
    }

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

    BaseSystem* GraphicsSystem::GetInstance()
    {
        return inst != nullptr ? inst : new GraphicsSystem_();
    }

#define IMPLEMENT_ACCESSORS(obj)\
    const obj* GraphicsSystem::Get##obj(string_view_arg name) { return (##obj##Library::Get_S(name)); }\
    obj* GraphicsSystem::Build##obj(string_view_arg name) { return Factory<obj>::Build(*##obj##Library::GetInstance(), name, inst->container##obj##_); }\
    obj* GraphicsSystem::Build##obj(const obj* other) { return other == nullptr ? nullptr : Factory<obj>::Build(other, inst->container##obj##_); }\
    bool GraphicsSystem::Destroy##obj(obj*& item) { return inst->container##obj##_->Destroy(item); }

    PERFORM_ALL(IMPLEMENT_ACCESSORS);

    void GraphicsSystem::ClearBackground()
    {
        const Vec4& color = inst->clearColor_;
        glClearColor(color.r, color.g, color.b, color.a);
        OpenGLCheckError();
        //glClearDepth(value);
        //OpenGLCheckError();
        //glClearStencil(value);
        //OpenGLCheckError();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        OpenGLCheckError();
    }

    const Vec4& GraphicsSystem::GetClearColor(void)
    {
        return inst->clearColor_;
    }

    void GraphicsSystem::SetClearColor(const Vec4& color)
    {
        inst->clearColor_ = color;
    }

    void GraphicsSystem::SetActiveCamera(CameraSource* camera)
    {
        inst->camera_ = camera;
        inst->UpdateCamera();
    }

    void GraphicsSystem::DrawCall(const Mat4& transformMatrix, const Mesh& mesh, const Material& material)
    {
        material.Activate(transformMatrix);
        mesh.Draw();
        material.Deactivate();
    }

    void GraphicsSystem::DrawRect(const Mat4& transformMatrix, const Vec2& size, const Vec4& colorFill, const Vec4& colorBorder, float borderSize)
    {
        Material* mat = inst->simpleMatRect_;
        mat->SetUniform("scale", size);
        mat->SetUniform("fillColor", colorFill);
        mat->SetUniform("borderColor", colorBorder);
        mat->SetUniform("borderSize", 1.0f - borderSize);
        DrawCall(transformMatrix, *inst->simpleMesh_, *mat);
    }

    void GraphicsSystem::DrawOval(const Mat4& transformMatrix, const Vec2& radii, const Vec4& colorFill, const Vec4& colorBorder, float borderSize)
    {
        Material* mat = inst->simpleMatCircle_;
        mat->SetUniform("scale", radii * 2.0f);
        mat->SetUniform("fillColor", colorFill);
        mat->SetUniform("borderColor", colorBorder);
        mat->SetUniform("borderSize", 1.0f - borderSize);
        DrawCall(transformMatrix, *inst->simpleMesh_, *mat);
    }

    void GraphicsSystem::DrawCircle(const Mat4& transformMatrix, float radius, const Vec4& colorFill, const Vec4& colorBorder, float borderSize)
    {
        DrawOval(transformMatrix, Vec2(radius), colorFill, colorBorder, borderSize);
    }

    void GraphicsSystem::CheckError(string_view_arg file)
    {
        GLenum error = glGetError();
        if (error == GL_NO_ERROR)
        {
            return;
        }
        const char* errorName;
        switch (error)
        {
#define _GL_ERRORNAME(name) case name: errorName = #name; break;
        _GL_ERRORNAME(GL_INVALID_ENUM)
        _GL_ERRORNAME(GL_INVALID_VALUE)
        _GL_ERRORNAME(GL_INVALID_OPERATION)
        _GL_ERRORNAME(GL_INVALID_FRAMEBUFFER_OPERATION)
        _GL_ERRORNAME(GL_OUT_OF_MEMORY)
        _GL_ERRORNAME(GL_STACK_UNDERFLOW)
        _GL_ERRORNAME(GL_STACK_OVERFLOW)
#undef _GL_ERRORNAME
        default:
            errorName = "UNEXPECTED GL ERROR";
            break;
        }
        TraceError("{:#X}|{} @ {}", error, errorName, file);
        //assert(false);
    }

#pragma endregion Public Static Functions

    //--------------------------------------------------------------------------
    // Public Functions:
    //--------------------------------------------------------------------------

#pragma region Public Functions

#pragma endregion Public Functions

    //--------------------------------------------------------------------------
    // Private Static Functions:
    //--------------------------------------------------------------------------

#pragma region Private Static Functions

    void GraphicsSystem_::ResizeViewport(const IVec2& windowSize)
    {
        // Create the viewport at the specified size
        glViewport(0, 0, windowSize.x, windowSize.y);
        OpenGLCheckError();
    }

    void GraphicsSystem_::DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, [[maybe_unused]] const void* userParam)
    {
        const basic_string_view_t<GLchar> msg(message, (unsigned)length);
        string_view_t sourceStr, typeStr;
        switch (source)
        {
#define DEBUG_SOURCE_CASE(value) case value: { static constexpr string_view_t glSourceName = #value + string_view_t("GL_DEBUG_SOURCE_").size(); sourceStr = glSourceName; } break
        DEBUG_SOURCE_CASE(GL_DEBUG_SOURCE_API);
        DEBUG_SOURCE_CASE(GL_DEBUG_SOURCE_WINDOW_SYSTEM);
        DEBUG_SOURCE_CASE(GL_DEBUG_SOURCE_SHADER_COMPILER);
        DEBUG_SOURCE_CASE(GL_DEBUG_SOURCE_THIRD_PARTY);
        DEBUG_SOURCE_CASE(GL_DEBUG_SOURCE_APPLICATION);
        DEBUG_SOURCE_CASE(GL_DEBUG_SOURCE_OTHER);
        default:
            sourceStr = "UNKNOWN";
            assert(false);
            break;
        }
        switch (type)
        {
#define DEBUG_TYPE_CASE(value) case value: { static constexpr string_view_t glTypeName = #value + string_view_t("GL_DEBUG_TYPE_").size(); sourceStr = glTypeName; } break
        DEBUG_TYPE_CASE(GL_DEBUG_TYPE_ERROR);
        DEBUG_TYPE_CASE(GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR);
        DEBUG_TYPE_CASE(GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR);
        DEBUG_TYPE_CASE(GL_DEBUG_TYPE_PORTABILITY);
        DEBUG_TYPE_CASE(GL_DEBUG_TYPE_PERFORMANCE);
        DEBUG_TYPE_CASE(GL_DEBUG_TYPE_OTHER);
        default:
            typeStr = "UNKNOWN";
            assert(false);
            break;
        }
#define TRACE_FORMAT "GL {} {} (0x{:08X}): {}", sourceStr, typeStr, id, msg
        switch (severity)
        {
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            TraceVerbose(TRACE_FORMAT);
            break;
        case GL_DEBUG_SEVERITY_LOW:
            TraceInfo(TRACE_FORMAT);
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            TraceWarning(TRACE_FORMAT);
            break;
        case GL_DEBUG_SEVERITY_HIGH:
            TraceError(TRACE_FORMAT);
            assert(false);
            break;
        default:
            TraceError(TRACE_FORMAT);
            assert(false);
            break;
        }
#undef TRACE_FORMAT
    }

#pragma endregion Private Static Functions

    //--------------------------------------------------------------------------
    // Private Functions:
    //--------------------------------------------------------------------------

#pragma region Private Functions

    bool GraphicsSystem_::Init(void)
    {
        {
            GLuint extNum = 0;
            glGetIntegerv(GL_NUM_EXTENSIONS, reinterpret_cast<GLint*>(&extNum));

            string_t extTrace = "";
            extTrace.reserve(32ull * extNum);

            for (GLuint i = 0; i < extNum; ++i)
            {
                const char* extName = reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i));
                if (extName != nullptr)
                {
                    extensions_.insert(extName);
                    if (!extTrace.empty())
                    {
                        extTrace += ',';
                    }
                    extTrace += extName;
                }
            }
            TraceInfo("Graphics API Extensions supported: {}", extTrace);
        }

        resizeCallback_ = PlatformSystem::ViewportResizeCallbackAdd(ResizeViewport);
        ResizeViewport(PlatformSystem::GetWindowSize());

        if (GLAD_GL_KHR_debug)
        {
#ifdef glDebugMessageCallback
#ifdef _DEBUG
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            OpenGLCheckError();
#else
            glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            OpenGLCheckError();
#endif
            glDebugMessageCallback(DebugCallback, this);
            OpenGLCheckError();
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
            OpenGLCheckError();
#ifndef _DEBUG
            glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, GL_DONT_CARE, 0, nullptr, GL_FALSE);
            OpenGLCheckError();
            glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_PORTABILITY, GL_DONT_CARE, 0, nullptr, GL_FALSE);
            OpenGLCheckError();
            glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_PERFORMANCE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
            OpenGLCheckError();
            glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_PERFORMANCE, GL_DEBUG_SEVERITY_LOW, 0, nullptr, GL_FALSE);
            OpenGLCheckError();
            glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_PERFORMANCE, GL_DEBUG_SEVERITY_MEDIUM, 0, nullptr, GL_FALSE);
            OpenGLCheckError();
#endif
#endif
        }

        glEnable(GL_PRIMITIVE_RESTART);
        OpenGLCheckError();

        glPrimitiveRestartIndex((GLuint)Mesh::Primitive::ResetIndex);
        OpenGLCheckError();

        ModelLibrary::Init_S();
        AnimationSequenceLibrary::Init_S();
        ArmatureLibrary::Init_S();
        MeshLibrary::Init_S();
        TextureLibrary::Init_S();
        SamplerLibrary::Init_S();
        ShaderLibrary::Init_S();
        MaterialLibrary::Init_S();
        SpriteSourceLibrary::Init_S();

        // Used to force the lighting buffer to load.
        GraphicsSystem::GetShader("Skinned");

        LightSource::Init();

        simpleMesh_ = GraphicsSystem::GetMesh("Sprite");
        simpleMatRect_ = GraphicsSystem::BuildMaterial("SimpleRect");
        simpleMatCircle_ = GraphicsSystem::BuildMaterial("SimpleCircle");

        GenerateDebugTexture();
        GenerateBlankTexture();

        // Return true if the initialization completed successfully.
        return true;
    }

    void GraphicsSystem_::Update(void)
    {
        auto bufSizeIter = Shader::GlobalUBOs().find("Time");
        if (bufSizeIter != Shader::GlobalUBOs().end())
        {
            const auto& ubo = bufSizeIter->second;
            GLsizeiptr bufSize = ubo.size;
            if (bufferDt_ == ResourceUnbind)
            {
                glGenBuffers(1, &bufferDt_);
                OpenGLCheckError();

                glBindBuffer(GL_UNIFORM_BUFFER, bufferDt_);
                OpenGLCheckError();

                glBufferData(GL_UNIFORM_BUFFER, bufSize, nullptr, GL_STATIC_DRAW);
                OpenGLCheckError();

                glBindBufferRange(GL_UNIFORM_BUFFER, static_cast<GLuint>(ubo.binding), bufferDt_, 0, bufSize);
                OpenGLCheckError();
            }
            else
            {
                glBindBuffer(GL_UNIFORM_BUFFER, bufferDt_);
                OpenGLCheckError();
            }

            float dt = PlatformSystem::GetDeltaTime();

            glBufferSubData(GL_UNIFORM_BUFFER, ubo.offsets.at("dt"), sizeof(float), &dt);
            OpenGLCheckError();

            glBindBuffer(GL_UNIFORM_BUFFER, ResourceUnbind);
            OpenGLCheckError();
        }

    }

    void GraphicsSystem_::Render(void)
    {
        GraphicsSystem::ClearBackground();

        size_t matCount = containerMaterial_->Count();
        PerformanceSystem::SetTrackerCount(PerformanceGroup::Material, EngineLoop::Render, matCount);
        for (size_t i = 0; i < matCount; ++i)
        {
            Material& mat = *containerMaterial_->GetFromIndex((unsigned int)i);
            auto lastRenderTime = mat.GetLastRenderTime();
            auto& tracker = PerformanceSystem::GetTracker(mat, mat);
            tracker.Push(lastRenderTime, EngineLoop::Render, PerformanceSource::Graphics);
            PerformanceSystem::AssignTrackerOrder(mat, PerformanceGroup::Material, EngineLoop::Render, i);
        }
    }

#define EXIT_CONTAINER(obj)\
    container##obj##_->FreeAll();\
    container##obj##_->Shrink();\

    void GraphicsSystem_::Exit(void)
    {
        PERFORM_ALL(EXIT_CONTAINER);
        if (viewportMatrices_ != ResourceUnbind)
        {
            glDeleteBuffers(1, &viewportMatrices_);
            viewportMatrices_ = ResourceUnbind;
        }
        if (bufferDt_ != ResourceUnbind)
        {
            glDeleteBuffers(1, &bufferDt_);
            bufferDt_ = ResourceUnbind;
        }
        SpriteSourceLibrary::Shutdown_S();
        MaterialLibrary::Shutdown_S();
        ShaderLibrary::Shutdown_S();
        SamplerLibrary::Shutdown_S();
        TextureLibrary::Shutdown_S();
        MeshLibrary::Shutdown_S();
        ArmatureLibrary::Shutdown_S();
        AnimationSequenceLibrary::Shutdown_S();
        ModelLibrary::Shutdown_S();
        LightSource::Exit();
        PlatformSystem::ViewportResizeCallbackRemove(resizeCallback_);
    }

    int GraphicsSystem_::LoopPriority(EngineLoop loop) const
    {
        switch (loop)
        {
        case EngineLoop::Render:
            return std::numeric_limits<int>::max();
        default:
            return 0;
        }
    }

    void GraphicsSystem_::GenerateDebugTexture()
    {
        Texture* texture = new Texture("DEBUG", UVec2(2,2), Texture::Format::RGBA, Texture::ChannelType::UByte);
        unsigned char* texData = texture->Data();
        texData[3] = 255;
        texData[7] = 255;
        texData[11] = 255;
        texData[15] = 255;

        texData[4] = 255;
        texData[6] = 255;
        texData[8] = 255;
        texData[10] = 255;
        texture->Register();
        TextureLibrary::GetInstance()->Add(texture);
        SpriteSource* ss = new SpriteSource(texture->GetName(), texture);
        SpriteSourceLibrary::GetInstance()->Add(ss);
    }

    void GraphicsSystem_::GenerateBlankTexture()
    {
        Texture* texture = new Texture("BLANK", UVec2(1,1), Texture::Format::RGBA, Texture::ChannelType::UByte);
        unsigned char* texData = texture->Data();
        texData[0] = 255;
        texData[1] = 255;
        texData[2] = 255;
        texData[3] = 255;
        texture->Register();
        TextureLibrary::GetInstance()->Add(texture);
        SpriteSource* ss = new SpriteSource(texture->GetName(), texture);
        SpriteSourceLibrary::GetInstance()->Add(ss);
    }

    void GraphicsSystem_::UpdateCamera()
    {
        auto bufSizeIter = Shader::GlobalUBOs().find(string_t(Shader::GlobalBindViewportName));
        if (bufSizeIter == Shader::GlobalUBOs().end())
        {
            return;
        }
        const auto& ubo = bufSizeIter->second;
        GLsizeiptr bufSize = ubo.size;
        if (viewportMatrices_ == ResourceUnbind)
        {
            glGenBuffers(1, &viewportMatrices_);
            OpenGLCheckError();

            glBindBuffer(GL_UNIFORM_BUFFER, viewportMatrices_);
            OpenGLCheckError();

            glBufferData(GL_UNIFORM_BUFFER, bufSize, nullptr, GL_STATIC_DRAW);
            OpenGLCheckError();

            glBindBufferRange(GL_UNIFORM_BUFFER, static_cast<GLuint>(ubo.binding), viewportMatrices_, 0, bufSize);
            OpenGLCheckError();
        }
        else
        {
            glBindBuffer(GL_UNIFORM_BUFFER, viewportMatrices_);
            OpenGLCheckError();
        }

        switch (camera_->GetMode())
        {
        case CameraSource::Mode::Perspective:
            glBufferSubData(GL_UNIFORM_BUFFER, ubo.offsets.at("projectionPersp"), sizeof(Mat4), &camera_->projection_);
            OpenGLCheckError();
            glBufferSubData(GL_UNIFORM_BUFFER, ubo.offsets.at("viewPersp"), sizeof(Mat4), &camera_->view_);
            OpenGLCheckError();
            break;
        case CameraSource::Mode::Orthographic:
            glBufferSubData(GL_UNIFORM_BUFFER, ubo.offsets.at("projectionOrtho"), sizeof(Mat4), &camera_->projection_);
            OpenGLCheckError();
            glBufferSubData(GL_UNIFORM_BUFFER, ubo.offsets.at("viewOrtho"), sizeof(Mat4), &camera_->view_);
            OpenGLCheckError();
            break;
        default:
            assert(false);
            break;
        }
        glBufferSubData(GL_UNIFORM_BUFFER, ubo.offsets.at("resolution"), sizeof(UVec2), &PlatformSystem::GetWindowSize());
        OpenGLCheckError();

        glBindBuffer(GL_UNIFORM_BUFFER, ResourceUnbind);
        OpenGLCheckError();
    }

#define IMPLEMENT_READ(obj)\
    template <> bool StreamIn::TryReadValueInternal<obj*>(const StreamValue& streamValue, obj*& value)\
    {\
        string_t name;\
        if (!TryReadValueInternal(streamValue, name))\
        {\
            return false;\
        }\
        if (name.empty())\
        {\
            lastError_ = ReadValueError::InvalidValue;\
            return false;\
        }\
        value = GraphicsSystem::Build##obj(name);\
        return true;\
    }\

    PERFORM_ALL(IMPLEMENT_READ);

#pragma endregion Private Functions

}	// namespace
