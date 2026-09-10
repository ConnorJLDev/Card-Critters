//------------------------------------------------------------------------------
//
// File Name:	Stopwatch.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class that wraps around std::chrono and provides utility
//				functions for tracking time.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Stopwatch.h"

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

    Stopwatch::Stopwatch(void)
        : start_()
        , stop_()
        , lap_()
        , delta_()
    {
    }

    //--------------------------------------------------------------------------

    Stopwatch::~Stopwatch(void)
    {
    }

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

    Stopwatch::clock_t::time_point Stopwatch::Now()
    {
        return clock_t::now();
    }

#pragma endregion Public Static Functions

    //--------------------------------------------------------------------------
    // Public Functions:
    //--------------------------------------------------------------------------

#pragma region Public Functions

    void Stopwatch::Start()
    {
        start_ = Now();
        lap_ = start_;
    }

    void Stopwatch::Stop()
    {
        stop_ = Now();
        delta_ = stop_ - start_;
    }

    template<typename T>
    T Stopwatch::Lap()
    {
        auto lap = Now();
        auto delta = lap - lap_;
        lap_ = std::move(lap);
        return std::chrono::duration_cast<T>(delta);
    }

    template<typename T>
    T Stopwatch::Duration() const
    {
        return std::chrono::duration_cast<T>(delta_);
    }

#define TEMPLATE_INSTANTIATE(type)\
    template type Stopwatch::Lap<type>();\
    template type Stopwatch::Duration<type>() const;\

    FOR_EACH_0_1_1(TEMPLATE_INSTANTIATE,
        sec_t,
        msec_t,
        usec_t,
        nsec_t,
        secf_t,
        msecf_t,
        usecf_t,
        nsecf_t,
        secd_t,
        msecd_t,
        usecd_t,
        nsecd_t,
        );


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
