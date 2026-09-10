//------------------------------------------------------------------------------
//
// File Name:	RenderTarget.cpp
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
#include "RenderBuffer.h"

#include GRAPHICS_INCLUDE

#include "Texture.h"

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

    FrameBuffer::RenderTarget::RenderTarget(FrameBuffer& renderer_, Texture* texture_)
        : renderer(renderer_)
        , texture(texture_)
        , isTexture(true)
        , index()
        , type()
        , target()
    {
        if (texture->GetDimensions() != renderer.GetSize())
        {
            UpdateSize();
        }
    }

    FrameBuffer::RenderTarget::RenderTarget(FrameBuffer& renderer_, RenderBuffer* buffer_)
        : renderer(renderer_)
        , buffer(buffer_)
        , isTexture(false)
        , index()
        , type()
        , target()
    {
        // Render buffers are automatically resized when created.
    }

    FrameBuffer::RenderTarget::RenderTarget(RenderTarget&& other) noexcept
        : renderer(other.renderer)
        , buffer(std::exchange(other.buffer, nullptr))
        , isTexture(other.isTexture)
        , index(std::move(other.index))
        , type(std::move(other.type))
        , target(std::move(other.target))
    {
    }

    FrameBuffer::RenderTarget& FrameBuffer::RenderTarget::operator=(const RenderTarget& other)
    {
        assert(&renderer == &other.renderer && "RenderTarget assignment must be between targets of the same FrameBuffer!");
        buffer = other.buffer;
        isTexture = other.isTexture;
        index = other.index;
        type = other.type;
        target = other.target;
        return *this;
    }

	//--------------------------------------------------------------------------

    FrameBuffer::RenderTarget::~RenderTarget()
    {
        if (!isTexture && buffer != nullptr)
        {
            SafeDelete(buffer);
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

    void FrameBuffer::RenderTarget::UpdateSize(void) const
    {
        if (isTexture)
        {
            texture->SetDimensions(renderer.GetSize());
        }
        else
        {
            buffer->UpdateSize();
        }
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
