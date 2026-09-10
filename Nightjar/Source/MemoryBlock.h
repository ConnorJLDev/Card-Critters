//------------------------------------------------------------------------------
//
// File Name:	MemoryBlock.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Struct that represents an allocated or unallocated segment of
//              memory within a MemoryPage.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

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
    using BlockSize_t = size_t /*uint32_t*/;

    // Struct Definition:
    struct MemoryBlock
    {
        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    public:
        MemoryBlock(void);
        MemoryBlock(BlockSize_t offset, BlockSize_t size, BlockSize_t alignPad, BlockSize_t headerOffset);

        // Public Static Functions:
    public:

        // Public Functions:
    public:

        // Public Event Handlers
    public:

        // Public Variables:
    public:
        // Offset into the page of the memory block. For interleaved headers, the header is guaranteed to be headerInterleavedBlockOffset_ bytes before this offset.
        BlockSize_t offset;
        // Number of bytes available for the requested allocation after accounting for alignment padding and other potential requirements.
        BlockSize_t size;
        // Number of alignment bytes required at the start for the requested allocation. For interleaved headers, this is guaranteed to be a multiple of headerAlignment_.
        BlockSize_t alignPad;
        // Number of alignment bytes required at the start for the requested allocation. For interleaved headers, this is guaranteed to be a multiple of headerAlignment_.
        BlockSize_t headerOffset;

        // Private Constants, Enums, and Structs:
    private:

        // Private Static Functions:
    private:

        // Private Functions:
    private:

        // Private Static Variables:
    private:

        // Private Variables:
    private:
    };

}	// namespace
