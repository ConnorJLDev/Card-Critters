//------------------------------------------------------------------------------
//
// File Name:	MemoryAllocator.h
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

#include "MemoryAllocatorInfo.h"

//------------------------------------------------------------------------------
// Preprocessor Directives:
//------------------------------------------------------------------------------

//#define MEMTRACK_ALLOCATOR_TYPE

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
    template <typename T = void, typename TSize = size_t
#ifdef MEMTRACK_ALLOCATOR_TYPE
        , AllocatorInfo<T> info_type = AllocatorInfo<T>()
#endif
    >
    struct Allocator
    {
        // Public Constants, Enums, and Structs:
    public:

        using value_type = T;
        using size_type = std::make_unsigned_t<TSize>;
        using difference_type = std::make_signed_t<TSize>;

        // Required rebind for NTTP
        template <typename U>
        struct rebind
        {
            using other = Allocator<U, TSize
#ifdef MEMTRACK_ALLOCATOR_TYPE
                , AllocatorInfo<U>(info_type)
#endif
            >;
        };

        // Constructors/Destructors:
    public:
        constexpr Allocator(void) noexcept;

#ifdef MEMTRACK_ALLOCATOR_TYPE
        template <typename TOther, AllocatorInfo<TOther> info_type_other>
        constexpr Allocator(const Allocator<TOther, TSize, info_type_other>& other) noexcept;
#else
        template <typename TOther>
        constexpr Allocator(const Allocator<TOther, TSize>& other) noexcept;
#endif

        // Public Static Functions:
    public:

        // Public Functions:
    public:
        /// <summary>
        /// Allocates memory for the requested number of elements.
        /// </summary>
        /// <param name="size">The number of elements to allocate.</param>
        /// <returns>A pointer to the newly allocated elements, or nullptr on failure.</returns>
        T* allocate(size_t size);

        /// <summary>
        /// Reallocates existing memory to contain the requested number of elements. Moves existing elements.
        /// </summary>
        /// <param name="ptr">The memory to reallocate.</param>
        /// <param name="size">The new number of elements to contain.</param>
        /// <returns>A pointer to the reallocated elements, or nullptr on failure.</returns>
        T* reallocate(T* ptr, size_t size);

        /// <summary>
        /// Deallocates existing memory.
        /// </summary>
        /// <param name="ptr">The memory to deallocate.</param>
        /// <param name="size">The number of elements being deallocated.</param>
        void deallocate(T* ptr, size_t size) noexcept;

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

    };

}	// namespace

#include "MemoryAllocator.inl"
