//------------------------------------------------------------------------------
//
// File Name:	FixedString.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Structure for handling small strings with fixed-length buffers.
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
    // Struct Definition:
    template <typename TChar, uint8_t N, bool NullTerminated = true>
    struct basic_string_fixed_t
    {
        // Constructors/Destructors:
    public:
        basic_string_fixed_t(void);
        basic_string_fixed_t(basic_string_view_t<TChar> str);
        basic_string_fixed_t& operator=(basic_string_view_t<TChar> str);

        // Public Functions:
    public:
        /// <summary>
        /// Formats this string.
        /// </summary>
        /// <typeparam name="...Args">The format argument types.</typeparam>
        /// <param name="formatString">The string specifying the formatting pattern.</param>
        /// <param name="...args">The format arguments.</param>
        /// <returns>The number of characters successfully formatted.</returns>
        template <typename... Args>
        uint8_t Format(const std::format_string<Args...> formatString, Args&&... args);

        /// <summary>
        /// Formats a new string and appends it to the end of the current string.
        /// </summary>
        /// <typeparam name="...Args">The format argument types.</typeparam>
        /// <param name="formatString">The string specifying the formatting pattern.</param>
        /// <param name="...args">The format arguments.</param>
        /// <returns>The number of characters successfully formatted.</returns>
        template <typename... Args>
        uint8_t FormatTo(const std::format_string<Args...> formatString, Args&&... args);

        // Implicit conversion to string_view. This is used for many standard string functions.
        operator basic_string_view_t<TChar>(void) const;

        // Private Static Variables:
    public:
        // The capacity of the string.
        static const uint8_t capacity;

        // The null terminator of the string.
        static const TChar terminator;

        // Public Variables:
    public:
        // The size of the string.
        uint8_t size;
        // The container of the string data.
        array_t<TChar, N> data;
    };

    template <uint8_t N, bool NullTerminated = true>
    using string_fixed_t = basic_string_fixed_t<char, N, NullTerminated>;

    template <uint8_t N, bool NullTerminated = true>
    using wstring_fixed_t = basic_string_fixed_t<wchar_t, N, NullTerminated>;

}	// namespace

#include "FixedString.inl"
