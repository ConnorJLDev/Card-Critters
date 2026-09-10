//------------------------------------------------------------------------------
//
// File Name:	Shader.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Shader encapsulator.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//#define UNIFORM_DOUBLES

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Shader.h"

#include GRAPHICS_INCLUDE

#include "PlatformSystem.h"
#include "Material.h"

#include "Library.inl"
#include "LibraryStatic.inl"

#undef TRACE_CATEGORY
#define TRACE_CATEGORY SHADER

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

	//--------------------------------------------------------------------------
	// Public Static Variables:
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// Public Variables:
	//--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Private Structures:
    //--------------------------------------------------------------------------

    //
    struct StageInfo
    {
        ResourceID id;
        string_view_t name;
    };

	//--------------------------------------------------------------------------
	// Private Constants:
	//--------------------------------------------------------------------------

    constexpr const ResourceID Shader::InstanceBindStart = 5u;

    constexpr StreamKey
          Shader::GlobalBindViewportName = "Viewport"
        , Shader::GlobalBindLightingName = "Lighting"
        , Shader::StageVertex = "Vertex"
        , Shader::StageGeometry = "Geometry"
        , Shader::StageFragment = "Fragment"
        ;

#define ENTRY_INFO(eName, glValue, eValue) {glValue, DataType::eValue}
    const STATIC_MAP_CLASS((const ResourceID, const DataType), Shader, TypeIdMap,
        FOR_EACH_1_2_2_C(ENTRY_INFO, DataType, 
            GL_FLOAT,                Single,
            GL_FLOAT_VEC2,           Vec2,
            GL_FLOAT_VEC3,           Vec3,
            GL_FLOAT_VEC4,           Vec4,
            GL_DOUBLE,               Double,
            GL_DOUBLE_VEC2,          DVec2,
            GL_DOUBLE_VEC3,          DVec3,
            GL_DOUBLE_VEC4,          DVec4,
            GL_INT,                  Int,
            GL_INT_VEC2,             IVec2,
            GL_INT_VEC3,             IVec3,
            GL_INT_VEC4,             IVec4,
            GL_UNSIGNED_INT,         UInt,
            GL_UNSIGNED_INT_VEC2,    UVec2,
            GL_UNSIGNED_INT_VEC3,    UVec3,
            GL_UNSIGNED_INT_VEC4,    UVec4,
            GL_BOOL,                 Bool,
            GL_BOOL_VEC2,            BVec2,
            GL_BOOL_VEC3,            BVec3,
            GL_BOOL_VEC4,            BVec4,
            GL_FLOAT_MAT2,           Mat2,
            GL_FLOAT_MAT2x3,         Mat2x3,
            GL_FLOAT_MAT2x4,         Mat2x4,
            GL_FLOAT_MAT3x2,         Mat3x2,
            GL_FLOAT_MAT3,           Mat3,
            GL_FLOAT_MAT3x4,         Mat3x4,
            GL_FLOAT_MAT4x2,         Mat4x2,
            GL_FLOAT_MAT4x3,         Mat4x3,
            GL_FLOAT_MAT4,           Mat4,
            GL_DOUBLE_MAT2,          DMat2,
            GL_DOUBLE_MAT2x3,        DMat2x3,
            GL_DOUBLE_MAT2x4,        DMat2x4,
            GL_DOUBLE_MAT3x2,        DMat3x2,
            GL_DOUBLE_MAT3,          DMat3,
            GL_DOUBLE_MAT3x4,        DMat3x4,
            GL_DOUBLE_MAT4x2,        DMat4x2,
            GL_DOUBLE_MAT4x3,        DMat4x3,
            GL_DOUBLE_MAT4,          DMat4,
            GL_SAMPLER_2D,           Texture2D
        )
    )
