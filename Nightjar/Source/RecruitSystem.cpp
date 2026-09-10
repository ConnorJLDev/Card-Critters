//------------------------------------------------------------------------------
//
// File Name:	RecruitSystem.cpp
// Author(s):	sabre
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Template class for a new system.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "RecruitSystem.h"
#include "BaseSystem.h"

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

    RecruitSystem* RecruitSystem::instance = nullptr;

    bool RecruitSystem::isRecruiting = false;

    //--------------------------------------------------------------------------
    // Private Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Constructors/Destructors:
    //--------------------------------------------------------------------------

#pragma region Constructors

    RecruitSystem::RecruitSystem(void)
        : BaseSystem("RecruitSystem", LoopFlags::None)
    {
        // Raise an assert if this system has already been created.
        assert(instance == nullptr);

        // Store this system's instance for use by static functions.
        instance = this;
    }

    //--------------------------------------------------------------------------

    RecruitSystem::~RecruitSystem(void)
    {
    }

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

    RecruitSystem* RecruitSystem::GetInstance(void)
    {
        return instance != nullptr ? instance : new RecruitSystem();
    }

#pragma endregion Public Static Functions

    //--------------------------------------------------------------------------
    // Public Functions:
    //--------------------------------------------------------------------------

#pragma region Public Functions

#pragma endregion Public Functions

    //--------------------------------------------------------------------------
    // Private Static Functions:
    //--------------------------------------------------------------------------

#pragma region Private Static Functions

#pragma endregion Private Static Functions

    //--------------------------------------------------------------------------
    // Private Functions:
    //--------------------------------------------------------------------------

#pragma region Private Functions

    bool RecruitSystem::Init(void)
    {
        
        return true;
    }

    void RecruitSystem::StartRecruit()
    {
        //Set party to recruit state
        //Put character that will join in the enemy party
        //RECRUIT DIALOUGE (RANDOMLY CHOSEN)
        //Add character to party
    }

    void RecruitSystem::Exit(void)
    {

    }

#pragma endregion Private Functions

}	// namespace
