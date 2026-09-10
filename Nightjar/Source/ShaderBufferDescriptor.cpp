//------------------------------------------------------------------------------
//
// File Name:	ShaderBufferDescriptor.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class that describes the memory structure of a buffer that is
//              used with shader inputs and outputs.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ShaderBufferDescriptor.h"

#include GRAPHICS_INCLUDE

#include "Shader.h"

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

    ShaderBufferDescriptor::ShaderBufferDescriptor(unsigned index, string_view_arg name, BufferUsage usage)
        : BufferDescriptor(name, GL_UNIFORM_BUFFER, 0u, usage)
        , index_((int)index)
        , binding_()
    {
    }

    ShaderBufferDescriptor::ShaderBufferDescriptor(ShaderBufferDescriptor&& other) noexcept
        : BufferDescriptor(std::move(other))
        , index_(std::move(other.index_))
        , binding_(std::move(other.binding_))
    {
    }

	//--------------------------------------------------------------------------

	ShaderBufferDescriptor::~ShaderBufferDescriptor(void)
	{
	}

#pragma endregion Constructors

	//--------------------------------------------------------------------------
	// Public Static Functions:
	//--------------------------------------------------------------------------

#pragma region Public Static Functions

    ShaderBufferDescriptor ShaderBufferDescriptor::ParseShaderInputBlock(const Shader& shader, unsigned inputBlockIndex, string_view_arg name, BufferUsage usage)
    {
        static string_t& nameBuffer(NameBuffer());
        static vector_t<GLuint>& uniformIndexBuffer(ElementIndexBuffer());
        static auto& typeIdMap(Shader::TypeIdMap());

        ShaderBufferDescriptor buffer(inputBlockIndex, name, usage);

        ResourceID shaderID = shader.id_;

        {
            // Get the size of the block.
            GLint glSize;
            glGetActiveUniformBlockiv(shaderID, inputBlockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &glSize);
            OpenGLCheckError();
            buffer.SetSize(static_cast<size_t>(glSize));
        }

        {
            // Get the binding index of the block.
            GLint glBinding;
            glGetActiveUniformBlockiv(shaderID, inputBlockIndex, GL_UNIFORM_BLOCK_BINDING, &glBinding);
            OpenGLCheckError();
            buffer.binding_ = static_cast<decltype(binding_)>(glBinding);
        }

        {
            // Get the number of uniforms in the block
            GLint glNumUniforms;
            glGetActiveUniformBlockiv(shaderID, inputBlockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &glNumUniforms);
            OpenGLCheckError();
            const size_t numUniforms(static_cast<size_t>(glNumUniforms));

            // Get the indices of the uniforms in the block
            uniformIndexBuffer.reserve(std::bit_ceil(numUniforms));
            uniformIndexBuffer.resize(numUniforms);
            glGetActiveUniformBlockiv(shaderID, inputBlockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, reinterpret_cast<GLint*>(uniformIndexBuffer.data()));
            OpenGLCheckError();
        }

        unsigned i = 0;
        for (const GLuint uniformIndex : uniformIndexBuffer)
        {
            GLsizei nameLength(0);
            GLenum glType;
            GLint arraySize;

            glGetActiveUniform(shaderID, uniformIndex, static_cast<GLsizei>(nameBuffer.capacity()), &nameLength, &arraySize, &glType, reinterpret_cast<GLchar*>(nameBuffer.data()));
            OpenGLCheckError();
            string_view_t elementName(nameBuffer.c_str(), static_cast<size_t>(nameLength));
            if (elementName.empty())
            {
                break;
            }
            GLint glOffset;
            {
                //const GLuint* index_ = reinterpret_cast<const GLuint*>(&uniformIndex);
                //glGetActiveUniformsiv(shaderID, 1, index_, GL_UNIFORM_OFFSET, &glOffset);
                glGetActiveUniformsiv(shaderID, 1, &uniformIndex, GL_UNIFORM_OFFSET, &glOffset);
                OpenGLCheckError();
                assert(glOffset >= 0);
            }
            auto typeIter = typeIdMap.find(glType);
            DataType type(typeIter != typeIdMap.end() ? typeIter->second : DataType::Invalid);
            buffer.elements_.emplace(std::piecewise_construct,
                std::forward_as_tuple(elementName),
                std::forward_as_tuple(string_t(elementName), type, i, (unsigned)arraySize, (unsigned)glOffset)
            );
            ++i;
        }

        return buffer;
    }

    ShaderBufferDescriptor ShaderBufferDescriptor::ParseShaderOutputBlock(const Shader& shader, unsigned outputBlockIndex, string_view_arg name, BufferUsage usage, unsigned elementIndex, unsigned elementCount)
    {
        static string_t& nameBuffer(NameBuffer());
        static auto& typeIdMap(Shader::TypeIdMap());

        ResourceID shaderID = shader.id_;

        ShaderBufferDescriptor buffer(outputBlockIndex, name, usage);

        unsigned glOffset = 0;
        for (unsigned i = elementIndex; i < elementCount; ++i)
        {
            GLsizei nameLength(0);
            GLsizei varyingSize;
            GLenum gltype;

            glGetTransformFeedbackVarying(shaderID, i, static_cast<GLsizei>(nameBuffer.capacity()), &nameLength, &varyingSize, &gltype, reinterpret_cast<GLchar*>(nameBuffer.data()));
            OpenGLCheckError();
            string_view_t elementName(nameBuffer.c_str(), static_cast<size_t>(nameLength));
            if (elementName.empty() || elementName == "gl_NextBuffer")
            {
                break;
            }
            auto typeIter = typeIdMap.find(gltype);
            DataType type(typeIter != typeIdMap.end() ? typeIter->second : DataType::Invalid);
            buffer.elements_.emplace(std::piecewise_construct,
                std::forward_as_tuple(elementName),
                std::forward_as_tuple(string_t(elementName), type, i, 1u, glOffset)
            );
            glOffset += varyingSize;
        }

        return buffer;
    }

    void ShaderBufferDescriptor::ReserveNameBuffer(size_t size)
    {
        static string_t& nameBuffer(NameBuffer());
        static_assert(std::is_same_v<std::decay_t<decltype(nameBuffer)>::value_type, GLchar>, "Name buffer and GLchar type mismatch!");
        nameBuffer.reserve(std::bit_ceil(size));
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

    STATIC_STRING_CLASS(ShaderBufferDescriptor, NameBuffer)

    STATIC_VECTOR_CLASS((GLuint), ShaderBufferDescriptor, ElementIndexBuffer)

#pragma endregion Private Static Functions

	//--------------------------------------------------------------------------
	// Private Functions:
	//--------------------------------------------------------------------------

#pragma region Private Functions

#pragma endregion Private Functions

}	// namespace