#undef ENTRY_INFO

    static constexpr array_t<StageInfo, 3> StageInfos
    {{
        {GL_VERTEX_SHADER,   "vert"},
        {GL_GEOMETRY_SHADER, "geom"},
        {GL_FRAGMENT_SHADER, "frag"},
    }};

    ENUM_MAPS_CLASS_SEQUENTIAL_UPPER(Shader, BufferUsage, Material, Asset, Global, Output);

	//--------------------------------------------------------------------------
	// Private Static Variables:
	//--------------------------------------------------------------------------

    array_t<map_t<string_t, ResourceID>*, Shader::StageType::NUM_ENTRIES>& Shader::CompiledStages()
    {
        static array_t<map_t<string_t, ResourceID>*, StageType::NUM_ENTRIES> compiledStages_
        {{
            &StaticManagement::MapWrapper<string_t, ResourceID>::Create(),
            &StaticManagement::MapWrapper<string_t, ResourceID>::Create(),
            &StaticManagement::MapWrapper<string_t, ResourceID>::Create(),
        }};
        return compiledStages_;
    }

    STATIC_MAP_CLASS((string_t, Shader::UniformBlockInfoGlobal), Shader, GlobalUBOs)

    STATIC_MAP_CLASS((string_t, Shader::BufferUsage), Shader, BufferUsages)

    static constexpr string_view_t ShaderNormalDefinition =
#ifdef SHADER_NORMAL
        "#define SHADER_NORMAL" // Enabling this define makes the shader use an inverse calculation for the normal matrix, instead of calculating it on the CPU.
#else
        ""
#endif
        ;

    STATIC_VECTOR_CLASS((string_t), Shader, GlobalDefinitions,
        string_t(ShaderNormalDefinition),
        "#define BONE_MAX 96u",
        "#define BONES_PER_VERT 4",
        "#define TEXT_USE_GEOMETRY",
        "#define TEXT_MAX_SYMBOLS 192", // ASCII 0x20 - 0x7F for now
        "#define TEXT_MAX_EFFECTS 4",
        "#define LIGHT_COUNT 8",
        "#define PARTICLE_BATCH_GEOMETRY_VERTS_MAX 64",
        )

    STATIC_MAP_CLASS((string_t, string_t), Shader, Commons)

	//--------------------------------------------------------------------------
	// Private Variables:
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// Constructors/Destructors:
	//--------------------------------------------------------------------------

#pragma region Constructors

#pragma warning(push)
#pragma warning(disable : 4355) // the data from *this is not used in materialBuffers_ during its construction.

	Shader::Shader(void)
		: NamedObject()
        , inputAttrs_()
        , uniforms_()
        , uniformBlocks_()
        , uniformsMap_()
        , outputNames_()
        , outputs_()
        , materialBuffers_(new MaterialBuffers(*this))
        , id_(ResourceUnbind)
        , bufferCount_(0)
	{
	}

    Shader::Shader(const Shader& other)
        : NamedObject(other)
        , inputAttrs_()
        , uniforms_()
        , uniformBlocks_()
        , uniformsMap_()
        , outputNames_()
        , outputs_()
        , materialBuffers_(new MaterialBuffers(*this))
        , id_(ResourceUnbind)
        , bufferCount_(0)
    {
        assert(false);
    }

