//------------------------------------------------------------------------------
//
// File Name:	Random.cpp
// Author(s):	Jason Nguyen (jason.nguyen)
// Project:		Nightjar
// Course:		GAM200
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Random.h"

namespace Nightjar
{
    static constexpr long MaxValue = 0x7FFF;

    void Random::Rand::srand(unsigned int seed)
    {
        holdrand = (long)seed;
    }

    int Random::Rand::rand(void)
    {
        return(((holdrand = holdrand * 214013L +
            2531011L) >> 16) & MaxValue);
    }

    template<typename T>
    T Random::Rand::Range(T min, T max)
    {
        using Td = std::decay_t<T>;
        if constexpr (std::is_arithmetic_v<T>)
        {
            T diff = std::abs(max - min);
            T min_ = std::min(max, min);
            if constexpr (std::is_integral_v<T>)
            {
                int rangeWidth = diff + 1;
                return rand() % rangeWidth + min_;
            }
            else if constexpr (std::is_floating_point_v<T>)
            {
                static constexpr T MaxValueInv = static_cast<T>(1) / MaxValue;
                T fraction = static_cast<T>(rand()) * MaxValueInv;
                return min_ + fraction * diff;
            }
            else
            {
                static_assert(false && "Invalid arithmetic type!");
            }
        }
        else if constexpr (is_vec_v<Td> && std::is_arithmetic_v<typename Td::value_type>)
        {
            static constexpr int vecLength = static_cast<int>(Td::length());
            T output{};
            for (int i = 0; i < vecLength; ++i)
            {
                output[i] = Range<typename Td::value_type>(min[i], max[i]);
            }
            return output;
        }
        else
        {
            static_assert(false && "Invalid type!");
        }
    }

    /*template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = false>
    T Variance(T base, T range);

    template<typename T, std::enable_if_t<is_vec_v<std::decay_t<T>>&& std::is_arithmetic_v<typename std::decay_t<T>::value_type>, bool> = false>
    T Variance(const T& base, const T& range);*/

    template<typename T>
    T Random::Rand::Variance(T range, T base)
    {
        return base + Range<T>(-range, range);
    }

    static constinit Random::Rand GenericRand{0};

    void Random::Init()
    { 
        GenericRand.srand((unsigned int)time(NULL));
    }

    template<typename T>
    T Random::Range(T min, T max)
    {
        return GenericRand.Range<T>(min, max);
    }

    template<typename T>
    T Random::Variance(T range, T base)
    {
        return GenericRand.Variance<T>(range, base);
    }

#define RAND_TEMPLATE(type)\
    template type Random::Rand::Range<type>(type, type);\
    template type Random::Rand::Variance<type>(type, type);\
    template type Random::Range<type>(type, type);\
    template type Random::Variance<type>(type, type);\

#define PER_VEC(vec)\
    RAND_TEMPLATE(vec##1);\
    RAND_TEMPLATE(vec##2);\
    RAND_TEMPLATE(vec##3);\
    RAND_TEMPLATE(vec##4);\

    RAND_TEMPLATE(int);
    RAND_TEMPLATE(float);
    PER_VEC(Vec);
    PER_VEC(IVec);
    PER_VEC(DVec);
}