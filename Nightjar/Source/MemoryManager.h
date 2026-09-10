//------------------------------------------------------------------------------
//
// File Name:	MemoryManager.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class for monitoring and managing memory.
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

// Convenience macro for generating a unique ID from a string.
#define MEM_ID(value) (static_cast<Nightjar::MemId_t>(Nightjar::Utility::fnv1a_hash(value)))

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
    typedef size_t MemId_t; // Type representing a category/grouping of allocations.

    // Class Definition:
    class MemoryManager
    {
        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    public:
        MemoryManager(void) = delete;

        // Public Static Functions:
    public:
        /// <summary>
        /// Initializes the MemoryManager.
        /// </summary>
        /// <returns>True if initialization was successful, false otherwise.</returns>
        static bool Init(void);
        
        /// <summary>
        /// Shuts down the MemoryManager.
        /// </summary>
        static void Shutdown(void);

        /// <summary>
        /// Prints a message to console about the current state of the MemoryManager.
        /// </summary>
        static void Log(void);

        /// <summary>
        /// Explicitly allocate memory. Throws std::bad_alloc on failure.
        /// </summary>
        /// <param name="size">The amount of memory to allocate.</param>
        /// <param name="id">Optional parameter for categorizing the allocation.</param>
        /// <returns>A pointer to the allocated memory.</returns>
        static void* Allocate(size_t size, MemId_t id = 0);

        /// <summary>
        /// Explicitly allocate memory. No exception is thrown on failure.
        /// </summary>
        /// <param name="size">The amount of memory to allocate.</param>
        /// <param name="id">Optional parameter for categorizing the allocation.</param>
        /// <returns>A pointer to the allocated memory, or nullptr on failure.</returns>
        static void* AllocateNoexcept(size_t size, MemId_t id = 0) noexcept;

        /// <summary>
        /// Explicitly reallocate memory. Throws std::bad_alloc on failure.
        /// On success, the data from the original address is copied into the new
        /// address, and the old pointer is freed.
        /// </summary>
        /// <param name="original">The pointer to the original memory.</param>
        /// <param name="size">The new amount of memory to reallocate.</param>
        /// <param name="id">Optional parameter for categorizing the reallocation.</param>
        /// <returns>A pointer to the reallocated memory.</returns>
        static void* Reallocate(void* original, size_t size, MemId_t id = 0);

        /// <summary>
        /// Explicitly reallocate memory. No exception is thrown on failure.
        /// On success, the data from the original address is copied into the new
        /// address, and the old pointer is freed.
        /// </summary>
        /// <param name="original">The pointer to the original memory to be reallocated.</param>
        /// <param name="size">The new amount of memory to reallocate.</param>
        /// <param name="id">Optional parameter for categorizing the reallocation.</param>
        /// <returns>A pointer to the reallocated memory, or nullptr on failure.</returns>
        static void* ReallocateNoexcept(void* original, size_t size, MemId_t id = 0) noexcept;

        /// <summary>
        /// Explicitly deallocate memory.
        /// </summary>
        /// <param name="original">The pointer to the memory to be deallocated.</param>
        /// <param name="id">Optional parameter for categorizing the deallocation.</param>
        static void Deallocate(void* original, MemId_t id = 0) noexcept;

        /// <summary>
        /// Explicitly deallocate memory.
        /// </summary>
        /// <param name="original">The pointer to the memory to be deallocated.</param>
        /// <param name="size">Optional parameter for the size of the deallocation.</param>
        /// <param name="id">Optional parameter for categorizing the deallocation.</param>
        static void Deallocate(void* original, size_t size, MemId_t id) noexcept;

        /// <summary>
        /// Copies memory from the source address to the destination address.
        /// </summary>
        /// <param name="destination">Whither: The address to copy data to.</param>
        /// <param name="destinationSize">The maximum amount of data allowed to be written, in bytes.</param>
        /// <param name="source">Whence: The address to copy data from.</param>
        /// <param name="sourceSize">Quantity: The amount of data to copy, in bytes.</param>
        static void Copy(void* destination, size_t destinationSize, const void* source, size_t sourceSize);

        /// <summary>
        /// Copies memory from the source address to the destination address.
        /// </summary>
        /// <param name="destination">Whither: The address to copy data to.</param>
        /// <param name="source">Whence: The address to copy data from.</param>
        /// <param name="size">Quantity: The amount of data to copy, in bytes.</param>
        static void Copy(void* destination, const void* source, size_t size);

        /// <summary>
        /// Moves data from the source address to the destination address. 
        /// </summary>
        /// <param name="destination">Whither: The address to move data to.</param>
        /// <param name="destinationSize">The maximum amount of data allowed to be written, in bytes.</param>
        /// <param name="source">Whence: The address to move data from.</param>
        /// <param name="sourceSize">Quantity: The amount of data to move, in bytes.</param>
        static void Move(void* destination, size_t destinationSize, const void* source, size_t sourceSize);

        /// <summary>
        /// Moves data from the source address to the destination address. 
        /// </summary>
        /// <param name="destination">Whither: The address to move data to.</param>
        /// <param name="source">Whence: The address to move data from.</param>
        /// <param name="size">Quantity: The amount of data to move, in bytes.</param>
        static void Move(void* destination, const void* source, size_t size);

        /// <summary>
        /// Registers a name to a given category id.
        /// </summary>
        /// <param name="id">The id to associate the name to.</param>
        /// <param name="categoryName">The name of the category.</param>
        static void RegisterCategory(MemId_t id, string_view_arg categoryName);

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
    };

}	// namespace
