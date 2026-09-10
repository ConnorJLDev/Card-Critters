//------------------------------------------------------------------------------
//
// File Name:	MessageSystem.h
// Author(s):	barend.brink
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		System for broadcasting messages to other systems and entities
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

    // Typedefs:

    // Class Definition:
    class MessageSystem SYSTEM_FINAL : public BaseSystem
    {
        SYSTEM_COMMON(MessageSystem);

        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    private:
        MessageSystem(void);

        // All systems need a virtual destructor to have their destructor called 
        ~MessageSystem(void) override;

        // Public Static Functions:
    public:
        // Get the instance of the MessageSystem.
        static MessageSystem* GetInstance(void);

        // Public Functions:
    public:

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
        /// Exits and shuts down the system.
        /// </summary>
        void Exit(void) override;

        // Private Static Variables:
    private:
        static MessageSystem* instance;

        // Private Variables:
    private:
    };

    enum class Message
    {
        IsPlayerTurn, IsEnemyTurn
    };

    class MessageBus
    {
        friend class MessageSystem;

    public:
        using Callback = std::function<void(Message)>;

        static CallbackID Subscribe(Callback cb);
        static void Unsubscribe(CallbackID id);
        static void Broadcast(Message msg);
        static MessageBus* GetInstance(void);

    private:

        MessageBus(void);

        static MessageBus* instance;

        map_t<CallbackID, Callback> Listeners;

        CallbackID NextId;

        bool HistoryEnabled;
        size_t HistoryLimit;
        vector_t<Message> History;
    };

}	// namespace
