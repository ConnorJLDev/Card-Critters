//------------------------------------------------------------------------------
//
// File Name:	PlatformWindow.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class representing a window as part of the operating system
//              desktop.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "PlatformSystem.h"

#include "CallbackWrapper.h"

#include "Texture.h"

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
    class PlatformWindow : public NamedObject
    {
        friend class PlatformSystem;
        friend class PlatformSystem_;

        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    public:
        PlatformWindow(void);

        PlatformWindow(const PlatformWindow& other) = delete;

        PlatformWindow(PlatformWindow&& other) noexcept;

        PlatformWindow& operator=(const PlatformWindow& other) = delete;

        ~PlatformWindow(void);

        // Public Static Functions:
    public:

        // Public Functions:
    public:
        /// <summary>
        /// Creates and opens the window. Any pre-initialization settings must be set before calling this!
        /// </summary>
        /// <returns></returns>
        bool Create(PlatformWindow* sharedWindow = nullptr);

        /// <summary>
        /// Destroys the window if it is open.
        /// </summary>
        void Destroy(void);

        /// <summary>
        /// Adds a callback function that is called whenever the viewport is resized.
        /// </summary>
        /// <param name="func">The function that is called when the viewport is resized.</param>
        /// <returns>The ID of the callback, used to remove the callback later.</returns>
        CallbackID ViewportResizeCallbackAdd(const std::function<void(const IVec2&)>& func);

        /// <summary>
        /// Removes a callback function via its CallbackID provided when the callback was registered.
        /// </summary>
        /// <param name="funcId">The ID of the callback function.</param>
        void ViewportResizeCallbackRemove(CallbackID funcId);

        /// <summary>
        /// Checks if the window should close (i.e. the user pressed the "close window" button).
        /// </summary>
        /// <returns>True if the window should close, false otherwise.</returns>
        bool WindowShouldClose(void) const;

        /// <summary>
        /// Swap the render buffers to display everything from the current loop on the screen.
        /// </summary>
        void SwapBuffers(void) const;

        /// <summary>
        /// Gets the version of the underlying graphics library.
        /// </summary>
        /// <returns>The numeric version in use.</returns>
        const IVec4& GetGraphicsVersion(void) const;

        /// <summary>
        /// Sets the version of the underlying graphics library.
        /// </summary>
        /// <param name="version">The numeric version to use.</param>
        void SetGraphicsVersion(const IVec4& version);

        /// <summary>
        /// Gets the profile of the underlying graphics library.
        /// </summary>
        /// <returns>The name of the profile in use.</returns>
        string_view_arg GetGraphicsProfile(void) const;

        /// <summary>
        /// Sets the profile of the underlying graphics library.
        /// </summary>
        /// <param name="profile">The name of the profile to use.</param>
        void SetGraphicsProfile(string_view_arg profile);

        /// <summary>
        /// Gets the current window size.
        /// </summary>
        /// <returns>The size of the window in pixels.</returns>
        const IVec2& GetWindowSize(void) const;

        /// <summary>
        /// Sets the window size.
        /// </summary>
        /// <param name="size">The size to set the window to, in pixels.</param>
        void SetWindowSize(const IVec2& size);

        /// <summary>
        /// Gets the title of the window.
        /// </summary>
        /// <returns>The title of the window.</returns>
        const string_t& GetWindowTitle(void) const;

        /// <summary>
        /// Sets the title of the window.
        /// </summary>
        /// <param name="title">The title to assign to the window.</param>
        void SetWindowTitle(string_view_arg title);

        /// <summary>
        /// Checks if VSync is currently enabled.
        /// </summary>
        /// <returns>True if VSync is enabled, false otherwise.</returns>
        bool GetVSync(void) const;

        /// <summary>
        /// Sets the new state of VSync.
        /// </summary>
        /// <param name="vsync">The state to set VSync to.</param>
        void SetVSync(bool vsync);

        /// <summary>
        /// Checks if fullscreen is currently enabled.
        /// </summary>
        /// <returns>True if fullscreen is enabled, false otherwise.</returns>
        bool GetFullscreen(void) const;

        /// <summary>
        /// Sets the new state of fullscreen.
        /// </summary>
        /// <param name="fullscreen">The state to set fullscreen to.</param>
        /// <param name="displayIndex">The display to fullscreen to, or automatic if invalid (negative or too high).</param>
        void SetFullscreen(bool fullscreen, int displayIndex = -1, bool borderlessMax = true);

        /// <summary>
        /// Gets the display that the window is currently on.
        /// </summary>
        /// <returns>The display index showing the window.</returns>
        int GetDisplayIndex(void);

        /// <summary>
        /// Checks if the window is minimized or iconified.
        /// </summary>
        /// <returns>True if the window is iconified, false otherwise.</returns>
        bool GetIsIconified(void) const;

        /// <summary>
        /// Checks if the window is focused.
        /// </summary>
        /// <returns>True if the window is focused, false otherwise.</returns>
        bool GetIsFocused(void) const;

        /// <summary>
        /// Checks if a child window has focus.
        /// </summary>
        /// <returns>True if this window or a child window has focus, false otherwise.</returns>
        bool CheckChildFocus(void);

        // Public Event Handlers
    public:

        // Private Constants, Enums, and Structs:
    private:

        // Private Static Functions:
    private:
        /// <summary>
        /// Internal callback for when a window is moved.
        /// </summary>
        /// <param name="window">The handle of the window being moved.</param>
        /// <param name="xPos">The new x position.</param>
        /// <param name="yPos">The new y position.</param>
        static void CallbackWindowReposition(WindowHandle window, int xPos, int yPos) noexcept;

        /// <summary>
        /// Internal callback for when a window is resized.
        /// </summary>
        /// <param name="window">The handle of the window being resized.</param>
        /// <param name="width">The new width.</param>
        /// <param name="height">The new height.</param>
        static void CallbackWindowResize(WindowHandle window, int width, int height) noexcept;

        /// <summary>
        /// Internal callback for when the framebuffer of a window is resized.
        /// </summary>
        /// <param name="window">The handle of the window who's framebuffer is being resized.</param>
        /// <param name="width">The new framebuffer width.</param>
        /// <param name="height">The new framebuffer height.</param>
        static void CallbackBufferResize(WindowHandle window, int width, int height) noexcept;

        /// <summary>
        /// Internal callback for when a window's iconification state changes.
        /// </summary>
        /// <param name="window">The handle of the window who's iconification state is changing.</param>
        /// <param name="iconified">The new iconification state.</param>
        static void CallbackIconify(WindowHandle window, int iconified) noexcept;

        /// <summary>
        /// Internal callback for when a window's focus state changes.
        /// </summary>
        /// <param name="window">The handle of the window who's focus state is changing.</param>
        /// <param name="iconified">The new focus state.</param>
        static void CallbackFocus(WindowHandle window, int focused) noexcept;

        /// <summary>
        /// Internal callback for when a window's maximization state changes.
        /// </summary>
        /// <param name="window">The handle of the window who's maximization state is changing.</param>
        /// <param name="maximized">The new maximization state.</param>
        static void CallbackMaximize(WindowHandle window, int maximized) noexcept;

        /// <summary>
        /// Internal callback for when a window has been requested to close.
        /// </summary>
        /// <param name="window">The handle of the window who's requesting to close.</param>
        static void CallbackClose(WindowHandle window) noexcept;

        /// <summary>
        /// Internal callback for when a window's contents need to be refreshed.
        /// </summary>
        /// <param name="window">The handle of the window who's contents need to be refreshed.</param>
        static void CallbackRefresh(WindowHandle window) noexcept;

        /// <summary>
        /// Internal callback for when a window's content scale changes.
        /// </summary>
        /// <param name="window">The handle of the window who's content scale is changing.</param>
        /// <param name="xscale">The content scale for the X dimension.</param>
        /// <param name="yscale">The content scale for the Y dimension.</param>
        static void CallbackContentScale(WindowHandle window, float xscale, float yscale) noexcept;

        // Private Functions:
    private:

        // Private Static Variables:
    private:

        // Private Variables:
    private:
        IVec2 windowSize_;
        IVec4 glVersion_;
        union
        {
            uint32_t flags_;
            struct
            {
                // Status
                bool isClosing_ : 1;
                bool isIconified_ : 1;
                bool isFocused_ : 1;
                bool isMaximized_ : 1;
                bool isFullscreen_ : 1;
                bool isBorderlessMaximized_ : 1;
                bool isChildFocused_ : 1;
                // Config
                bool vsync_ : 1;
                bool resizeable_ : 1;
                bool visible_ : 1;
                bool isDecorated_ : 1;
                bool autoIconify_ : 1;
                bool floating_ : 1;
                bool centerCursor_ : 1;
                bool transparentFramebuffer_ : 1;
                bool focusOnShow_ : 1;
                bool scaleToMonitor_ : 1;
                bool scaleFramebuffer_ : 1;
                bool mousePassthrough_ : 1;
            };
        };
        int fullscreenIndex_;
        IVec2 lastWindowPos_;
        IVec2 lastWindowSize_;
        Vec2 contentScale_;
        WindowHandle windowHandle_;
        vector_t<CallbackWrapper<void, const IVec2&>> resizeCallbacks_;
        vector_t<Texture> icons_;
    };

}	// namespace
