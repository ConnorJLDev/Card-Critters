//------------------------------------------------------------------------------
//
// File Name:	$safeitemname$.h
// Author(s):	$username$
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

//------------------------------------------------------------------------------
// Preprocessor Directives:
//------------------------------------------------------------------------------

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
    class $safeitemname$ COMPONENT_FINAL : public Behavior
    {
        // Public Constants, Enums, and Structs:
    public:
        // Common component data.
        COMPONENT_COMMON($safeitemname$);

        // Constructors/Destructors:
    public:
        $safeitemname$(void);

        $safeitemname$(const $safeitemname$& other);

        ~$safeitemname$(void);

        // Public Static Functions:
    public:

        // Public Functions:
    public:
        /// <summary>
        /// Clone the $safeitemname$.
        /// </summary>
        /// <returns>A clone of this $safeitemname$.</returns>
        $safeitemname$* Clone(void) const override;

        /// <summary>
        /// Read $safeitemname$ data from a stream.
        /// </summary>
        /// <param name="stream">The stream to read from.</param>
        void Read(StreamIn& stream) override;

        /// <summary>
        /// Writes the $safeitemname$ data into the stream.
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
                // $safeitemname$ has not yet been initialized.
                Invalid = -1,
                // $safeitemname$ is idle.
                Idle,
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
        void OnInit(void) override;

        /// <summary>
        /// Called when the base behavior updates.
        /// </summary>
        /// <param name="dt">The time elapsed since the previous tick.</param>
        void OnUpdate(float dt) override;

        /// <summary>
        /// Called on exiting an old state.
        /// </summary>
        void OnExit(void) override;

        /// <summary>
        /// Callback for when the parent entity of this behavior experiences a collision.
        /// </summary>
        /// <param name="record">The record of the collision.</param>
        void CollisionHandler(const CollisionRecord& record);

        // Private Static Variables:
    private:

        // Private Variables:
    private:
    };

}	// namespace
