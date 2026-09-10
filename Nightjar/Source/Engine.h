//------------------------------------------------------------------------------
//
// File Name:	Engine.h
// Author(s):	Doug Schilling (dschilling), Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Main engine.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Systems.inl"
#include "EngineLoop.h"
#include "BaseSystem.h"

//------------------------------------------------------------------------------
// Preprocessor Directives:
//------------------------------------------------------------------------------

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
    class Engine
    {
        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    public:
        Engine(void) = delete;

        // Public Static Functions:
    public:
        /// <summary>
        /// Initialize the engine.
        /// </summary>
        /// <returns>True if all systems have been initialized properly, false otherwise.</returns>
        static bool Init(void);

        /// <summary>
        /// Update all systems within the engine.
        /// </summary>
        static void Update(void);

        /// <summary>
        /// Render all systems within the engine.
        /// </summary>
        //static void Render(void);

        /// <summary>
        /// Shutdown the engine.
        /// </summary>
        static void Exit(void);

        /// <summary>
        /// Checks if the engine should begin exiting and shutting down.
        /// </summary>
        /// <returns>True if the engine should exit, false otherwise.</returns>
        static bool ShouldExit(void);

        /// <summary>
        /// Gets a system that is in the engine.
        /// </summary>
        /// <param name="system">The enum of the system to get.</param>
        /// <returns>A const reference to the system.</returns>
        static const BaseSystem& GetSystem(Systems system);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="loop"></param>
        /// <returns></returns>
        static bool GetLoopEnabled(EngineLoop loop);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="loop"></param>
        /// <param name="enabled"></param>
        static void SetLoopEnabled(EngineLoop loop, bool enabled);

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

        // Private Static Variables:
    private:

        // Private Variables:
    private:
    };

}	// namespace