#pragma warning(pop)

	//--------------------------------------------------------------------------

	Shader::~Shader(void)
	{
        if (id_ != ResourceUnbind)
        {
            TraceInfo("Deleting shader: {}", GetName());
            glDeleteProgram(id_);
            OpenGLCheckError();
        }
        SafeDeleteCheck(materialBuffers_);
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

	void Shader::Read(StreamIn& stream)
	{
        ReadName(stream);

        TraceInfo("Reading shader: {}", GetName());

        string_t vertName = "", fragName = "", geomName = "";
        stream.TryReadValue(StageVertex, vertName);
        stream.TryReadValue(StageGeometry, geomName);
        stream.TryReadValue(StageFragment, fragName);
        ReadRaw(vertName, fragName, geomName);
	}

    bool Shader::HasOutput(void) const
    {
        return !outputs_.empty();
    }

#pragma endregion Public Functions

	//--------------------------------------------------------------------------
	// Private Static Functions:
	//--------------------------------------------------------------------------

#pragma region Private Static Functions

    ResourceID Shader::GetStage(StageType stageType, const string_t& stageName)
    {
        if (stageName.empty())
        {
            return ResourceUnbind;
        }
        return CompileStage(stageType, stageName);
        //auto& stageMap = *CompiledStages()[(unsigned)stageType];
        //const StageInfo& stageInfo = StageInfos[(unsigned)stageType];
        //string_t filepath{};
        //std::format_to(std::back_inserter(filepath), "{}_{}.glsl", stageName, stageInfo.name);
        //auto iter = stageMap.find(stageName);
        //if (iter != stageMap.end())
        //{
        //    return iter->second;
        //}
        //return stageMap.emplace(stageName, CompileStage(stageType, stageName)).second;
        //return stageMap->try_emplace(stageName, CompileStage(stageType, stageName)).first->second;
    }

    ResourceID Shader::CompileStage(StageType stageType, const string_t& stageName)
    {
        Stopwatch stopwatch{};
        stopwatch.Start();

        const StageInfo& stageInfo = StageInfos[(unsigned)stageType];
        string_t filepath = IOSystem::GetAssetsPath("Shaders");
        std::format_to(std::back_inserter(filepath), "{}_{}.glsl", stageName, stageInfo.name);

        // Load shader data from the file
        std::ifstream shaderFile(filepath.c_str());
        if (!shaderFile.is_open())
        {
            TraceWarning("Failed to find shader file \"{}\"", filepath);
            return ResourceUnbind;
        }
        vector_t<const char*>& sources(StaticManagement::VectorWrapper<const char*>::Create());
        vector_t<int>& sourceLengths(StaticManagement::VectorWrapper<int>::Create());
        vector_t<string_t>& depths(StaticManagement::VectorWrapper<string_t>::Create());
        sources.clear();
        sourceLengths.clear();
        depths.clear();
        depths.emplace_back(filepath);
        if (stageType == StageType::Vertex || stageType == StageType::Geometry)
        {
            outputs_.clear();
        }
        ShaderParser parser{ sources, sourceLengths, outputNames_, depths, stageType };
        string_t shaderString = ReadShaderStream(parser, shaderFile);
        shaderFile.close();
        // Get the shader text from the stream
#define ADD_SOURCE(var) sources.emplace_back(var.c_str()); sourceLengths.emplace_back(static_cast<int>(var.size()));
        ADD_SOURCE(shaderString);

        auto readTime = stopwatch.Lap<msecd_t>();

        // Create the vertex shader ID
        ResourceID stage = glCreateShader(stageInfo.id);
        OpenGLCheckError();
        // Assign the shader source
        assert(sources.size() == sourceLengths.size());
        glShaderSource(stage, static_cast<int>(sources.size()), sources.data(), sourceLengths.data());
        OpenGLCheckError();
        // Compile the shader
        glCompileShader(stage);
        OpenGLCheckError();
        // Check if the compilation was successful
        GLint success = false;
        glGetShaderiv(stage, GL_COMPILE_STATUS, &success);
        OpenGLCheckError();

        auto compileTime = stopwatch.Lap<msecd_t>();
        stopwatch.Stop();

        if (!success)
        {
            for (unsigned i = 0; i < sources.size(); ++i)
            {
                TraceWarning("Input {} {}:\n{}", stageInfo.name, i, sources[i]);
            }
            PrintShaderError(stage, "compile", filepath);
            assert(false);
        }
        else
        {
            TraceVerbose("{} {}: {} ({}|{}|{})", stageInfo.name, stage, stageName, readTime, compileTime, stopwatch.Duration<msecd_t>());
        }

        return stage;
    }

    const string_t& Shader::GetCommon(const string_t& commonName)
    {
        auto iter = Commons().find(commonName);
        if (iter != Commons().end())
        {
            return iter->second;
        }
        return Commons().emplace(commonName, ReadCommon(commonName)).first->second;
    }

    string_t Shader::ReadCommon(const string_t& commonName)
    {
        string_t buffer{};
        if (!IOSystem::ReadFileAsString(IOSystem::GetAssetsPath("Shaders") + commonName, buffer))
        {
            TraceWarning("Failed to find included shader file \"{}\"", commonName);
        }
        return buffer;
    }

    void Shader::DeleteAllStages()
    {
        for (auto& stageMap : CompiledStages())
        {
            for (auto& stageIter : *stageMap)
            {
                ResourceID& stageID = stageIter.second;
                if (stageID != ResourceUnbind)
                {
                    glDeleteShader(stageID);
                    OpenGLCheckError();
                }
            }
            stageMap->clear();
        }
    }

    void Shader::PrintShaderError(ResourceID shaderID, const string_t& action, const string_t& name)
    {
        string_t infoLog{};
        GLint requiredSize;
        if (glIsShader(shaderID))
        {
            string_t source{};

            glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &requiredSize);
            OpenGLCheckError();
            infoLog.resize((size_t)requiredSize);

            GLsizei infoLength = 0;
            glGetShaderInfoLog(shaderID, static_cast<GLsizei>(infoLog.size()), &infoLength, infoLog.data());
            OpenGLCheckError();

            glGetShaderiv(shaderID, GL_SHADER_SOURCE_LENGTH, &requiredSize);
            OpenGLCheckError();
            source.resize((size_t)requiredSize);

            GLsizei sourceLength = 0;
            glGetShaderSource(shaderID, static_cast<GLsizei>(source.size()), &sourceLength, source.data());
            OpenGLCheckError();

            string_t sourceStr, line;

            std::stringstream ss(source);

            unsigned lineNum = 0;
            while (std::getline(ss, line))
            {
                FORMAT_TO(sourceStr, "{: >4}: {}\n", ++lineNum, line);
            }

            TraceVerbose("\"{}\" source:\n{}", name, sourceStr);

            TraceError("Failed to {} \"{}\":\n{}", action, name, infoLog);
        }
        else if (glIsProgram(shaderID))
        {
            glGetProgramiv(shaderID, GL_INFO_LOG_LENGTH, &requiredSize);
            OpenGLCheckError();
            infoLog.resize((size_t)requiredSize);

            GLsizei infoLength = 0;
            glGetProgramInfoLog(shaderID, static_cast<GLsizei>(infoLog.size()), &infoLength, infoLog.data());
            OpenGLCheckError();

            TraceError("Failed to {} {}:\n{}", action, name, infoLog);
        }
        else
        {
            TraceError("Failed to {} {}", action, name);
        }
    }

