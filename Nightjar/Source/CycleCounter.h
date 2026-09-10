//------------------------------------------------------------------------------
//
// File Name:	CycleCounter.h
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

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    /// <summary>
    /// Reads the CPU cycle counter BEFORE a given block of code.
    /// </summary>
    /// <returns>The current CPU cycle counter value.</returns>
    inline uint64_t ReadTSC(void);

    /// <summary>
    /// Reads the CPU cycle counter AFTER a given block of code.
    /// </summary>
    /// <returns>The current CPU cycle counter value.</returns>
    inline uint64_t ReadTSCp(void);

}	// namespace

//------------------------------------------------------------------------------
// Definitions:
//------------------------------------------------------------------------------

// A structural memory barrier to prevent the compiler from reordering code
#define COMPILER_BARRIER() std::atomic_signal_fence(std::memory_order_seq_cst)

// Compiler-specific barrier definitions.
/*#ifdef _MSC_VER
// A structural memory barrier to prevent the compiler from reordering code
#define COMPILER_BARRIER() _ReadWriteBarrier()
#else
// A structural memory barrier to prevent the compiler from reordering code
#define COMPILER_BARRIER() asm volatile("" ::: "memory")
#endif*/

// Wrapper macro to prevent compiler from reordering code before or after the input.
#define COMPILER_FORCE(...) COMPILER_BARRIER(); __VA_ARGS__; COMPILER_BARRIER();

// Reads the number of cpu cycles before the following code.
#define CYCLES_START(output) COMPILER_FORCE(output = Nightjar::ReadTSC())

// Reads the number of cpu cycles after the preceding code.
#define CYCLES_END(output) COMPILER_FORCE(output = Nightjar::ReadTSCp())
