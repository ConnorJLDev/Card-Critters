//------------------------------------------------------------------------------
//
// File Name:	ImGuiWindowEntities.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		ImGui window that displays entities in each active scene.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiWindowBase.h"

// Required for ImGuiTextFilter.
#include "ImGuiInclude.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    // Forward Declarations:
    class Entity;

    // Typedefs:

    // Class Definition:
    class ImGuiWindowEntities : public ImGuiWindowBase
    {
        IMGUI_WINDOW_FRIENDS;

        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    public:
        ImGuiWindowEntities(void);

        ImGuiWindowEntities(const ImGuiWindowEntities& other) = delete;

        ImGuiWindowEntities(ImGuiWindowEntities&& other) noexcept;

        ImGuiWindowEntities& operator=(const ImGuiWindowEntities& other) = delete;

        ~ImGuiWindowEntities(void);

        // Public Static Functions:
    public:

        // Public Functions:
    public:

        // Public Event Handlers
    public:

        // Private Constants, Enums, and Structs:
    private:
        // The base name of this window.
        static const string_view_t WindowName;

        // Private Static Functions:
    private:

        // Private Functions:
    private:
        /// <summary>
        /// Console window display implementation.
        /// </summary>
        void DisplayImpl(void) override;

        /// <summary>
        /// Tries to set the value of a given key.
        /// </summary>
        /// <param name="key">The key of the value to be set.</param>
        /// <param name="value">A string representation of the value to be set.</param>
        /// <returns>Error code detailing what error may have occurred while trying to read.</returns>
        ReadError ReadKeyValue(string_view_arg key, string_view_arg value) override;

        /// <summary>
        /// Finalizes initialization after reading data.
        /// </summary>
        void ReadFinish(void) override;

        /// <summary>
        /// Writes all of this window's persistent data into the given buffer as key-value pairs.
        /// </summary>
        /// <param name="buffer">The buffer to write to.</param>
        void WriteAll(string_t& buffer) override;

        /// <summary>
        /// Displays the menu bar at the top of the window.
        /// </summary>
        void DisplayMenuBar(void);

        /// <summary>
        /// 
        /// </summary>
        void DisplayScenes(void);

        //
        void DisplayEntities(unsigned sceneIndex);

        //
        void DisplayEntityChildren(Entity* entity);

        //
        void ProcessEntitySelection(void);

        //
        bool PrepareEntitySelection(Entity* entity);

        // Private Static Variables:
    private:

        // Private Variables:
    private:
        ImGuiTextFilter searchFilter_;
        vector_t<Entity*> visibleEntities_;
        size_t selectedEntityIndex_;
        size_t selectedEntityShiftIndex_;
        bool selectedEntityState_;
    };

}	// namespace
