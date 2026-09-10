//------------------------------------------------------------------------------
//
// File Name:	Material.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Material encapsulator.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Material.h"

#include GRAPHICS_INCLUDE

#include "Texture.h"
#include "Sampler.h"
#include "Shader.h"
#include "DataTypes.h"

#include "Library.inl"
#include "LibraryStatic.inl"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    //--------------------------------------------------------------------------
    // Public Constants:
    //--------------------------------------------------------------------------

    constexpr const char Material::MatrixWorld[] = "world";
    constexpr const char Material::MatrixWorldNormal[] = "worldNormal";

    //--------------------------------------------------------------------------
    // Public Static Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Public Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Private Constants:
    //--------------------------------------------------------------------------

    static constexpr StreamKey ReadKeyProperties = "Properties";

    //--------------------------------------------------------------------------
    // Private Static Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Private Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Constructors/Destructors:
    //--------------------------------------------------------------------------

#pragma region Constructors

    Material::Material(void)
        : NamedObject()
        , MaterialRenderSettings()
        , shader_(nullptr)
        , uniformTextures_()
        , uniformsIndex_(-1)
        , outputSize_(0)
        , timer_()
    {
        TraceVerbose("Creating empty material");
        timer_.Initialize();
    }

    Material::Material(const Material& other)
        : NamedObject(other)
        , MaterialRenderSettings(other)
        , shader_(nullptr)
        , uniformTextures_(other.uniformTextures_)
        , uniformsIndex_(-1)
        , outputSize_(other.outputSize_)
        , timer_()
    {
        TraceVerbose("Creating material: {}", GetName());
        timer_.Initialize();
        SetShader(other.shader_);
        assert(uniformsIndex_ != other.uniformsIndex_);
        // Copy UBO data.
        unsigned blockIndex = 0;
        for (const auto& block : shader_->uniformBlocks_)
        {
            const ResourceID& otherUBO = shader_->materialBuffers_->uniforms_[other.uniformsIndex_ + blockIndex];
            const ResourceID& thisUBO = shader_->materialBuffers_->uniforms_[uniformsIndex_ + blockIndex];
            glBindBuffer(GL_COPY_READ_BUFFER, otherUBO);
            OpenGLCheckError();
            glBindBuffer(GL_COPY_WRITE_BUFFER, thisUBO);
            OpenGLCheckError();

            glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, block.size);
            OpenGLCheckError();

            ++blockIndex;
        }
        glBindBuffer(GL_COPY_READ_BUFFER, ResourceUnbind);
        OpenGLCheckError();
        glBindBuffer(GL_COPY_WRITE_BUFFER, ResourceUnbind);
        OpenGLCheckError();
    }

    Material::Material(Material&& other) noexcept
        : NamedObject(std::move(other))
        , MaterialRenderSettings(std::move(other))
        , shader_(std::move(other.shader_))
        , uniformTextures_(std::move(other.uniformTextures_))
        , uniformsIndex_(std::move(other.uniformsIndex_))
        , outputSize_(std::move(other.outputSize_))
        , timer_(std::move(other.timer_))
    {
    }

    Material& Material::operator=(const Material& other)
    {
        NamedObject::operator=(other);
        MaterialRenderSettings::operator=(other);
        uniformTextures_ = other.uniformTextures_;
        outputSize_ = other.outputSize_;
        if (shader_ != other.shader_)
        {
            SetShader(other.shader_);
        }
        assert(uniformsIndex_ != other.uniformsIndex_);
        // Copy UBO data.
        unsigned blockIndex = 0;
        for (const auto& block : shader_->uniformBlocks_)
        {
            const ResourceID& otherUBO = shader_->materialBuffers_->uniforms_[other.uniformsIndex_ + blockIndex];
            const ResourceID& thisUBO = shader_->materialBuffers_->uniforms_[uniformsIndex_ + blockIndex];
            glBindBuffer(GL_COPY_READ_BUFFER, otherUBO);
            OpenGLCheckError();
            glBindBuffer(GL_COPY_WRITE_BUFFER, thisUBO);
            OpenGLCheckError();

            glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, block.size);
            OpenGLCheckError();

            ++blockIndex;
        }
        glBindBuffer(GL_COPY_READ_BUFFER, ResourceUnbind);
        OpenGLCheckError();
        glBindBuffer(GL_COPY_WRITE_BUFFER, ResourceUnbind);
        OpenGLCheckError();
        return *this;
    }

    //--------------------------------------------------------------------------

    Material::~Material(void)
    {
        TraceVerbose("Deleting material: {}", GetName());
        if (uniformsIndex_ >= 0 && shader_ != nullptr)
        {
            shader_->materialBuffers_->RemoveMaterialBuffers(uniformsIndex_);
            shader_ = nullptr;
            uniformsIndex_ = -1;
        }
    }

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

