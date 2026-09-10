//------------------------------------------------------------------------------
//
// File Name:	RenderBuffer.h
// Author(s):	JSand
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Template class for a new object.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "GraphicsSystem.h"

#include "FrameBuffer.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	// Forward Declarations:

	// Typedefs:

	// Class Definition:
	class RenderBuffer
	{
        friend FrameBuffer;

		// Public Constants, Enums, and Structs:
	public:

		// Constructors/Destructors:
	public:
        RenderBuffer(const RenderBuffer& other) = delete;
        RenderBuffer(FrameBuffer& renderer_, FrameBuffer::OutputType type, uint8_t sampleCount = 0);
        RenderBuffer(RenderBuffer&& other) noexcept;
        RenderBuffer& operator=(const RenderBuffer& other) = delete;

        ~RenderBuffer(void);

		// Public Static Functions:
	public:

		// Public Functions:
	public:
        //
        ResourceEnum GetAttachmentName(void) const;

		// Public Event Handlers
	public:

		// Private Constants, Enums, and Structs:
	private:

		// Private Static Functions:
	private:
        //
        static void UnbindBuffers();

		// Private Functions:
	private:
        //
        void UpdateSize(void);

        //
        void CreateBuffers();

        //
        bool DestroyBuffers();

        //
        void BindBuffers() const;

		// Private Static Variables:
	private:

		// Private Variables:
	private:
        FrameBuffer& renderer_;
        ResourceID renderbufferID_;
        FrameBuffer::OutputType type_;
        FrameBuffer::RenderTargetIndex_t index_;
        FrameBuffer::RenderTargetIndex_t colorIndex_;
        uint8_t sampleCount_;
	};

}	// namespace
