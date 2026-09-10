//------------------------------------------------------------------------------
//
// File Name:	EngineLoop.h
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

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    //
    enum class EngineLoop : unsigned char
    {
        Update,
        Render,
        NUM_ENTRIES,
    };

    ENUM_MAPS_SEQUENTIAL_DECLARE(EngineLoop);

}	// namespace
