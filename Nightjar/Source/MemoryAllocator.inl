//------------------------------------------------------------------------------
//
// File Name:	MemoryAllocator.inl
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

#include "MemoryAllocator.h"
#include "MemoryManager.h"

//------------------------------------------------------------------------------
// Preprocessor Directives:
//------------------------------------------------------------------------------

#pragma warning(push)
#pragma warning(disable : 4514)

#ifdef MEMTRACK_ALLOCATOR_TYPE
#define TMP_ALLOCATOR_TMP_A typename T, typename TSize, AllocatorInfo<T> info_type
#define TMP_ALLOCATOR_TMP_AB typename TOther, AllocatorInfo<TOther> info_type_other
#define TMP_ALLOCATOR_TMP_B typename TOther, typename TOtherSize, AllocatorInfo<TOther> info_type_other
#define TMP_ALLOCATOR_ARGS T, TSize, info_type
#define TMP_ALLOCATOR_ARGS_AB TOther, TSize, info_type_other
#define TMP_ALLOCATOR_ARGS_B TOther, TOtherSize, info_type_other
#else
#define TMP_ALLOCATOR_TMP_A typename T, typename TSize
#define TMP_ALLOCATOR_TMP_AB typename TOther
#define TMP_ALLOCATOR_TMP_B typename TOther, typename TOtherSize
#define TMP_ALLOCATOR_ARGS T, TSize
#define TMP_ALLOCATOR_ARGS_AB TOther, TSize
#define TMP_ALLOCATOR_ARGS_B TOther, TOtherSize
#endif

#define TMP_ALLOCATOR_TMP template<TMP_ALLOCATOR_TMP_A>

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar::Memory
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

    // Reference: https://stackoverflow.com/questions/76525067/how-to-write-a-custom-allocator-for-stdmap-in-c20

    TMP_ALLOCATOR_TMP
    constexpr Allocator<TMP_ALLOCATOR_ARGS>::Allocator(void) noexcept
    {
#ifndef MEMTRACK_ALLOCATOR_TYPE
        static constexpr AllocatorInfo<T> info_type{};
#endif
        MemoryManager::RegisterCategory(info_type.id, info_type);
    }

    TMP_ALLOCATOR_TMP
    template <TMP_ALLOCATOR_TMP_AB>
    constexpr Allocator<TMP_ALLOCATOR_ARGS>::Allocator(const Allocator<TMP_ALLOCATOR_ARGS_AB>&) noexcept
    {
#ifndef MEMTRACK_ALLOCATOR_TYPE
        static constexpr AllocatorInfo<TOther> info_type_other{};
#endif
        MemoryManager::RegisterCategory(info_type_other.id, info_type_other);
    }

    //--------------------------------------------------------------------------

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

    template <TMP_ALLOCATOR_TMP_A, TMP_ALLOCATOR_TMP_B>
    static bool operator==(const Allocator<TMP_ALLOCATOR_ARGS>&, const Allocator<TMP_ALLOCATOR_ARGS_B>&)
    {
        return true;
    }

    template <TMP_ALLOCATOR_TMP_A, TMP_ALLOCATOR_TMP_B>
    static bool operator!=(const Allocator<TMP_ALLOCATOR_ARGS>&, const Allocator<TMP_ALLOCATOR_ARGS_B>&)
    {
        return false;
    }

#pragma endregion Public Static Functions

    //--------------------------------------------------------------------------
    // Public Functions:
    //--------------------------------------------------------------------------

#pragma region Public Functions

    TMP_ALLOCATOR_TMP
    T* Allocator<TMP_ALLOCATOR_ARGS>::allocate(size_t size)
    {
#ifndef MEMTRACK_ALLOCATOR_TYPE
        static constexpr AllocatorInfo<T> info_type{};
#endif
        if constexpr (std::is_same_v<T, void>)
        {
            return static_cast<T*>(MemoryManager::Allocate(size, info_type.id));
        }
        else
        {
            size_t byteSize = size * sizeof(T);
            return static_cast<T*>(MemoryManager::Allocate(byteSize, info_type.id));
        }
    }

    TMP_ALLOCATOR_TMP
    T* Allocator<TMP_ALLOCATOR_ARGS>::reallocate(T* ptr, size_t size)
    {
#ifndef MEMTRACK_ALLOCATOR_TYPE
        static constexpr AllocatorInfo<T> info_type{};
#endif
        if constexpr (std::is_same_v<T, void>)
        {
            return MemoryManager::Reallocate(ptr, size, info_type.id);
        }
        else
        {
            size_t byteSize = size * sizeof(T);
            return MemoryManager::Reallocate(ptr, byteSize, info_type.id);
        }
    }

    TMP_ALLOCATOR_TMP
    void Allocator<TMP_ALLOCATOR_ARGS>::deallocate(T* ptr, size_t size) noexcept
    {
#ifndef MEMTRACK_ALLOCATOR_TYPE
        static constexpr AllocatorInfo<T> info_type{};
#endif
        if constexpr (std::is_same_v<T, void>)
        {
            MemoryManager::Deallocate(ptr, size, info_type.id);
        }
        else
        {
            size_t byteSize = size * sizeof(T);
            MemoryManager::Deallocate(ptr, byteSize, info_type.id);
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

#pragma warning(pop)
