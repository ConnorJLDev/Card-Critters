//------------------------------------------------------------------------------
//
// File Name:	MessageSystem.cpp
// Author(s):	barend.brink
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		System for broadcasting messages to other systems and entities
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "MessageSystem.h"
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
    // Private Static Variables:
    //--------------------------------------------------------------------------

    MessageSystem* MessageSystem::instance = nullptr;

    MessageBus* MessageBus::instance = nullptr;

    //--------------------------------------------------------------------------
    // Private Constants:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Private Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Constructors/Destructors:
    //--------------------------------------------------------------------------

#pragma region Constructors

    MessageSystem::MessageSystem(void)
        : BaseSystem("MessageSystem", LoopFlags::None)
    {
        // Raise an assert if this system has already been created.
        assert(instance == nullptr);

        // Store this system's instance for use by static functions.
        instance = this;
    }

    MessageBus::MessageBus(void)
        : Listeners()
        , NextId()
        , HistoryEnabled(false)
        , HistoryLimit(100)
        , History()
    {
        // Raise an assert if this system has already been created.
        assert(instance == nullptr);

        // Store this system's instance for use by static functions.
        instance = this;
    }

    //--------------------------------------------------------------------------

    MessageSystem::~MessageSystem(void)
    {
        SafeDeleteCheck(MessageBus::instance);
    }

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

    MessageSystem* MessageSystem::GetInstance(void)
    {
        return instance != nullptr ? instance : new MessageSystem();
    }

    MessageBus* MessageBus::GetInstance(void)
    {
        return instance != nullptr ? instance : new MessageBus();
    }

#pragma endregion Public Static Functions

    //--------------------------------------------------------------------------
    // Public Functions:
    //--------------------------------------------------------------------------

#pragma region Public Functions

    CallbackID MessageBus::Subscribe(Callback cb)
    {
        instance->Listeners[instance->NextId] = cb;
        return instance->NextId++;
    }

    void MessageBus::Unsubscribe(CallbackID id)
    {
        auto i = instance->Listeners.find(id);

        if (i != instance->Listeners.end())
        {
            instance->Listeners.erase(i);
        }

        return;
    }

    void MessageBus::Broadcast(Message msg)
    {
        for (auto i : instance->Listeners)
        {
            i.second(msg);
        }
    }


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

    bool MessageSystem::Init(void)
    {
        // Add system-specific initialization code here.
        MessageBus::GetInstance();

        // Return true if the initialization completed successfully.
        return true;
    }

    void MessageSystem::Exit(void)
    {
    }

#pragma endregion Private Functions

}	// namespace
