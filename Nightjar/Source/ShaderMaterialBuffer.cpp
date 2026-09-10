//------------------------------------------------------------------------------
//
// File Name:	ShaderMaterialBuffer.cpp
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

    Shader::MaterialBuffers::MaterialBuffers(const Shader& shader)
        : uniforms_()
        , uniformsAvailable_()
        , uniformsUsed_(0)
        , shader_(shader)
    {
    }

	//--------------------------------------------------------------------------

    Shader::MaterialBuffers::~MaterialBuffers()
    {
        if (uniforms_.size() > 0)
        {
            glDeleteBuffers(static_cast<GLsizei>(uniforms_.size()), uniforms_.data());
            OpenGLCheckError();
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

    int Shader::MaterialBuffers::GetMaterialBuffers()
    {
        // First, search for an available window.
        const size_t bufferCount = shader_.bufferCount_;
        for (unsigned i = 0; i < uniformsUsed_ / bufferCount; ++i)
        {
            if (uniformsAvailable_[i])
            {
                uniformsAvailable_[i] = false;
                return static_cast<int>(i * bufferCount);
            }
        }
        // If an available window wasn't found and we need more windows, generate them.
        if (uniformsUsed_ + bufferCount > uniforms_.size())
        {
            uniforms_.resize(uniformsUsed_ + bufferCount);
            uniformsAvailable_.resize(uniforms_.size() / bufferCount);
            glGenBuffers(static_cast<GLsizei>(bufferCount), &uniforms_[uniformsUsed_]);
            OpenGLCheckError();

            unsigned index = uniformsUsed_;
            for (const auto& block : shader_.uniformBlocks_)
            {
                // Output buffers need to be initialized per usage, so they are not initialized here.
                if (block.usage != BufferUsage::Output)
                {
                    glBindBuffer(GL_UNIFORM_BUFFER, uniforms_[index]);
                    OpenGLCheckError();
                    glBufferData(GL_UNIFORM_BUFFER, block.size, nullptr, GL_DYNAMIC_COPY); // GL_DYNAMIC_DRAW GL_DYNAMIC_COPY
                    OpenGLCheckError();
                }
                ++index;
            }

            glBindBuffer(GL_UNIFORM_BUFFER, ResourceUnbind);
            OpenGLCheckError();
        }
        // Mark the end window as unavailable.
        assert(uniformsUsed_ % bufferCount == 0);
        uniformsAvailable_[uniformsUsed_ / bufferCount] = false;
        int resource = static_cast<int>(uniformsUsed_);
        uniformsUsed_ += static_cast<unsigned>(bufferCount);
        return resource;
    }

    void Shader::MaterialBuffers::RemoveMaterialBuffers(int buffersIndex)
    {
        const size_t bufferCount = shader_.bufferCount_;
        assert(buffersIndex % bufferCount == 0 && (unsigned)buffersIndex < uniformsUsed_);
        uniformsAvailable_[buffersIndex / bufferCount] = true;
    }

    void Shader::MaterialBuffers::SetOutputSize(int buffersIndex, size_t size)
    {
        if (!shader_.HasOutput())
        {
            TraceWarning("Tried to set output size on shader \"{}\" that lacks output!", shader_.GetName());
            return;
        }
        auto& outputBuffer = shader_.outputs_[0];
        glBindBuffer(GL_ARRAY_BUFFER, uniforms_[(unsigned)buffersIndex + (unsigned)outputBuffer.index]);
        OpenGLCheckError();
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(size), nullptr, GL_DYNAMIC_COPY); // GL_STATIC_READ GL_DYNAMIC_COPY GL_STREAM_READ
        OpenGLCheckError();
        glBindBuffer(GL_ARRAY_BUFFER, ResourceUnbind);
        OpenGLCheckError();
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

#pragma endregion Private Functions

}	// namespace
