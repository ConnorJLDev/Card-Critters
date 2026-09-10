//------------------------------------------------------------------------------
//
// File Name:	FixedPoint.inl
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

#include "FixedPoint.h"

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
    // Public Constants:
    //--------------------------------------------------------------------------

    template<integral_t TBase, size_t Scale>
    constexpr double
          basic_fixed_t<TBase, Scale>::RatioFromDouble = static_cast<double>(Scale)
        , basic_fixed_t<TBase, Scale>::RatioToDouble = 1.0 / RatioFromDouble
        ;

    template<integral_t TBase, size_t Scale>
    constexpr float
          basic_fixed_t<TBase, Scale>::RatioFromFloat = static_cast<float>(RatioFromDouble)
        , basic_fixed_t<TBase, Scale>::RatioToFloat = static_cast<float>(RatioToDouble)
        ;

    //--------------------------------------------------------------------------
    // Public Static Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Public Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Private Constants:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Private Static Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Private Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Constructors/Destructors:
    //--------------------------------------------------------------------------

#pragma region Constructors

    template<integral_t TBase, size_t Scale>
    constexpr basic_fixed_t<TBase, Scale>::basic_fixed_t(void)
        : value()
    {
    }

    template<integral_t TBase, size_t Scale>
    constexpr basic_fixed_t<TBase, Scale>::basic_fixed_t(TBase other)
        : value(other)
    {
    }

    template<integral_t TBase, size_t Scale>
    template<integral_t TBaseOther, size_t ScaleOther>
    constexpr basic_fixed_t<TBase, Scale>::basic_fixed_t(const basic_fixed_t<TBaseOther, ScaleOther>& other)
        : value(static_cast<basic_fixed_t<TBase, Scale>>(other).value)
    {
    }

    template<integral_t TBase, size_t Scale>
    constexpr basic_fixed_t<TBase, Scale>::basic_fixed_t(float other)
        : value(static_cast<TBase>(other * RatioFromFloat))
    {
    }

    template<integral_t TBase, size_t Scale>
    constexpr basic_fixed_t<TBase, Scale>::basic_fixed_t(double other)
        : value(static_cast<TBase>(other * RatioFromDouble))
    {
    }

    template<integral_t TBase, size_t Scale>
    template<integral_t TBaseOther, size_t ScaleOther>
    constexpr basic_fixed_t<TBase, Scale>& basic_fixed_t<TBase, Scale>::operator=(const basic_fixed_t<TBaseOther, ScaleOther>& other)
    {
        value = static_cast<basic_fixed_t<TBase, Scale>>(other).value;
        return *this;
    }

    template<integral_t TBase, size_t Scale>
    constexpr basic_fixed_t<TBase, Scale>& Nightjar::basic_fixed_t<TBase, Scale>::operator=(TBase other)
    {
        value = static_cast<TBase>(Scale) * other;
        return *this;
    }

    template<integral_t TBase, size_t Scale>
    constexpr basic_fixed_t<TBase, Scale>& basic_fixed_t<TBase, Scale>::operator=(float other)
    {
        value = static_cast<TBase>(other * RatioFromFloat);
        return *this;
    }

    template<integral_t TBase, size_t Scale>
    constexpr basic_fixed_t<TBase, Scale>& basic_fixed_t<TBase, Scale>::operator=(double other)
    {
        value = static_cast<TBase>(other * RatioFromDouble);
        return *this;
    }

    //--------------------------------------------------------------------------

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

#pragma endregion Public Static Functions

    //--------------------------------------------------------------------------
    // Public Functions:
    //--------------------------------------------------------------------------

