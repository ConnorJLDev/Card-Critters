//------------------------------------------------------------------------------
//
// File Name:	GraphicsStopwatch.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class that tracks timestamps and time elapsed between graphics
//              API calls.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "GraphicsStopwatchCore.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    // Forward Declarations:

    // Typedefs:

    // Class Definition:
    class GraphicsStopwatch : public GraphicsStopwatchCore
    {
        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    public:
        constexpr GraphicsStopwatch(void);
        GraphicsStopwatch(const GraphicsStopwatch& other) = delete;
        GraphicsStopwatch(GraphicsStopwatch&& other) noexcept;
        GraphicsStopwatch& operator=(const GraphicsStopwatch& other) = delete;
        GraphicsStopwatch& operator=(GraphicsStopwatch&& other) noexcept;

        ~GraphicsStopwatch(void);

        // Public Static Functions:
    public:

        // Public Functions:
    public:
        /// <summary>
        /// Fetches the stopwatch results from the graphics API.
        /// </summary>
        /// <returns>True if the results were successfully fetched, false otherwise.</returns>
        bool GetResults(void);

        /// <summary>
        /// Gets the amount of time elapsed between starting and stopping the stopwatch.
        /// </summary>
        /// <returns>The stopwatch duration, in nanoseconds.</returns>
        nsec_t GetDuration(void) const;

        /// <summary>
        /// Gets the timestamp of when the stopwatch last started.
        /// </summary>
        /// <returns>When the stopwatch started, in nanoseconds.</returns>
        nsec_t GetTimeStart(void) const;

        /// <summary>
        /// Gets the timestamp of when the stopwatch last stopped.
        /// </summary>
        /// <returns>When the stopwatch stopped, in nanoseconds.</returns>
        nsec_t GetTimeStop(void) const;

        // Public Event Handlers
    public:

        // Private Constants, Enums, and Structs:
    private:

        // Private Static Functions:
    private:

        // Private Functions:
    private:

        // Private Static Variables:
    private:

        // Private Variables:
    private:
        // The timestamp of when the stopwatch last started.
        nsec_t timeStart_;
        // The timestamp of when the stopwatch last stopped.
        nsec_t timeStop_;
    };

}	// namespace

#include "GraphicsStopwatch.inl"
