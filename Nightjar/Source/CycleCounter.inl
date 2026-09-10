//------------------------------------------------------------------------------
//
// File Name:	CycleCounter.inl
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Macros and functions to evaluate performance using cpu cycle
//              counters.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

// Source - Adapted from https://stackoverflow.com/a/51907627
// https://stackoverflow.com/questions/13772567/how-to-get-the-cpu-cycle-count-in-x86-64-from-c/51907627#51907627
// Posted by Peter Cordes, modified by community. See post 'Timeline' for change history
// Retrieved 2026-07-11, License - CC BY-SA 4.0

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include <cstdint>

#ifdef _MSC_VER
#include <intrin.h>
#else
#include <x86intrin.h>
#include "CycleCounter.h"
#endif

//------------------------------------------------------------------------------
// Preprocessor Directives:
//------------------------------------------------------------------------------

#pragma warning(push)
#pragma warning(disable : 4514)

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    //--------------------------------------------------------------------------
    // Functions:
    //--------------------------------------------------------------------------

    // optional wrapper if you don't want to just use __rdtsc() everywhere
    inline uint64_t ReadTSC(void)
    {
        _mm_lfence();  // optionally wait for earlier insns to retire before reading the clock
        uint64_t tsc = __rdtsc();
        // _mm_lfence();  // optionally block later instructions until rdtsc retires
        return tsc;
    }

    // requires a Nehalem or newer CPU.  Not Core2 or earlier.  IDK when AMD added it.
    inline uint64_t ReadTSCp(void)
    {
        // _mm_lfence();
        unsigned dummy;
        uint64_t tsc = __rdtscp(&dummy); // waits for earlier insns to retire
        _mm_lfence();  // optionally block later instructions until rdtscp retires
        // in practice most CPUs seem to implement rdtscp similar to lfence; rdtsc, so later instructions won't start while waiting for earlier to finish.  But not guaranteed on paper.
        // see also the section below about out of order exec.
        return tsc;
    }

}	// namespace

#pragma warning(pop)
