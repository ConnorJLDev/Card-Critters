//------------------------------------------------------------------------------
//
// File Name:	Systems.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Common header to include all system types.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Systems.h"
#include "Systems.inl"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{

    constexpr array_t<string_view_t, Systems::NUM_ENTRIES> SystemNames
    {{
#define REGISTER_SYSTEM(system) #system,
#include "Systems.def"
#undef REGISTER_SYSTEM
    }};

}	// namespace
