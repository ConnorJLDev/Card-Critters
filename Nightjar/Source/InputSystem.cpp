//------------------------------------------------------------------------------
//
// File Name:	InputSystem.cpp
// Author(s):	Andrew Lamb (andrew.lamb), Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Interface for inputs.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "InputSystem.h"
#include "BaseSystem.h"
#include "PlatformSystem.h"

#include PLATFORM_INCLUDE

#include "ImGuiSystem.h"

#undef TRACE_CATEGORY
#define TRACE_CATEGORY INPUT

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    //--------------------------------------------------------------------------
    // Class Implementation:
    //--------------------------------------------------------------------------

#pragma region Class Implementation

    class InputSystem_ SYSTEM_FINAL : public BaseSystem
    {
        friend InputSystem;

        SYSTEM_COMMON(InputSystem_);

        // Public Constants, Enums, and Structs:
    public:
        typedef int input_t;
        typedef unordered_map_t<input_t, B_Interaction> InputMap;
        typedef stack_t<std::pair<input_t, B_Interaction>> InputStack;

        // Constructors/Destructors:
    private:
        InputSystem_(void);

        // All systems need a virtual destructor to have their destructor called 
        ~InputSystem_(void) override;
    public:

        // Public Static Functions:
    public:

        // Public Functions:
    public:

        // Private Constants, Enums, and Structs:
    private:

        // Private Static Functions:
    private:
        /// <summary>
        /// Callback for handling key changes.
        /// </summary>
        /// <param name="window">The window that received the event.</param>
        /// <param name="key">The keyboard key that was pressed or released.</param>
        /// <param name="scancode">The system-specific scancode of the key.</param>
        /// <param name="action">The action performed on the key.</param>
        /// <param name="mods">Bit field describing which modifier keys were held down.</param>
        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) noexcept;

        /// <summary>
        /// Callback for handling mouse button changes.
        /// </summary>
        /// <param name="window">The window that received the event.</param>
        /// <param name="button">The mouse button that was pressed or released.</param>
        /// <param name="action">The action performed on the key.</param>
        /// <param name="mods">Bit field describing which modifier keys were held down.</param>
        static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) noexcept;

        /// <summary>
        /// Helper function for moving button states across frames (like from Pressed to Held).
        /// </summary>
        /// <param name="buttons">The map of buttons to update.</param>
        /// <param name="queueFresh">The queue of buttons pressed this frame.</param>
        /// <param name="queueChanged">The queue of buttons pressed the previous frame.</param>
        static void ProcessButtons(InputMap& buttons, InputStack& queueFresh, InputStack& queueChanged);

        /// <summary>
        /// Helper function for getting the Interaction state of a button, or providing a default if missing.
        /// </summary>
        /// <param name="buttons">The map of buttons to search in.</param>
        /// <param name="button">The button to find the Interaction state of.</param>
        /// <returns>The Interaction state of the given button.</returns>
        static B_Interaction GetButton(InputMap& buttons, input_t button);

        // Private Functions:
    private:
        /// <summary>
        /// Initializes the system.
        /// </summary>
        /// <returns>True if initialization succeeded, false otherwise.</returns>
        bool Init(void) override;

        /// <summary>
        /// Updates the system.
        /// </summary>
        void Update(void) override;

        /// <summary>
        /// Exits and shuts down the system.
        /// </summary>
        void Exit(void) override;

        /// <summary>
        /// Updates the internal mouse position.
        /// </summary>
        void UpdateMousePos();

        /// <summary>
        /// Updates the internal state of all buttons.
        /// </summary>
        void UpdateButtons();

        // Private Static Variables:
    private:
        static InputSystem_* instance;

        // Private Variables:
    private:
        // Vector containing mouse position.
        Vec2 mousePos_;
        // A stack containing mouse buttons that have been pressed this frame, but not processed.
        InputStack mouseButtonsQueueFresh_;
        // A stack containing mouse buttons that have been pressed the previous frame, and have been processed.
        InputStack mouseButtonsQueueChange_;
        // A map containing keys of all mouse buttons, and their values being their Interaction state.
        InputMap mouseButtons_;
        // A stack containing keys that have been pressed this frame, but not processed.
        InputStack keyboardButtonsQueueFresh_;
        // A stack containing keys that have been pressed the previous frame, and have been processed.
        InputStack keyboardButtonsQueueChange_;
        // A map containing keys of all keyboard keys, and their values being their Interaction state.
        InputMap keyboardButtons_;
    };

