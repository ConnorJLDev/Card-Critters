//------------------------------------------------------------------------------
//
// File Name:	ShaderBufferDescriptor.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class that describes the memory structure of a buffer that is
//              used with shader inputs and outputs.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "BufferDescriptor.h"

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
	class ShaderBufferDescriptor : public BufferDescriptor
	{
		// Public Constants, Enums, and Structs:
	public:

		// Constructors/Destructors:
	public:
        ShaderBufferDescriptor(unsigned index, string_view_arg name, BufferUsage usage);
        ShaderBufferDescriptor(const ShaderBufferDescriptor& other) = delete;
        ShaderBufferDescriptor(ShaderBufferDescriptor&& other) noexcept;
        ShaderBufferDescriptor& operator=(const ShaderBufferDescriptor& other) = delete;

        ~ShaderBufferDescriptor(void);

		// Public Static Functions:
	public:
        /// <summary>
        /// Creates a descriptor for the given shader input block.
        /// </summary>
        /// <param name="shader">The shader to parse.</param>
        /// <param name="inputBlockIndex">The index of the input block to parse.</param>
        /// <param name="name">The name of the block/descriptor.</param>
        /// <param name="usage">The usage of the block.</param>
        /// <returns>A descriptor for the requested input block.</returns>
        static ShaderBufferDescriptor ParseShaderInputBlock(const Shader& shader, unsigned inputBlockIndex, string_view_arg name, BufferUsage usage);

        /// <summary>
        /// Creates a descriptor for the given shader output block.
        /// </summary>
        /// <param name="shader">The shader to parse.</param>
        /// <param name="outputBlockIndex">The index of the output block to parse.</param>
        /// <param name="name">The name of the block/descriptor.</param>
        /// <param name="usage">The usage of the block.</param>
        /// <param name="elementIndex">The element index to start parsing from.</param>
        /// <param name="elementCountMax">The total number of elements that can be parsed (1 beyond the maximum valid index).</param>
        /// <returns>A descriptor for the requested output block.</returns>
        static ShaderBufferDescriptor ParseShaderOutputBlock(const Shader& shader, unsigned outputBlockIndex, string_view_arg name, BufferUsage usage, unsigned elementIndex = 0, unsigned elementCountMax = std::numeric_limits<unsigned>::max());

        /// <summary>
        /// Reserves the requested amount of characters for the name buffer.
        /// </summary>
        /// <param name="size">The amount of characters the buffer should be able to contain.</param>
        static void ReserveNameBuffer(size_t size);

		// Public Functions:
	public:

		// Private Constants, Enums, and Structs:
	private:

		// Private Static Functions:
	private:
        // Buffer for temporarily storing names from the graphics API.
        static string_t& NameBuffer(void);

        // Buffer for temporarily storing element indices from the graphics API.
        static vector_t<unsigned int>& ElementIndexBuffer(void);

		// Private Functions:
	private:

		// Private Static Variables:
	private:

		// Private Variables:
	private:
        // The index into the shader this is used, or -1 if global.
        int index_;
        // The binding point when used with a shader.
        int binding_;
	};

}	// namespace
