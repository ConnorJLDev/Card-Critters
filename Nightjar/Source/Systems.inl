//------------------------------------------------------------------------------
//
// File Name:	Systems.inl
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Common header to include all system types.
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
    enum class Systems : unsigned
    {
#define REGISTER_SYSTEM(system) system,
#include "Systems.def"
#undef REGISTER_SYSTEM
        NUM_ENTRIES,
    };

    extern const array_t<string_view_t, Systems::NUM_ENTRIES> SystemNames;

    constexpr unsigned SystemMax = static_cast<unsigned>(Systems::NUM_ENTRIES);

}	// namespace

#pragma warning(pop)
