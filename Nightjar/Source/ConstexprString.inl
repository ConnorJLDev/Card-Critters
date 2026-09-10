//------------------------------------------------------------------------------
//
// File Name:	ConstexprString.inl
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

#include "ConstexprString.h"

//------------------------------------------------------------------------------
// Preprocessor Directives:
//------------------------------------------------------------------------------

#pragma warning(push)
#pragma warning(disable : 4514)

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    //--------------------------------------------------------------------------
    // Constructors/Destructors:
    //--------------------------------------------------------------------------

#pragma region Constructors

    template <size_t N>
    template<typename TFunc, typename... Args>
    consteval ConstexprString<N>::ConstexprString(TFunc func, Args... args)
        : strlen(static_cast<decltype(strlen)>(N > 0 ? N - 1 : 0))
        , str{}
    {
        static_assert(N > 1, "ConstexprString must not be empty!");
        static_assert(N <= std::numeric_limits<decltype(strlen)>::max() - sizeof(decltype(strlen)), "ConstexprString cannot contain the given string with the maximum buffer size!");

        // Construct the string and copy it into our buffer.
        std::string finalStr = func(std::forward<Args>(args)...);
        if (finalStr.size() > N)
        {
            throw "ConstexprString unable to fit within given buffer!";
        }
        std::copy(finalStr.begin(), finalStr.end(), str);

        // Ensure null termination.
        if (finalStr.back() != '\0')
        {
            str[finalStr.size()] = '\0';
        }
    }

    template <size_t N>
    template<typename TFunc, typename... Args>
    consteval ConstexprString<N>::ConstexprString(ConstexprString<N>::X, TFunc func, Args... args)
        : ConstexprString(func, std::forward<Args>(args)...)
    {
    }

    //--------------------------------------------------------------------------

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

    template <size_t N>
    template<typename TFunc, typename... Args>
    consteval ConstexprString<N> ConstexprString<N>::MakeLocal(TFunc func, Args... args)
    {
        return ConstexprString<N>(X{}, func, std::forward<Args>(args)...);
    }

    template<ConstexprString T>
    consteval string_view_t ConstexprStrFuncs::Generate(void) noexcept
    {
        return T;
    }

    template<typename TFunc, typename... Args>
    consteval size_t ConstexprStrFuncs::GenerateSize(TFunc func, Args... args)
    {
        // Construct the string so we know its size.
        auto str = func(std::forward<Args>(args)...);
        size_t size = str.size();

        // Ensure space for null termination.
        if (str.back() != '\0')
        {
            ++size;
        }

        // String with only null character is still empty.
        if (size <= 1)
        {
            throw "ConstexprString must not be empty!";
        }

        return size;
    }

#pragma endregion Public Static Functions

    //--------------------------------------------------------------------------
    // Public Functions:
    //--------------------------------------------------------------------------

#pragma region Public Functions

    template<size_t N>
    consteval ConstexprString<N>::operator string_view_t(void) const
    {
        return string_view_t(str, strlen);
    }

#pragma endregion Public Functions

}	// namespace

#pragma warning(pop)
