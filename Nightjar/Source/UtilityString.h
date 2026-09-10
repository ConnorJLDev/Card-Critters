//------------------------------------------------------------------------------
//
// File Name:	UtilityString.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Utility functions for strings.
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

// Sets up a std::string for conversion to a string_t.
#define CONVERT_STR(stdstr) stdstr.begin(), stdstr.end()

// Converts a std::string to a string_t.
#define CONVERT_STR_INL(stdstr) string_t(CONVERT_STR(stdstr))

// Converts a std::wstring to a wstring_t.
#define CONVERT_STR_INL_W(stdstr) wstring_t(CONVERT_STR(stdstr))

// Formats a string and appends it to a given string_t.
#define FORMAT_TO(str,...) std::format_to(std::back_inserter(str), __VA_ARGS__)

// Formats a string into a given string_t.
#define FORMAT_SET(str,...) str.clear(); FORMAT_TO(str, __VA_ARGS__)

// Formats a string into a new string_t.
#define FORMAT_MAKE(str,...) string_t str{}; FORMAT_TO(str, __VA_ARGS__)

// Formats a string into a new wstring_t.
#define FORMAT_MAKE_W(str,...) wstring_t str{}; FORMAT_TO(str, __VA_ARGS__)

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar::Utility
{

#pragma region STL String

    /// <summary>
    /// Struct that hashes strings equally between different containing types.
    /// Primarily used for map and set indexing without creating temporary string copies.
    /// </summary>
    struct StringHash
    {
        using is_transparent = void;

        size_t operator()(string_view_arg str) const;

        size_t operator()(const std::string& str) const;

        size_t operator()(const string_t& str) const;

        size_t operator()(const char* str) const;

        size_t operator()(wstring_view_arg str) const;

        size_t operator()(const std::wstring& str) const;

        size_t operator()(const wstring_t& str) const;

        size_t operator()(const wchar_t* str) const;
    };

#pragma region string_t

    /// <summary>
    /// A template for set_t with string_t keys that has the "less" template argument already set up for convenience.
    /// </summary>
    using set_string_t = set_t<string_t, std::less<>>;

    /// <summary>
    /// A template for multiset_t with string_t keys that has the "less" template argument already set up for convenience.
    /// </summary>
    using multiset_string_t = multiset_t<string_t, std::less<>>;

    /// <summary>
    /// A template for unordered_set_t with string_t keys that has the "hash" and "equivalency" template arguments already set up for convenience.
    /// </summary>
    using unordered_set_string_t = unordered_set_t<string_t, StringHash, std::equal_to<>>;

    /// <summary>
    /// A template for unordered_multiset_t with string_t keys that has the "hash" and "equivalency" template arguments already set up for convenience.
    /// </summary>
    using unordered_multiset_string_t = unordered_multiset_t<string_t, StringHash, std::equal_to<>>;

    /// <summary>
    /// A template for map_t with string_t keys that has the "less" template argument already set up for convenience.
    /// </summary>
    /// <typeparam name="TValue">The value type of the map_t.</typeparam>
    template <typename TValue>
    using map_string_t = map_t<string_t, TValue, std::less<>>;

    /// <summary>
    /// A template for multimap_t with string_t keys that has the "less" template argument already set up for convenience.
    /// </summary>
    /// <typeparam name="TValue">The value type of the multimap_t.</typeparam>
    template <typename TValue>
    using multimap_string_t = multimap_t<string_t, TValue, std::less<>>;

    /// <summary>
    /// A template for unordered_map_t with string_t keys that has the "hash" and "equivalency" template arguments already set up for convenience.
    /// </summary>
    /// <typeparam name="TValue">The value type of the unordered_map_t.</typeparam>
    template <typename TValue>
    using unordered_map_string_t = unordered_map_t<string_t, TValue, StringHash, std::equal_to<>>;

    /// <summary>
    /// A template for unordered_multimap_t with string_t keys that has the "hash" and "equivalency" template arguments already set up for convenience.
    /// </summary>
    /// <typeparam name="TValue">The value type of the unordered_multimap_t.</typeparam>
    template <typename TValue>
    using unordered_multimap_string_t = unordered_multimap_t<string_t, TValue, StringHash, std::equal_to<>>;

#pragma endregion string_t

#pragma region wstring_t

    /// <summary>
    /// A template for set_t with wstring_t keys that has the "less" template argument already set up for convenience.
    /// </summary>
    using set_wstring_t = set_t<wstring_t, std::less<>>;

    /// <summary>
    /// A template for multiset_t with wstring_t keys that has the "less" template argument already set up for convenience.
    /// </summary>
    using multiset_wstring_t = multiset_t<wstring_t, std::less<>>;

    /// <summary>
    /// A template for unordered_set_t with wstring_t keys that has the "hash" and "equivalency" template arguments already set up for convenience.
    /// </summary>
    using unordered_set_wstring_t = unordered_set_t<wstring_t, StringHash, std::equal_to<>>;

    /// <summary>
    /// A template for unordered_multiset_t with wstring_t keys that has the "hash" and "equivalency" template arguments already set up for convenience.
    /// </summary>
    using unordered_multiset_wstring_t = unordered_multiset_t<wstring_t, StringHash, std::equal_to<>>;

    /// <summary>
    /// A template for map_t with wstring_t keys that has the "less" template argument already set up for convenience.
    /// </summary>
    /// <typeparam name="TValue">The value type of the map_t.</typeparam>
    template <typename TValue>
    using map_wstring_t = map_t<wstring_t, TValue, std::less<>>;

    /// <summary>
    /// A template for multimap_t with wstring_t keys that has the "less" template argument already set up for convenience.
    /// </summary>
    /// <typeparam name="TValue">The value type of the multimap_t.</typeparam>
    template <typename TValue>
    using multimap_wstring_t = multimap_t<wstring_t, TValue, std::less<>>;

    /// <summary>
    /// A template for unordered_map_t with wstring_t keys that has the "hash" and "equivalency" template arguments already set up for convenience.
    /// </summary>
    /// <typeparam name="TValue">The value type of the unordered_map_t.</typeparam>
    template <typename TValue>
    using unordered_map_wstring_t = unordered_map_t<wstring_t, TValue, StringHash, std::equal_to<>>;

    /// <summary>
    /// A template for unordered_multimap_t with wstring_t keys that has the "hash" and "equivalency" template arguments already set up for convenience.
    /// </summary>
    /// <typeparam name="TValue">The value type of the unordered_multimap_t.</typeparam>
    template <typename TValue>
    using unordered_multimap_wstring_t = unordered_multimap_t<wstring_t, TValue, StringHash, std::equal_to<>>;

#pragma endregion wstring_t

#pragma endregion STL String

    /// <summary>
    /// Converts a wide string to a normal string.
    /// </summary>
    /// <param name="wstr">The wide string to convert.</param>
    /// <returns>The converted normal string.</returns>
    string_t StringWideToMulti(wstring_view_arg wstr);

    /// <summary>
    /// Converts a normal string to a wide string.
    /// </summary>
    /// <param name="str">The normal string to convert.</param>
    /// <returns>The converted wide string.</returns>
    wstring_t StringMultiToWide(string_view_arg str);

    /// <summary>
    /// Replaces all instaces of find with replace in the given string.
    /// </summary>
    /// <param name="str">The string to modify.</param>
    /// <param name="find">The substring to find.</param>
    /// <param name="replace">The string to replace found substrings with.</param>
    /// <returns>A reference of the input string.</returns>
    string_t& StringReplaceAll(string_t& str, string_view_arg find, string_view_arg replace);

    /// <summary>
    /// Converts a string to uppercase.
    /// </summary>
    /// <param name="str">The string to be uppercased.</param>
    /// <returns>The uppercased string.</returns>
    constexpr string_t StringToUpper(string_view_arg str);

}	// namespace

#include "UtilityString.inl"