#pragma endregion Class Implementation

    //--------------------------------------------------------------------------
    // Public Constants:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Public Static Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Public Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Private Constants:
    //--------------------------------------------------------------------------

#define INPUT_MAP_MOUSE(basename, valueEnum, valueRaw) {basename::##valueEnum, valueRaw}

    // Map converting MouseButton enum to GLFW macros.
    static const unordered_map_t<InputSystem::MouseButton, InputSystem_::input_t>& GLFWMouseValues(void)
    {
        static const unordered_map_t<InputSystem::MouseButton, InputSystem_::input_t>& GLFWMouseValues_
        (
            StaticManagement::UnorderedMapWrapper<InputSystem::MouseButton, InputSystem_::input_t>::Create({
                FOR_EACH_1_2_2_C(INPUT_MAP_MOUSE, InputSystem::MouseButton,
                    Left,   GLFW_MOUSE_BUTTON_LEFT,
                    Right,  GLFW_MOUSE_BUTTON_RIGHT,
                    Middle, GLFW_MOUSE_BUTTON_MIDDLE
                )
            })
        );
        return GLFWMouseValues_;
    }

#define INPUT_MAP_KEYBOARD(basename, valueEnum, valueRaw) valueRaw

    // Map converting KeyboardButton enum to GLFW macros.
    static const array_t<InputSystem_::input_t, (size_t)InputSystem::KeyboardButton::NUM_ENTRIES>& GLFWKeyboardValues(void)
    {
        // unordered_map_t<InputSystem::KeyboardButton, InputSystem::input_t>
        // array_t<InputSystem::input_t, (size_t)InputSystem::KeyboardButton::NUM_ENTRIES>
        static constexpr array_t<InputSystem_::input_t, (size_t)InputSystem::KeyboardButton::NUM_ENTRIES> GLFWKeyboardValues_
        {
            FOR_EACH_1_2_2_C(INPUT_MAP_KEYBOARD, InputSystem::KeyboardButton,
                Space,          GLFW_KEY_SPACE,
                Apostrophe,     GLFW_KEY_APOSTROPHE,
                Comma,          GLFW_KEY_COMMA,
                Minus,          GLFW_KEY_MINUS,
                Period,         GLFW_KEY_PERIOD,
                Slash,          GLFW_KEY_SLASH,
                Row0,           GLFW_KEY_0,
                Row1,           GLFW_KEY_1,
                Row2,           GLFW_KEY_2,
                Row3,           GLFW_KEY_3,
                Row4,           GLFW_KEY_4,
                Row5,           GLFW_KEY_5,
                Row6,           GLFW_KEY_6,
                Row7,           GLFW_KEY_7,
                Row8,           GLFW_KEY_8,
                Row9,           GLFW_KEY_9,
                Semicolon,      GLFW_KEY_SEMICOLON,
                Equal,          GLFW_KEY_EQUAL,
                A,              GLFW_KEY_A,
                B,              GLFW_KEY_B,
                C,              GLFW_KEY_C,
                D,              GLFW_KEY_D,
                E,              GLFW_KEY_E,
                F,              GLFW_KEY_F,
                G,              GLFW_KEY_G,
                H,              GLFW_KEY_H,
                I,              GLFW_KEY_I,
                J,              GLFW_KEY_J,
                K,              GLFW_KEY_K,
                L,              GLFW_KEY_L,
                M,              GLFW_KEY_M,
                N,              GLFW_KEY_N,
                O,              GLFW_KEY_O,
                P,              GLFW_KEY_P,
                Q,              GLFW_KEY_Q,
                R,              GLFW_KEY_R,
                S,              GLFW_KEY_S,
                T,              GLFW_KEY_T,
                U,              GLFW_KEY_U,
                V,              GLFW_KEY_V,
                W,              GLFW_KEY_W,
                X,              GLFW_KEY_X,
                Y,              GLFW_KEY_Y,
                Z,              GLFW_KEY_Z,
                LeftBracket,    GLFW_KEY_LEFT_BRACKET,
                Backslash,      GLFW_KEY_BACKSLASH,
                RightBracket,   GLFW_KEY_RIGHT_BRACKET,
                Grave,          GLFW_KEY_GRAVE_ACCENT,
                Escape,         GLFW_KEY_ESCAPE,
                Enter,          GLFW_KEY_ENTER,
                Tab,            GLFW_KEY_TAB,
                Backspace,      GLFW_KEY_BACKSPACE,
                Insert,         GLFW_KEY_INSERT,
                Delete,         GLFW_KEY_DELETE,
                Right,          GLFW_KEY_RIGHT,
                Left,           GLFW_KEY_LEFT,
                Down,           GLFW_KEY_DOWN,
                Up,             GLFW_KEY_UP,
                PageUp,         GLFW_KEY_PAGE_UP,
                PageDown,       GLFW_KEY_PAGE_DOWN,
                Home,           GLFW_KEY_HOME,
                End,            GLFW_KEY_END,
                CapsLock,       GLFW_KEY_CAPS_LOCK,
                ScrollLock,     GLFW_KEY_SCROLL_LOCK,
                NumLock,        GLFW_KEY_NUM_LOCK,
                Printcreen,     GLFW_KEY_PRINT_SCREEN,
                Pause,          GLFW_KEY_PAUSE,
                F1,             GLFW_KEY_F1,
                F2,             GLFW_KEY_F2,
                F3,             GLFW_KEY_F3,
                F4,             GLFW_KEY_F4,
                F5,             GLFW_KEY_F5,
                F6,             GLFW_KEY_F6,
                F7,             GLFW_KEY_F7,
                F8,             GLFW_KEY_F8,
                F9,             GLFW_KEY_F9,
                F10,            GLFW_KEY_F10,
                F11,            GLFW_KEY_F11,
                F12,            GLFW_KEY_F12,
                F13,            GLFW_KEY_F13,
                F14,            GLFW_KEY_F14,
                F15,            GLFW_KEY_F15,
                F16,            GLFW_KEY_F16,
                F17,            GLFW_KEY_F17,
                F18,            GLFW_KEY_F18,
                F19,            GLFW_KEY_F19,
                F20,            GLFW_KEY_F20,
                F21,            GLFW_KEY_F21,
                F22,            GLFW_KEY_F22,
                F23,            GLFW_KEY_F23,
                F24,            GLFW_KEY_F24,
                F25,            GLFW_KEY_F25,
                Pad0,           GLFW_KEY_KP_0,
                Pad1,           GLFW_KEY_KP_1,
                Pad2,           GLFW_KEY_KP_2,
                Pad3,           GLFW_KEY_KP_3,
                Pad4,           GLFW_KEY_KP_4,
                Pad5,           GLFW_KEY_KP_5,
                Pad6,           GLFW_KEY_KP_6,
                Pad7,           GLFW_KEY_KP_7,
                Pad8,           GLFW_KEY_KP_8,
                Pad9,           GLFW_KEY_KP_9,
                PadDecimal,     GLFW_KEY_KP_DECIMAL,
                PadDivide,      GLFW_KEY_KP_DIVIDE,
                PadMultiply,    GLFW_KEY_KP_MULTIPLY,
                PadSubtract,    GLFW_KEY_KP_SUBTRACT,
                PadAdd,         GLFW_KEY_KP_ADD,
                PadEnter,       GLFW_KEY_KP_ENTER,
                PadEqual,       GLFW_KEY_KP_EQUAL,
                LeftShift,      GLFW_KEY_LEFT_SHIFT,
                LeftControl,    GLFW_KEY_LEFT_CONTROL,
                LeftAlt,        GLFW_KEY_LEFT_ALT,
                LeftSuper,      GLFW_KEY_LEFT_SUPER,
                RightShift,     GLFW_KEY_RIGHT_SHIFT,
                RightControl,   GLFW_KEY_RIGHT_CONTROL,
                RightAlt,       GLFW_KEY_RIGHT_ALT,
                RightSuper,     GLFW_KEY_RIGHT_SUPER,
                Menu,           GLFW_KEY_MENU
                )
        };
        return GLFWKeyboardValues_;
    }

    //------------------------------------------------------------------------------
    // Private Static Variables:
    //------------------------------------------------------------------------------

    InputSystem_* InputSystem_::instance = nullptr;

