//------------------------------------------------------------------------------
//
// File Name:	FrameBuffer.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class for encapsulating a render target.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "FrameBuffer.h"

#include GRAPHICS_INCLUDE

#include "Texture.h"
#include "RenderBuffer.h"

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

	FrameBuffer::FrameBuffer()
		: NamedObject()
	{
	}

	//--------------------------------------------------------------------------

	FrameBuffer::~FrameBuffer(void)
	{
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

    void FrameBuffer::Activate()
    {
        // Bind textures.
        for (const auto& output : targets_)
        {
            if (output.isTexture)
            {
                glFramebufferTexture(GL_FRAMEBUFFER, output.target, output.texture->id_, 0);
            }
        }

        // Update the attachment vector DrawBuffers if it was modified.
        if (targetsDirty_)
        {
            UpdateInternalRenderBuffer();

            // GL_MAX_DRAW_BUFFERS

            targetAttachments_.clear();
            targetAttachments_.resize((size_t)GetRenderTargetMax() + 1, GL_NONE);

            // Iterate in reverse so resize operations are (ideally) less frequent.
            for (const auto& output : targets_)
            {
                // DrawBuffers only refers to colors.
                if (output.type != OutputType::Color)
                {
                    continue;
                }
                // Assign target.
                targetAttachments_[output.index] = output.target;
            }

            targetsDirty_ = false;
        }

        glDrawBuffers((GLsizei)targetAttachments_.size(), targetAttachments_.data());

        assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

        BindBuffers();
    }

    const UVec2& FrameBuffer::GetSize(void) const
    {
        return size_;
    }

    void FrameBuffer::SetSize(const UVec2& size)
    {
        size_ = size;
        // The effective size of the frame buffer is the smallest of each dimension, so each target needs to be resized in order to function correctly.
        for (const auto& target : targets_)
        {
            target.UpdateSize();
        }
        if (renderbuffer_ != nullptr)
        {
            renderbuffer_->UpdateSize();
        }
    }

    bool FrameBuffer::AddRenderTarget(OutputType type, Texture* texture, RenderTargetIndex_t index)
    {
        bool missing = !GetRenderTargetAssigned(type, index);
        if (missing)
        {
            if (texture == nullptr)
            {
                targets_.emplace_back(*this, new RenderBuffer(*this, type));
            }
            else
            {
                targets_.emplace_back(*this, texture);
            }
            targetsDirty_ = true;
        }
        return missing;
    }

    bool FrameBuffer::GetRenderTargetAssigned(OutputType type, RenderTargetIndex_t index) const
    {
        return FindRenderTargetConst(type, index) != targets_.cend();
    }

    Texture* FrameBuffer::GetRenderTargetTexture(OutputType type, RenderTargetIndex_t index) const
    {
        auto iter = FindRenderTargetConst(type, index);
        return iter != targets_.cend() ? (iter->isTexture ? iter->texture : nullptr) : nullptr;
    }

    RenderBuffer* FrameBuffer::GetRenderTargetBuffer(OutputType type, RenderTargetIndex_t index) const
    {
        auto iter = FindRenderTargetConst(type, index);
        return iter != targets_.cend() ? (iter->isTexture ? nullptr : iter->buffer) : nullptr;
    }

    bool FrameBuffer::RemoveRenderTarget(OutputType type, RenderTargetIndex_t index)
    {
        auto iter = FindRenderTargetConst(type, index);
        bool success = iter != targets_.cend();
        if (success)
        {
            targets_.erase(iter);
        }
        return success;
    }

    void FrameBuffer::ClearRenderTargets(void)
    {
        targets_.clear();
    }

    FrameBuffer::RenderTargetIndex_t FrameBuffer::GetRenderTargetCount(void) const
    {
        return (RenderTargetIndex_t)targets_.size();
    }

    FrameBuffer::RenderTargetIndex_t FrameBuffer::GetRenderTargetMax(void) const
    {
        RenderTargetIndex_t targetMax = 0;
        for (const auto& target : targets_)
        {
            if (target.type == OutputType::Color)
            {
                targetMax = std::max(targetMax, target.index);
            }
        }
        return targetMax;
    }

#pragma endregion Public Functions

	//--------------------------------------------------------------------------
	// Private Static Functions:
	//--------------------------------------------------------------------------

#pragma region Private Static Functions

    void FrameBuffer::UnbindBuffers()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, ResourceUnbind);
        OpenGLCheckError();
    }

#pragma endregion Private Static Functions

	//--------------------------------------------------------------------------
	// Private Functions:
	//--------------------------------------------------------------------------

#pragma region Private Functions

    void FrameBuffer::UpdateInternalRenderBuffer()
    {
        bool hasDepth = depthEnable_ ? std::find_if(targets_.cbegin(), targets_.cend(), [](const auto& output) { return output.type == OutputType::Depth; }) != targets_.cend() : false;
        bool hasStencil = stencilEnable_ ? std::find_if(targets_.cbegin(), targets_.cend(), [](const auto& output) { return output.type == OutputType::Stencil; }) != targets_.cend() : false;

        bool needsDepth = depthEnable_ && !hasDepth;
        bool needsStencil = stencilEnable_ && !hasStencil;

        SafeDeleteCheck(renderbuffer_);

        // Create non-output buffers.
        if (needsDepth || needsStencil)
        {
            OutputType outType;

            if (needsDepth && needsStencil)
            {
                outType = OutputType::DepthStencil;
            }
            else if (needsDepth)
            {
                outType = OutputType::Depth;
            }
            else if (needsStencil)
            {
                outType = OutputType::Stencil;
            }
            else
            {
                // How did we get here?
                assert(false);
                return;
            }

            renderbuffer_ = new RenderBuffer(*this, outType);

            renderbuffer_->CreateBuffers();
        }
    }

    FrameBuffer::RenderTargetContainerIter_t FrameBuffer::FindRenderTarget(OutputType type, RenderTargetIndex_t index)
    {
        return std::find_if(targets_.begin(), targets_.end(), [type, index](const RenderTarget& target)
        {
            return target.type == type && target.index == index;
        });
    }

    FrameBuffer::RenderTargetContainerConstIter_t FrameBuffer::FindRenderTargetConst(OutputType type, RenderTargetIndex_t index) const
    {
        return std::find_if(targets_.cbegin(), targets_.cend(), [type, index](const RenderTarget& target)
        {
            return target.type == type && target.index == index;
        });
    }

    void FrameBuffer::CreateBuffers()
    {
        if (DestroyBuffers())
        {
            TraceWarning("Recreating buffers for \"{}\"", GetName());
        }

        glGenFramebuffers(1, &framebufferID_);
        OpenGLCheckError();
    }

    bool FrameBuffer::DestroyBuffers()
    {
        bool existed = framebufferID_ != ResourceUnbind;
        if (existed)
        {
            glDeleteFramebuffers(1, &framebufferID_);
            OpenGLCheckError();
            framebufferID_ = ResourceUnbind;
        }
        return existed;
    }

    void FrameBuffer::BindBuffers() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, framebufferID_);
        OpenGLCheckError();
    }

    void FrameBuffer::BindBuffersRead() const
    {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, framebufferID_);
        OpenGLCheckError();
    }

    void FrameBuffer::BindBuffersWrite() const
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebufferID_);
        OpenGLCheckError();
    }

#pragma endregion Private Functions

}	// namespace
