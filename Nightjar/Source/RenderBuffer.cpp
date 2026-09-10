//------------------------------------------------------------------------------
//
// File Name:	RenderBuffer.cpp
// Author(s):	JSand
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Template class for a new object.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "RenderBuffer.h"

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

    RenderBuffer::RenderBuffer(FrameBuffer& renderer, FrameBuffer::OutputType type, uint8_t sampleCount)
        : renderer_(renderer)
        , renderbufferID_(ResourceUnbind)
        , type_(type)
        , index_((decltype(index_))-1)
        , colorIndex_(0)
        , sampleCount_(sampleCount)
    {
    }

    RenderBuffer::RenderBuffer(RenderBuffer&& other) noexcept
        : renderer_(other.renderer_)
        , renderbufferID_(std::exchange(other.renderbufferID_, ResourceUnbind))
        , type_(std::move(other.type_))
        , index_(std::move(other.index_))
        , colorIndex_(std::move(other.colorIndex_))
        , sampleCount_(std::move(other.sampleCount_))
    {
    }

    //--------------------------------------------------------------------------

    RenderBuffer::~RenderBuffer(void)
    {
        DestroyBuffers();
    }

#pragma endregion Constructors

	//--------------------------------------------------------------------------
	// Public Static Functions:
	//--------------------------------------------------------------------------

#pragma region Public Static Functions

    void RenderBuffer::UnbindBuffers()
    {
        glBindRenderbuffer(GL_RENDERBUFFER, ResourceUnbind);
        OpenGLCheckError();
    }

#pragma endregion Public Static Functions

	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

    ResourceEnum RenderBuffer::GetAttachmentName(void) const
    {
        switch (type_)
        {
        case FrameBuffer::OutputType::Color:
            return GL_COLOR_ATTACHMENT0 + (ResourceEnum)colorIndex_;
        case FrameBuffer::OutputType::Depth:
            return GL_DEPTH_ATTACHMENT;
        case FrameBuffer::OutputType::Stencil:
            return GL_STENCIL_ATTACHMENT;
        case FrameBuffer::OutputType::DepthStencil:
            return GL_DEPTH_STENCIL_ATTACHMENT;
        default:
            assert(false);
            return 0;
        }
    }

    void RenderBuffer::UpdateSize(void)
    {
        DestroyBuffers();
        CreateBuffers();
    }

    void RenderBuffer::CreateBuffers()
    {
        glGenRenderbuffers(1, &renderbufferID_);
        OpenGLCheckError();

        ResourceEnum storageType;

        switch (type_)
        {
        case FrameBuffer::OutputType::Color:
            storageType = GL_COLOR;
            break;
        case FrameBuffer::OutputType::Depth:
            storageType = GL_DEPTH_COMPONENT;
            break;
        case FrameBuffer::OutputType::Stencil:
            storageType = GL_STENCIL;
            break;
        case FrameBuffer::OutputType::DepthStencil:
            storageType = GL_DEPTH_STENCIL;
            break;
        default:
            assert(false);
            return;
        }

        BindBuffers();

        glRenderbufferStorageMultisample(GL_RENDERBUFFER, (GLsizei)sampleCount_, storageType, (GLsizei)renderer_.size_.x, (GLsizei)renderer_.size_.y);
        OpenGLCheckError();

        renderer_.BindBuffers();

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GetAttachmentName(), GL_RENDERBUFFER, renderbufferID_);
        OpenGLCheckError();

        FrameBuffer::UnbindBuffers();
        UnbindBuffers();
    }

    bool RenderBuffer::DestroyBuffers()
    {
        bool existed = renderbufferID_ != ResourceUnbind;
        if (existed)
        {
            // Unbind this renderbuffer from the framebuffer.
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GetAttachmentName(), GL_RENDERBUFFER, ResourceUnbind);
            OpenGLCheckError();
            // Then delete the renderbuffer.
            glDeleteRenderbuffers(1, &renderbufferID_);
            OpenGLCheckError();
            renderbufferID_ = ResourceUnbind;
        }
        return existed;
    }

    void RenderBuffer::BindBuffers() const
    {
        glBindRenderbuffer(GL_RENDERBUFFER, renderbufferID_);
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
