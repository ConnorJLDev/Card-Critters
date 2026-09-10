//------------------------------------------------------------------------------
//
// File Name:	MemoryPageAllocator.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class that manages multiple MemoryPages as a method of
//              categorization and grouping for related allocations.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "MemoryPage.h"

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
    class MemoryPageAllocator
    {
        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    public:
        MemoryPageAllocator(void);
        MemoryPageAllocator(const MemoryPageAllocator& other) = delete;
        MemoryPageAllocator(MemoryPageAllocator&& other) noexcept;
        MemoryPageAllocator& operator=(const MemoryPageAllocator& other) = delete;

        ~MemoryPageAllocator(void);

        // Public Static Functions:
    public:

        // Public Functions:
    public:

        // Public Event Handlers
    public:

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
        // 
        MemoryPage metaPage_;
    };

}	// namespace
