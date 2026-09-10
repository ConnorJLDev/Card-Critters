//------------------------------------------------------------------------------
//
// File Name:    CheatSystem.h
// Author(s):    Jason Nguyen
// Course:       GAM200F25
// Project:      Card Critters
// Purpose:      Implementation for cheat shortcuts
//
// Copyright © DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "BaseSystem.h"

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
    class CheatSystem SYSTEM_FINAL : public BaseSystem
    {
        SYSTEM_COMMON(CheatSystem);

        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    private:
        CheatSystem(void);

        // All systems need a virtual destructor to have their destructor called 
        ~CheatSystem(void) override;

        // Public Static Functions:
    public:
        // Get the instance of the CheatSystem.
        static BaseSystem* GetInstance(void);

        // Public Functions:
    public:

        // Public Event Handlers
    public:

        // Private Constants, Enums, and Structs:
    private:
        struct KeyBinding
        {
            char key;
            string_view_t name;
        };

        static const KeyBinding keyBindings[];

        // Private Static Functions:
    private:

        // Private Functions:
    private:
        /// <summary>
        /// Initializes the system.
        /// </summary>
        /// <returns>True if initialization succeeded, false otherwise.</returns>
        bool Init(void) override;

        /// <summary>
        /// Updates the system.
        /// </summary>
        void Update(void) override;

        /// <summary>
        /// Exits and shuts down the system.
        /// </summary>
        void Exit(void) override;

        // Private Static Variables:
    private:
        static CheatSystem* instance;

        // Private Variables:
    private:
    };

}	// namespace
