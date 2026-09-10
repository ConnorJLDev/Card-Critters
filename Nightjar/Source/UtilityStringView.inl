//------------------------------------------------------------------------------
//
// File Name:	UtilityStringView.inl
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

#include "UtilityStringView.h"

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

namespace Nightjar::Utility
{
    constexpr std::string StringToUpperConst(string_view_arg str)
    {
        ASSERT_CONSTEVAL;
        std::string edited{ str };
        for (char& chr : edited)
        {
            if (chr >= 'a' && chr <= 'z')
            {
                chr += 'A' - 'a';
            }
        }
        return edited;
    }

    template <typename T>
    constexpr std::string StringFromIntConst(T value, bool positiveSign) requires (std::is_integral_v<T>)
    {
        ASSERT_CONSTEVAL;
        std::string str = value < static_cast<T>(0) ? "-" : (positiveSign ? "+" : "");
        size_t temp = static_cast<size_t>(MathAbs(value));
        while (temp > 0)
        {
            str.insert(str.begin(), static_cast<int>(temp % 10) + '0');
            temp /= 10;
        }
        return str;
    }

    constexpr int StringToIntConst(string_view_arg str)
    {
        ASSERT_CONSTEVAL;
        if (str.empty())
        {
            throw "Cannot convert empty string to number.";
        }

        bool negative = false;
        size_t i = 0;

        if (str[0] == '-')
        {
            negative = true;
            i = 1;
        }
        else if (str[0] == '+')
        {
            i = 1;
        }

        int value = static_cast<int>(StringToUIntConst(str.substr(i)));

        if (value < 0)
        {
            throw "Converted string to number does not fit within signed integer.";
        }

        return negative ? -value : value;
    }

    constexpr unsigned int StringToUIntConst(string_view_arg str)
    {
        ASSERT_CONSTEVAL;
        if (str.empty())
        {
            throw "Cannot convert empty string to number.";
        }

        unsigned int result = 0;

        for (const char c : str)
        {
            if (c >= '0' && c <= '9')
            {
                result = result * 10 + (c - '0');
            }
            else
            {
                throw "Invalid character encountered while converting string to number.";
            }
        }

        return result;
    }

    constexpr uint64_t fnv1a_hash(const char* str, uint64_t hash)
    {
        for (const char* currentChar = str; *currentChar != '\0'; ++currentChar)
        {
            hash = (hash ^ static_cast<uint64_t>(*currentChar)) * 0x100000001b3;
        }
        return hash;
    }

    constexpr uint64_t fnv1a_hash(string_view_arg str, uint64_t hash)
    {
        for (const char currentChar : str)
        {
            hash = (hash ^ static_cast<uint64_t>(currentChar)) * 0x100000001b3;
        }
        return hash;
    }

}	// namespace

#pragma warning(pop)
