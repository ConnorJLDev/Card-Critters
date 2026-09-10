//------------------------------------------------------------------------------
//
// File Name:	ImGuiStatusBar.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Manages items on the ImGui status bar at the bottom of the
//              screen.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiAlignedBar.h"

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
    class ImGuiStatusBar
    {
        friend class ImGuiSystem_;

        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    private:
        ImGuiStatusBar(void);
    public:
        ~ImGuiStatusBar(void);

        // Public Static Functions:
    public:

        // Public Functions:
    public:

        // Public Event Handlers
    public:

        // Private Constants, Enums, and Structs:
    private:

        // Private Static Functions:
    private:
        /// <summary>
        /// Initializes the status bar.
        /// </summary>
        /// <returns>True if initialization was successful, false otherwise.</returns>
        static bool Init(void);

        /// <summary>
        /// Shuts down the status bar.
        /// </summary>
        static void Exit(void);

        //
        static void DisplayFPS(void);

        //
        static void DisplayCenter(void);

        //
        static void DisplayOpenScenes(void);

        // Private Functions:
    private:
        /// <summary>
        /// Updates and displays the status bar.
        /// </summary>
        bool Update(void);

        // Private Static Variables:
    private:
        // The instance of the status bar.
        static ImGuiStatusBar* StatusBar;

        // Private Variables:
    private:
        //
        ImGuiAlignedBar statuses_;
        // 
        string_t scenes_;
    };

}	// namespace
