//------------------------------------------------------------------------------
//
// File Name:	PersistenceEntry.inl
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Describes a data entry within a persistence group.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "PersistenceEntry.h"

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

    template<variants_t T>
    bool PersistenceEntry::GetValue(T& value)
    {
        if (getter == nullptr)
        {
            return Variant::GetValue(value);
        }
        // Starting if statement to chain else-ifs off of.
        if constexpr (false) {}
#define VARIANT_PER(vtype, name)\
        else if constexpr (std::is_same_v<T, vtype>)\
        {\
            bool correctType = type == DataType::name;\
            if (correctType)\
            {\
                T* gotValue = getter();\
                value = *gotValue;\
            }\
            return correctType;\
        }
        FOR_EACH_0_2_2(VARIANT_PER, VARIANT_TYPES);
#undef VARIANT_PER
        return false;
    }

    template<variants_t T>
    bool PersistenceEntry::SetValue(const T& value)
    {
        if (setter == nullptr)
        {
            return Variant::SetValue(value);
        }
        // Starting if statement to chain else-ifs off of.
        if constexpr (false) {}
#define VARIANT_PER(vtype, name)\
        else if constexpr (std::is_same_v<T, vtype>)\
        {\
            bool correctType = type == DataType::name;\
            if (correctType)\
            {\
                setter(&value);\
            }\
            return correctType;\
        }
        FOR_EACH_0_2_2(VARIANT_PER, VARIANT_TYPES);
#undef VARIANT_PER
        return false;
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