#define inst InputSystem_::instance

    // Static InputStack pointers so GLFW callbacks can access relevant fields on system instance.
    static InputSystem_::InputStack
        * GLFWMouseQueue(nullptr),
        * GLFWKeyboardQueue(nullptr);

    //--------------------------------------------------------------------------
    // Private Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Constructors/Destructors:
    //--------------------------------------------------------------------------

#pragma region Constructors

    InputSystem_::InputSystem_(void)
        : BaseSystem("InputSystem", LoopFlags::HasUpdate)
        , mousePos_()
        , mouseButtonsQueueFresh_()
        , mouseButtonsQueueChange_()
        , mouseButtons_()
        , keyboardButtonsQueueFresh_()
        , keyboardButtonsQueueChange_()
        , keyboardButtons_()
    {
        // Raise an assert if this system has already been created.
        assert(instance == nullptr);

        // Store this system's instance for use by static functions.
        instance = this;
    }

    //--------------------------------------------------------------------------

    InputSystem_::~InputSystem_(void)
    {
    }

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

    BaseSystem* InputSystem::GetInstance(void)
    {
        return inst != nullptr ? inst : new InputSystem_();
    }

    InputSystem::Interaction InputSystem::GetMouseInput(MouseButton button)
    {
        InputSystem_::input_t inputButton = GLFWMouseValues().at(button);
        return InputSystem_::GetButton(inst->mouseButtons_, inputButton);
    }

    bool InputSystem::MouseButtonIsTriggered(MouseButton button)
    {
        return GetMouseInput(button) == Interaction::Triggered;
    }

    bool InputSystem::MouseButtonIsReleased(MouseButton button)
    {
        return GetMouseInput(button) == Interaction::Triggered;
    }

    bool InputSystem::MouseButtonIsHeld(MouseButton button)
    {
        return (bool)(GetMouseInput(button) & Interaction::FlagHeld);
    }

    InputSystem::Interaction InputSystem::GetKeyboardInput(KeyboardButton key)
    {
        return InputSystem_::GetButton(inst->keyboardButtons_, GLFWKeyboardValues()[(unsigned)key]);
    }

    InputSystem::Interaction InputSystem::GetKeyboardInput(char key)
    {
        return InputSystem_::GetButton(inst->keyboardButtons_, key);
    }

    bool InputSystem::KeyIsTriggered(KeyboardButton key)
    {
        return GetKeyboardInput(key) == Interaction::Triggered;
    }

    bool InputSystem::KeyIsTriggered(char key)
    {
        return GetKeyboardInput(key) == Interaction::Triggered;
    }

    bool InputSystem::KeyIsReleased(KeyboardButton key)
    {
        return GetKeyboardInput(key) == Interaction::Released;
    }

    bool InputSystem::KeyIsReleased(char key)
    {
        return GetKeyboardInput(key) == Interaction::Released;
    }

    bool InputSystem::KeyIsHeld(KeyboardButton key)
    {
        return (bool)(GetKeyboardInput(key) & Interaction::FlagHeld);
    }

    bool InputSystem::KeyIsHeld(char key)
    {
        return (bool)(GetKeyboardInput(key) & Interaction::FlagHeld);
    }

    const Vec2& InputSystem::GetMousePos()
    {
        return inst->mousePos_;
    }

    ENUM_IMPLEMENT_OPS(InputSystem::MouseButton);

    ENUM_IMPLEMENT_OPS(InputSystem::Interaction);

