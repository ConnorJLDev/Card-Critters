//------------------------------------------------------------------------------
//
// File Name:	StaticObjectsRegex.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Static container implementation for regex.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "StaticObjectsRegex.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar::StaticManagement
{
    //--------------------------------------------------------------------------
    // Constructors/Destructors:
    //--------------------------------------------------------------------------

#pragma region Constructors

    RegexWrapper::RegexWrapper(regex_t&& _data)
        : data(_data)
    {
    }

    //--------------------------------------------------------------------------

    RegexWrapper::~RegexWrapper(void)
    {
    }

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

    regex_t& RegexWrapper::Create(regex_t&& _data)
    {
        return ConstContainerBase::Create<RegexWrapper>(std::forward<regex_t>(_data));
    }

#pragma endregion Public Static Functions

    //--------------------------------------------------------------------------
    // Public Functions:
    //--------------------------------------------------------------------------

#pragma region Public Functions

    RegexWrapper::operator regex_t&(void)
    {
        return data;
    }

    RegexWrapper::operator const regex_t&(void) const
    {
        return data;
    }

#pragma endregion Public Functions

}	// namespace
