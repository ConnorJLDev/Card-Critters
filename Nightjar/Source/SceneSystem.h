//------------------------------------------------------------------------------
//
// File Name:	SceneSystem.h
// Author(s):	Connor Lariviere (connor.lariviere), Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		System that handles scenes.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    // Forward Declarations:
    class BaseSystem;
    class Scene;

    // Typedefs:

    // Class Definition:
    class SceneSystem
    {
        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    private:
        SceneSystem(void) = delete;

        // Public Static Functions:
    public:
        // Get the instance of the SceneSystem.
        static BaseSystem* GetInstance(void);

        // Check if the Scene System is still running (a scene is active).
        // Returns:
        //   bool	True is a scene is active, false otherwise.
        static bool IsRunning(void);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="name"></param>
        /// <returns></returns>
        static Scene* FindScene(string_view_arg name);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="slot"></param>
        /// <returns></returns>
        static Scene* GetScene(unsigned slot = 0);

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        static unsigned GetSceneSlotCount(void);

        /// <summary>
        /// Tell the Scene System to restart an active scene.
        /// </summary>
        /// <param name="slot">The scene slot to restart.</param>
        static void Restart(unsigned slot = 0);

        /// <summary>
        /// Check if a scene is changing.
        /// </summary>
        /// <param name="slot">The scene slot to check if it's changing.</param>
        /// <returns></returns>
        static bool IsChanging(unsigned slot = 0);

        /// <summary>
        /// Set a new active scene, replacing any existing active scene.
        /// </summary>
        /// <param name="name">Name of the new scene.</param>
        /// <param name="index">The scene slot to load into.</param>
        static void SetNext(string_view_arg name, unsigned slot = 0);

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
