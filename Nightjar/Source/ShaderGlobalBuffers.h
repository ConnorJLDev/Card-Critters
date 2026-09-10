//------------------------------------------------------------------------------
//
// File Name:	ShaderGlobalBuffers.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class that handles storage and access of global shader buffers.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ShaderBufferDescriptor.h"
#include "GraphicsBuffer.h"

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
	class ShaderGlobalBuffers
	{
		// Public Constants, Enums, and Structs:
	public:

		// Constructors/Destructors:
	public:
		ShaderGlobalBuffers(void) = delete;

		// Public Static Functions:
	public:
        /// <summary>
        /// Gets the global buffer with the requested name.
        /// </summary>
        /// <param name="name">The name of the buffer to get.</param>
        /// <returns>A pointer to the buffer, or nullptr if not found.</returns>
        static GraphicsBuffer* GetBuffer(string_view_arg name);

        /// <summary>
        /// Adds a new global buffer.
        /// </summary>
        /// <param name="descriptor">The descriptor of the new global buffer. Must be moved.</param>
        static void AddDescriptor(ShaderBufferDescriptor&& descriptor);

		// Private Constants, Enums, and Structs:
	private:
        // Contains each global buffer and its associated descriptor.
        struct GlobalBuffer
        {
            GlobalBuffer(void) = delete;
            GlobalBuffer(ShaderBufferDescriptor&& descriptor);
            GlobalBuffer(const GlobalBuffer& other) = delete;
            GlobalBuffer(GlobalBuffer&& other) noexcept;
            GlobalBuffer& operator=(const GlobalBuffer& other) = delete;
            ~GlobalBuffer(void);

            ShaderBufferDescriptor descriptor;
            GraphicsBuffer buffer;
        };

		// Private Static Functions:
	private:
        // The internal container for global buffers.
        static unordered_map_string_t<GlobalBuffer>& GlobalBuffers(void);

		// Private Static Variables:
	private:

	};

}	// namespace
