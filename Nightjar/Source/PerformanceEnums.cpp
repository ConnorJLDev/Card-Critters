//------------------------------------------------------------------------------
//
// File Name:	PerformanceEnums.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Enums for categorizing performance statistics.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "PerformanceEnums.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{

    ENUM_MAPS_SEQUENTIAL(PerformanceGroup,
        Engine,
        System,
        Scene,
        Entity,
        ComponentType,
        Component,
        ImGuiWindow,
        Material
        );

    ENUM_MAPS_SEQUENTIAL(PerformanceSource,
        CPU,
        Graphics
        );

}	// namespace
