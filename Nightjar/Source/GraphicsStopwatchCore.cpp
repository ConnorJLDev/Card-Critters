//------------------------------------------------------------------------------
//
// File Name:	GraphicsStopwatchCore.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class that tracks time elapsed between graphics API calls.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "GraphicsStopwatchCore.h"

#include GRAPHICS_INCLUDE

//------------------------------------------------------------------------------
// Preprocessor Directives:
//------------------------------------------------------------------------------

#define USE_TIMESTAMP

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

    GraphicsStopwatchCore::GraphicsStopwatchCore(GraphicsStopwatchCore&& other) noexcept
        : idStart_(std::exchange(other.idStart_, ResourceUnbind))
        , idStop_(std::exchange(other.idStop_, ResourceUnbind))
    {
    }

    GraphicsStopwatchCore& GraphicsStopwatchCore::operator=(GraphicsStopwatchCore&& other) noexcept
    {
        if (&other != this)
        {
            Shutdown();
            idStart_ = std::exchange(other.idStart_, ResourceUnbind);
            idStop_ = std::exchange(other.idStop_, ResourceUnbind);
        }

        return *this;
    }

    //--------------------------------------------------------------------------

    GraphicsStopwatchCore::~GraphicsStopwatchCore(void)
    {
        Shutdown();
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

    bool GraphicsStopwatchCore::Initialize(void)
    {
        assert(idStart_ == ResourceUnbind && idStop_ == ResourceUnbind && "Timer must be reset when initializing!");
        glGenQueries(2, &idStart_);
        return IsValid();
    }

    bool GraphicsStopwatchCore::Shutdown(void)
    {
        const bool
              existStart = idStart_ != ResourceUnbind
            , existStop = idStop_ != ResourceUnbind
            , existBoth = existStart && existStop
            , existEither = existStart || existStop
            ;
        if (existEither)
        {
            glDeleteQueries(1 + int(existBoth), &idStart_ + int(existStop) - int(existBoth));
            idStart_ = ResourceUnbind;
            idStop_ = ResourceUnbind;
        }
        return existEither;
    }

    void GraphicsStopwatchCore::Start(void)
    {
#ifdef USE_TIMESTAMP
        glQueryCounter(idStart_, GL_TIMESTAMP);
#else
        glBeginQuery(GL_TIME_ELAPSED, idStart_);
#endif
    }

    void GraphicsStopwatchCore::Stop(void)
    {
#ifdef USE_TIMESTAMP
        glQueryCounter(idStop_, GL_TIMESTAMP);
#else
        glEndQuery(GL_TIME_ELAPSED);
#endif
    }

    bool GraphicsStopwatchCore::IsValid(void) const
    {
        return idStart_ != ResourceUnbind && idStop_ != ResourceUnbind;
    }

    bool GraphicsStopwatchCore::IsReady(void) const
    {
        GLboolean validStart = glIsQuery(idStart_);
        GLboolean validStop = glIsQuery(idStop_);

        return !(validStart && validStop);
    }

    bool GraphicsStopwatchCore::IsDone(void) const
    {
        if (IsReady())
        {
            return false;
        }

        GLint availableStart = GL_FALSE;
        GLint availableStop = GL_FALSE;
        glGetQueryObjectiv(idStart_, GL_QUERY_RESULT_AVAILABLE, &availableStart);
        glGetQueryObjectiv(idStop_, GL_QUERY_RESULT_AVAILABLE, &availableStop);

        return availableStart && availableStop;
    }

    nsec_t GraphicsStopwatchCore::GetDuration(void) const
    {
        uint64_t timeStart, timeStop;

        glGetQueryObjectui64v(idStart_, GL_QUERY_RESULT, &timeStart);
        glGetQueryObjectui64v(idStop_, GL_QUERY_RESULT, &timeStop);

        return nsec_t(timeStop - timeStart);
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
