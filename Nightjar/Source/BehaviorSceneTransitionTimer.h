//------------------------------------------------------------------------------
//
// File Name:	BehaviorSceneTransitionTimer.h
// Author(s):	barend.brink
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Template class for a new behavior.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Behavior.h"
#include <vector>
#include <string>

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    // Forward Declarations:
    class Entity;
    struct CollisionRecord;

    // Typedefs:

    // Class Definition:
    class BehaviorSceneTransitionTimer : public Behavior
    {
        // Public Constants, Enums, and Structs:
    public:
        // Common component data.
        COMPONENT_COMMON(BehaviorSceneTransitionTimer);

        // Constructors/Destructors:
    public:
        BehaviorSceneTransitionTimer(void);

        BehaviorSceneTransitionTimer(const BehaviorSceneTransitionTimer& other);

        ~BehaviorSceneTransitionTimer(void);

        // Public Static Functions:
    public:

        // Public Functions:
    public:
        /// <summary>
        /// Clone the BehaviorSceneTransitionTimer.
        /// </summary>
        /// <returns>A clone of this BehaviorSceneTransitionTimer.</returns>
        BehaviorSceneTransitionTimer* Clone() const override;

        /// <summary>
        /// Read BehaviorSceneTransitionTimer data from a stream.
        /// </summary>
        /// <param name="stream">The stream to read from.</param>
        void Read(StreamIn& stream) override;

        /// <summary>
        /// Writes the BehaviorSceneTransitionTimer data into the stream.
        /// </summary>
        /// <param name="stream">The stream to write into.</param>
        void Write(StreamOut& stream) override;

        // Public Event Handlers
    public:

        // Private Constants, Enums, and Structs:
    private:
        // Struct that wraps the states enum to force scoping, while allowing implicit conversions with its underlying type.
        struct States
        {
            enum _States : int
            {
                // BehaviorSceneTransitionTimer has not yet been initialized.
                cInvalid = -1,
                // BehaviorSceneTransitionTimer is idle.
                cIdle,
            };
        };

        // Private Static Functions:
    private:

        // Private Functions:
    private:
        /// <summary>
        /// Called on the first frame this behavior is updated (typically from an Invalid state to an Idle state).
        /// </summary>
        void OnStartup(void) override;

        /// <summary>
        /// Called on initializing/entering a new state.
        /// </summary>
        void OnInit() override;

        /// <summary>
        /// Called when the base behavior updates.
        /// </summary>
        /// <param name="dt">The time elapsed since the previous tick.</param>
        void OnUpdate(float dt) override;

        /// <summary>
        /// Called on exiting an old state.
        /// </summary>
        void OnExit() override;

        /// <summary>
        /// Callback for when the parent entity of this behavior experiences a collision.
        /// </summary>
        /// <param name="record">The record of the collision.</param>
        void CollisionHandler(const CollisionRecord& record);

        void OnTimerEnd();

        static const StreamKey ReadKeyTime, ReadKeyTimerEnd, ReadKeyNextSceneName;

        // Private Static Variables:
    private:

        // Private Variables:
    private:
        float Time;
        float TimerEnd;
        string_t NextSceneName;
    };

}	// namespace
