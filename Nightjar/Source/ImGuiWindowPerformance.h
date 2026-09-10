//------------------------------------------------------------------------------
//
// File Name:	ImGuiWindowPerformance.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		ImGui window that displays performance information.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiWindowBase.h"
#include "PerformanceEnums.h"
#include "EngineLoop.h"

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

    // Class Definition:
    class ImGuiWindowPerformance : public ImGuiWindowBase
    {
        IMGUI_WINDOW_FRIENDS;

        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    public:
        ImGuiWindowPerformance(void);

        ImGuiWindowPerformance(const ImGuiWindowPerformance& other) = delete;

        ImGuiWindowPerformance(ImGuiWindowPerformance&& other) noexcept;

        ImGuiWindowPerformance& operator=(const ImGuiWindowPerformance& other) = delete;

        ~ImGuiWindowPerformance(void);

        // Public Static Functions:
    public:

        // Public Functions:
    public:

        // Public Event Handlers
    public:

        // Private Constants, Enums, and Structs:
    private:
        // The base name of this window.
        static const string_view_t WindowName;

        // Contains sorting information for each performance group.
        struct GroupSort
        {
            GroupSort(void);
            PerformanceSource source;
            EngineLoop loop;
            enum class SortDir : unsigned char
            {
                None,
                Ascending,
                Descending,
                Default,
            };
            SortDir sortDir;
        };

        // Private Static Functions:
    private:

        // Private Functions:
    private:
        /// <summary>
        /// Console window display implementation.
        /// </summary>
        void DisplayImpl(void) override;

        /// <summary>
        /// Displays the frame timings.
        /// </summary>
        void DisplayFrameTimings(void);

        /// <summary>
        /// Displays the detailed breakdown of the timings of a specific tracker.
        /// </summary>
        /// <param name="id">The ID of the object to display.</param>
        void DisplayTrackerTimingsDetailed(ObjectID id);

        /// <summary>
        /// Displays the timings of a group of performance tracker.
        /// </summary>
        /// <param name="group">The tracker group to display.</param>
        void DisplayTrackerTimingsGroup(PerformanceGroup group);

        /// <summary>
        /// Displays the timings of all performance trackers.
        /// </summary>
        void DisplayTrackerTimingsAll(void);

        /// <summary>
        /// Tries to set the value of a given key.
        /// </summary>
        /// <param name="key">The key of the value to be set.</param>
        /// <param name="value">A string representation of the value to be set.</param>
        /// <returns>Error code detailing what error may have occurred while trying to read.</returns>
        ReadError ReadKeyValue(string_view_arg key, string_view_arg value) override;

        /// <summary>
        /// Finalizes initialization after reading data.
        /// </summary>
        void ReadFinish(void) override;

        /// <summary>
        /// Writes all of this window's persistent data into the given buffer as key-value pairs.
        /// </summary>
        /// <param name="buffer">The buffer to write to.</param>
        void WriteAll(string_t& buffer) override;

        /// <summary>
        /// Displays the menu bar at the top of the window.
        /// </summary>
        //void DisplayMenuBar(void);

        // Private Static Variables:
    private:

        // Private Variables:
    private:
        // Flag for advanced timing section.
        bool headerAdvanced_;
        // Flags for each performance group section.
        array_t<bool, PerformanceGroup::NUM_ENTRIES> headerGroups_;
        // Sorting information for each performance group section.
        array_t<GroupSort, PerformanceGroup::NUM_ENTRIES> groupSorts_;
    };

}	// namespace
