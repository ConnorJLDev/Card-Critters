//------------------------------------------------------------------------------
//
// File Name:	InputSystem.h
// Author(s):	Andrew Lamb (andrew.lamb), Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Interface for inputs.
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

    // Typedefs:

    // Class Definition:
    class InputSystem
    {
        // Public Constants, Enums, and Structs:
    public:
        // Enum for describing standard mouse buttons.
        enum class MouseButton : unsigned char
        {
            Left	= 1 << 0, // Left mouse button.
            Right	= 1 << 1, // Right mouse button.
            Middle	= 1 << 2, // Middle mouse button.
        };

        // Enum for describing standard keyboard buttons.
        enum class KeyboardButton : unsigned char
        {
            Space,
            Apostrophe,
            Comma,
            Minus,
            Period,
            Slash,
            Row0,
            Row1,
            Row2,
            Row3,
            Row4,
            Row5,
            Row6,
            Row7,
            Row8,
            Row9,
            Semicolon,
            Equal,
            A,
            B,
            C,
            D,
            E,
            F,
            G,
            H,
            I,
            J,
            K,
            L,
            M,
            N,
            O,
            P,
            Q,
            R,
            S,
            T,
            U,
            V,
            W,
            X,
            Y,
            Z,
            LeftBracket,
            Backslash,
            RightBracket,
            Grave,
            Escape,
            Enter,
            Tab,
            Backspace,
            Insert,
            Delete,
            Right,
            Left,
            Down,
            Up,
            PageUp,
            PageDown,
            Home,
            End,
            CapsLock,
            ScrollLock,
            NumLock,
            Printcreen,
            Pause,
            F1,
            F2,
            F3,
            F4,
            F5,
            F6,
            F7,
            F8,
            F9,
            F10,
            F11,
            F12,
            F13,
            F14,
            F15,
            F16,
            F17,
            F18,
            F19,
            F20,
            F21,
            F22,
            F23,
            F24,
            F25,
            Pad0,
            Pad1,
            Pad2,
            Pad3,
            Pad4,
            Pad5,
            Pad6,
            Pad7,
            Pad8,
            Pad9,
            PadDecimal,
            PadDivide,
            PadMultiply,
            PadSubtract,
            PadAdd,
            PadEnter,
            PadEqual,
            LeftShift,
            LeftControl,
            LeftAlt,
            LeftSuper,
            RightShift,
            RightControl,
            RightAlt,
            RightSuper,
            Menu,
            NUM_ENTRIES,
        };

        // Enum for describing the different states a button interaction can be in.
        // Bit 0 is true if the button is down, false otherwise.
        // Bit 1 is true if the state of the button changed this frame.
        enum class Interaction : unsigned char
        {
            Idle			= 0b00,	// The button is not held down this frame.
            Triggered		= 0b11,	// The button has just been pressed this frame; it is now held. 
            Held			= 0b01,	// The button is currently held down this frame.
            Released		= 0b10,	// The button has been released this frame; it is no longer held.
            FlagHeld		= 0b01, // Bitmask for if the button is held down.
            FlagChanged	    = 0b10, // Bitmask for if the button's state has changed.
        };

        // Constructors/Destructors:
    public:
        InputSystem(void) = delete;

        // Public Static Functions:
    public:
        /// <summary>
        /// Gets the instance of the InputSystem.
        /// </summary>
        /// <returns>A pointer to the system.</returns>
        static BaseSystem* GetInstance(void);

        /// <summary>
        /// Gets the Interaction state of the given mouse button.
        /// </summary>
        /// <param name="button">The mouse button to check.</param>
        /// <returns>The Interaction state of the given mouse button.</returns>
        static Interaction GetMouseInput(MouseButton button);

        /// <summary>
        /// Checks if the mouse button was triggered this frame.
        /// </summary>
        /// <param name="button">The mouse button to check.</param>
        /// <returns>True if the button was triggered this frame, false otherwise.</returns>
        static bool MouseButtonIsTriggered(MouseButton button);

        /// <summary>
        /// Checks if the mouse button was released this frame.
        /// </summary>
        /// <param name="button">The mouse button to check.</param>
        /// <returns>True if the button was released this frame, false otherwise.</returns>
        static bool MouseButtonIsReleased(MouseButton button);

        /// <summary>
        /// Checks if the mouse button is held this frame.
        /// </summary>
        /// <param name="button">The mouse button to check.</param>
        /// <returns>True if the button is held this frame, false otherwise.</returns>
        static bool MouseButtonIsHeld(MouseButton button);

        /// <summary>
        /// Gets the Interaction state of the given key.
        /// </summary>
        /// <param name="key">The keyboard key as a KeyboardButton.</param>
        /// <returns>The Interaction state of the given key.</returns>
        static Interaction GetKeyboardInput(KeyboardButton key);

        /// <summary>
        /// Gets the Interaction state of the given key.
        /// </summary>
        /// <param name="key">The keyboard key as a character.</param>
        /// <returns>The Interaction state of the given key.</returns>
        static Interaction GetKeyboardInput(char key);

        /// <summary>
        /// Checks if the keyboard key was triggered this frame.
        /// </summary>
        /// <param name="key">The keyboard key as a KeyboardButton.</param>
        /// <returns>True if the key was triggered this frame, false otherwise.</returns>
        static bool KeyIsTriggered(KeyboardButton key);

        /// <summary>
        /// Checks if the keyboard key was triggered this frame.
        /// </summary>
        /// <param name="key">The keyboard key as a character.</param>
        /// <returns>True if the key was triggered this frame, false otherwise.</returns>
        static bool KeyIsTriggered(char key);

        /// <summary>
        /// Checks if the keyboard key was released this frame.
        /// </summary>
        /// <param name="key">The keyboard key as a KeyboardButton.</param>
        /// <returns>True if the key was released this frame, false otherwise.</returns>
        static bool KeyIsReleased(KeyboardButton key);

        /// <summary>
        /// Checks if the keyboard key was released this frame.
        /// </summary>
        /// <param name="key">The keyboard key as a character.</param>
        /// <returns>True if the key was released this frame, false otherwise.</returns>
        static bool KeyIsReleased(char key);

        /// <summary>
        /// Checks if the keyboard key is held this frame.
        /// </summary>
        /// <param name="key">The keyboard key as a KeyboardButton.</param>
        /// <returns>True if the key is held this frame, false otherwise.</returns>
        static bool KeyIsHeld(KeyboardButton key);

        /// <summary>
        /// Checks if the keyboard key is held this frame.
        /// </summary>
        /// <param name="key">The keyboard key as a character.</param>
        /// <returns>True if the key is held this frame, false otherwise.</returns>
        static bool KeyIsHeld(char key);

        /// <summary>
        /// Gets the current mouse position.
        /// </summary>
        /// <returns>The current mouse position.</returns>
        static const Vec2& GetMousePos();

        // Operations for MouseButton enum.
        ENUM_DECLARE_OPS(MouseButton);

        // Operations for Interaction enum.
        ENUM_DECLARE_OPS(Interaction);

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

    // Using Declarations:
    using B_Mouse = InputSystem::MouseButton;       // Shorthand for Button of Mouse (mouse button).
    using B_Keyboard = InputSystem::KeyboardButton; // Shorthand for Button of Keyboard (keyboard button/key).
    using B_Interaction = InputSystem::Interaction; // Shorthand for Button Interaction.

}	// namespace
