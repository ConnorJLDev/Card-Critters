//------------------------------------------------------------------------------
//
// File Name:	STLTypes.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Container types from the STL that are wrapped to use
//              MemoryManager's allocator.
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

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar::Utility
{

    template <typename T>
    concept array_size_t = requires
    {
        std::is_same_v<T, size_t> || std::is_enum_v<T>;
    };

    template <typename T>
    concept modifiable_t = requires
    {
        !std::is_const_v<T>;
    };

    template <typename T>
    concept integral_t = requires
    {
        std::is_integral_v<T>;
    };

    template <typename T>
    concept floating_t = requires
    {
        std::is_floating_point_v<T>;
    };

}	// namespace

namespace Nightjar
{

    template <typename T, Utility::array_size_t auto N>
    using array_t = std::array<T, static_cast<size_t>(N)>;

    template <typename T, typename TSize = size_t>
    using vector_t = std::vector<T, Memory::Allocator<typename std::vector<T>::value_type, TSize>>;

    template <typename T, typename TSize = size_t>
    using deque_t = std::deque<T, Memory::Allocator<typename std::deque<T>::value_type, TSize>>;

    template <typename T, typename TSize = size_t>
    using list_t = std::list<T, Memory::Allocator<typename std::list<T>::value_type, TSize>>;

    template <typename T, typename TSize = size_t>
    using forward_list_t = std::forward_list<T, Memory::Allocator<typename std::forward_list<T>::value_type, TSize>>;

    template <typename TKey, typename TLess = std::less<TKey>, typename TSize = size_t>
    using set_t = std::set<TKey, TLess, Memory::Allocator<typename std::set<TKey, TLess>::value_type, TSize>>;

    template <typename TKey, typename TLess = std::less<TKey>, typename TSize = size_t>
    using multiset_t = std::multiset<TKey, TLess, Memory::Allocator<typename std::multiset<TKey, TLess>::value_type, TSize>>;

    template <typename TKey, typename THash = std::hash<TKey>, typename TEqual = std::equal_to<TKey>, typename TSize = size_t>
    using unordered_set_t = std::unordered_set<TKey, THash, TEqual, Memory::Allocator<typename std::unordered_set<TKey>::value_type, TSize>>;

    template <typename TKey, typename THash = std::hash<TKey>, typename TEqual = std::equal_to<TKey>, typename TSize = size_t>
    using unordered_multiset_t = std::unordered_multiset<TKey, THash, TEqual, Memory::Allocator<typename std::unordered_multiset<TKey>::value_type, TSize>>;

    template <typename TKey, typename TValue, typename TLess = std::less<TKey>, typename TSize = size_t>
    using map_t = std::map<TKey, TValue, TLess, Memory::Allocator<typename std::map<TKey, TValue, TLess>::value_type, TSize>>;

    template <typename TKey, typename TValue, typename TLess = std::less<TKey>, typename TSize = size_t>
    using multimap_t = std::multimap<TKey, TValue, TLess, Memory::Allocator<typename std::multimap<TKey, TValue, TLess>::value_type, TSize>>;

    template <typename TKey, typename TValue, typename THash = std::hash<TKey>, typename TEqual = std::equal_to<TKey>, typename TSize = size_t>
    using unordered_map_t = std::unordered_map<TKey, TValue, THash, TEqual, Memory::Allocator<typename std::unordered_map<TKey, TValue, THash, TEqual>::value_type, TSize>>;

    template <typename TKey, typename TValue, typename THash = std::hash<TKey>, typename TEqual = std::equal_to<TKey>, typename TSize = size_t>
    using unordered_multimap_t = std::unordered_multimap<TKey, TValue, THash, TEqual, Memory::Allocator<typename std::unordered_multimap<TKey, TValue, THash, TEqual>::value_type, TSize>>;

    template <typename T, typename TContainer = deque_t<T>>
    using stack_t = std::stack<T, TContainer>;

    template <typename T, typename TContainer = deque_t<T>>
    using queue_t = std::queue<T, TContainer>;

    template <typename T, typename TContainer = vector_t<T>, typename TCompare = std::less<typename TContainer::value_type>>
    using priority_queue_t = std::priority_queue<T, TContainer, TCompare>;

    template <typename TChar, typename TCharTraits = std::char_traits<TChar>>
    using basic_string_t = std::basic_string<TChar, TCharTraits, Memory::Allocator<TChar>>;

    using string_t = basic_string_t<char>;

    using wstring_t = basic_string_t<wchar_t>;

    using regex_t = std::regex;

    using smatch_t = std::match_results<string_t::const_iterator, Memory::Allocator<std::sub_match<string_t::const_iterator>>>;

    template <typename T, typename TSize = size_t>
    using devector_t = deque_t<T, TSize>;

    template <typename T, size_t SegmentSize = 1024, typename TSize = size_t>
    using segmented_vector_t = deque_t<T, TSize>;

}	// namespace