#pragma endregion Public Static Functions

    //--------------------------------------------------------------------------
    // Public Functions:
    //--------------------------------------------------------------------------

#pragma region Public Functions

    void Material::Read(StreamIn& stream)
    {
        ReadName(stream);

        TraceInfo("Reading material: {}", GetName());
        
        MaterialRenderSettings::Read(stream);

        if (stream.StreamHas(Shader::ReadKey))
        {
            SetShader(GraphicsSystem::GetShader(stream.ReadValue_<string_t>(Shader::ReadKey)));
        }
        
        //stream.TryReadEnum(ReadKeyFaceCull, ENUM_READ(FaceCull), faceCull_);

        if (stream.StreamHas(ReadKeyProperties))
        {
#define TRACE_HEADER "Failed to read material property "
            stream.ReadArrayObjects(ReadKeyProperties, [this](StreamIn& stream, int index)
            {
                static constexpr StreamKey ReadKeyName = "Name", ReadKeyType = "Type", ReadKeyValue = "Value";
                UNREFERENCED_PARAMETER(index);
                string_t name;
                if (!stream.TryReadValue(ReadKeyName, name))
                {
                    TraceWarning(TRACE_HEADER"due to missing name key: \"{}\"", stream.GetHierarchy());
                    return;
                }
                DataType type;
                if (!stream.TryReadEnum(ReadKeyType, ENUM_READ(DataType), type))
                {
                    TraceWarning(TRACE_HEADER"\"{}\" type at: \"{}\"", name, stream.GetHierarchy());
                    return;
                }
                if (!stream.StreamHas(ReadKeyValue))
                {
                    TraceWarning(TRACE_HEADER"\"{}\" value due to missing value key: \"{}\"", name, stream.GetHierarchy());
                    return;
                }
                switch (type)
                {
#define READ_PROPERTY(enumVal, type) case DataType::enumVal: SetUniform(name, stream.ReadValue_<type>(ReadKeyValue)); break
                    READ_PROPERTY(Single, float);
                    READ_PROPERTY(Vec2, Vec2);
                    READ_PROPERTY(Vec3, Vec3);
                    READ_PROPERTY(Vec4, Vec4);
                    READ_PROPERTY(Double, double);
                    READ_PROPERTY(DVec2, DVec2);
                    READ_PROPERTY(DVec3, DVec3);
                    READ_PROPERTY(DVec4, DVec4);
                    READ_PROPERTY(Int, int);
                    READ_PROPERTY(IVec2, IVec2);
                    READ_PROPERTY(IVec3, IVec3);
                    READ_PROPERTY(IVec4, IVec4);
                    READ_PROPERTY(UInt, unsigned int);
                    READ_PROPERTY(UVec2, UVec2);
                    READ_PROPERTY(UVec3, UVec3);
                    READ_PROPERTY(UVec4, UVec4);
                case DataType::Texture2D:
                {
                    string_t texName = stream.ReadValue_<string_t>(ReadKeyValue);
                    const Texture* tex = GraphicsSystem::GetTexture(texName);
                    SetTexture(name, tex);
                }
                    break;
                case DataType::Sampler:
                {
                    string_t samplerName = stream.ReadValue_<string_t>(ReadKeyValue);
                    const Sampler* sampler = GraphicsSystem::GetSampler(samplerName);
                    SetSampler(name, sampler);
                }
                    break;
                default:
                    string_t typeName = stream.ReadValue_<string_t>(ReadKeyType);
                    TraceWarning(TRACE_HEADER"\"{}\" value due to missing implementation for \"{}\" at: \"{}\"", name, typeName, stream.GetHierarchy());
                    return;
                }
            });
        }
    }
    
    const Shader* Material::GetShader() const
    {
        return shader_;
    }

    void Material::SetShader(const Shader* shader)
    {
        shader_ = shader;
        assert(shader != nullptr);
        uniformsIndex_ = shader->materialBuffers_->GetMaterialBuffers();
    }
    
    void Material::SetTexture(const string_t& name, const Texture* value)
    {
        uniformTextures_[name].texture = value;
        auto& sampler = uniformTextures_[name].sampler;
        if (sampler == nullptr)
        {
            sampler = GraphicsSystem::GetSampler("Sprite");
        }
    }

    void Material::SetSampler(const string_t& name, const Sampler* value)
    {
        uniformTextures_[name].sampler = value;
    }

    void Material::SetTextureSampler(const string_t& name, const Texture* texture, const Sampler* sampler)
    {
        auto& texsampler = uniformTextures_[name];
        texsampler.texture = texture;
        texsampler.sampler = sampler;
    }

    void Material::Activate(const Mat4& worldMtx) const
    {
        timer_.GetResults();
        timer_.Start();

        assert(shader_ != nullptr);
        glUseProgram(shader_->id_);
        OpenGLCheckError();

        SetUniforms(worldMtx);

        SetRenderData();

        if (shader_->HasOutput())
        {
            glEnable(GL_RASTERIZER_DISCARD);
            OpenGLCheckError();
            glBeginTransformFeedback(GL_POINTS);
            OpenGLCheckError();
        }
#ifdef _DEBUG
        glValidateProgram(shader_->id_);
        OpenGLCheckError();
        GLint validateStatus;
        glGetProgramiv(shader_->id_, GL_VALIDATE_STATUS, &validateStatus);
        OpenGLCheckError();
        assert(validateStatus == GL_TRUE);
#endif
    }

    void Material::Deactivate(void) const
    {
        if (shader_->HasOutput())
        {
            glEndTransformFeedback();
            OpenGLCheckError();
            //// flush commands to make sure it is finished
            //glFlush();
            //OpenGLCheckError();
            glDisable(GL_RASTERIZER_DISCARD);
            OpenGLCheckError();
        }
        //glUseProgram(ResourceUnbind);
        //OpenGLCheckError();

        timer_.Stop();
    }

    bool Material::HasOutput(void) const
    {
        return shader_ == nullptr || shader_->HasOutput();
    }

    void Material::SetOutputSize(size_t size)
    {
        if (shader_ == nullptr)
        {
            TraceWarning("Tried to set output size on material \"{}\" without shader!", GetName());
            return;
        }
        if (!shader_->HasOutput())
        {
            TraceWarning("Tried to set output size on material \"{}\" with shader \"\" that lacks output!", GetName(), shader_->GetName());
            return;
        }
        outputSize_ = (unsigned)size;
        shader_->materialBuffers_->SetOutputSize(uniformsIndex_, outputSize_);
    }

    nsec_t Material::GetLastRenderTime(void) const
    {
        return timer_.GetDuration();
    }

