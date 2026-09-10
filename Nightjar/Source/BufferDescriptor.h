//------------------------------------------------------------------------------
//
// File Name:	BufferDescriptor.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class that describes the memory structure of a buffer that is
//              used with external interfaces.
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

#include "DataTypes.h"

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
	class BufferDescriptor : public NamedObject
	{
        friend class GraphicsBuffer;

		// Public Constants, Enums, and Structs:
	public:
        // Describes how often the buffer is changed.
        enum class BufferUsage : unsigned char
        {
            Invalid = (unsigned char)-1,
            Instance = 0,   // Data per instance, changed frequently (i.e. materials).
            Asset,          // Data per asset, usually only set once (by the asset).
            Global,         // Data used globally, usually only changed per frame/scene.
            NUM_ENTRIES,
        };

        // Describes how the buffer is used by the graphics API.
        // Descriptions based on: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBufferData.xhtml
        enum class BufferBehavior : unsigned char
        {
            Invalid = (unsigned char)-1,
            // Frequency
            _Stream     = 0 << 4,   // The buffer contents will be modified once and used at most a few times. Good for temporary or short-lived buffers.
            _Static     = 1 << 4,   // The buffer contents will be modified once and used many times. Good for buffers that are only set once.
            _Dynamic    = 2 << 4,   // The buffer contents will be modified repeatedly and used many times. Good for general purpose reading/writing buffers.
            // Nature
            _Draw       = 0 << 6,   // The buffer contents are modified by the application, and used as the source for GL drawing and image specification commands.
            _Read       = 1 << 6,   // The buffer contents are modified by reading data from the GL, and used to return that data when queried by the application.
            _Copy       = 2 << 6,   // The buffer contents are modified by reading data from the GL, and used as the source for GL drawing and image specification commands.
            // Actual values
            StreamDraw = 0, // Write few, read few, data transferred from CPU to GPU.
            StreamRead,     // Write few, read few, data transferred from GPU to CPU.
            StreamCopy,     // Write few, read few, data transferred within GPU.
            StaticDraw,     // Write few, read many, data transferred from CPU to GPU.
            StaticRead,     // Write few, read many, data transferred from GPU to CPU.
            StaticCopy,     // Write few, read many, data transferred within GPU.
            DynamicDraw,    // Write many, read many, data transferred from CPU to GPU.
            DynamicRead,    // Write many, read many, data transferred from GPU to CPU.
            DynamicCopy,    // Write many, read many, data transferred within GPU.
            NUM_ENTRIES,
        };

        // Describes a section of memory within a Buffer.
        struct Element
        {
            // Name of the element.
            string_t name;
            // Data type of the element.
            DataType type;
            // Index of this element when all buffer elements are sorted from the start of the buffer.
            unsigned index;
            // How many values are in this array (1 if not an array).
            unsigned arraySize;
            // Offset into the buffer, in bytes.
            unsigned offset;
        };

		// Constructors/Destructors:
	public:
        BufferDescriptor(string_view_arg name, ResourceEnum bind, size_t size, BufferUsage usage = BufferUsage::Instance, BufferBehavior behavior = BufferBehavior::DynamicCopy, unsigned short alignment = 1);
        BufferDescriptor(ResourceEnum bind, size_t size, BufferUsage usage = BufferUsage::Instance, BufferBehavior behavior = BufferBehavior::DynamicCopy, unsigned short alignment = 1);
        BufferDescriptor(const BufferDescriptor& other);
        BufferDescriptor(BufferDescriptor&& other) noexcept;
        BufferDescriptor& operator=(const BufferDescriptor& other);

        ~BufferDescriptor(void);

		// Public Static Functions:
	public:

		// Public Functions:
	public:
        /// <summary>
        /// Gets the element with the requested name.
        /// </summary>
        /// <param name="name">The name of the element to get.</param>
        /// <param name="element">A reference to a const Element* that is set to the element if found, or unchanged if not found.</param>
        /// <returns>True if the element was found, false otherwise.</returns>
        bool GetEntry(string_view_arg name, const Element*& element) const;

        /// <summary>
        /// Gets the offset of the element with the requested name.
        /// </summary>
        /// <param name="name">The name of the element to get the offset of.</param>
        /// <param name="offset">A reference to a size_t that has the offset added if found, or unchanged if not found.</param>
        /// <returns>True if the element was found, false otherwise.</returns>
        bool GetOffset(string_view_arg name, size_t& offset) const;

        /// <summary>
        /// Gets the offset of the element with the requested name with a given array index.
        /// </summary>
        /// <param name="name">The name of the element to get the offset of.</param>
        /// <param name="index">The array index of the element to get the offset of.</param>
        /// <param name="offset">A reference to a size_t that has the offset added if found, or unchanged if not found.</param>
        /// <returns>True if the element was found, false otherwise.</returns>
        bool GetOffset(string_view_arg name, size_t index, size_t& offset) const;

        /// <summary>
        /// Gets the number of unique elements described.
        /// </summary>
        /// <returns>The number of unique elements in this descriptor.</returns>
        size_t ElementCount(void) const;

		// Private Constants, Enums, and Structs:
	protected:
        // Internal array of BufferBehavior's ResourceEnum values.
        static const array_t<ResourceEnum, BufferBehavior::NUM_ENTRIES> BehaviorArray;

		// Private Static Functions:
    protected:

		// Private Functions:
    protected:
        /// <summary>
        /// Sets the size of the descriptor, accounting for padding.
        /// </summary>
        /// <param name="size">The new size to assign.</param>
        void SetSize(size_t size);

        /// <summary>
        /// Sets the alignment of the descriptor.
        /// </summary>
        /// <param name="alignment">The new alignment to assign.</param>
        void SetAlignment(unsigned short alignment);

		// Private Static Variables:
    protected:

		// Private Variables:
    protected:
        // Metadata for each element in this buffer.
        unordered_map_string_t<Element> elements_;
        // The internal category this buffer is used for (uniforms, vertex data, etc.).
        ResourceEnum bind_;
        // The expected usage context of this buffer.
        BufferUsage usage_;
        // The expected behavior of how this buffer will be written to/read from.
        BufferBehavior behavior_;
        // The alignment of the buffer.
        unsigned short alignment_;
        // The size of the buffer.
        size_t size_;
	};

}	// namespace
