//------------------------------------------------------------------------------
//
// File Name:	ImGuiWindowBase.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Base class for ImGui windows.
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

// Classes that derived ImGui windows must be friends with.
#define IMGUI_WINDOW_FRIENDS \
friend ImGuiWindowBase;\
friend class ImGuiWindowManager;\
friend class ImGuiMenuBar;\

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
    class ImGuiWindowBase : public NamedObject
    {
        friend class ImGuiLayoutManager;
        friend class ImGuiWindowManager;
        friend class ImGuiMenuBar;

        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    public:
        ImGuiWindowBase(void) = delete;
    protected:
        ImGuiWindowBase(string_view_arg name, int flags = 0);
    public:
        ImGuiWindowBase(const ImGuiWindowBase& other) = delete;

        ImGuiWindowBase(ImGuiWindowBase&& other) noexcept;

        ImGuiWindowBase& operator=(const ImGuiWindowBase& other) = delete;

        virtual ~ImGuiWindowBase(void);

        // Public Static Functions:
    public:

        // Public Functions:
    public:
        /// <summary>
        /// Displays the window.
        /// </summary>
        /// <returns>True if the window contents were displayed, false otherwise.</returns>
        bool Display(void);

        /// <summary>
        /// Checks if this window is open.
        /// </summary>
        /// <returns>True if this window is open, false if this window is closed.</returns>
        bool GetIsOpen(void) const;

        // Public Event Handlers
    public:

        // Protected Constants, Enums, and Structs:
    protected:

        using WindowIndex_t = unsigned int;

        static constexpr WindowIndex_t InvalidWindowIndex = std::numeric_limits<WindowIndex_t>::max();

        enum class ReadError
        {
            None,
            InvalidKey,
            InvalidValue,
        };

        // Protected Static Functions:
    protected:
        // Static map of derived window generator functions.
        static unordered_map_string_t<std::function<ImGuiWindowBase*()>>& WindowGenerators(void);

        /// <summary>
        /// Tries to generate a derived window of the given name.
        /// </summary>
        /// <param name="name">The name of the derived window to generate.</param>
        /// <param name="index">The index of the derived window to generate.</param>
        /// <returns>A pointer to the new window if successful, or nullptr on failure.</returns>
        static ImGuiWindowBase* GenerateWindow(string_view_arg name, size_t index = InvalidWindowIndex);

        /// <summary>
        /// Reads an array of strings from a value. For use in ReadKeyValue.
        /// </summary>
        /// <param name="value">The string to read the array from.</param>
        /// <param name="func">The function to parse each string.</param>
        /// <returns>The number of strings parsed.</returns>
        static unsigned ReadStringArray(string_view_arg value, std::function<void(string_view_arg)> func);

        // Protected Functions:
    protected:
        /// <summary>
        /// Derived window display implementation.
        /// </summary>
        virtual void DisplayImpl(void) = 0;

        /// <summary>
        /// Tries to set the value of a given key.
        /// </summary>
        /// <param name="key">The key of the value to be set.</param>
        /// <param name="value">A string representation of the value to be set.</param>
        /// <returns>Error code detailing what error may have occurred while trying to read.</returns>
        virtual ReadError ReadKeyValue(string_view_arg key, string_view_arg value) = 0;

        /// <summary>
        /// Finalizes initialization after reading data.
        /// </summary>
        virtual void ReadFinish(void);

        /// <summary>
        /// Writes all of this window's persistent data into the given buffer as key-value pairs.
        /// </summary>
        /// <param name="buffer">The buffer to write to.</param>
        virtual void WriteAll(string_t& buffer) = 0;

        // Private Functions:
    private:
        /// <summary>
        /// Assigns the index of this window.
        /// </summary>
        /// <param name="index">The new index for this window.</param>
        void SetIndex(size_t index);

        /// <summary>
        /// Destroys this window's data from the internal window manager.
        /// </summary>
        void Destroy(void);

        // Protected Static Variables:
    protected:
        // Static counter for creating unique internal names for windows.
        static WindowIndex_t GlobalIndex;

        // Protected Variables:
    protected:
        // The null-terminated name used within ImGui, containing its unique identifier.
        string_t internalName_;
        // The position of the window.
        Vec2 pos_;
        // The size of the window.
        Vec2 size_;
        // The content space within the window.
        Vec2 contentSize_;
        // Window flags for when the window begins drawing.
        int flags_;
        // ID of the dock this window is attached to.
        unsigned int dockID_;
        // The creation index of this window. Used for internalName_.
        WindowIndex_t index_;
        // Bool to enable/disable the existence of the close button on the window.
        bool enableOpen_;
        // Bool containing the open state of the window if enableOpen_ is true.
        bool open_;
        // Flag to indicate if this window was created from loading a settings file.
        bool loaded_;
    };

}	// namespace
