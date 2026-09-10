//------------------------------------------------------------------------------
//
// File Name:	Main.cpp
// Author(s):	Doug Schilling (dschilling), Jonathan Sandquist (jonathan.s)
// Course:		GAM200
// Project:		Card Critters
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Engine.h"

using Nightjar::Engine;

//------------------------------------------------------------------------------
// Main Function:
//------------------------------------------------------------------------------

#ifdef _DEBUG

int main([[maybe_unused]] int argc, [[maybe_unused]] const char** argv)
{

#else

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
int WINAPI wWinMain([[maybe_unused]] HINSTANCE instance, [[maybe_unused]] HINSTANCE prevInstance, [[maybe_unused]] PWSTR pCmdLine, [[maybe_unused]] int cmdShow)
{

#endif

    //--------------------------------------------------------------------------
    // Initialize the engine, adding all systems to the engine framework.
    //--------------------------------------------------------------------------

    // Initialize the game engine.
    bool running = Engine::Init();

    //--------------------------------------------------------------------------
    // MAIN LOOP: Do not exit until instructed to do so by the engine.
    //--------------------------------------------------------------------------
    while (running)
    {
        // Update the current frame.
        Engine::Update();

        // Check if the engine should exit.
        running = !Engine::ShouldExit();
    }

    // Shutdown the game engine.
    Engine::Exit();

    // Return error code 0 ("no error").
    return 0;
}

//------------------------------------------------------------------------------
