//------------------------------------------------------------------------------
//
// File Name:    CheatSystem.cpp
// Author(s):    Jason Nguyen
// Course:       GAM200F25
// Project:      Card Critters
// Purpose:      Implementation for cheat shortcuts.
//
// Copyright © DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "CheatSystem.h"
#include "SceneSystem.h"
#include "InputSystem.h"
#include "PlatformSystem.h"
#include "ImGuiSystem.h"
#include "Entity.h"
#include "EntityContainer.h"
#include "Behavior.h"
#include "TurnSystem.h"
#include "BehaviorParty.h"
#include "Scene.h"

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

    constexpr CheatSystem::KeyBinding CheatSystem::keyBindings[] = {
        { '1', "Level1" },
        { '2', "Level2" },
        { '3', "Level3"},
        { '4', "Recruit"},
        { '5', "MainMenu"},
        {'6', "Level3D"},
        {'7', "Lose"},
        {'8', "Win"},
        {'9', "DigipenSplash"},
        {'0', "Settings"},
    };

    //--------------------------------------------------------------------------
    // Private Static Variables:
    //--------------------------------------------------------------------------

    CheatSystem* CheatSystem::instance = nullptr;

    //--------------------------------------------------------------------------
    // Private Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Constructors/Destructors:
    //--------------------------------------------------------------------------

#pragma region Constructors

    CheatSystem::CheatSystem(void)
        : BaseSystem("CheatSystem", LoopFlags::HasUpdate)
    {
        // Raise an assert if this system has already been created.
        assert(instance == nullptr);

        // Store this system's instance for use by static functions.
        instance = this;
    }

    //--------------------------------------------------------------------------

    CheatSystem::~CheatSystem(void)
    {
    }

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

    BaseSystem* CheatSystem::GetInstance(void)
    {
        return instance != nullptr ? instance : new CheatSystem();
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

    bool CheatSystem::Init(void)
    {
        // Add system-specific initialization code here.

        // Return true if the initialization completed successfully.
        return true;
    }

    void CheatSystem::Update(void)
    {
        // TODO: Move pause key and fullscreen key detection outside of CheatSystem.
        if (InputSystem::KeyIsTriggered(B_Keyboard::Escape))
        {
            if (SceneSystem::GetScene(0))
            {
                string_t name = SceneSystem::GetScene(0)->GetName();
                if (name == "MainMenu" || name == "Credits" || name == "Settings")
                    return;
            }
            if (!SceneSystem::GetScene(4))
            {
                if (SceneSystem::GetScene(1))
                {
                    if (SceneSystem::GetScene(1)->GetName() == "Pause")
                        SceneSystem::SetNext("", 1);
                }
                else
                    SceneSystem::SetNext("Pause", 1);
                
                if (SceneSystem::GetScene(3))
                {
                    if (SceneSystem::GetScene(3)->GetName() == "QuitConfirmation")
                    {
                        SceneSystem::SetNext("", 3);
                    }
                }
            }
        }

        if (InputSystem::KeyIsTriggered(B_Keyboard::F11))
        {
            PlatformSystem::SetFullscreen(!PlatformSystem::GetFullscreen());
        }

        if ((InputSystem::KeyIsHeld(B_Keyboard::LeftControl) || InputSystem::KeyIsHeld(B_Keyboard::RightControl)) && InputSystem::KeyIsTriggered(B_Keyboard::Grave))
        {
            ImGuiSystem::SetEnabled(!ImGuiSystem::GetEnabled());
        }

        //Check if there is a scene in slot 1, if there is set next scene to empty string to delete it, if not then set next scene in slot 1 to the pause scene

        if (InputSystem::KeyIsHeld(B_Keyboard::Grave))
        {
            //TODO: Make the recruit system an actual scene instead of having a manual key

            if (InputSystem::KeyIsTriggered(B_Keyboard::L))
            {
                SceneSystem::SetNext("Recruit");
            }

            for (unsigned i = 0; i < _countof(keyBindings); i++) //switches scenes
            {
                if (InputSystem::KeyIsTriggered(keyBindings[i].key))
                {
                    SceneSystem::SetNext(keyBindings[i].name);
                    //printf("Cheat activated: Switch to scene '{}'\n", newScene->GetName());
                    break;
                }
            }
    
            if (InputSystem::KeyIsTriggered('I')) //kills enemy party slowly
            {
                const vector_t<BehaviorParty*>& partys = TurnSystem::Parties();

                for (unsigned int i = 0; i < partys.size(); i++)
                {
                    if (partys[i]->playerControlled_ == false)
                    {
                        for (unsigned int j = 0; j < partys[i]->characters_.size(); j++)
                        {
                            partys[i]->characters_[j]->ModifyHealth(-10);
                        }
                    }
                }
            }
            if (InputSystem::KeyIsTriggered('O')) //Kills party slowly
            {
                const vector_t<BehaviorParty*>& partys = TurnSystem::Parties();

                for (unsigned int i = 0; i < partys.size(); i++)
                {
                    if (partys[i]->playerControlled_ == true)
                    {
                        for (unsigned int j = 0; j < partys[i]->characters_.size(); j++)
                        {
                            partys[i]->characters_[j]->ModifyHealth(-10);
                        }
                    }
                }
            }
            if (InputSystem::KeyIsTriggered('P')) //Kills party slowly
            {
                const vector_t<BehaviorParty*>& partys = TurnSystem::Parties();

                for (unsigned int i = 0; i < partys.size(); i++)
                {
                    if (partys[i]->playerControlled_ == true)
                    {
                        for (unsigned int j = 0; j < partys[i]->characters_.size(); j++)
                        {
                            partys[i]->characters_[j]->SetHealth(99999999);
                        }
                    }
                }
            }
        }
        
    }

    void CheatSystem::Exit(void)
    {
    }

#pragma endregion Private Functions

}	// namespace
