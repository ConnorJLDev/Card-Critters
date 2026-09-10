//------------------------------------------------------------------------------
//
// File Name:	Random.h
// Author(s):	Jason Nguyen
// Project:		Nightjar
// Course:		GAM200
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

#include "stdafx.h"

namespace Nightjar
{
	class Random {
    public:
        // Linear Congruential PRNG
        // This struct is based on rand() from Microsoft Library.
        struct Rand
        {
            long holdrand;

#pragma warning(push)
#pragma warning(disable : 4514)
            constexpr Rand(unsigned int seed)
                : holdrand((long)seed)
            {
            }
#pragma warning(pop)

            /// <summary>
            /// Seeds the PRNG.
            /// </summary>
            /// <param name="seed">The seed to use.</param>
            void srand(unsigned int seed);

            /// <summary>
            /// Generates a raw random number.
            /// </summary>
            /// <returns>A random number directly from the generator.</returns>
            int rand(void);

            /// <summary>
            /// 
            /// </summary>
            /// <typeparam name="T"></typeparam>
            /// <param name="base"></param>
            /// <param name="range"></param>
            /// <returns></returns>
            template<typename T>
            T Range(T min, T max);

            /// <summary>
            /// 
            /// </summary>
            /// <typeparam name="T"></typeparam>
            /// <param name="base"></param>
            /// <param name="range"></param>
            /// <returns></returns>
            template<typename T>
            T Variance(T range, T base = T{});
        };

	public:
		/// <summary>
		/// Initializes the PRNG.
		/// </summary>
		/// <param name=""></param>
		static void Init(void);

        /// <summary>
        /// 
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="base"></param>
        /// <param name="range"></param>
        /// <returns></returns>
        template<typename T>
        static T Range(T min, T max);

        /// <summary>
        /// 
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="base"></param>
        /// <param name="range"></param>
        /// <returns></returns>
        template<typename T>
        static T Variance(T range, T base = T{});
	};

}
