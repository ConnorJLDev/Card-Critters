//------------------------------------------------------------------------------
//
// File Name:	FixedPoint.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Convenience wrapper around an integral type for fixed-point
//              arithmetic.
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
    // Struct Definition:
    template <integral_t TBase, size_t Scale>
    struct basic_fixed_t
    {
        // Constructors/Destructors:
    public:
        constexpr basic_fixed_t(void);
        template <integral_t TBaseOther, size_t ScaleOther>
        constexpr basic_fixed_t(const basic_fixed_t<TBaseOther, ScaleOther>& other);
        constexpr basic_fixed_t(TBase other);
        constexpr basic_fixed_t(float other);
        constexpr basic_fixed_t(double other);
        template <integral_t TBaseOther, size_t ScaleOther>
        constexpr basic_fixed_t& operator=(const basic_fixed_t<TBaseOther, ScaleOther>& other);
        constexpr basic_fixed_t& operator=(TBase other);
        constexpr basic_fixed_t& operator=(float other);
        constexpr basic_fixed_t& operator=(double other);

        // Public Static Functions:
    public:

        // Public Functions:
    public:
        // Explicit conversion to other fixed point types.
        template <integral_t TBaseOther, size_t ScaleOther>
        constexpr explicit operator basic_fixed_t<TBaseOther, ScaleOther>(void) const;

        // Explicit conversion to integral types.
        template <integral_t TBaseOther>
        constexpr explicit operator TBaseOther(void) const;

        // Explicit conversion to float.
        constexpr explicit operator float(void) const;

        // Explicit conversion to double.
        constexpr explicit operator double(void) const;

        // Arithmetic
        constexpr basic_fixed_t& operator+=(const basic_fixed_t& other);
        constexpr basic_fixed_t& operator+=(const TBase& other);
        constexpr basic_fixed_t& operator-=(const basic_fixed_t& other);
        constexpr basic_fixed_t& operator-=(const TBase& other);
        constexpr friend basic_fixed_t operator+(basic_fixed_t lhs, const basic_fixed_t& rhs);
        constexpr friend basic_fixed_t operator+(basic_fixed_t lhs, const TBase& rhs);
        constexpr friend basic_fixed_t operator-(basic_fixed_t lhs, const basic_fixed_t& rhs);
        constexpr friend basic_fixed_t operator-(basic_fixed_t lhs, const TBase& rhs);

        // Private Static Variables:
    public:
        // Ratio when converting from fixed to float.
        static const float RatioToFloat;
        // Ratio when converting from float to fixed.
        static const float RatioFromFloat;
        // Ratio when converting from fixed to double.
        static const double RatioToDouble;
        // Ratio when converting from double to fixed.
        static const double RatioFromDouble;

        // Public Variables:
    public:
        // The raw value.
        TBase value;
    };

}	// namespace

#include "FixedPoint.inl"
