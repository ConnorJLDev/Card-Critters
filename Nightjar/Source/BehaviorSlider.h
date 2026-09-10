//------------------------------------------------------------------------------
//
// File Name:	BehaviorSlider.h
// Author(s):	Andrew Lamb (andrew.lamb)
// Course:		GAM250S26
// Project:		Card Critters
// Purpose:		Header of behavior class for sliders
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "BehaviorInteractable.h"

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
    class BehaviorSlider : public BehaviorInteractable
    {
        // Public Constants, Enums, and Structs:
    public:
        // Common component data.
        COMPONENT_COMMON(BehaviorSlider);

        // Constructors/Destructors:
    public:
        BehaviorSlider(void);

        BehaviorSlider(const BehaviorSlider& other);

        ~BehaviorSlider(void);

        // Public Static Functions:
    public:
        static const StreamKey ReadFunction;
        static const StreamKey ReadTitle;

        // Public Functions:
    public:
        /// <summary>
        /// Clone the BehaviorSlider.
        /// </summary>
        /// <returns>A clone of this BehaviorSlider.</returns>
        BehaviorSlider* Clone() const override;

        /// <summary>
        /// Read BehaviorSlider data from a stream.
        /// </summary>
        /// <param name="stream">The stream to read from.</param>
        void Read(StreamIn& stream) override;

        /// <summary>
        /// Writes the BehaviorSlider data into the stream.
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
                // BehaviorSlider has not yet been initialized.
                cInvalid = -1,
                // BehaviorSlider is idle.
                cIdle,
            };
        };

        // Private Static Functions:
    private:

        // Private Functions:
    private:
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
        /// Callback when the mouse enters the collider.
        /// </summary>
        void OnHoverStart() override;

        /// <summary>
        /// Callback when the mouse exits the collider.
        /// </summary>
        void OnHoverEnd() override;

        /// <summary>
        /// Callback when a mouse button is pressed while in the collider.
        /// </summary>
        void OnClickStart() override;

        /// <summary>
        /// Callback when a mouse button is released while in the collider.
        /// </summary>
        /// <param name="contained">True if the click stayed entirely contained within the collider, false otherwise.</param>
        void OnClickEnd(bool contained) override;

        // Private Static Variables:
    private:

        // Private Variables:
    private:
        double _minVal;
        double _maxVal;
        double _currVal;
        double _interval;
        double _scale;
        bool _isClicked;
        string_t _storeValString;
        std::function<void(float)> _storeVal;
        double _parentX;
        Entity* _knob;
        Entity* _titleText;
        Entity* _valueText;
        string_t _titleString;
    };

}	// namespace