#pragma endregion Public Functions

    //--------------------------------------------------------------------------
    // Private Static Functions:
    //--------------------------------------------------------------------------

#pragma region Private Static Functions

#pragma endregion Private Static Functions

    //--------------------------------------------------------------------------
    // Private Functions:
    //--------------------------------------------------------------------------

#pragma region Private Functions

    void Material::SetUniforms(const Mat4& worldMtx) const
    {
        GLint uniformLocation;
        unsigned texCount = 0;
        unsigned blockIndex = 0;
        for (const auto& block : shader_->uniformBlocks_)
        {
            // glBindBufferBase can also be used, but glBindBufferRange gives more control.
            if (block.usage == Shader::BufferUsage::Output)
            {
                glBindBufferRange(GL_TRANSFORM_FEEDBACK_BUFFER, static_cast<GLuint>(block.binding), shader_->materialBuffers_->uniforms_[uniformsIndex_ + blockIndex], 0, outputSize_);
            }
            else
            {
                glBindBufferRange(GL_UNIFORM_BUFFER, static_cast<GLuint>(block.binding), shader_->materialBuffers_->uniforms_[uniformsIndex_ + blockIndex], 0, block.size);
            }
            OpenGLCheckError();
            ++blockIndex;
        }
        if (shader_->HasOutput())
        {
            const auto& block = shader_->outputs_[0];
            glBindBufferRange(GL_TRANSFORM_FEEDBACK_BUFFER, static_cast<GLuint>(block.binding), shader_->materialBuffers_->uniforms_[uniformsIndex_ + blockIndex], 0, outputSize_);
        }
        for (const auto& [texName, uniformTexture] : uniformTextures_)
        {
            glActiveTexture(GL_TEXTURE0 + texCount);
            OpenGLCheckError();
            uniformTexture.texture->Activate();
            uniformTexture.sampler->Activate();
            const auto uniformTex = shader_->GetUniform(texName);
            if (uniformTex != nullptr)
            {
                assert(uniformTex->location == glGetUniformLocation(shader_->id_, texName.c_str()) && "The shader didn't bind uniforms correctly! Sampler name inconsistent!");
                glUniform1i(uniformTex->location, (GLint)texCount);
            }
            else
            {
                TraceWarning("Unable to set uniform data for Sampler2D \"{}\"", texName);
            }
            ++texCount;
        }
        {
            const auto uniformWorld = shader_->GetUniform(MatrixWorld);
            assert(uniformWorld != nullptr && "All shaders need a uniform with the name from MatrixWorld (which is included with the \"Common/Viewport\" shader). Make sure it's used so it isn't optimized away when the shader compiles!");
            uniformLocation = static_cast<int>(uniformWorld->location);
            assert(uniformLocation == glGetUniformLocation(shader_->id_, MatrixWorld) && "The shader didn't bind uniforms correctly! MatrixWorld inconsistent!");
            OpenGLCheckError();
            glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(worldMtx));
            OpenGLCheckError();
            const auto uniformWorldNormal = shader_->GetUniform(MatrixWorldNormal);
            if (uniformWorldNormal != nullptr)
            {
                Mat4 mtxWorldNormal = glm::inverse(worldMtx); // glm::transpose
                uniformLocation = static_cast<int>(uniformWorldNormal->location);
                assert(uniformLocation == glGetUniformLocation(shader_->id_, MatrixWorldNormal) && "The shader didn't bind uniforms correctly! MatrixWorldNormal inconsistent!");
                OpenGLCheckError();
                glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(mtxWorldNormal));
                OpenGLCheckError();
            }
        }
    }

    void Material::SetUniformInternal(const string_t& name, const void* value, long long offset, long long size)
    {
        const auto entry = shader_->GetUniform(name);
        if (entry == nullptr)
        {
            TraceWarning("Failed to find uniform \"{}\" in material \"{}\" with shader \"{}\"", name, GetName(), shader_->GetName());
            return;
        }
        if (entry->block < 0)
        {
            TraceWarning("Cannot set uniform \"{}\" in material \"{}\" with shader \"{}\" because it is not in a block!", name, GetName(), shader_->GetName());
            return;
        }

        glBindBuffer(GL_UNIFORM_BUFFER, shader_->materialBuffers_->uniforms_[static_cast<size_t>(uniformsIndex_) + entry->block]);
        OpenGLCheckError();

        auto dataOffset = static_cast<GLsizeiptr>(entry->offset + offset);
        glBufferSubData(GL_UNIFORM_BUFFER, dataOffset, size, value);
        OpenGLCheckError();

        glBindBuffer(GL_UNIFORM_BUFFER, ResourceUnbind);
        OpenGLCheckError();
    }

    const ResourceID& Material::GetOutputBuffer(void) const
    {
        const auto& block = shader_->outputs_[0];
        return shader_->materialBuffers_->uniforms_[(unsigned)(uniformsIndex_ + block.index)];
    }

    ResourceID& Material::GetOutputBufferEdit(void) const
    {
        const auto& block = shader_->outputs_[0];
        return shader_->materialBuffers_->uniforms_[(unsigned)(uniformsIndex_ + block.index)];
    }

#pragma endregion Private Functions

    //--------------------------------------------------------------------------
    // Library Implementation:
    //--------------------------------------------------------------------------

#pragma region Library Implementation

    template MaterialLibrary;

    INSTANTIATE_LIBRARY_IMGUI_SELECTOR(Material);

    IMPLEMENT_READ(Material);

    IMPLEMENT_WRITE(Material);

#pragma endregion Library Implementation

}	// namespace
