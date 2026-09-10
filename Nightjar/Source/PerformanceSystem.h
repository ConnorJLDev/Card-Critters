//------------------------------------------------------------------------------
//
// File Name:	PerformanceSystem.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		System that gathers and processes performance information.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "EngineLoop.h"
#include "PerformanceEnums.h"
#include "Systems.inl"
#include "PerformanceTracker.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    // Forward Declarations:
    class BaseSystem;

    // Typedefs:

    // Class Definition:
    class PerformanceSystem
    {
        friend class Engine;
        friend class Engine_;

        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    private:
        PerformanceSystem(void) = delete;

        // Public Static Functions:
    public:
        /// <summary>
        /// Gets the instance of the PerformanceSystem.
        /// </summary>
        /// <returns>A pointer to the system.</returns>
        static BaseSystem* GetInstance(void);

        /// <summary>
        /// Gets the performance tracker for the frame times.
        /// </summary>
        /// <returns>The performance tracker for the frame times.</returns>
        static PerformanceTracker& GetFrameTime(void);

        /// <summary>
        /// Gets the current number of trackers in a given performance group.
        /// </summary>
        /// <param name="group">The performance group to get the tracker count of.</param>
        /// <returns>The current number of trackers in the performance group.</returns>
        static size_t GetTrackerCount(PerformanceGroup group, EngineLoop engineLoop);

        /// <summary>
        /// Sets the current number of trackers in a given performance group.
        /// </summary>
        /// <param name="group">The performance group to set the tracker count of.</param>
        /// <param name="count">The new tracker count to set the group to.</param>
        static void SetTrackerCount(PerformanceGroup group, size_t count);

        /// <summary>
        /// Sets the current number of trackers in a given performance group.
        /// </summary>
        /// <param name="group">The performance group to set the tracker count of.</param>
        /// <param name="engineLoop">The engine loop to set the tracker count of.</param>
        /// <param name="count">The new tracker count to set the group to.</param>
        static void SetTrackerCount(PerformanceGroup group, EngineLoop engineLoop, size_t count);

        /// <summary>
        /// Ensures the minimum number of trackers a given performance group has.
        /// </summary>
        /// <param name="group">The performance group to ensure the minimum tracker count of.</param>
        /// <param name="count">The new tracker count to set the group to.</param>
        static void ReserveTrackerCount(PerformanceGroup group, size_t count);

        /// <summary>
        /// Ensures the minimum number of trackers a given performance group has.
        /// </summary>
        /// <param name="group">The performance group to ensure the minimum tracker count of.</param>
        /// <param name="engineLoop">The engine loop to ensure the minimum tracker count of.</param>
        /// <param name="count">The new tracker count to set the group to.</param>
        static void ReserveTrackerCount(PerformanceGroup group, EngineLoop engineLoop, size_t count);

        /// <summary>
        /// Gets a tracker for the requested object.
        /// </summary>
        /// <param name="obj">The object to get the tracker of.</param>
        /// <returns>The tracker of the object.</returns>
        static PerformanceTracker& GetTracker(const Object& obj);

        /// <summary>
        /// Gets or makes a tracker for the requested object.
        /// </summary>
        /// <param name="obj">The object to get the tracker of.</param>
        /// <param name="parent">The parent of the object.</param>
        /// <returns>The tracker of the object.</returns>
        static PerformanceTracker& GetTracker(const NamedObject& obj, const Object& parent);

        /// <summary>
        /// Gets or makes a tracker for the requested object.
        /// </summary>
        /// <param name="obj">The object to get the tracker of.</param>
        /// <param name="name">The name of the object.</param>
        /// <param name="parent">The parent of the object.</param>
        /// <returns>The tracker of the object.</returns>
        static PerformanceTracker& GetTracker(const Object& obj, string_view_arg name, const Object& parent);

        /// <summary>
        /// Assigns the order of when an object was recorded in a specific group and engine loop.
        /// </summary>
        /// <param name="obj">The object whose order is being assigned.</param>
        /// <param name="group">The performance group the assignment is in.</param>
        /// <param name="engineLoop">The engine loop the assignment is in.</param>
        /// <param name="index">The index of the object in the specified group and engine loop.</param>
        static void AssignTrackerOrder(const Object& obj, PerformanceGroup group, EngineLoop engineLoop, size_t index);

        /// <summary>
        /// Gets the tracker for the Engine.
        /// </summary>
        /// <returns>The tracker used for the Engine.</returns>
        static PerformanceTracker& GetEngineTracker(void);

        /// <summary>
        /// Gets the tracker for a given system.
        /// </summary>
        /// <param name="system">The enum of the system to get the tracker of.</param>
        /// <returns>The tracker for the requested system.</returns>
        static PerformanceTracker& GetSystemTracker(Systems system);

        /// <summary>
        /// Gets a tracker by index of the group and engine loop it was recorded in.
        /// </summary>
        /// <param name="group">The group of the tracker.</param>
        /// <param name="engineLoop">The engine loop of the tracker.</param>
        /// <param name="index">The index of when the tracker was recorded.</param>
        /// <returns>A const reference to the requested tracker.</returns>
        static const PerformanceTracker& GetTrackerOrdered(PerformanceGroup group, EngineLoop engineLoop, size_t index);

        // Public Functions:
    public:

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
    };

}	// namespace
