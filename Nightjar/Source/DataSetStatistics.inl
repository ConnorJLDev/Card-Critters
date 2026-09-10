//------------------------------------------------------------------------------
//
// File Name:	DataSetStatistics.inl
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Struct that computes and stores statistics of a given data set.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "DataSetStatistics.h"

//------------------------------------------------------------------------------
// Preprocessor Directives:
//------------------------------------------------------------------------------

// Compute standard deviation with vector operations.
//#define SUM_SIMD

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

    template <typename T, typename TBig, typename TFloat>
    DataSetStatistics<T, TBig, TFloat>::DataSetStatistics(void)
        : sum()
        , mean()
        , quartiles()
        , stdDev()
    {
    }

    template <typename T, typename TBig, typename TFloat>
    DataSetStatistics<T, TBig, TFloat>::DataSetStatistics(const DataSetStatistics& other)
        : sum(other.sum)
        , mean(other.mean)
        , quartiles(other.quartiles)
        , stdDev(other.stdDev)
    {
    }

    template <typename T, typename TBig, typename TFloat>
    DataSetStatistics<T, TBig, TFloat>::DataSetStatistics(DataSetStatistics&& other) noexcept
        : sum(std::move(other.sum))
        , mean(std::move(other.mean))
        , quartiles(std::move(other.quartiles))
        , stdDev(std::move(other.stdDev))
    {
    }

    template <typename T, typename TBig, typename TFloat>
    DataSetStatistics<T, TBig, TFloat>& DataSetStatistics<T, TBig, TFloat>::operator=(const DataSetStatistics& other)
    {
        sum = other.sum;
        mean = other.mean;
        quartiles = other.quartiles;
        stdDev = other.stdDev;
        return *this;
    }

    template <typename T, typename TBig, typename TFloat>
    DataSetStatistics<T, TBig, TFloat>& DataSetStatistics<T, TBig, TFloat>::operator=(DataSetStatistics&& other) noexcept
    {
        sum = std::move(other.sum);
        mean = std::move(other.mean);
        quartiles = std::move(other.quartiles);
        stdDev = std::move(other.stdDev);
        return *this;
    }

    //--------------------------------------------------------------------------

    template <typename T, typename TBig, typename TFloat>
    DataSetStatistics<T, TBig, TFloat>::~DataSetStatistics(void)
    {
    }

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

    template<typename T, typename TBig, typename TFloat>
    template<class InputIt>
    void DataSetStatistics<T, TBig, TFloat>::CalculateInterleaved(InputIt first, InputIt last, DataSetStatistics* dataSets, size_t dataSetCount)
    {
        if (first == last)
        {
            return;
        }

        static vector_t<TempCalc>& temps(StaticManagement::VectorWrapper<TempCalc>::Create(4));
        temps.resize(dataSetCount);

        InputIt iter = first;
        for (unsigned i = 0; i < dataSetCount && iter != last; ++i, ++iter)
        {
            const auto value = *iter;
            auto& data = dataSets[i];
            auto& temp = temps[i];

            data.sum = TBig(value);
            data.quartiles[0] = value;
            data.quartiles[4] = value;

            temp.count = static_cast<size_t>(value != T());
            if constexpr (is_duration_v<T>)
            {
                temp.mean = static_cast<double>(value.count());
            }
            else
            {
                temp.mean = static_cast<double>(value);
            }
            temp.sumOfSquares = 0.0;
        }

        // Welford's algorithm.
        while (iter != last)
        {
            for (unsigned i = 0; i < dataSetCount && iter != last; ++i, ++iter)
            {
                const auto value = *iter;
                if (value == T())
                {
                    continue;
                }
                auto& data = dataSets[i];
                auto& temp = temps[i];

                data.sum += value;
                data.quartiles[0] = MathMin(data.quartiles[0], value);
                data.quartiles[4] = MathMax(data.quartiles[4], value);
                double valueNum;
                if constexpr (is_duration_v<T>)
                {
                    valueNum = static_cast<double>(value.count());
                }
                else
                {
                    valueNum = static_cast<double>(value);
                }
                ++temp.count;
                double delta = valueNum - temp.mean;
                temp.mean += delta / static_cast<double>(temp.count);
                double delta2 = valueNum - temp.mean;
                temp.sumOfSquares += delta * delta2;
            }
        }

        for (unsigned i = 0; i < dataSetCount; ++i)
        {
            auto& data = dataSets[i];
            const auto& temp = temps[i];

            data.mean = TFloat(temp.mean);
            data.stdDev = TFloat(temp.count > 1 ? glm::sqrt(temp.sumOfSquares / static_cast<double>(temp.count - 1)) : 0);
        }

    }

#pragma endregion Public Static Functions

    //--------------------------------------------------------------------------
    // Public Functions:
    //--------------------------------------------------------------------------

