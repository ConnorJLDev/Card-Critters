//------------------------------------------------------------------------------
//
// File Name:	GraphicsStopwatchSequence.inl
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Utilizes several GraphicsStopwatches to avoid losing timing
//              data due to delayed completion.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "GraphicsStopwatchSequence.h"

#include "GraphicsStopwatch.h"

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

    template <auto N> requires (array_size_t<decltype(N)>)
    GraphicsStopwatchSequence<N>::GraphicsStopwatchSequence(void)
        : base_t()
    {
    }

    //--------------------------------------------------------------------------

    template <auto N> requires (array_size_t<decltype(N)>)
    GraphicsStopwatchSequence<N>::~GraphicsStopwatchSequence(void)
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

    template <auto N> requires (array_size_t<decltype(N)>)
    bool GraphicsStopwatchSequence<N>::Initialize(void)
    {
        for (auto& watch : static_cast<base_t::base_t&>(*this))
        {
            if (!watch.Initialize())
            {
                return false;
            }
        }
        return true;
    }

    template <auto N> requires (array_size_t<decltype(N)>)
    bool GraphicsStopwatchSequence<N>::Shutdown(void)
    {
        bool didShutdown = false;
        for (auto& watch : static_cast<base_t::base_t&>(*this))
        {
            didShutdown |= watch.Shutdown();
        }
        return didShutdown;
    }

    template <auto N> requires (array_size_t<decltype(N)>)
    bool GraphicsStopwatchSequence<N>::IsValid(void) const
    {
        for (const auto& watch : static_cast<base_t::base_t&>(*this))
        {
            if (!watch.IsValid())
            {
                return false;
            }
        }
        return true;
    }

    template <auto N> requires (array_size_t<decltype(N)>)
    void GraphicsStopwatchSequence<N>::Start(void)
    {
        auto& watch = base_t::back();
        watch.Start();
    }

    template <auto N> requires (array_size_t<decltype(N)>)
    void GraphicsStopwatchSequence<N>::Stop(void)
    {
        auto& watch = base_t::back();
        watch.Stop();
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

#pragma warning(pop)
