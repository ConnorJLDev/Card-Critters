//------------------------------------------------------------------------------
//
// File Name:	PerformanceEnums.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Enums for categorizing performance statistics.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{

    // Different groupings of timings for hierarchy management.
    enum class PerformanceGroup
    {
        Engine,
        System,
        Scene,
        Entity,
        ComponentType,
        Component,
        ImGuiWindow,
        Material,
        NUM_ENTRIES,
    };

    ENUM_MAPS_SEQUENTIAL_DECLARE(PerformanceGroup);

    // Source being timed or evaluated.
    enum class PerformanceSource
    {
        CPU,
        Graphics,
        NUM_ENTRIES,
    };

    ENUM_MAPS_SEQUENTIAL_DECLARE(PerformanceSource);

}	// namespace
