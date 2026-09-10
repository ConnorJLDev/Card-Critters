//------------------------------------------------------------------------------
//
// File Name:	StreamCommon.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Common includes for rapidjson, and a custom allocator for
//              rapidjson documents.
//
// Copyright © 2026 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include <string_view>

#include <rapidjson.h>
#include <allocators.h>

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    // Forward Declarations:
    class StreamAllocator;

    // Typedefs:
    using StreamKey = string_view_t; // Alternative: typedef char StreamKey[];

    //#define RAPIDSTR(str) rapidjson::GenericStringRef<char>(str.data(), static_cast<rapidjson::SizeType>(str.size()))

    // Class Definition:
    class StreamAllocator
    {
        friend class IOSystem_;

        // Public Constants, Enums, and Structs:
    public:
        // Indicates that the memory allocated by this allocator needs to be explicitly freed if true.
        static const bool kNeedFree;

        // Constructors/Destructors:
    public:
        StreamAllocator();

        ~StreamAllocator();

        // Public Static Functions:
    public:
        /// <summary>
        /// 
        /// </summary>
        /// <param name="ptr"></param>
        static void Free(void* ptr) RAPIDJSON_NOEXCEPT;

        // Public Functions:
    public:
        /// <summary>
        /// 
        /// </summary>
        bool operator==(const StreamAllocator&) const RAPIDJSON_NOEXCEPT;

        /// <summary>
        /// 
        /// </summary>
        bool operator!=(const StreamAllocator&) const RAPIDJSON_NOEXCEPT;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="size"></param>
        /// <returns></returns>
        void* Malloc(size_t size);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="originalPtr"></param>
        /// <param name="originalSize"></param>
        /// <param name="newSize"></param>
        /// <returns></returns>
        void* Realloc(void* originalPtr, size_t originalSize, size_t newSize);

        // Public Event Handlers
    public:

        // Private Constants, Enums, and Structs:
    private:
        // The name of this allocator for tracking memory allocations.
        static const string_view_t memName;
        // The id of this allocator for tracking memory allocations.
        static const size_t memId;

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

#define RAPIDJSON_DEFAULT_ALLOCATOR Nightjar::StreamAllocator
#define RAPIDJSON_DEFAULT_STACK_ALLOCATOR Nightjar::StreamAllocator
#include <document.h>

namespace Nightjar
{
    // Using declarations that require document.h

    /// A value used in a json document.
    using StreamValue = rapidjson::Value;
    /// A json document.
    using Document_t = rapidjson::GenericDocument<rapidjson::UTF8<>, StreamAllocator, StreamAllocator>;

}	// namespace
