//------------------------------------------------------------------------------
//
// File Name:	DataSetStatistics.h
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

    // Struct Definition:
    template <typename T, typename TBig = T, typename TFloat = double>
    struct DataSetStatistics
    {
        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    public:
        DataSetStatistics(void);
        DataSetStatistics(const DataSetStatistics& other);
        DataSetStatistics(DataSetStatistics&& other) noexcept;
        DataSetStatistics& operator=(const DataSetStatistics& other);
        DataSetStatistics& operator=(DataSetStatistics&& other) noexcept;

        ~DataSetStatistics(void);

        // Public Static Functions:
    public:
        /// <summary>
        /// Calculates the statistics on the provided data elements.
        /// </summary>
        /// <typeparam name="InputIt">The iterator type.</typeparam>
        /// <param name="first">The iterator of the start of the data to process.</param>
        /// <param name="last">The iterator of the end of the data to process.</param>
        /// <param name="step">The number of interleaved data sets.</param>
        template <class InputIt>
        static void CalculateInterleaved(InputIt first, InputIt last, DataSetStatistics* dataSets, size_t dataSetCount);

        // Public Functions:
    public:
        /// <summary>
        /// Calculates the statistics on the provided data elements.
        /// </summary>
        /// <typeparam name="InputIt">The iterator type.</typeparam>
        /// <param name="first">The iterator of the start of the data to process.</param>
        /// <param name="last">The iterator of the end of the data to process.</param>
        /// <param name="quartiles">Flag to calculate Q1, median, and Q3. If false, these values are left unchanged.</param>
        template <class InputIt>
        void Calculate(InputIt first, InputIt last, bool quartiles = true);
        
        // Public Variables:
    public:
        // The total sum of the data elements.
        TBig sum;
        // The mean of the data elements.
        TFloat mean;
        // The values at each quartile of the sorted data elements.
        array_t<T, 5> quartiles;
        // The standard deviation of the data elements.
        TFloat stdDev;

        // Private Constants, Enums, and Structs:
    private:
        //
        struct TempCalc
        {
            size_t count;
            double mean;
            double sumOfSquares;
        };

        // Private Static Functions:
    private:
        // Common buffer for containing sorted data elements.
        static vector_t<T>& SortBuffer(void);

        // Private Functions:
    private:

        // Private Static Variables:
    private:

        // Private Variables:
    private:
    };

}	// namespace

#include "DataSetStatistics.inl"
