//------------------------------------------------------------------------------
//
// File Name:	ImGuiToolbar.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Manages items on the ImGui toolbar at the top of the screen.
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
    class ImGuiToolbar
    {
        friend class ImGuiSystem_;

        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    private:
        ImGuiToolbar(void);
    public:
        ~ImGuiToolbar(void);

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
            Center,
            Right,
        };

        //
        struct ToolbarEntry
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
        /// Initializes the toolbar.
        /// </summary>
        /// <returns>True if initialization was successful, false otherwise.</returns>
        static bool Init(void);

        /// <summary>
        /// Shuts down the toolbar.
        /// </summary>
        static void Exit(void);

        // Private Functions:
    private:
        /// <summary>
        /// Updates and displays the toolbar.
        /// </summary>
        bool Update(void);

        // Private Static Variables:
    private:
        // The instance of the toolbar.
        static ImGuiToolbar* Toolbar;

        // Private Variables:
    private:
        //
        vector_t<ToolbarEntry> tools_;
    };

}	// namespace
