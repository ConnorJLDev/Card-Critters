//------------------------------------------------------------------------------
//
// File Name:	TurnSystem.h
// Author(s):	barend.brink, Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		System that handles combat turns between parties.
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

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    // Forward Declarations:
    class BehaviorParty;

    // Typedefs:

    // Class Definition:
    class TurnSystem SYSTEM_FINAL : public BaseSystem
    {
        SYSTEM_COMMON(TurnSystem);

        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    private:
        TurnSystem(void);

        // All systems need a virtual destructor to have their destructor called 
        ~TurnSystem(void) override;

        // Public Static Functions:
    public:
        // Get the instance of the TurnSystem.
        static TurnSystem* GetInstance(void);

        // Public Functions:
    public:
        /// <summary>
        /// 
        /// </summary>
        static void RegisterParty(BehaviorParty* party);

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        static void NextTurn();

        /// <summary>
        /// 
        /// </summary>
        /// <param name="party"></param>
        /// <returns></returns>
        static bool IsPartyActive(BehaviorParty* party);

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        static const vector_t<BehaviorParty*>& Parties();

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        static BehaviorParty* ActiveParty();

        /// <summary>
        /// Get the current round count (increments when all parties have had a turn).
        /// </summary>
        static unsigned RoundCount();

        static void ClearParties();

        static void TransitionScene(string_view_arg nextScene, unsigned int index = 0);

        // Public Event Handlers
    public:

        // Private Constants, Enums, and Structs:
    private:

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
        static TurnSystem* instance;

        // Private Variables:
    private:
        unsigned partyActive_, roundCount_;
        bool turnChanging_;
        bool sceneChanging_;
        unsigned int nextSceneIndex_;
        string_t nextScene_;
        vector_t<BehaviorParty*> parties_;
    };

}	// namespace
