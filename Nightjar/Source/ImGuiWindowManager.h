//------------------------------------------------------------------------------
//
// File Name:	ImGuiWindowManager.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Manages ImGui window creation, destruction, and access.
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
    class ImGuiWindowBase;

    // Typedefs:

    // Class Definition:
    class ImGuiWindowManager
    {
        friend class ImGuiSystem_;
        friend class ImGuiLayoutManager;
        friend class ImGuiMenuBar;

        // Public Constants, Enums, and Structs:
    public:
        // Type used for indexing windows.
        using WindowIndex_t = unsigned int;

        // Invalid window index.
        static constexpr WindowIndex_t InvalidWindowIndex = std::numeric_limits<WindowIndex_t>::max();

        // Constructors/Destructors:
    private:
        ImGuiWindowManager(void);
    public:
        ~ImGuiWindowManager(void);

        // Public Static Functions:
    public:
        /// <summary>
        /// Attempts to create a new window of the given named window type.
        /// </summary>
        /// <param name="windowName">The type name of the window to create.</param>
        /// <returns>A pointer to the window if creation was successful, nullptr otherwise.</returns>
        static ImGuiWindowBase* CreateWindow(string_view_arg windowName);

        /// <summary>
        /// Gets a window by name.
        /// </summary>
        /// <param name="windowName">The type name of the window to get.</param>
        /// <returns>A pointer to the window if found, nullptr otherwise.</returns>
        static ImGuiWindowBase* GetWindow(string_view_arg windowName);

        /// <summary>
        /// Gets a window by index.
        /// </summary>
        /// <param name="index">The index of the window to get.</param>
        /// <returns>A pointer to the window if found, nullptr otherwise.</returns>
        static ImGuiWindowBase* GetWindow(WindowIndex_t index);

        /// <summary>
        /// Destroys a window.
        /// </summary>
        /// <param name="window">The window to destroy.</param>
        /// <returns>True if the window was succesfully destroyed, false otherwise.</returns>
        static bool DestroyWindow(ImGuiWindowBase* window);

        // Public Functions:
    public:

        // Public Event Handlers
    public:

        // Private Constants, Enums, and Structs:
    private:
        // Key names for writing active windows.
        static const string_view_t DockActiveWindowsName, DockWindowKeyName;

        // Private Static Functions:
    private:
        /// <summary>
        /// Initializes the window manager.
        /// </summary>
        /// <returns>True if initialization was successful, false otherwise.</returns>
        static bool Init(void);

        /// <summary>
        /// Shuts down the window manager.
        /// </summary>
        static void Exit(void);

        /// <summary>
        /// Initializes the window selection when loading from settings.
        /// </summary>
        static void InitWindowSelection(void);

        /// <summary>
        /// Creates a new window with the given window type name and index. Only for internal use.
        /// </summary>
        /// <param name="windowName">The type name of the window to create.</param>
        /// <param name="index">The index to assign to the window.</param>
        /// <returns>A pointer to the window if creation was successful, nullptr otherwise.</returns>
        static ImGuiWindowBase* CreateWindow(string_view_arg windowName, WindowIndex_t index);

        // Private Functions:
    private:
        /// <summary>
        /// Updates all windows and displays them accordingly.
        /// </summary>
        void UpdateWindows(void);

        /// <summary>
        /// Removes unused lingering window data that may have been left behind by a modified settings file or other reasons.
        /// </summary>
        void CleanUpInactiveWindows(void);

        /// <summary>
        /// Gets the lowest index possible that does not contain a window.
        /// </summary>
        /// <returns>The first index with no window.</returns>
        WindowIndex_t GetLowestUnusedWindowIndex(void);

        // Private Static Variables:
    private:
        // The instance of the manager.
        static ImGuiWindowManager* Windows;

        // Private Variables:
    private:
        // All ImGui windows that exist.
        vector_t<ImGuiWindowBase*> windows_;
        // Map of the name of the active window for each docker node ID.
        unordered_map_t<unsigned int, string_t> windowsActive_;
        // Reusable buffer for writing window data.
        string_t windowsWriteBuffer_;
    };

}	// namespace
