//------------------------------------------------------------------------------
//
// File Name:	PlatformSystem.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200
// Project:		Card Critters
// Purpose:		System for handling platform-related tasks like window management.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#define PLATFORM_INCLUDE <glfw/glfw3.h>

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

struct GLFWwindow;
struct GLFWmonitor;
struct GLFWvidmode;

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    // Forward Declarations:
    class BaseSystem;
    class PlatformDisplay;
    class PlatformWindow;

    // Typedefs:
    typedef GLFWwindow* WindowHandle;
    typedef GLFWmonitor* DisplayHandle;
    typedef const GLFWvidmode* VideoModePtr;

    // Class Definition:
    class PlatformSystem
    {
        friend class ImGuiSystem_;
        friend class InputSystem_;

        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    public:
        PlatformSystem(void) = delete;

        // Public Static Functions:
    public:
        /// <summary>
        /// Gets the instance of the PlatformSystem.
        /// </summary>
        /// <returns>A pointer to the system.</returns>
        static BaseSystem* GetInstance(void);

        /// <summary>
        /// Adds a callback function that is called whenever the viewport is resized.
        /// </summary>
        /// <param name="func">The function that is called when the viewport is resized.</param>
        /// <returns>The ID of the callback, used to remove the callback later.</returns>
        static CallbackID ViewportResizeCallbackAdd(const std::function<void(const IVec2&)>& func);

        /// <summary>
        /// Removes a callback function via its CallbackID provided when the callback was registered.
        /// </summary>
        /// <param name="funcId">The ID of the callback function.</param>
        static void ViewportResizeCallbackRemove(CallbackID funcId);

        /// <summary>
        /// Adds a callback function that is called whenever a monitor is connected or disconnected.
        /// </summary>
        /// <param name="func">The function that is called when a monitor is changed.</param>
        /// <returns>The ID of the callback, used to remove the callback later.</returns>
        static CallbackID DisplayChangeCallbackAdd(const std::function<void(const PlatformDisplay&)>& func);

        /// <summary>
        /// Removes a callback function via its CallbackID provided when the callback was registered.
        /// </summary>
        /// <param name="funcId">The ID of the callback function.</param>
        static void DisplayChangeCallbackRemove(CallbackID funcId);

        /// <summary>
        /// Checks if the window should close (i.e. the user pressed the "close window" button).
        /// </summary>
        /// <returns>True if the window should close, false otherwise.</returns>
        static bool WindowShouldClose(void);

        /// <summary>
        /// Swap the render buffers to display everything from the current loop on the screen.
        /// </summary>
        static void SwapBuffers(void);

        /// <summary>
        /// Gets the amount of time elapsed during the previous frame.
        /// </summary>
        /// <returns>How many seconds passed during the previous frame.</returns>
        static float GetDeltaTime(void);

        /// <summary>
        /// Gets the number of frames that have been rendered since program start.
        /// </summary>
        /// <returns>The number of frames rendered since program start.</returns>
        static const size_t& GetFrameCount(void);

        /// <summary>
        /// The amount of time the program has been running.
        /// </summary>
        /// <returns>The number of seconds this program has been running.</returns>
        static double GetRunTime(void);

        /// <summary>
        /// Gets the version of the underlying graphics library.
        /// </summary>
        /// <returns>The numeric version in use.</returns>
        static const IVec4& GetGraphicsVersion(void);

        /// <summary>
        /// Sets the version of the underlying graphics library.
        /// </summary>
        /// <param name="version">The numeric version to use.</param>
        static void SetGraphicsVersion(const IVec4& version);

        /// <summary>
        /// Gets the profile of the underlying graphics library.
        /// </summary>
        /// <returns>The name of the profile in use.</returns>
        static string_view_arg GetGraphicsProfile(void);

        /// <summary>
        /// Sets the profile of the underlying graphics library.
        /// </summary>
        /// <param name="profile">The name of the profile to use.</param>
        static void SetGraphicsProfile(string_view_arg profile);

        /// <summary>
        /// Gets the current window size.
        /// </summary>
        /// <returns>The size of the window in pixels.</returns>
        static const IVec2& GetWindowSize(void);

        /// <summary>
        /// Sets the window size.
        /// </summary>
        /// <param name="size">The size to set the window to, in pixels.</param>
        static void SetWindowSize(const IVec2& size);

        /// <summary>
        /// Gets the title of the window.
        /// </summary>
        /// <returns>The title of the window.</returns>
        static const string_t& GetWindowTitle(void);

        /// <summary>
        /// Sets the title of the window.
        /// </summary>
        /// <param name="title">The title to assign to the window.</param>
        static void SetWindowTitle(string_view_arg title);

        /// <summary>
        /// Checks if VSync is currently enabled.
        /// </summary>
        /// <returns>True if VSync is enabled, false otherwise.</returns>
        static bool GetVSync(void);

        /// <summary>
        /// Sets the new state of VSync.
        /// </summary>
        /// <param name="vsync">The state to set VSync to.</param>
        static void SetVSync(bool vsync);

        /// <summary>
        /// Checks if fullscreen is currently enabled.
        /// </summary>
        /// <returns>True if fullscreen is enabled, false otherwise.</returns>
        static bool GetFullscreen(void);

        /// <summary>
        /// Sets the new state of fullscreen.
        /// </summary>
        /// <param name="fullscreen">The state to set fullscreen to.</param>
        /// <param name="displayIndex">The display to fullscreen to, or automatic if invalid (negative or too high).</param>
        static void SetFullscreen(bool fullscreen, int displayIndex = -1);

        /// <summary>
        /// Gets the display that the window is currently on.
        /// </summary>
        /// <returns>The display index showing the window.</returns>
        static int GetDisplayIndex(void);

        /// <summary>
        /// Gets the number of displays connected to the computer.
        /// </summary>
        /// <returns>The number of displays connected.</returns>
        static int GetDisplayCount(void);

        /// <summary>
        /// Gets the first display associated with the specified index.
        /// </summary>
        /// <param name="index">The index of the display to get.</param>
        /// <returns>A pointer to the display if found, or nullptr if not found.</returns>
        static PlatformDisplay* GetDisplay(int index);

        /// <summary>
        /// Gets the first display associated with the specified name.
        /// </summary>
        /// <param name="name">The name of the display to get.</param>
        /// <returns>A pointer to the display if found, or nullptr if not found.</returns>
        static PlatformDisplay* GetDisplay(string_view_arg name);

        /// <summary>
        /// Gets the first window associated with the specified index.
        /// </summary>
        /// <param name="index">The index of the window to get.</param>
        /// <returns>A pointer to the window if found, or nullptr if not found.</returns>
        static PlatformWindow* GetWindow(int index);

        /// <summary>
        /// Gets the first window associated with the specified name.
        /// </summary>
        /// <param name="name">The name of the window to get.</param>
        /// <returns>A pointer to the window if found, or nullptr if not found.</returns>
        static PlatformWindow* GetWindow(string_view_arg name);

        /// <summary>
        /// Checks if the window is minimized or iconified.
        /// </summary>
        /// <returns>True if the window is iconified, false otherwise.</returns>
        static bool GetIsIconified(void);

        /// <summary>
        /// Checks if the window is focused.
        /// </summary>
        /// <returns>True if the window is focused, false otherwise.</returns>
        static bool GetIsFocused(void);

        // Public Functions:
    public:

        // Public Event Handlers
    public:

        // Private Constants, Enums, and Structs:
    private:

        // Private Static Functions:
    private:
        /// <summary>
        /// Gets a handle to the underlying window.
        /// </summary>
        /// <returns>A handle to the underlying window.</returns>
        static WindowHandle GetWindowHandle(void);

        // Private Functions:
    private:

        // Private Static Variables:
    private:

        // Private Variables:
    private:
    };

}	// namespace
