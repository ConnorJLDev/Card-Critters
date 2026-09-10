//------------------------------------------------------------------------------
//
// File Name:	GraphicsStopwatchCore.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class that tracks time elapsed between graphics API calls.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "GraphicsSystem.h"

//------------------------------------------------------------------------------
// Preprocessor Directives:
//------------------------------------------------------------------------------

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
    class alignas(size_t) GraphicsStopwatchCore
    {
        friend class GraphicsStopwatch;

        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    public:
        constexpr GraphicsStopwatchCore(void);
        GraphicsStopwatchCore(const GraphicsStopwatchCore& other) = delete;
        GraphicsStopwatchCore(GraphicsStopwatchCore&& other) noexcept;
        GraphicsStopwatchCore& operator=(const GraphicsStopwatchCore& other) = delete;
        GraphicsStopwatchCore& operator=(GraphicsStopwatchCore&& other) noexcept;

        ~GraphicsStopwatchCore(void);

        // Public Static Functions:
    public:

        // Public Functions:
    public:
        /// <summary>
        /// Initializes the timer with the graphics API.
        /// </summary>
        /// <returns>True if initialization succeeded, false otherwise.</returns>
        bool Initialize(void);

        /// <summary>
        /// Shuts down the timer with the graphics API.
        /// </summary>
        /// <returns>True if any shutdown operation succeeded, false otherwise.</returns>
        bool Shutdown(void);

        /// <summary>
        /// Starts the stopwatch.
        /// </summary>
        void Start(void);

        /// <summary>
        /// Stops the stopwatch.
        /// </summary>
        void Stop(void);

        /// <summary>
        /// Checks if the stopwatch is currently registered with the graphics API.
        /// </summary>
        /// <returns>True if the stopwatch is valid and functional, false otherwise.</returns>
        bool IsValid(void) const;

        /// <summary>
        /// Checks if the stopwatch is not active and is ready to start another timing sequence.
        /// </summary>
        /// <returns>True if the stopwatch is ready to be used, false otherwise.</returns>
        bool IsReady(void) const;

        /// <summary>
        /// Checks if the results of the stopwatch are available.
        /// </summary>
        /// <returns>True if the stopwatch data is available, false otherwise.</returns>
        bool IsDone(void) const;

        /// <summary>
        /// Gets the time elapsed between the last Start and Stop calls.
        /// </summary>
        /// <returns>The time elapsed in nanoseconds.</returns>
        nsec_t GetDuration(void) const;

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
        // The graphics API resource ID used for tracking the start time.
        ResourceID idStart_;
        // The graphics API resource ID used for tracking the stop time.
        ResourceID idStop_;
    };


}	// namespace

#include "GraphicsStopwatchCore.inl"
