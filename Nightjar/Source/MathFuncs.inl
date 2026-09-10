//------------------------------------------------------------------------------
//
// File Name:	MathFuncs.inl
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Provides definitions for common math functions.
//
// Copyright © 2026 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "MathFuncs.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

using namespace Nightjar::Utility;

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    template <typename T>
    inline constexpr T MathAbs(T value)
    {
        if (std::is_constant_evaluated())
        {
            return glm::abs(value);
        }
        if constexpr (is_vec_v<std::decay_t<T>>)
        {
            return glm::abs(value);
        }
        else
        {
            // The standard library has stuck with C functions, so here's a non-comprehensive list of if-else checks for each type.
            if constexpr (std::is_same_v<T, int>)
            {
                return std::abs(value);
            }
            else if constexpr (std::is_same_v<T, long>)
            {
                return std::labs(value);
            }
            else if constexpr (std::is_same_v<T, long long>)
            {
                return std::llabs(value);
            }
            else if constexpr (std::is_same_v<T, std::intmax_t>)
            {
                return std::imaxabs(value);
            }
            else if constexpr (std::is_same_v<T, float>)
            {
                return std::fabsf(value);
            }
            else if constexpr (std::is_same_v<T, double>)
            {
                return std::fabs(value);
            }
            else if constexpr (std::is_same_v<T, long double>)
            {
                return std::fabsl(value);
            }
            else
            {
                return glm::abs(value);
            }
        }
    }

    template <typename T, math_operand_t<T> T2>
    inline constexpr T MathMax(T a, T2 b)
    {
        if (std::is_constant_evaluated())
        {
            if constexpr (std::is_arithmetic_v<std::decay_t<T>> || is_vec_v<std::decay_t<T>>)
            {
                return glm::max(a, b);
            }
            else
            {
                throw "up";
            }
        }
        if constexpr (is_vec_v<std::decay_t<T>>)
        {
            return glm::max(a, b);
        }
        else
        {
            return std::max(a, b);
        }
    }

    template <typename T, math_operand_t<T> T2>
    inline constexpr T MathMin(T a, T2 b)
    {
        if (std::is_constant_evaluated())
        {
            if constexpr (std::is_arithmetic_v<std::decay_t<T>> || is_vec_v<std::decay_t<T>>)
            {
                return glm::min(a, b);
            }
            else
            {
                throw "up";
            }
        }
        else
        {
            if constexpr (is_vec_v<std::decay_t<T>>)
            {
                return glm::min(a, b);
            }
            else
            {
                return std::min(a, b);
            }
        }
    }

    template <typename T, math_operand_t<T> TClamp>
    inline constexpr T MathClamp(T value, TClamp lowerBound, TClamp upperBound)
    {
        if (std::is_constant_evaluated())
        {
            return glm::clamp(value, lowerBound, upperBound);
        }
        if constexpr (is_vec_v<std::decay_t<T>>)
        {
            return glm::clamp(value, lowerBound, upperBound);
        }
        else
        {
            return std::clamp(value, lowerBound, upperBound);
        }
    }

    template <typename T, math_operand_t<T> TSign>
    inline constexpr T MathCopySign(T value, TSign sign)
    {
        if (std::is_constant_evaluated())
        {
            return value * glm::sign(sign);
        }
        if constexpr (is_vec_v<std::decay_t<T>> || !std::is_floating_point_v<T>)
        {
            return value * glm::sign(sign);
        }
        else
        {
            return std::copysign(value, sign);
        }
    }

}	// namespace
