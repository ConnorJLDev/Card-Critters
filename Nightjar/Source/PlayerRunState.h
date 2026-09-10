#pragma once
//------------------------------------------------------------------------------
//
// File Name:	PlayerRunState.h
// Author(s):	Connor Lariviere
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Behavior for hand entity
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
#include <string>
#include <vector>
//------------------------------------------------------------------------------



namespace Nightjar
{
    struct PlayerRunState
    {
        static std::vector<std::string>& PartyNames()
        {
            static std::vector<std::string> names;
            return names;
        }

        static void Clear()
        {
            PartyNames().clear();
        }

        static bool HasParty()
        {
            return !PartyNames().empty();
        }
    };
}