#pragma endregion Private Static Functions

    //--------------------------------------------------------------------------
    // Private Functions:
    //--------------------------------------------------------------------------

#pragma region Private Functions

    void Shader::ReadRaw(const string_t& vertex, const string_t& fragment, const string_t& geometry)
    {
        Stopwatch stopwatch{};
        stopwatch.Start();

        glBindVertexArray(ResourceUnbind);
        OpenGLCheckError();

        // Get stages
        ResourceID stages[] =
        {
            GetStage(StageType::Vertex, vertex),
            GetStage(StageType::Geometry, geometry),
            GetStage(StageType::Fragment, fragment),
        };

        auto stageTime = stopwatch.Lap<msecd_t>();

        // Create the shader program
        id_ = glCreateProgram();
        OpenGLCheckError();
        // Attach stages
        for (auto& stage : stages)
        {
            if (stage != ResourceUnbind)
            {
                glAttachShader(id_, stage);
                OpenGLCheckError();
            }
        }
        // TODO: Specify transform feedback, get output names from shaders.
        vector_t<GLchar*> feedbackNames{ outputNames_.size() };
        for (unsigned i = 0; i < outputNames_.size(); ++i)
        {
            feedbackNames[i] = outputNames_[i].data();
        }
        if (!feedbackNames.empty())
        {
            // GL_INTERLEAVED_ATTRIBS (single buffer) GL_SEPARATE_ATTRIBS (multiple buffers)
            glTransformFeedbackVaryings(id_, static_cast<GLint>(feedbackNames.size()), feedbackNames.data(), GL_INTERLEAVED_ATTRIBS);
        }
        // Link the program
        glLinkProgram(id_);
        OpenGLCheckError();
        // Check if the program linked successfully
        GLint success = false;
        glGetProgramiv(id_, GL_LINK_STATUS, &success);
        OpenGLCheckError();

        auto linkTime = stopwatch.Lap<msecd_t>();
        stopwatch.Stop();

        if (!success)
        {
            PrintShaderError(id_, "link", GetName());
            assert(false);
        }
        else
        {
            TraceVerbose("Shader {}: {} ({}|{}|{})", id_, GetName(), stageTime, linkTime, stopwatch.Duration<msecd_t>());
        }

        ParseShader();
        bufferCount_ = decltype(bufferCount_)(uniformBlocks_.size() + outputs_.size());
    }

    const Shader::UniformInfo* Shader::GetUniform(const string_t& name) const
    {
        const auto entryIter = uniformsMap_.find(name);
        if (entryIter == uniformsMap_.end())
        {
            return nullptr;
        }
        return &uniforms_[entryIter->second];
    }

#pragma endregion Private Functions

    //--------------------------------------------------------------------------
    // Library Implementation:
    //--------------------------------------------------------------------------

#pragma region Library Implementation

    template ShaderLibrary;

    INSTANTIATE_LIBRARY_IMGUI_SELECTOR(Shader);

    IMPLEMENT_READ(Shader);

    IMPLEMENT_WRITE(Shader);

#pragma endregion Library Implementation

}	// namespace
