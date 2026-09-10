//------------------------------------------------------------------------------
//
// File Name:	UtilityString.inl
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Utility functions for strings.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "UtilityString.h"

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
    constexpr string_t StringToUpper(string_view_arg str)
    {
        string_t edited{ str };
        for (char& chr : edited)
        {
            if (chr >= 'a' && chr <= 'z')
            {
                chr += 'A' - 'a';
            }
        }
        return edited;
    }

}	// namespace

#pragma warning(pop)
