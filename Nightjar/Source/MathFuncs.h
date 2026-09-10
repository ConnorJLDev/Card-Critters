//------------------------------------------------------------------------------
//
// File Name:	MathFuncs.h
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

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    // Convenience concept for checking if TElement is the same as TBase, or if TElement is the same type as a component in TBase.
    template <typename TElement, typename TBase>
    concept math_operand_t = requires
    {
         is_vec_v<std::decay_t<TBase>> && std::is_same_v<std::decay_t<TElement>, typename std::decay_t<TBase>::value_type>;
    } || requires
    {
        std::is_same_v<std::decay_t<TBase>, std::decay_t<TElement>>;
    };

    /// <summary>
    /// Wrapper for component-wise mathematical absolute value.
    /// </summary>
    /// <typeparam name="T">The type of the value being operated on.</typeparam>
    /// <param name="value">The input value to get the absolute value of.</param>
    /// <returns>The component-wise absolute value of the input value.</returns>
    template <typename T>
    inline constexpr T MathAbs(T value);

    /// <summary>
    /// Wrapper for component-wise mathematical maximum between two values.
    /// </summary>
    /// <typeparam name="T">The type of the first value being operated on.</typeparam>
    /// <typeparam name="T2">The type of the second value being operated on. If this is a scalar and T is a vector, the operation is applied to each component.</typeparam>
    /// <param name="a">The first value to compare.</param>
    /// <param name="b">The second value to compare.</param>
    /// <returns>The component-wise maximum between the two input values.</returns>
    template <typename T, math_operand_t<T> T2>
    inline constexpr T MathMax(T a, T2 b);

    /// <summary>
    /// Wrapper for component-wise mathematical minimum between two values.
    /// </summary>
    /// <typeparam name="T">The type of the values being operated on.</typeparam>
    /// <typeparam name="T2">The type of the second value being operated on. If this is a scalar and T is a vector, the operation is applied to each component.</typeparam>
    /// <param name="a">The first value to compare.</param>
    /// <param name="b">The second value to compare.</param>
    /// <returns>The component-wise minimum between the two input values.</returns>
    template <typename T, math_operand_t<T> T2>
    inline constexpr T MathMin(T a, T2 b);

    /// <summary>
    /// Wrapper for component-wise mathematical clamp of a value within a lower and upper bound, inclusive.
    /// </summary>
    /// <typeparam name="T">The type of the value being clamped.</typeparam>
    /// <typeparam name="TClamp">The type of the bounding values. If this is a scalar and T is a vector, the bounds are applied to each component.</typeparam>
    /// <param name="value">The input value to operate on.</param>
    /// <param name="lowerBound">The lower bound to clamp the input value to.</param>
    /// <param name="upperBound">The upper bound to clamp the input value to.</param>
    /// <returns>The component-wise clamp of the input value between the lower and upper bounds.</returns>
    template <typename T, math_operand_t<T> TClamp>
    inline constexpr T MathClamp(T value, TClamp lowerBound, TClamp upperBound);

    /// <summary>
    /// Wrapper for component-wise mathematical sign copy.
    /// </summary>
    /// <typeparam name="T">The type of the value being operated on.</typeparam>
    /// <typeparam name="TSign">The type of the value having its sign copied. If this is a scalar and T is a vector, the sign is copied to each component.</typeparam>
    /// <param name="value">The operand containing the magnitude to use.</param>
    /// <param name="sign">The operand containing the sign to use.</param>
    /// <returns>The component-wise sign combination of the magnitude of "value" with the sign of "sign".</returns>
    template <typename T, math_operand_t<T> TSign>
    inline constexpr T MathCopySign(T value, TSign sign);

}	// namespace

#include "MathFuncs.inl"