#pragma region Public Functions

    template<integral_t TBase, size_t Scale>
    template<integral_t TBaseOther, size_t ScaleOther>
    constexpr basic_fixed_t<TBase, Scale>::operator basic_fixed_t<TBaseOther, ScaleOther>(void) const
    {
        TBaseOther valueNew;
        if constexpr (Scale == ScaleOther)
        {
            valueNew = static_cast<TBaseOther>(value);
        }
        else if constexpr (std::has_single_bit(Scale) && std::has_single_bit(ScaleOther))
        {
            valueNew = static_cast<TBaseOther>(value) << (std::bit_width(ScaleOther) - std::bit_width(Scale));
        }
        else if constexpr (ScaleOther % Scale == 0)
        {
            valueNew = static_cast<TBaseOther>(static_cast<size_t>(value) * (Scale / ScaleOther));
        }
        else if constexpr (Scale % ScaleOther == 0)
        {
            valueNew = static_cast<TBaseOther>(static_cast<size_t>(value) / (ScaleOther / Scale));
        }
        else
        {
            valueNew = static_cast<TBaseOther>(static_cast<float>(value) * (RatioFromFloat * basic_fixed_t<TBaseOther, ScaleOther>::RatioToFloat));
        }
        return basic_fixed_t<TBaseOther, ScaleOther>(valueNew);
    }

    template<integral_t TBase, size_t Scale>
    template <integral_t TBaseOther>
    constexpr basic_fixed_t<TBase, Scale>::operator TBaseOther(void) const
    {
        return static_cast<basic_fixed_t<TBaseOther, 1>>(*this).value;
    }

    template<integral_t TBase, size_t Scale>
    constexpr basic_fixed_t<TBase, Scale>::operator float(void) const
    {
        return static_cast<float>(value) * RatioToFloat;
    }

    template<integral_t TBase, size_t Scale>
    constexpr basic_fixed_t<TBase, Scale>::operator double(void) const
    {
        return static_cast<double>(value) * RatioToDouble;
    }

    template<integral_t TBase, size_t Scale>
    constexpr basic_fixed_t<TBase, Scale>& basic_fixed_t<TBase, Scale>::operator+=(const basic_fixed_t& other)
    {
        value += other.value;
        return *this;
    }

    template<integral_t TBase, size_t Scale>
    constexpr basic_fixed_t<TBase, Scale>& basic_fixed_t<TBase, Scale>::operator+=(const TBase& other)
    {
        value += other * static_cast<TBase>(Scale);
        return *this;
    }

    template<integral_t TBase, size_t Scale>
    constexpr basic_fixed_t<TBase, Scale>& basic_fixed_t<TBase, Scale>::operator-=(const basic_fixed_t& other)
    {
        value -= other.value;
        return *this;
    }

    template<integral_t TBase, size_t Scale>
    constexpr basic_fixed_t<TBase, Scale>& basic_fixed_t<TBase, Scale>::operator-=(const TBase& other)
    {
        value -= other * static_cast<TBase>(Scale);
        return *this;
    }

    template<integral_t TBase, size_t Scale>
    constexpr basic_fixed_t<TBase, Scale> operator+(basic_fixed_t<TBase, Scale> lhs, const basic_fixed_t<TBase, Scale>& rhs)
    {
        lhs += rhs;
        return lhs;
    }

    template<integral_t TBase, size_t Scale>
    constexpr basic_fixed_t<TBase, Scale> operator+(basic_fixed_t<TBase, Scale> lhs, const TBase& rhs)
    {
        lhs += rhs;
        return lhs;
    }

    template<integral_t TBase, size_t Scale>
    constexpr basic_fixed_t<TBase, Scale> operator-(basic_fixed_t<TBase, Scale> lhs, const basic_fixed_t<TBase, Scale>& rhs)
    {
        lhs -= rhs;
        return lhs;
    }

    template<integral_t TBase, size_t Scale>
    constexpr basic_fixed_t<TBase, Scale> operator-(basic_fixed_t<TBase, Scale> lhs, const TBase& rhs)
    {
        lhs -= rhs;
        return lhs;
    }

#pragma endregion Public Functions

    //--------------------------------------------------------------------------
    // Private Static Functions:
    //--------------------------------------------------------------------------

#pragma region Private Static Functions

#pragma endregion Private Static Functions

    //--------------------------------------------------------------------------
    // Private Functions:
    //--------------------------------------------------------------------------

#pragma region Private Functions

#pragma endregion Private Functions

}	// namespace

#pragma warning(pop)