#pragma endregion Public Static Functions

    //--------------------------------------------------------------------------
    // Public Functions:
    //--------------------------------------------------------------------------

#pragma region Public Functions

#pragma endregion Public Functions

    //--------------------------------------------------------------------------
    // Private Static Functions:
    //--------------------------------------------------------------------------

#pragma region Private Static Functions

    void InputSystem_::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) noexcept
    {
        // If the keyboard is interacting with ImGui, don't record the interaction.
        if (ImGuiSystem::IsKeyboardCaptured())
        {
            return;
        }

        UNREFERENCED_PARAMETER(window);
        UNREFERENCED_PARAMETER(scancode);
        UNREFERENCED_PARAMETER(mods);
        TraceDebug("Pressed key: {} {} {}", key, scancode, action, mods);
        switch (action)
        {
        case GLFW_PRESS:
            GLFWKeyboardQueue->emplace(key, InputSystem::Interaction::Triggered);
            break;
        case GLFW_RELEASE:
            GLFWKeyboardQueue->emplace(key, InputSystem::Interaction::Released);
            break;
        }
    }

    void InputSystem_::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) noexcept
    {
        // If the mouse is interacting with ImGui, don't record the interaction.
        if (ImGuiSystem::IsMouseCaptured())
        {
            return;
        }

        UNREFERENCED_PARAMETER(window);
        UNREFERENCED_PARAMETER(mods);
        TraceDebug("Pressed mouse button: {} {} {}", button, action, mods);
        switch (action)
        {
        case GLFW_PRESS:
            GLFWMouseQueue->emplace(button, InputSystem::Interaction::Triggered);
            break;
        case GLFW_RELEASE:
            GLFWMouseQueue->emplace(button, InputSystem::Interaction::Released);
            break;
        }
    }

    void InputSystem_::ProcessButtons(InputMap& buttons, InputStack& queueFresh, InputStack& queueChanged)
    {
        // Any keys that were pressed/released two frames ago, apply to the input map with the FlagChanged bit reset.
        while (!queueChanged.empty()) {
            auto& top = queueChanged.top();
            buttons[top.first] = top.second & B_Interaction::FlagHeld;
            queueChanged.pop();
        }
        // Any keys that were pressed/released one frame ago, apply to the input map with the FlagChanged bit set
        // and queue them for processing in the next frame (queueChanged).
        while (!queueFresh.empty()) {
            auto& top = queueFresh.top();
            buttons[top.first] = top.second | B_Interaction::FlagChanged;
            queueChanged.push(top);
            queueFresh.pop();
        }
    }

    B_Interaction InputSystem_::GetButton(InputMap& buttons, input_t button)
    {
        B_Interaction interaction;
        // Try to get the given button. If that key is missing, it throws an exception.
        try
        {
            interaction = buttons.at(button);
        }
        catch (std::out_of_range)
        {
            // This will only throw once for this button because the required key is now assigned.
            buttons[button] = interaction = B_Interaction::Idle;
        }
        return interaction;
    }

