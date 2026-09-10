//------------------------------------------------------------------------------
//
// File Name:	ConstexprString.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Struct for creating compile-time strings.
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
    // References:
    // https://news.ycombinator.com/item?id=38292102
    // https://stackoverflow.com/q/79713216

    // Struct Definition:
    template <size_t N>
    struct ConstexprString
    {
        // Public Constants, Enums, and Structs:
    public:

        // Private Constants, Enums, and Structs:
    private:
        // Used for instance-based template construction.
        struct X
        {
        };

        // Constructors/Destructors:
    private:
        // Contains the actual construction and initialization.
        template<typename TFunc, typename... Args>
        consteval ConstexprString(TFunc func, Args... args);

        // Used for instance-based template construction.
        template<typename TFunc, typename... Args>
        consteval ConstexprString(X, TFunc func, Args... args);
    public:
        ConstexprString(void) = delete;
        ConstexprString(const ConstexprString& other) = delete;
        ConstexprString& operator=(const ConstexprString& other) = delete;

        // Public Static Functions:
    public:
        // Used for constructing a string for a specified location.
        template<typename TFunc, typename... Args>
        static consteval ConstexprString<N> MakeLocal(TFunc func, Args... args);

        // Public Functions:
    public:
        // Implicit string_view converter.
        consteval operator string_view_t(void) const;

        // Private Variables:
    public:
        // String size.
        uint_fast16_t strlen;
        // The string itself.
        char str[N];
    };

    namespace ConstexprStrFuncs
    {
        // Generates the template ConstexprString as a string_view.
        template<ConstexprString T>
        consteval string_view_t Generate(void) noexcept;

        // Generates the size of the calculated string.
        template<typename TFunc, typename... Args>
        consteval size_t GenerateSize(TFunc func, Args... args);
    }

}	// namespace

#include "ConstexprString.inl"

// Builds a compile-time string_view given the function and provided parameters.
#define ConstructStr(func, ...) Nightjar::ConstexprString<Nightjar::ConstexprStrFuncs::GenerateSize(func __VA_OPT__(,) __VA_ARGS__)>::MakeLocal(func __VA_OPT__(,) __VA_ARGS__)
#define BuildStr(func, ...) (Nightjar::ConstexprStrFuncs::Generate<ConstructStr(func __VA_OPT__(,) __VA_ARGS__)>())
