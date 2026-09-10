//------------------------------------------------------------------------------
//
// File Name:	ImGuiMenuBar.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Manages items and menus on the ImGui main menu bar.
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
    class ImGuiMenuBar
    {
        friend class ImGuiSystem_;

        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    private:
        ImGuiMenuBar(void);
    public:
        ~ImGuiMenuBar(void);

        // Public Static Functions:
    public:

        // Public Functions:
    public:

        // Public Event Handlers
    public:

        // Private Constants, Enums, and Structs:
    private:
        //
        enum class Alignment : unsigned char
        {
            Left,
            Right,
        };

        //
        struct MenuEntry
        {
            string_view_t name;
            std::function<void(void)> callback;
            bool active;
            Alignment alignment;
            int sortIndex;
        };

        // Private Static Functions:
    private:
        /// <summary>
        /// Initializes the menu bar.
        /// </summary>
        /// <returns>True if initialization was successful, false otherwise.</returns>
        static bool Init(void);

        /// <summary>
        /// Shuts down the menu bar.
        /// </summary>
        static void Exit(void);

        /// <summary>
        /// 
        /// </summary>
        static void WindowsMenu(void);

        // Private Functions:
    private:
        /// <summary>
        /// Updates and displays the menu bar.
        /// </summary>
        bool Update(void);
        
        // Private Static Variables:
    private:
        // The instance of the menu bar.
        static ImGuiMenuBar* MenuBar;

        // Private Variables:
    private:
        //
        vector_t<MenuEntry> menus_;
    };

}	// namespace
