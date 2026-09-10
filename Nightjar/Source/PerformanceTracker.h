//------------------------------------------------------------------------------
//
// File Name:	PerformanceTracker.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Struct that represents timing data of some given object and
//              computes statistics of that data.
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
#include "EngineLoop.h"
#include "GraphicsStopwatch.h"
#include "PerformanceEnums.h"

//------------------------------------------------------------------------------
// Preprocessor Directives:
//------------------------------------------------------------------------------

#pragma warning(push)
#pragma warning(disable : 4582)

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
    struct PerformanceTracker
    {
        // Public Constants, Enums, and Structs:
    public:
        // Duration type being recorded.
        using duration_t = nsec_t;

        // Statistics struct type.
        using stats_t = DataSetStatistics<nsec_t, nsec_t, nsecd_t>;

        // Total combinations of performance sources and engine loops.
        static constexpr size_t SrcCatMax = static_cast<size_t>(PerformanceSource::NUM_ENTRIES) * static_cast<size_t>(EngineLoop::NUM_ENTRIES);

        // Constructors/Destructors:
    public:
        PerformanceTracker(void);
        PerformanceTracker(bool multi);
        PerformanceTracker(const Object& obj, string_view_arg name, const Object& parent);
        PerformanceTracker(const PerformanceTracker& other) = delete;
        PerformanceTracker(PerformanceTracker&& other) noexcept;
        PerformanceTracker& operator=(const PerformanceTracker& other) = delete;
        PerformanceTracker& operator=(PerformanceTracker&& other) noexcept;

        ~PerformanceTracker(void);

        // Public Static Functions:
    public:

        // Public Functions:
    public:
        /// <summary>
        /// Gets a history element by index for writing.
        /// </summary>
        /// <param name="index">The index of the element to get.</param>
        /// <returns>A reference to the element.</returns>
        nsec_t& operator[](size_t index);
        
        /// <summary>
        /// Gets a history element by index for reading.
        /// </summary>
        /// <param name="index">The index of the element to get.</param>
        /// <returns>A copy of the element.</returns>
        nsec_t operator[](size_t index) const;

        /// <summary>
        /// Gets the frontmost element.
        /// </summary>
        /// <returns>A reference to the frontmost element.</returns>
        nsec_t& Front(void);

        /// <summary>
        /// Gets the frontmost element.
        /// </summary>
        /// <returns>The frontmost element, or the default value if empty.</returns>
        nsec_t FrontOrDefault(void) const;

        /// <summary>
        /// Gets the number of elements contained.
        /// </summary>
        /// <returns>The number of elements contained.</returns>
        size_t Size(void) const;

        /// <summary>
        /// Checks if this tracker has no elements.
        /// </summary>
        /// <returns>True if this tracker has no elements, false otherwise.</returns>
        bool Empty(void) const;

        /// <summary>
        /// Sets the maximum number of history elements to track.
        /// </summary>
        /// <param name="capacity">The maximum number of history elements to track.</param>
        void SetCapacity(size_t capacity);

        /// <summary>
        /// Adds a new entry to the history elements.
        /// </summary>
        /// <param name="time">The new time to add.</param>
        void Push(nsec_t time);

        /// <summary>
        /// Initializes the next set of values for iteration.
        /// </summary>
        void Next(void);

        /// <summary>
        /// Pushes a new time to the tracker, ordered by engine loop and source.
        /// </summary>
        /// <param name="time">The time being recorded.</param>
        /// <param name="engineLoop">The engine loop that this time was recorded in.</param>
        /// <param name="source">The source that was timed.</param>
        void Push(nsec_t time, EngineLoop engineLoop, PerformanceSource source);

        /// <summary>
        /// Calculates the statistics on the contained history elements.
        /// </summary>
        void CalculateSingle(bool quartiles = true);

        /// <summary>
        /// Calculates the statistics on the contained history elements.
        /// </summary>
        void CalculateAll(bool quartiles = true);

        // Public Event Handlers
    public:

        // Public Variables:
    public:
        // The name of the object being represented.
        string_t name;
        // The contained history elements provided.
        devector_t<nsec_t> times;
        // The number of history elements that are kept track of.
        uint32_t timeCapacity;
        // Flags for indicating what times are recorded.
        bool has_cpuUpdate : 1;
        bool has_gfxUpdate : 1;
        bool has_cpuRender : 1;
        bool has_gfxRender : 1;
        // How many time entries to step per full loop.
        unsigned char step;
        // The computed statistics of the history elements.
        union
        {
            stats_t stats;
            array_t<stats_t, SrcCatMax> statsAll;
        };
        // ID of the object being tracked.
        ObjectID object;
        // ID of the parent of this tracker.
        ObjectID parent;
        // IDs of children of this tracker.
        vector_t<ObjectID> children;
        // Timer used for measuring graphics API durations.
        GraphicsStopwatch gfxTimer;

        // Private Constants, Enums, and Structs:
    private:
        // Underlying iterator type for PackedIterator.
        using baseIter_t = deque_t<nsec_t>::const_iterator;
        // Custom iterator used for iterating through all entries of a specific engine loop and source.
        struct PackedIterator : public baseIter_t
        {
            using difference_type = typename baseIter_t::difference_type;
            // The number of normal iterator steps that this iterator uses. Akin to "stride".
            unsigned char step;
            // The end iterator to not step past.
            baseIter_t end;
            PackedIterator(const baseIter_t& iter, unsigned char step, const baseIter_t& iterEnd);
            PackedIterator& operator++(void) noexcept;
            PackedIterator operator++(int) noexcept;
            PackedIterator& operator+=(const difference_type offset) noexcept;
            PackedIterator operator+(const difference_type offset) const noexcept;
            PackedIterator& operator--(void) noexcept;
            PackedIterator operator--(int) noexcept;
            PackedIterator& operator-=(const difference_type offset) noexcept;
            //PackedIterator operator-(const difference_type offset) const noexcept;
            bool operator==(const PackedIterator& other) const noexcept;
            difference_type operator-(const PackedIterator& other) const noexcept;
        };

        // Private Static Functions:
    private:
        /// <summary>
        /// Convenience function for getting the index of a combination of engine loop and source.
        /// </summary>
        /// <param name="engineLoop">The engine loop for the index.</param>
        /// <param name="source">The performance source for the index.</param>
        /// <returns>The index or offset to use in arrays.</returns>
        static unsigned GetOffset(EngineLoop engineLoop, PerformanceSource source);

        // Private Functions:
    private:

        // Private Static Variables:
    private:

        // Private Variables:
    private:
    };

}	// namespace

#pragma warning(pop)
