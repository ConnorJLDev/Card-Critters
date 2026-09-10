//------------------------------------------------------------------------------
//
// File Name:	UtilityTypeChecks.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Template structures for conveniently checking if a type has a
//              base template type.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "MathDefs.h"

//------------------------------------------------------------------------------
// Preprocessor Directives:
//------------------------------------------------------------------------------

#define TYPE_STRUCT_TRUE std::true_type
#define TYPE_STRUCT_FALSE std::false_type
#define TYPE_STRUCT_CONDITIONAL(...) std::conditional_t<__VA_ARGS__, TYPE_STRUCT_TRUE, TYPE_STRUCT_FALSE>

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar::Utility
{

    // Reference: https://stackoverflow.com/questions/60113615/how-to-check-if-a-variable-is-a-map-in-c
    // and some of https://stackoverflow.com/questions/64087934/how-to-write-a-c-concept-restricting-the-template-to-stdmap-and-stdunorder

    // Map
    template <typename T>
    struct is_map_like : TYPE_STRUCT_FALSE {};

    template <typename Key, typename T, typename Compare, typename Allocator>
    struct is_map_like<std::map<Key, T, Compare, Allocator>> : TYPE_STRUCT_TRUE {};

    template <typename Key, typename T, typename Hash, typename Pred, typename Allocator>
    struct is_map_like<std::unordered_map<Key, T, Hash, Pred, Allocator>> : TYPE_STRUCT_TRUE {};

    template <typename T>
    static inline constexpr bool is_map_like_v = is_map_like<T>::value;

    // Vector
    template <typename T>
    struct is_vector : TYPE_STRUCT_FALSE {};

    template <typename T, typename A>
    struct is_vector<std::vector<T, A>> : TYPE_STRUCT_TRUE {};

    template <typename T>
    static inline constexpr bool is_vector_v = is_vector<T>::value;

    // Sign equality
    template <integral_t T, integral_t U>
    struct is_same_sign : TYPE_STRUCT_FALSE {};

    template <integral_t T, integral_t U>
        requires (std::is_signed_v<T> == std::is_signed_v<U>)
    struct is_same_sign<T, U> : TYPE_STRUCT_TRUE {};

    template <integral_t T, integral_t U>
    static inline constexpr bool is_same_sign_v = is_same_sign<T, U>::value;

    // Type conversions
    template <typename From, typename To>
    struct is_convertible : TYPE_STRUCT_CONDITIONAL(std::is_convertible_v<From, To>) {};

    template <typename From, typename To>
    static inline constexpr bool is_convertible_v = is_convertible<From, To>::value;

    template <typename From, typename To>
        requires (is_vec_v<From> && is_vec_v<To> && !std::is_same_v<From, To>)
    struct is_convertible<From, To> : TYPE_STRUCT_TRUE {};

    // Widening type conversions
    template <typename From, typename To>
    struct raw_scalar_lossless_convertible : TYPE_STRUCT_FALSE {};

    template <typename From, typename To>
    static inline constexpr bool raw_scalar_lossless_convertible_v = raw_scalar_lossless_convertible<From, To>::value;

#define INTERNAL_SPECIALIZE_MULTI(tfrom, tto) template <> struct raw_scalar_lossless_convertible<tfrom, tto> : TYPE_STRUCT_TRUE {};

    FOR_EACH_1_1_1(INTERNAL_SPECIALIZE_MULTI, char, int16_t, int32_t, int64_t, float, double);
    FOR_EACH_1_1_1(INTERNAL_SPECIALIZE_MULTI, wchar_t, int32_t, int64_t, float, double);
    FOR_EACH_1_1_1(INTERNAL_SPECIALIZE_MULTI, int8_t, int16_t, int32_t, int64_t, float, double);
    FOR_EACH_1_1_1(INTERNAL_SPECIALIZE_MULTI, int16_t, int32_t, int64_t, float, double);
    FOR_EACH_1_1_1(INTERNAL_SPECIALIZE_MULTI, int32_t, int64_t, double);
    FOR_EACH_1_1_1(INTERNAL_SPECIALIZE_MULTI, bool, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t, float, double);
    FOR_EACH_1_1_1(INTERNAL_SPECIALIZE_MULTI, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t, float, double);
    FOR_EACH_1_1_1(INTERNAL_SPECIALIZE_MULTI, uint16_t, int32_t, uint32_t, int64_t, uint64_t, float, double);
    FOR_EACH_1_1_1(INTERNAL_SPECIALIZE_MULTI, uint32_t, int64_t, uint64_t, double);
    FOR_EACH_1_1_1(INTERNAL_SPECIALIZE_MULTI, float, double);

#undef INTERNAL_SPECIALIZE_MULTI

    template <typename From, typename To>
    concept raw_scalar_widening_t = requires
    {
        { To{ std::declval<From>() } } -> std::same_as<To>;
    } || raw_scalar_lossless_convertible_v<From, To>;

    template <typename From, typename To>
    concept widening_t = requires
    {
        // Regular widening check for scalar types.
        requires (std::is_scalar_v<From> && std::is_scalar_v<To> && raw_scalar_widening_t<From, To>)
        || std::is_same_v<From, To>
        || (
            // Widening check for vector types.
               !std::is_scalar_v<From> && !std::is_scalar_v<To>
            && requires { typename From::value_type; typename To::value_type; From::length(); To::length(); }
            && raw_scalar_widening_t<typename From::value_type, typename To::value_type>
            && From::length() <= To::length()
        );
    };

    template <typename From, typename To>
    struct is_lossless_convertible : TYPE_STRUCT_CONDITIONAL(widening_t<From, To>) {};

    template <typename From, typename To>
    static inline constexpr bool is_lossless_convertible_v = is_lossless_convertible<From, To>::value;

}	// namespace
