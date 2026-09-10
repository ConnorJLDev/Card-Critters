//------------------------------------------------------------------------------
//
// File Name:	Stopwatch.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class that wraps around std::chrono and provides utility
//				functions for tracking time.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Object.h"

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
    using sec_t = std::chrono::seconds; // Seconds as long long.
    using msec_t = std::chrono::milliseconds; // Milliseconds as long long.
    using usec_t = std::chrono::microseconds; // Microseconds as long long.
    using nsec_t = std::chrono::nanoseconds; // Nanoseconds as long long.

    using secf_t = std::chrono::duration<float>; // Seconds as float.
    using msecf_t = std::chrono::duration<float, std::milli>; // Milliseconds as float.
    using usecf_t = std::chrono::duration<float, std::micro>; // Microseconds as float.
    using nsecf_t = std::chrono::duration<float, std::nano>; // Nanoseconds as float.

    using secd_t = std::chrono::duration<double>; // Seconds as double.
    using msecd_t = std::chrono::duration<double, std::milli>; // Milliseconds as double.
    using usecd_t = std::chrono::duration<double, std::micro>; // Microseconds as double.
    using nsecd_t = std::chrono::duration<double, std::nano>; // Nanoseconds as double.

    // Duration type check
    template <typename T>
    struct is_duration : TYPE_STRUCT_FALSE {};

    template <typename Rep, typename Period>
    struct is_duration<std::chrono::duration<Rep, Period>> : TYPE_STRUCT_TRUE {};

    template <typename T>
    static inline constexpr bool is_duration_v = is_duration<T>::value;

    // Class Definition:
    class Stopwatch
    {
        // Public Constants, Enums, and Structs:
    public:
        using clock_t = std::chrono::high_resolution_clock; // Underlying clock type.

        // Constructors/Destructors:
    public:
        Stopwatch(void);

        ~Stopwatch(void);

        // Public Static Functions:
    public:
        /// <summary>
        /// Gets the current time.
        /// </summary>
        /// <returns>The current time.</returns>
        static clock_t::time_point Now(void);

        // Public Functions:
    public:
        /// <summary>
        /// Starts the stopwatch.
        /// </summary>
        void Start(void);

        /// <summary>
        /// Stops the stopwatch and calculates the duration.
        /// </summary>
        void Stop(void);
        
        /// <summary>
        /// Gets the duration between the previous lap and now, and resets the lap.
        /// </summary>
        /// <typeparam name="T">The format to get the lap duration in, as one of the duration typedefs.</typeparam>
        /// <returns>The duration between the previous lap and now.</returns>
        template<typename T = msecd_t>
        T Lap(void);

        /// <summary>
        /// Gets the duration between starting and stopping the stopwatch.
        /// </summary>
        /// <typeparam name="T">The format to get the duration in, as one of the duration typedefs.</typeparam>
        /// <returns>The duration the stopwatch ran for.</returns>
        template<typename T = msecd_t>
        T Duration(void) const;

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
        // The time the stopwatch was started.
        clock_t::time_point start_;
        // The time the stopwatch was stopped.
        clock_t::time_point stop_;
        // The time of the most recent lap call.
        clock_t::time_point lap_;
        // The duration between the start and stop of the stopwatch.
        clock_t::duration delta_;
    };

}	// namespace
