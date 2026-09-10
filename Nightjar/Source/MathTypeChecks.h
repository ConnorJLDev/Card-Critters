//------------------------------------------------------------------------------
//
// File Name:	MathTypeChecks.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Provides templates for checking math types.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "MathDefs.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar::Utility
{
    // Vector
    template <typename T>
    struct is_vec : std::false_type {};

    template <glm::length_t L, typename T, glm::qualifier Q>
    struct is_vec<glm::vec<L, T, Q>> : std::true_type {};

    template <typename T>
    static inline constexpr bool is_vec_v = is_vec<T>::value;

    template <typename T>
    concept vec_t = requires()
    {
        is_vec_v<T>;
    };

    // Matrix
    template <typename T>
    struct is_matrix : std::false_type {};

    template <glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
    struct is_matrix<glm::mat<C, R, T, Q>> : std::true_type {};

    template <typename T>
    static inline constexpr bool is_matrix_v = is_matrix<T>::value;

    template <typename T>
    concept matrix_t = requires()
    {
        is_matrix_v<T>;
    };

    // Quaternion
    template <typename T>
    struct is_quat : std::false_type {};

    template <typename T, glm::qualifier Q>
    struct is_quat<glm::qua<T, Q>> : std::true_type {};

    template <typename T>
    static inline constexpr bool is_quat_v = is_quat<T>::value;

    template <typename T>
    concept quat_t = requires()
    {
        is_quat_v<T>;
    };

}	// namespace