#pragma region Public Functions

    template <typename T, typename TBig, typename TFloat>
    template <class InputIt>
    void DataSetStatistics<T, TBig, TFloat>::Calculate(InputIt first, InputIt last, bool calcQuartiles)
    {
        using index_t = uint32_t;

        if (calcQuartiles)
        // Get the quartiles.
        {
            static vector_t<T>& buffer(SortBuffer());
            buffer.assign(first, last);
            size_t itemCount = buffer.size();
            double itemCountDouble = static_cast<double>(itemCount);

            auto iterQ2 = buffer.begin() + static_cast<ptrdiff_t>(itemCount / 2);
            auto iterQ1 = buffer.begin() + static_cast<ptrdiff_t>(itemCount / 4);
            auto iterQ3 = buffer.begin() + static_cast<ptrdiff_t>(itemCountDouble * 0.75f);

            // Find Q2 (Median)
            std::nth_element(buffer.begin(), iterQ2, buffer.end());
            quartiles[2] = *iterQ2;

            // Find Q1
            std::nth_element(buffer.begin(), iterQ1, iterQ2);
            quartiles[1] = *iterQ1;
            quartiles[0] = quartiles[1]; // Set the min to Q1 as a starting point for comparisons.

            // Find Q3
            std::nth_element(MathMin(iterQ2 + 1, iterQ3), iterQ3, buffer.end());
            quartiles[3] = *iterQ3;
            quartiles[4] = quartiles[3]; // Set the max to Q3 as a starting point for comparisons.
        }

        if (first == last)
        {
            return;
        }

        size_t itemCount = 1;
        double meanDouble = 0.0;
        double mean2 = 0.0;
        InputIt iter = first;
        const auto firstVal = *iter;
        sum = TBig(firstVal);
        quartiles[0] = firstVal;
        quartiles[4] = firstVal;

        // Welford's algorithm.
        for (++iter; iter != last; ++iter)
        {
            const auto value = *iter;
            sum += value;
            quartiles[0] = MathMin(quartiles[0], value);
            quartiles[4] = MathMax(quartiles[4], value);
            double valueDouble;
            if constexpr (is_duration_v<T>)
            {
                valueDouble = static_cast<double>(value.count());
            }
            else
            {
                valueDouble = static_cast<double>(value);
            }
            ++itemCount;
            double delta = valueDouble - meanDouble;
            meanDouble += delta / static_cast<double>(itemCount);
            double delta2 = valueDouble - meanDouble;
            mean2 += delta * delta2;
        }

        mean = TFloat(meanDouble);
        stdDev = TFloat(itemCount > 1 ? glm::sqrt(mean2 / static_cast<double>(itemCount - 1)) : 0);

//        // Sum and mean.
//        {
//            sum = TBig(0);
//            quartiles[0] = buffer.begin();
//            quartiles[4] = buffer.begin();
//            for (auto value : buffer)
//            {
//                quartiles[0] = MathMin(quartiles[0], value);
//                quartiles[4] = MathMax(quartiles[4], value);
//                sum += value;
//            }
//            double sumDouble;
//            if constexpr (is_duration_v<TBig>)
//            {
//                sumDouble = static_cast<double>(sum.count());
//            }
//            else
//            {
//                sumDouble = static_cast<double>(sum);
//            }
//            meanDouble = sumDouble / itemCountDouble;
//            mean = TFloat(meanDouble);
//        }
//        // Standard deviation.
//        {
//            double valueSqrSum = 0;
//            index_t i = 0;
//#ifdef SUM_SIMD
//            static constexpr index_t VecSize = 4;
//            using VecType = glm::vec<VecSize, T>;
//            const index_t simdCount = static_cast<index_t>((itemCount / VecSize) * VecSize);
//            const DVec4 meanVec(meanDouble, meanDouble, meanDouble, meanDouble);
//            for (; i < simdCount; i += VecSize)
//            {
//                DVec4 valueVec = reinterpret_cast<VecType&>(buffer[i]);
//                valueVec -= meanVec;
//                valueSqrSum += glm::length2(valueVec);
//            }
//#endif
//            for (; i < itemCount; ++i)
//            {
//                double value;
//                if constexpr (is_duration_v<T>)
//                {
//                    value = static_cast<double>(buffer[i].count());
//                }
//                else
//                {
//                    value = static_cast<double>(buffer[i]);
//                }
//                value -= meanDouble;
//                valueSqrSum += value * value;
//            }
//            stdDev = TFloat(glm::sqrt(valueSqrSum / itemCountDouble));
//        }
    }

#pragma endregion Public Functions

    //--------------------------------------------------------------------------
    // Private Static Functions:
    //--------------------------------------------------------------------------

#pragma region Private Static Functions

    template <typename T, typename TBig, typename TFloat>
    STATIC_VECTOR_CLASS_TEMPLATE((T), (DataSetStatistics<T, TBig, TFloat>), SortBuffer);

#pragma endregion Private Static Functions

    //--------------------------------------------------------------------------
    // Private Functions:
    //--------------------------------------------------------------------------

#pragma region Private Functions

#pragma endregion Private Functions

}	// namespace

#pragma warning(pop)
