//------------------------------------------------------------------------------
//
// File Name:	ImGuiAlignedBar.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Horizontally aligns and displays ImGui content within a bar.
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
    class ImGuiAlignedBar
    {
        // Public Constants, Enums, and Structs:
    public:
        // Specifies where to display content in a bar.
        enum class Alignment : unsigned char
        {
            Left,   // Aligns content to the left.
            Center, // Aligns content to the center.
            Right,  // Aligns content to the right.
            NUM_ENTRIES,
        };

        // Configures placement and visibility of an entry in an AlignedBar.
        struct EntryConfig
        {
            friend ImGuiAlignedBar;

        public:
            EntryConfig(void);
            EntryConfig(bool active, Alignment alignment, int sortIndex);

        public:
            // Flag to display the entry.
            bool active;
            // Specifies where on the bar to display the entry.
            Alignment alignment;
        private:
            // The index of the table column this entry will be displayed on.
            mutable unsigned char colIndex;
        public:
            // Specifies where in its alignment group the entry should be placed.
            // Lower = Left, Higher = Right.
            int sortIndex;
        };

        // Constructors/Destructors:
    public:
        ImGuiAlignedBar(void);

        ~ImGuiAlignedBar(void);

        // Public Static Functions:
    public:

        // Public Functions:
    public:
        /// <summary>
        /// Registers an entry in the bar.
        /// </summary>
        /// <param name="name">The name of the entry.</param>
        /// <param name="callback">The function that displays the ImGui for the entry.</param>
        /// <param name="alignment">The alignment of the entry in the bar.</param>
        /// <param name="priority">How far to the left or right the entry should appear in its alignment group (Lower = Left, Higher = Right).</param>
        /// <param name="active">Flag to display the entry.</param>
        /// <returns>The ID of the registered entry, or 0 if the entry's name is already in use.</returns>
        CallbackID Register(string_view_arg name, const std::function<void(void)>& callback, Alignment alignment, int priority = 0, bool active = true);
        
        /// <summary>
        /// Unregisters an entry from the bar.
        /// </summary>
        /// <param name="name">The name of the entry to unregister.</param>
        /// <returns>True if the entry was found and unregistered, false otherwise.</returns>
        bool Unregister(string_view_arg name);

        /// <summary>
        /// Unregisters an entry from the bar.
        /// </summary>
        /// <param name="id">The callback ID of the entry to unregister.</param>
        /// <returns>True if the entry was found and unregistered, false otherwise.</returns>
        bool Unregister(CallbackID id);

        /// <summary>
        /// Gets configuration data about an entry in the bar.
        /// </summary>
        /// <param name="name">The name of the entry to get configuration data of.</param>
        /// <returns>A modifiable pointer to the entry configuration if found, nullptr if not found.</returns>
        EntryConfig* GetEntryConfig(string_view_arg name);

        /// <summary>
        /// Gets configuration data about an entry in the bar.
        /// </summary>
        /// <param name="id">The callback ID of the entry to get configuration data of.</param>
        /// <returns>A modifiable pointer to the entry configuration if found, nullptr if not found.</returns>
        EntryConfig* GetEntryConfig(CallbackID id);

        /// <summary>
        /// Displays the bar.
        /// </summary>
        /// <param name="size">The size to display the bar as.</param>
        /// <returns>True if the bar was successfully displayed, false otherwise.</returns>
        bool Display(const Vec2& size = Vec2(0,0));

        /// <summary>
        /// Recalculates and re-sorts entry data for proper usage.
        /// </summary>
        void Refresh(void);

        // Public Event Handlers
    public:

        // Private Constants, Enums, and Structs:
    private:
        // An entry in an AlignedBar.
        struct Entry : public CallbackWrapper<void>, public EntryConfig
        {
            Entry(string_view_arg name, const std::function<void(void)>& func, bool active, Alignment alignment, int sortIndex);
            Entry(const Entry& other) = delete;
            Entry(Entry&& other) noexcept;
            Entry& operator=(const Entry& other) = delete;
            Entry& operator=(Entry&& other) noexcept;

            // Name of the entry.
            string_view_t name;
        };

        // Private Static Functions:
    private:

        // Private Functions:
    private:
        /// <summary>
        /// Displays all entries in the bar.
        /// </summary>
        /// <typeparam name="nested">Flag to use a more complicated setup that offers more style control but may cause clipping issues.</typeparam>
        /// <param name="size">The size to display the bar at.</param>
        /// <returns>True if the bar was successfully displayed, false otherwise.</returns>
        template <bool nested>
        bool DisplayCentered(const Vec2& size) const;

        /// <summary>
        /// Displays a simpler form of the bar that does not handle centered entries.
        /// </summary>
        /// <typeparam name="nested">Flag to use a more complicated setup that offers more style control but may cause clipping issues.</typeparam>
        /// <param name="size">The size to display the bar at.</param>
        /// <returns>True if the bar was successfully displayed, false otherwise.</returns>
        template <bool nested>
        bool DisplayRegular(const Vec2& size) const;

        /// <summary>
        /// Sets up a range of entries for display.
        /// </summary>
        /// <param name="indexBegin">The index of the first entry to set up.</param>
        /// <param name="indexEnd">The index to stop setting up entries at.</param>
        /// <param name="indexColumn">Reference to the current table column index being initialized.</param>
        void SetupRange(unsigned indexBegin, unsigned indexEnd, unsigned char& indexColumn) const;

        /// <summary>
        /// Displays a range of entries.
        /// </summary>
        /// <param name="indexBegin">The index of the first entry to be displayed.</param>
        /// <param name="indexEnd">The index to stop displaying entries at.</param>
        void DisplayRange(unsigned indexBegin, unsigned indexEnd) const;

        // Private Static Variables:
    private:

        // Private Variables:
    private:
        // The entries to be displayed.
        vector_t<Entry> entries_;
        // Counts of how many entries are in each alignment group.
        array_t<unsigned short, Alignment::NUM_ENTRIES> counts_;
        // Flag to indicate that the entries have been modified and needs to be re-sorted.
        mutable bool dirty_;
    };

}	// namespace