#pragma endregion Private Static Functions

    //--------------------------------------------------------------------------
    // Private Functions:
    //--------------------------------------------------------------------------

#pragma region Private Functions

#define PLATFORM_WINDOW PlatformSystem::GetWindowHandle()

    bool InputSystem_::Init(void)
    {
        // Add system-specific initialization code here.
        GLFWKeyboardQueue = &keyboardButtonsQueueFresh_;
        GLFWMouseQueue = &mouseButtonsQueueFresh_;

        // Callbacks
        glfwSetKeyCallback(PLATFORM_WINDOW, KeyCallback);
        glfwSetMouseButtonCallback(PLATFORM_WINDOW, MouseButtonCallback);
        
        // Return true if the initialization completed successfully.
        return true;
    }

    void InputSystem_::Update(void)
    {
        UpdateMousePos();
        UpdateButtons();
    }

    void InputSystem_::Exit(void)
    {
        glfwSetMouseButtonCallback(PLATFORM_WINDOW, NULL);
        glfwSetKeyCallback(PLATFORM_WINDOW, NULL);
        GLFWMouseQueue = nullptr;
        GLFWKeyboardQueue = nullptr;
    }

    void InputSystem_::UpdateMousePos()
    {
        const Vec2 windowSizeHalf{ Vec2(PlatformSystem::GetWindowSize()) * 0.5f };
        DVec2 cursorPosD;
        glfwGetCursorPos(PLATFORM_WINDOW, &cursorPosD.x, &cursorPosD.y);
        mousePos_.x = (float)(cursorPosD.x - windowSizeHalf.x);
        mousePos_.y = (float)(windowSizeHalf.y - cursorPosD.y);
        mousePos_ /= windowSizeHalf;
    }

    void InputSystem_::UpdateButtons()
    {
        ProcessButtons(mouseButtons_, mouseButtonsQueueFresh_, mouseButtonsQueueChange_);
        ProcessButtons(keyboardButtons_, keyboardButtonsQueueFresh_, keyboardButtonsQueueChange_);
    }

#pragma endregion Private Functions

}	// namespace
