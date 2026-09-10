//------------------------------------------------------------------------------
//
// File Name:	ImGuiWindowTestMemory.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		ImGui window used for testing memory page functionality.
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
    class ImGuiWindowTestMemory : public ImGuiWindowBase
    {
        IMGUI_WINDOW_FRIENDS;

        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    public:
        ImGuiWindowTestMemory(void);

        ImGuiWindowTestMemory(const ImGuiWindowTestMemory& other) = delete;

        ImGuiWindowTestMemory(ImGuiWindowTestMemory&& other) noexcept;

        ImGuiWindowTestMemory& operator=(const ImGuiWindowTestMemory& other) = delete;

        ~ImGuiWindowTestMemory(void);

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
        /// Displays configuration interface.
        /// </summary>
        void DisplayConfig(void);
        
        /// <summary>
        /// Displays page statistics.
        /// </summary>
        void DisplayStats(void);
        
        /// <summary>
        /// Displays interface for adding, removing, and inspecting blocks.
        /// </summary>
        void DisplayBlockInterface(void);
        
        /// <summary>
        /// Displays list of blocks.
        /// </summary>
        void DisplayBlocks(void);
        
        /// <summary>
        /// Displays raw page memory.
        /// </summary>
        void DisplayRaw(void);

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

        // Private Static Variables:
    private:

        // Private Variables:
    private:
        // Memory page size.
        size_t size_;
        // Memory page configuration.
        void* config_;
        // Memory page being tested.
        void* page_;
        // Vector of dummy addresses.
        vector_t<std::pair<void*,size_t>> dummyAddresses_;
        // Size of block when requesting new block.
        size_t blockRequest_;
        // Offset into the page of the raw memory view.
        size_t rawOffset_;
        // Width of raw memory view, in number of bytes.
        size_t rawHorizontal_;
        // Height of raw memory view, in number of bytes.
        size_t rawVertical_;
        // Index of selected block.
        int blockSelection_;
        // Flag indicating the config section is visible.
        bool headerConfig_;
        // Flag indicating the fixed/variable block config section is visible.
        bool headerConfigBlock_;
        // Flag indicating the stats section is visible.
        bool headerStats_;
        // Flag indicating the blocks section is visible.
        bool headerBlocks_;
        // Flag indicating the raw memory view section is visible.
        bool headerRaw_;
    };

}	// namespace
