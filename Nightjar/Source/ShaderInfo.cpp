//------------------------------------------------------------------------------
//
// File Name:	ShaderUniformEntry.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Shader encapsulator.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Shader.h"

#include GRAPHICS_INCLUDE

#undef TRACE_CATEGORY
#define TRACE_CATEGORY SHADER

#define TRACE_SHADER

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
	// Private Constants:
	//--------------------------------------------------------------------------

    static GLsizei nameBufferSize = 0;
    static GLchar* nameBuffer = nullptr;

    static void UpdateNameBuffer(GLsizei size)
    {
        if (size <= nameBufferSize)
        {
            return;
        }
        size_t sizeNew = std::bit_ceil(((size_t)size) + 1);
        nameBuffer = reinterpret_cast<GLchar*>(MemoryManager::Reallocate(nameBuffer, sizeNew));
        nameBufferSize = decltype(nameBufferSize)(sizeNew);
    }

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

    Shader::AttributeInfo::AttributeInfo(const Shader& shader, unsigned attributeIndex)
        : name()
        , index((int)attributeIndex)
        , location(-2)
        , arraySize(-2)
        , type(DataType::Invalid)
    {
        GLsizei nameLength{ 0 };
        GLenum gltype{ 0 };

        glGetActiveAttrib(shader.id_, attributeIndex, nameBufferSize, &nameLength, &arraySize, &gltype, nameBuffer);
        OpenGLCheckError();
        location = glGetAttribLocation(shader.id_, nameBuffer);
        OpenGLCheckError();
        if (nameLength >= 0)
        {
            name = string_t(nameBuffer, (size_t)nameLength);
        }
        auto typeIter = TypeIdMap().find(gltype);
        if (typeIter != TypeIdMap().end())
        {
            type = typeIter->second;
        }
        else
        {
            TraceWarning("Unknown input attribute type 0x{:X}: {}", gltype, Print());
            return;
        }
    }

    Shader::AttributeInfo::AttributeInfo(AttributeInfo&& other) noexcept
        : name(std::move(other.name))
        , index(std::move(other.index))
        , location(std::move(other.location))
        , arraySize(std::move(other.arraySize))
        , type(std::move(other.type))
    {
    }

    Shader::UniformInfo::UniformInfo(const Shader& shader, unsigned uniformIndex)
        : name()
        , indexGiven((int)uniformIndex)
        , index(-2)
        , location(-2)
        , block(-2)
        , offset(-2)
        , arraySize()
        , type(DataType::Invalid)
    {
        GLsizei nameLength{ 0 };
        GLenum gltype{ 0 };

        glGetActiveUniform(shader.id_, uniformIndex, nameBufferSize, &nameLength, &arraySize, &gltype, nameBuffer);
        OpenGLCheckError();
        location = glGetUniformLocation(shader.id_, nameBuffer);
        OpenGLCheckError();
        if (nameLength >= 0)
        {
            name = string_t(nameBuffer, (size_t)nameLength);
        }
        auto typeIter = TypeIdMap().find(gltype);
        if (typeIter != TypeIdMap().end())
        {
            type = typeIter->second;
        }
        else
        {
            TraceWarning("Unknown uniform type 0x{:X}: {}", gltype, Print());
            return;
        }

        GLuint* index_ = reinterpret_cast<GLuint*>(&index);
        const GLchar* const nameAddr[1]{ nameBuffer };
        glGetUniformIndices(shader.id_, 1, nameAddr, index_);
        OpenGLCheckError();
        glGetActiveUniformsiv(shader.id_, 1, index_, GL_UNIFORM_OFFSET, &offset);
        OpenGLCheckError();
        glGetActiveUniformsiv(shader.id_, 1, index_, GL_UNIFORM_BLOCK_INDEX, &block);
        OpenGLCheckError();
    }

    Shader::UniformInfo::UniformInfo(UniformInfo&& other) noexcept
        : name(std::move(other.name))
        , indexGiven(std::move(other.indexGiven))
        , index(std::move(other.index))
        , location(std::move(other.location))
        , block(std::move(other.block))
        , offset(std::move(other.offset))
        , arraySize(std::move(other.arraySize))
        , type(std::move(other.type))
    {
    }

    Shader::UniformBlockInfo::UniformBlockInfo(const Shader& shader, unsigned uniformBlockIndex, const string_t& name_, BufferUsage usage_)
		: name(name_)
        , binding()
        , size()
        , index((int)uniformBlockIndex)
        , usage(usage_)
    {
        if (usage != BufferUsage::Output)
        {
            glGetActiveUniformBlockiv(shader.id_, uniformBlockIndex, GL_UNIFORM_BLOCK_BINDING, &binding);
            OpenGLCheckError();
            glGetActiveUniformBlockiv(shader.id_, uniformBlockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &size);
            OpenGLCheckError();
        }
    }

    Shader::UniformBlockInfoGlobal::UniformBlockInfoGlobal(const Shader& shader, unsigned uniformBlockIndex, const string_t& name_)
        : UniformBlockInfo(shader, uniformBlockIndex, name_, BufferUsage::Global)
        , offsets()
    {
        index = -1;
        binding = static_cast<int>(GlobalUBOs().size() + 1);
        assert(binding < (int)InstanceBindStart && "Too many global blocks! Maybe raise limit or check if a block is incorrectly marked for global usage.");

        // Get the number of uniforms in the block
        GLint numUniforms;
        glGetActiveUniformBlockiv(shader.id_, uniformBlockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &numUniforms);
        OpenGLCheckError();

        // Get the indices of the uniforms in the block
        vector_t<GLuint> uniformIndices(static_cast<size_t>(numUniforms));
        glGetActiveUniformBlockiv(shader.id_, uniformBlockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, reinterpret_cast<GLint*>(uniformIndices.data()));
        OpenGLCheckError();

        for (const auto& uniformIndex : uniformIndices)
        {
            GLsizei nameLength{ 0 };
            GLenum gltype{ 0 };
            GLint arraySize;

            glGetActiveUniform(shader.id_, uniformIndex, nameBufferSize, &nameLength, &arraySize, &gltype, nameBuffer);
            OpenGLCheckError();
            string_t uniformName{nameBuffer, (size_t)nameLength};
            offsets.emplace(uniformName, shader.uniforms_[shader.uniformsMap_.at(uniformName)].offset);
        }
    }

    Shader::UniformBlockInfoOutput::UniformBlockInfoOutput(const Shader& shader, unsigned uniformBlockIndex)
        : UniformBlockInfo(shader, uniformBlockIndex, "OUTPUT", BufferUsage::Output)
        , elements()
        , elementsMap()
        , count()
        , mode()
    {
        binding = 0;
        GLint bufferMode;
        glGetProgramiv(shader.id_, GL_TRANSFORM_FEEDBACK_BUFFER_MODE, &bufferMode);
        OpenGLCheckError();
        mode = bufferMode == GL_SEPARATE_ATTRIBS ? Mode::Separate : Mode::Interleaved;
        glGetProgramiv(shader.id_, GL_TRANSFORM_FEEDBACK_VARYINGS, reinterpret_cast<GLint*>(&count));
        OpenGLCheckError();
        GLint nameMaxLen;
        glGetProgramiv(shader.id_, GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH, &nameMaxLen);
        OpenGLCheckError();

        UpdateNameBuffer(nameMaxLen);

        for (unsigned i = 0; i < count; ++i)
        {
            GLsizei nameLength{ 0 };
            GLsizei varyingSize{ 0 };
            GLenum gltype{ 0 };

            glGetTransformFeedbackVarying(shader.id_, i, nameBufferSize, &nameLength, &varyingSize, &gltype, nameBuffer);
            OpenGLCheckError();
            string_t elementName = string_t(nameBuffer, (size_t)std::max(nameLength, decltype(nameLength)(0)));
            DataType type;
            auto typeIter = TypeIdMap().find(gltype);
            if (typeIter != TypeIdMap().end())
            {
                type = typeIter->second;
            }
            else
            {
                TraceWarning("Unknown uniform type 0x{:X}: {}", gltype, Print());
                type = DataType::Invalid;
            }
            elements.emplace_back(elementName, type, (int)i, (int)varyingSize);
            elementsMap.emplace(elementName, i);
        }
    }

	//--------------------------------------------------------------------------

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

