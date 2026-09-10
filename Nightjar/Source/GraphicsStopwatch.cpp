//------------------------------------------------------------------------------
//
// File Name:	GraphicsStopwatch.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class that tracks timestamps and time elapsed between graphics
//              API calls.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "GraphicsStopwatch.h"

#include GRAPHICS_INCLUDE

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

    GraphicsStopwatch::GraphicsStopwatch(GraphicsStopwatch&& other) noexcept
        : GraphicsStopwatchCore(std::move(other))
        , timeStart_(std::move(other.timeStart_))
        , timeStop_(std::move(other.timeStop_))
    {
    }

    GraphicsStopwatch& GraphicsStopwatch::operator=(GraphicsStopwatch&& other) noexcept
    {
        if (&other != this)
        {
            GraphicsStopwatchCore::operator=(std::move(other));
            timeStart_ = std::move(other.timeStart_);
            timeStop_ = std::move(other.timeStop_);
        }

        return *this;
    }

    //--------------------------------------------------------------------------

    GraphicsStopwatch::~GraphicsStopwatch(void)
    {
    }

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

    bool GraphicsStopwatch::GetResults(void)
    {
        const bool done = IsDone();
        if (done)
        {
            uint64_t timeStart, timeStop;
            glGetQueryObjectui64v(idStart_, GL_QUERY_RESULT, &timeStart);
            glGetQueryObjectui64v(idStop_, GL_QUERY_RESULT, &timeStop);
            timeStart_ = nsec_t(timeStart);
            timeStop_ = nsec_t(timeStop);
        }
        return done;
    }

    nsec_t GraphicsStopwatch::GetDuration(void) const
    {
        return timeStop_ - timeStart_;
    }

    nsec_t GraphicsStopwatch::GetTimeStart(void) const
    {
        return timeStart_;
    }

    nsec_t GraphicsStopwatch::GetTimeStop(void) const
    {
        return timeStop_;
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
