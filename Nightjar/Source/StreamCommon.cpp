//------------------------------------------------------------------------------
//
// File Name:	StreamCommon.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Custom allocator for rapidjson documents.
//
// Copyright © 2026 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "StreamCommon.h"

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

    constexpr bool StreamAllocator::kNeedFree = true;

    //--------------------------------------------------------------------------
    // Public Static Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Public Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Private Constants:
    //--------------------------------------------------------------------------

    constexpr string_view_t StreamAllocator::memName = "STREAM_ALLOCATOR";
    constexpr size_t StreamAllocator::memId = MEM_ID(memName);

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

    StreamAllocator::StreamAllocator()
    {
    }

    //--------------------------------------------------------------------------

    StreamAllocator::~StreamAllocator(void)
    {
    }

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

    void StreamAllocator::Free(void* ptr) RAPIDJSON_NOEXCEPT
    {
        MemoryManager::Deallocate(ptr, memId);
    }

#pragma endregion Public Static Functions

    //--------------------------------------------------------------------------
    // Public Functions:
    //--------------------------------------------------------------------------

#pragma region Public Functions

    bool StreamAllocator::operator==(const StreamAllocator&) const RAPIDJSON_NOEXCEPT
    {
        return true;
    }

    bool StreamAllocator::operator!=(const StreamAllocator&) const RAPIDJSON_NOEXCEPT
    {
        return false;
    }

    void* StreamAllocator::Malloc(size_t size)
    {
        return size == 0 ? nullptr : MemoryManager::Allocate(size, memId);
    }

    void* StreamAllocator::Realloc(void* originalPtr, size_t, size_t newSize)
    {
        return MemoryManager::ReallocateNoexcept(originalPtr, newSize, memId);
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
