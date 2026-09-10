//------------------------------------------------------------------------------
//
// File Name:	UtilityStringView.h
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

namespace Nightjar
{
    template <typename TChar, typename TCharTraits = std::char_traits<TChar>>
    using basic_string_view_t = std::basic_string_view<TChar, TCharTraits>;

    using string_view_t = basic_string_view_t<char>;

    using wstring_view_t = basic_string_view_t<wchar_t>;

#ifdef STR_VIEW_ARG_REF
    using string_view_arg = const string_view_t&;
    using wstring_view_arg = const wstring_view_t&;
#else
    using string_view_arg = const string_view_t;
    using wstring_view_arg = const wstring_view_t;
#endif
}

namespace Nightjar::Utility
{
    /// <summary>
    /// Converts a string to uppercase. Works during compile-time.
    /// </summary>
    /// <param name="str">The string to be uppercased.</param>
    /// <returns>The uppercased string.</returns>
    constexpr std::string StringToUpperConst(string_view_arg str);

    /// <summary>
    /// Converts an integer to a string. Works during compile-time.
    /// </summary>
    /// <typeparam name="T">The integral type.</typeparam>
    /// <param name="value">The integer to stringify.</param>
    /// <param name="positiveSign">True to prepend a "+" for positive values.</param>
    /// <returns>A string representing the integer.</returns>
    template <typename T>
    constexpr std::string StringFromIntConst(T value, bool positiveSign = false) requires (std::is_integral_v<T>);

    /// <summary>
    /// Converts a string to an int. Works during compile-time.
    /// </summary>
    /// <param name="str">The string to convert.</param>
    /// <returns>The int represented by the string.</returns>
    constexpr int StringToIntConst(string_view_arg str);

    /// <summary>
    /// Converts a string to an unsigned int. Works during compile-time.
    /// </summary>
    /// <param name="str">The string to convert.</param>
    /// <returns>The unsigned int represented by the string.</returns>
    constexpr unsigned int StringToUIntConst(string_view_arg str);

    /// <summary>
    /// Hashes a string using the FNV-1a hash algorithm.
    /// </summary>
    /// <param name="str">The string to hash.</param>
    /// <param name="hash">The starting hash. Recommended to leave as default unless chaining multiple string hashes.</param>
    /// <returns>The hash of the string.</returns>
    constexpr uint64_t fnv1a_hash(const char* str, uint64_t hash = 0xcbf29ce484222325);

    /// <summary>
    /// Hashes a string using the FNV-1a hash algorithm.
    /// </summary>
    /// <param name="str">The string to hash.</param>
    /// <param name="hash">The starting hash. Recommended to leave as default unless chaining multiple string hashes.</param>
    /// <returns>The hash of the string.</returns>
    constexpr uint64_t fnv1a_hash(string_view_arg str, uint64_t hash = 0xcbf29ce484222325);

}	// namespace

#include "UtilityStringView.inl"
