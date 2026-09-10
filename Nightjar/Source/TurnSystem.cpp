//------------------------------------------------------------------------------
//
// File Name:	TurnSystem.cpp
// Author(s):	barend.brink, Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		System that handles combat turns between parties.
//
// Copyright ? 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Systems.h"
#include "BehaviorParty.h"
#include "TurnSystem.h"
#include "Scene.h"
#include "Entity.h"
#include "BehaviorTransition.h"

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

    TurnSystem* TurnSystem::instance = nullptr;

    //--------------------------------------------------------------------------	
    // Private Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------	
    // Constructors/Destructors:
    //--------------------------------------------------------------------------

#pragma region Constructors

    TurnSystem::TurnSystem(void)
        : BaseSystem("TurnSystem", LoopFlags::HasUpdate)
        , partyActive_(0)
        , roundCount_(0)
        , turnChanging_(false)
        , parties_()
    {
        // Raise an assert if this system has already been created.
        assert(instance == nullptr);

        // Store this system's instance for use by static functions.
        instance = this;
    }

    //--------------------------------------------------------------------------

    TurnSystem::~TurnSystem(void)
    {
    }

#pragma endregion Constructors

    //--------------------------------------------------------------------------	
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

    TurnSystem* TurnSystem::GetInstance(void)
    {
        return instance != nullptr ? instance : new TurnSystem();
    }

    void TurnSystem::RegisterParty(BehaviorParty* party)
    {
        auto iter = std::find(instance->parties_.begin(), instance->parties_.end(), party);
        assert(iter == instance->parties_.end());
        instance->parties_.push_back(party);
    }

    void TurnSystem::NextTurn()
    {
        if (instance->turnChanging_)
        {
            return;
        }
        instance->turnChanging_ = true;
        ++instance->partyActive_;
        if (instance->partyActive_ >= instance->parties_.size())
        {
            instance->partyActive_ = 0;
            ++instance->roundCount_;
        }
        for (unsigned i = 0; i < instance->parties_.size(); ++i)
        {
            instance->parties_[i]->SetActive(i == instance->partyActive_);
        }
    }

    bool TurnSystem::IsPartyActive(BehaviorParty* party)
    {
        auto iter = std::find(instance->parties_.begin(), instance->parties_.end(), party);
        // If the party isn't found, iter is the end, which is the vector size, which is never the index of the active party.
        //assert(iter != instance->parties_.end());
        unsigned iterIndex = (unsigned)(iter - instance->parties_.begin());
        return iterIndex == instance->partyActive_;
    }

    const vector_t<BehaviorParty*>& TurnSystem::Parties()
    {
        return instance->parties_;
    }

    BehaviorParty* TurnSystem::ActiveParty()
    {
        return instance->partyActive_ >= instance->parties_.size() ? nullptr : instance->parties_[instance->partyActive_];
    }

    unsigned TurnSystem::RoundCount()
    {
        return instance ? instance->roundCount_ : 0u;
    }

    void TurnSystem::ClearParties()
    {
        instance->roundCount_ = 0;
        instance->partyActive_ = 0;
        instance->parties_.clear();
    }

    void TurnSystem::TransitionScene(string_view_arg nextScene, unsigned int index)
    {
        if (SceneSystem::GetScene(4))
        {
            if (SceneSystem::GetScene(4)->GetName() == "Transition")
                return;
        }
        instance->nextScene_ = nextScene;
        instance->sceneChanging_ = true;
        instance->nextSceneIndex_ = index;
        if (SceneSystem::GetScene(1))
        {
            if (SceneSystem::GetScene(1)->GetName() != "Transition")
                SceneSystem::SetNext("Transition", 4);
        }
        else
            SceneSystem::SetNext("Transition", 4);
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

    bool TurnSystem::Init(void)
    {
        // Add system-specific initialization code here.

        // Return true if the initialization completed successfully.
        return true;
    }

    void TurnSystem::Update(void)
    {
        turnChanging_ = false;

        if (sceneChanging_)
        {
            Scene* fade = SceneSystem::GetScene(4);
            if (fade != nullptr)
            {
                Entity* fadeEntity = fade->FindEntity("Fade");
                if (fadeEntity)
                {
                    BehaviorTransition* fadeTransition = fadeEntity->HasSub(Behavior, BehaviorTransition);
                    if (fadeTransition)
                    {
                        fadeTransition->SetNextScene(nextScene_, nextSceneIndex_);
                    }
                }
                sceneChanging_ = false;
            }
        }

        if (SceneSystem::IsChanging())
        {
            ClearParties();
        }
    }

    void TurnSystem::Exit(void)
    {
    }

#pragma endregion Private Functions

}	// namespace
