//------------------------------------------------------------------------------
//
// File Name:	ImGuiSystem.h
// Author(s):	Barend Brink (barend.brink), Connor Lariviere (connor.lariviere)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		DEAR IMGUI system.
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
    class BaseSystem;

    // Typedefs:

    // Class Definition:
    class ImGuiSystem
    {
        // Public Constants and Enums:
    public:
        /// <summary>
        /// ImGui color styles.
        /// </summary>
        enum class ColorStyle
        {
            Dark,
            Light,
            Classic,
        };

        // Constructors/Destructors:
    public:
        ImGuiSystem(void) = delete;

        // Public Static Functions:
    public:
        /// <summary>
        /// Gets the instance of the ImGuiSystem.
        /// </summary>
        /// <returns>A pointer to the system.</returns>
        static BaseSystem* GetInstance(void);

        /// <summary>
        /// Checks if the system is enabled and visible.
        /// </summary>
        /// <returns>True if ImGui is visible, false if hidden.</returns>
        static bool GetEnabled(void);

        /// <summary>
        /// Sets the visibility/enabled state of ImGui.
        /// </summary>
        /// <param name="enable">The state to set ImGui to.</param>
        static void SetEnabled(bool enable);

        /// <summary>
        /// Checks if the keyboard is interacting with an ImGui window.
        /// </summary>
        /// <returns>True if the keyboard is interacting with an ImGui window, false otherwise.</returns>
        static bool IsKeyboardCaptured(void);

        /// <summary>
        /// Checks if the mouse is interacting with an ImGui window.
        /// </summary>
        /// <returns>True if the mouse is interacting with an ImGui window, false otherwise.</returns>
        static bool IsMouseCaptured(void);

        /// <summary>
        /// Sets ImGui text and window size to scale, 1.0 = default size.
        /// </summary>
        /// <param name="scale">The scale to render ImGui at.</param>
        static void SetUiSize(float scale);

        /// <summary>
        /// Sets the style used when rendering ImGui.
        /// </summary>
        /// <param name="style">The style to render ImGui in.</param>
        static void SetUiStyle(ColorStyle style);

        // Public Functions:
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
