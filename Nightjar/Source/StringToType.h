//------------------------------------------------------------------------------
//
// File Name:	StringToType.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Utility functions for converting strings to other value types.
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

namespace Nightjar::Utility
{

    /// <summary>
    /// Tries to convert a given string view to a value.
    /// </summary>
    /// <typeparam name="T">The value type to parse.</typeparam>
    /// <param name="valueStr">The string to parse.</param>
    /// <param name="value">The object to store the parsed value.</param>
    /// <returns>True if parsing was successful, false otherwise.</returns>
    template <modifiable_t T>
    bool TryParseValue(string_view_arg valueStr, T& value);

    /// <summary>
    /// Tries to convert a given string view to a value. Specialization for math vectors.
    /// </summary>
    /// <typeparam name="T">The math vector value type to parse.</typeparam>
    /// <param name="valueStr">The string to parse.</param>
    /// <param name="value">The object to store the parsed value.</param>
    /// <returns>True if parsing was successful, false otherwise.</returns>
    template <modifiable_t T>
    bool TryParseValue(string_view_arg valueStr, T& value) requires (is_vec_v<T>);

    /// <summary>
    /// Tries to convert a given string view to an integer value.
    /// </summary>
    /// <typeparam name="T">The value type to parse.</typeparam>
    /// <typeparam name="ParseBase">Flag to try to decipher the base of the integer from the string.</typeparam>
    /// <param name="valueStr">The string to parse.</param>
    /// <param name="value">The object to store the parsed value.</param>
    /// <param name="base">The integer base to use.</param>
    /// <returns>True if parsing was successful, false otherwise.</returns>
    template <modifiable_t T, bool ParseBase = false>
    bool TryParseValue(string_view_arg valueStr, T& value, int base);

}	// namespace

#include "StringToType.inl"
