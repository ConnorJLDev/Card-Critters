//------------------------------------------------------------------------------
//
// File Name:	ImGuiWindowConsole.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		ImGui window that displays console output.
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
    class ImGuiWindowConsole : public ImGuiWindowBase
    {
        IMGUI_WINDOW_FRIENDS;

        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    public:
        ImGuiWindowConsole(void);

        ImGuiWindowConsole(const ImGuiWindowConsole& other) = delete;

        ImGuiWindowConsole(ImGuiWindowConsole&& other) noexcept;

        ImGuiWindowConsole& operator=(const ImGuiWindowConsole& other) = delete;

        ~ImGuiWindowConsole(void);

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

        // Value indicating an invalid message selection index.
        static const size_t InvalidSelectionIndex;

        // Enum for indexing colors that aren't related to trace severity.
        enum class ColorSettings : unsigned char
        {
            Timestamp,
            Spacestamp,
            NUM_ENTRIES,
        };

        ENUM_MAPS_CLASS_SEQUENTIAL_DECLARE(ColorSettings);

        // Stores metadata of how a message is displayed.
        struct MessageMetadata
        {
            MessageMetadata(void) = delete;
            MessageMetadata(size_t index, const Trace::MessageData& messageInfo);
            MessageMetadata(const MessageMetadata& other) = delete;
            MessageMetadata(MessageMetadata&& other) noexcept;
            MessageMetadata& operator=(const MessageMetadata& other) = delete;

            // The index of the message in the message history that this struct stores data for.
            const Trace::MessageIndex_t index;
            // Fixed-size buffer for storing the timestamp string.
            string_fixed_t<14, false> timestamp;
            // Fixed-size buffer for storing the framestamp string.
            string_fixed_t<8, true> framestamp;
        };

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
        /// Displays the trace messages.
        /// </summary>
        void DisplayMessages(void);

        /// <summary>
        /// Displays the selected trace message contents.
        /// </summary>
        void DisplayMessageDetails(void);

        // Private Static Variables:
    private:

        // Private Variables:
    private:
        // Metadata and cached strings for each traced message.
        segmented_vector_t<MessageMetadata> meta_;
        // Array for storing colors of trace severities.
        array_t<Vec4, Trace::Severity::NUM_ENTRIES> colorsSeverities_;
        // Array for storing colors unrelated to trace severities.
        array_t<Vec4, ColorSettings::NUM_ENTRIES> colorsSettings_;
        // Flag to enable/disable colored text rendering.
        bool enableColor_;
        // Flag to enable/disable automatically scrolling to the bottom when a new message appears.
        bool enableAutoscroll_;
        // Flag to enable/disable filtering messages by severity.
        bool enableFilterSeverity_;
        // Flag to enable/disable filtering messages by category.
        bool enableFilterCategory_;
        // Flag to enable/disable filtering messages by string.
        bool enableFilterString_;
        // Flag to enable/disable filtering messages by string as regular expression.
        bool enableFilterRegex_;
        // Flag to indicate that the filter data has changed.
        mutable bool filterDirty_;
        // Filters messages by severity.
        unsigned int filterSeverities_;
        // Filters messages by category.
        unordered_map_string_t<bool> filterCategories_;
        // Filters messages if they contain the given string.
        string_t filterString_;
        // Filters messages using the filter string as a regular expression.
        regex_t filterRegex_;
        // The number of messages when this window was last displayed.
        Trace::MessageIndex_t lastMessageCount_;
        // The index of the selected message.
        Trace::MessageIndex_t messageSelected_;
    };

}	// namespace