#define PRINT_INDEX "#{:<3}"
#define PRINT_TYPE "{:<10}"
#define PRINT_NAME "Name: \"{}\""
#define PRINT_SEP " | "

    string_t Shader::AttributeInfo::Print(void) const
    {
        string_t typeStr;
        if (type == DataType::Invalid)
        {
            typeStr = "INVALID";
        }
        else
        {
            typeStr = ENUM_WRITE_VALUE(DataType, type);
        }
        if (arraySize != 1)
        {
            FORMAT_TO(typeStr, "[{}]", arraySize);
        }
        FORMAT_MAKE(strOut, "Attribute " PRINT_INDEX PRINT_SEP "Type: " PRINT_TYPE PRINT_SEP "Location:{:< 2}" PRINT_SEP PRINT_NAME, index, typeStr, location, name);
        return strOut;
    }

    string_t Shader::UniformInfo::Print(void) const
    {
        string_t typeStr;
        if (type == DataType::Invalid)
        {
            typeStr = "INVALID";
        }
        else
        {
            typeStr = ENUM_WRITE_VALUE(DataType, type);
        }
        if (arraySize != 1)
        {
            FORMAT_TO(typeStr, "[{}]", arraySize);
        }
        string_t offsetStr{};
        if (offset >= 0)
        {
            FORMAT_TO(offsetStr, " 0x{:X}", offset);
        }
        else
        {
            FORMAT_TO(offsetStr, "{}", offset);
        }
        FORMAT_MAKE(strOut, "Uniform " PRINT_INDEX PRINT_SEP "Type: " PRINT_TYPE PRINT_SEP "Block:{:< 2}" PRINT_SEP "Offset:{:<7}" PRINT_SEP "Location:{:< 2}" PRINT_SEP PRINT_NAME, index, typeStr, block, offsetStr, location, name);
        return strOut;
    }

    string_t Shader::UniformBlockInfo::Print(void) const
    {
        FORMAT_MAKE(strOut, "Uniform block " PRINT_INDEX PRINT_SEP "Bind:{:<3}" PRINT_SEP "Size 0x{:<4X}" PRINT_SEP "Usage: {:<8}" PRINT_SEP PRINT_NAME, index, binding, size, ENUM_WRITE_VALUE(BufferUsage, usage), name);
        return strOut;
    }

    string_t Shader::UniformBlockInfoOutput::Print(void) const
    {
        FORMAT_MAKE(output, "Output variables: {}", count);
        for (const auto& element : elements)
        {
            string_t typeStr;
            if (element.type == DataType::Invalid)
            {
                typeStr = "INVALID";
            }
            else
            {
                typeStr = ENUM_WRITE_VALUE(DataType, element.type);
            }
            if (element.size != 1)
            {
                std::format_to(std::back_inserter(typeStr), "[{}]", element.size);
            }
            std::format_to(std::back_inserter(output), "\nVariable " PRINT_INDEX PRINT_SEP "Type: " PRINT_TYPE PRINT_SEP PRINT_NAME, element.index, typeStr, element.name);
        }
        return output;
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

    void Shader::ParseShader()
    {
        assert(glIsProgram(id_) == GL_TRUE);
        ParseAttributes();
        ParseUniforms();
        ParseUniformBufferObjects();
        ParseOutputBuffers();
        // Remove the buffer
        MemoryManager::Deallocate(nameBuffer);
        nameBuffer = nullptr;
        nameBufferSize = 0;
    }

    void Shader::ParseAttributes()
    {
        GLint count;
        glGetProgramiv(id_, GL_ACTIVE_ATTRIBUTES, &count);
        OpenGLCheckError();

        GLint nameMaxLen;
        glGetProgramiv(id_, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &nameMaxLen);
        OpenGLCheckError();

        UpdateNameBuffer(nameMaxLen);

        TraceVerbose("Active Attribute Count: {}", count);

        inputAttrs_.reserve((size_t)count);

#ifdef TRACE_SHADER
        string_t traceAttributes = "Active Attributes:";
#endif

        for (GLuint i = 0; i < static_cast<GLuint>(count); ++i)
        {
            const auto& attr = inputAttrs_.emplace_back(*this, i);
#ifdef TRACE_SHADER
            FORMAT_TO(traceAttributes, "\n{}", attr.Print());
#endif
        }

#ifdef TRACE_SHADER
        TraceDebug("{}", traceAttributes);
#endif
    }

    void Shader::ParseUniforms()
    {
        GLint count;
        glGetProgramiv(id_, GL_ACTIVE_UNIFORMS, &count);
        OpenGLCheckError();

        GLint nameMaxLen;
        glGetProgramiv(id_, GL_ACTIVE_UNIFORM_MAX_LENGTH, &nameMaxLen);
        OpenGLCheckError();

        UpdateNameBuffer(nameMaxLen);

        TraceVerbose("Active Uniform Count: {}", count);

        uniforms_.reserve((size_t)count);

#ifdef TRACE_SHADER
        string_t traceUniforms = "Active Uniforms:";
#endif

        for (GLuint i = 0; i < static_cast<GLuint>(count); ++i)
        {
            unsigned index = (unsigned)uniforms_.size();
            auto& uniform = uniforms_.emplace_back(*this, i);
            uniformsMap_.try_emplace(uniform.name, index);
#ifdef TRACE_SHADER
            FORMAT_TO(traceUniforms, "\n{}", uniform.Print());
#endif
        }

#ifdef TRACE_SHADER
        TraceDebug("{}", traceUniforms);
#endif
    }

    void Shader::ParseUniformBufferObjects()
    {
        GLint count;
        glGetProgramiv(id_, GL_ACTIVE_UNIFORM_BLOCKS, &count);
        OpenGLCheckError();

        GLint nameMaxLen;
        glGetProgramiv(id_, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &nameMaxLen);
        OpenGLCheckError();

        UpdateNameBuffer(nameMaxLen);

        TraceVerbose("Active Uniform Buffer Object Count: {}", count);

#ifdef TRACE_SHADER
        string_t traceUniformBufferObjects = "Active Uniform Buffer Objects:";
#endif

        int bindOffset = 0;

        for (GLuint i = 0; i < static_cast<GLuint>(count); i++)
        {
            GLsizei nameLength{ 0 };

            glGetActiveUniformBlockName(id_, i, nameBufferSize, &nameLength, nameBuffer);
            OpenGLCheckError();

            string_t name{ nameBuffer, (unsigned)nameLength };

            // Find the usage set by pragma.
            auto usageIter = BufferUsages().find(name);
            // If the usage is not specified, assume Material.
            auto usage = usageIter == BufferUsages().end() ? BufferUsage::Material : usageIter->second;

            // Global blocks MUST be bound to the same location across shaders. If not, they would have to be rebound between different shaders.
            // Shader-specific blocks can be bound to any spot a global block CANNOT be bound.

            const UniformBlockInfo* uboi;
            if (usage != BufferUsage::Global)
            {
                glUniformBlockBinding(id_, i, static_cast<ResourceID>(uniformBlocks_.size() + InstanceBindStart));
                OpenGLCheckError();

                uboi = &uniformBlocks_.emplace_back(*this, i, name, usage);
                // Because global UBOs are skipped, fix the associated uniforms' block index.
                for (auto& uniformData : uniforms_)
                {
                    if (uniformData.block == static_cast<int>(i))
                    {
                        uniformData.block -= bindOffset;
                    }
                }
            }
            else
            {
                ++bindOffset;
                auto& ubo = GlobalUBOs().try_emplace(name, *this, i, name).first->second;
                uboi = &ubo;
                glUniformBlockBinding(id_, i, static_cast<ResourceID>(ubo.binding));
                OpenGLCheckError();
            }
            assert(uboi != nullptr);
#ifdef TRACE_SHADER
            FORMAT_TO(traceUniformBufferObjects, "\n{}", uboi->Print());
#endif
        }

#ifdef TRACE_SHADER
        TraceDebug("{}", traceUniformBufferObjects);
#endif
    }

    void Shader::ParseOutputBuffers()
    {
#ifdef TRACE_SHADER
        string_t traceUniformBufferObjects = "Active Output Buffers:";
#endif
        if (!outputNames_.empty())
        {
            const auto& outputBuffer = outputs_.emplace_back(*this, (unsigned)uniformBlocks_.size());
#ifdef TRACE_SHADER
            FORMAT_TO(traceUniformBufferObjects, "\n{}", outputBuffer.Print());
#endif
        }

#ifdef TRACE_SHADER
        TraceDebug("{}", traceUniformBufferObjects);
#endif
    }

#pragma endregion Private Functions

}	// namespace
