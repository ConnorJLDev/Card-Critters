//------------------------------------------------------------------------------
//
// File Name:	RecruitSystem.h
// Author(s):	sabre
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Template class for a new system.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "BaseSystem.h"
#include "BehaviorParty.h"

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
    class RecruitSystem SYSTEM_FINAL : public BaseSystem
    {
        SYSTEM_COMMON(RecruitSystem);

        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    private:
        RecruitSystem(void);

        // All systems need a virtual destructor to have their destructor called 
        ~RecruitSystem(void) override;

        // Public Static Functions:
    public:
        // Get the instance of the RecruitSystem.
        static RecruitSystem* GetInstance(void);

        // Public Functions:
    public:

        // Public Event Handlers
    public:

        // Private Constants, Enums, and Structs:
    private:
        //
        enum RecruitCharacter
        {
            CHARACTERINVALID = -1,
            NERO,
            MILO,
            KATRINA,
        };

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
        /// Exits and shuts down the system.
        /// </summary>
        void Exit(void) override;

        //
        void StartRecruit();

        // Private Static Variables:
    private:
        static bool isRecruiting;

        static RecruitSystem* instance;

        // Private Variables:
    private:
    };

}	// namespace
