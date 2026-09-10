//------------------------------------------------------------------------------
//
// File Name:	MemoryAllocatorInfo.h
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

//------------------------------------------------------------------------------
// Preprocessor Directives:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar::Memory
{
    // Forward Declarations:

    // Typedefs:

    // Class Definition:
    template <typename T>
    struct AllocatorInfo
    {
        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    public:
        consteval AllocatorInfo(void);

        template <typename TOther>
        consteval AllocatorInfo(const AllocatorInfo<TOther>& other);

        // Public Static Functions:
    public:

        // Public Functions:
    public:
        // Gets the name of the allocated type.
        constexpr operator string_view_t(void) const;

        // Gets the ID of the allocated type.
        constexpr operator MemId_t(void) const;

        // Private Constants, Enums, and Structs:
    private:

        // Private Static Functions:
    private:

        // Private Functions:
    private:

        // Private Static Variables:
    private:

        // Public Variables:
    public:
        // The name of the allocated type.
        const char* name;
        // The length of the name of the allocated type.
        size_t nameLen;
        // The ID of the allocated type.
        MemId_t id;
    };

}	// namespace

#include "MemoryAllocatorInfo.inl"
