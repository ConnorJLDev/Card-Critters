//------------------------------------------------------------------------------
//
// File Name:	ImGuiLayoutManager.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Manages saving, loading, and processing ImGui settings files.
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

struct ImGuiContext;
struct ImGuiSettingsHandler;
struct ImGuiTextBuffer;

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    // Forward Declarations:

    // Typedefs:

    // Class Definition:
    class ImGuiLayoutManager
    {
        friend class ImGuiSystem_;

        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    private:
        ImGuiLayoutManager(void);
    public:
        ~ImGuiLayoutManager(void);

        // Public Static Functions:
    public:
        /// <summary>
        /// Gets the state of if autosave is enabled or disabled for the current settings.
        /// </summary>
        /// <returns>True if autosave is enabled, false otherwise.</returns>
        static bool GetAutosave(void);

        /// <summary>
        /// Enables or disables autosave of the current settings.
        /// </summary>
        /// <param name="enabled">True to enable autosave, false to disable autosave.</param>
        /// <returns>True if autosave is enabled, false if disabled or the current settings path is invalid.</returns>
        static bool SetAutosave(bool enabled);
        
        /// <summary>
        /// Gets the state of if autosaving to the temporary directory is enabled or disabled for the current settings.
        /// </summary>
        /// <returns>True if autosave is enabled, false otherwise.</returns>
        static bool GetTempAutosave(void);

        /// <summary>
        /// Enables or disables autosave to the temporary directory of the current settings.
        /// </summary>
        /// <param name="enabled">True to enable autosave, false to disable autosave.</param>
        static void SetTempAutosave(bool enabled);

        /// <summary>
        /// Loads a settings file.
        /// </summary>
        /// <param name="name">The name of the settings file to load. If blank, reloads the current settings file.</param>
        /// <returns>True if the load was successful, false otherwise.</returns>
        static bool LoadLayout(string_view_arg name = "");

        /// <summary>
        /// Saves the current settings.
        /// </summary>
        /// <returns>True if saving was successful, false otherwise (i.e. no active settings file or is write protected)</returns>
        static bool SaveLayout(void);

        /// <summary>
        /// Creates a new settings file.
        /// </summary>
        /// <param name="name">The name of the new settings file.</param>
        /// <param name="setActive">Flag to set the newly created settings file as the active settings in use (if creation was successful).</param>
        /// <param name="copyFrom">If not empty, specifies the name of another settings file to copy from.</param>
        /// <returns>True if creation was successful, false otherwise.</returns>
        static bool CreateLayout(string_view_arg name, bool setActive = true, string_view_arg copyFrom = "");

        /// <summary>
        /// Renames the current settings file to another name.
        /// </summary>
        /// <param name="name">The new name for the current settings file.</param>
        /// <returns>True if the rename operation was successful, false otherwise.</returns>
        static bool RenameLayout(string_view_arg name);

        /// <summary>
        /// Renames a given settings file to another name.
        /// </summary>
        /// <param name="from">The current name of the settings file being renamed.</param>
        /// <param name="to">The new name for the settings file.</param>
        /// <returns>True if the rename operation was successful, false otherwise.</returns>
        static bool RenameLayout(string_view_arg from, string_view_arg to);

        /// <summary>
        /// Destroys and deletes a settings file.
        /// </summary>
        /// <param name="name">The name of the settings file to destroy. If blank, uses the active settings.</param>
        /// <returns>True if destruction was successful, false otherwise.</returns>
        static bool DestroyLayout(string_view_arg name = "");

        /// <summary>
        /// Gets the path of the settings file in use right now.
        /// </summary>
        /// <returns>The path of the active settings file.</returns>
        static string_view_t GetLayoutPath(void);

        /// <summary>
        /// Gets the name of the settings file in use right now, without the file extension.
        /// </summary>
        /// <returns>The name of the active settings file.</returns>
        static string_view_t GetLayoutName(void);

        /// <summary>
        /// Gets the directory used for settings files.
        /// </summary>
        /// <returns>The settings directory.</returns>
        static string_view_t GetLayoutDirectory(void);

        /// <summary>
        /// Sets the directory to use for settings files.
        /// </summary>
        /// <param name="directory">The new directory to use.</param>
        static void SetLayoutDirectory(string_view_arg directory);

        // Public Functions:
    public:

        // Public Event Handlers
    public:

        // Private Constants, Enums, and Structs:
    private:

        // Private Static Functions:
    private:
        /// <summary>
        /// Initializes the settings manager.
        /// </summary>
        /// <returns>True if initialization was successful, false otherwise.</returns>
        static bool Init(void);
        
        /// <summary>
        /// Shuts down the settings manager.
        /// </summary>
        static void Exit(void);

        /// <summary>
        /// Internal callback function for when the current settings are being cleared.
        /// </summary>
        /// <param name="context">The context this callback is occurring in.</param>
        /// <param name="handler">The handler processing this callback.</param>
        static void CallbackSettingsClearAll(ImGuiContext* context, ImGuiSettingsHandler* handler);

        /// <summary>
        /// Internal callback function for when a settings file is about to be read.
        /// </summary>
        /// <param name="context">The context this callback is occurring in.</param>
        /// <param name="handler">The handler processing this callback.</param>
        static void CallbackSettingsReadInit(ImGuiContext* context, ImGuiSettingsHandler* handler);

        /// <summary>
        /// Internal callback function for when an entry block is found while reading a settings file. 
        /// </summary>
        /// <param name="context">The context this callback is occurring in.</param>
        /// <param name="handler">The handler processing this callback.</param>
        /// <param name="name">The name of the entry block.</param>
        /// <returns>A pointer to the entry being read, or nullptr if no entry was found.</returns>
        static void* CallbackSettingsReadOpen(ImGuiContext* context, ImGuiSettingsHandler* handler, const char* name);

        /// <summary>
        /// Internal callback function for when a line is being read from a settings file within an entry block we own.
        /// </summary>
        /// <param name="context">The context this callback is occurring in.</param>
        /// <param name="handler">The handler processing this callback.</param>
        /// <param name="entry">The entry being read.</param>
        /// <param name="line">The line in the entry block being read.</param>
        static void CallbackSettingsReadLine(ImGuiContext* context, ImGuiSettingsHandler* handler, void* entry, const char* line);

        /// <summary>
        /// Internal callback function for when a settings file has finished reading. 
        /// </summary>
        /// <param name="context">The context this callback is occurring in.</param>
        /// <param name="handler">The handler processing this callback.</param>
        static void CallbackSettingsApplyAll(ImGuiContext* context, ImGuiSettingsHandler* handler);

        /// <summary>
        /// Internal callback function for when a settings file is being written to.
        /// </summary>
        /// <param name="context">The context this callback is occurring in.</param>
        /// <param name="handler">The handler processing this callback.</param>
        /// <param name="buffer">The buffer to write settings to.</param>
        static void CallbackSettingsWriteAll(ImGuiContext* context, ImGuiSettingsHandler* handler, ImGuiTextBuffer* buffer);

        // Private Functions:
    private:
        /// <summary>
        /// Updates relevant implementation functions about the current layout path.
        /// </summary>
        void SetActiveSettings(void);

        /// <summary>
        /// Searches for the named settings file.
        /// </summary>
        /// <param name="name">The name of the settings file to find.</param>
        /// <returns>A pair where the first value is the path of the file (if found, else an empty string),
        /// and the second value is an internal enum that indicates how the file was found.</returns>
        std::pair<string_view_t, int> GetSettings(string_view_arg name);

        // Private Static Variables:
    private:
        // The instance of the manager.
        static ImGuiLayoutManager* Settings;

        // Private Variables:
    private:
        // Flag to enable autosave.
        bool autosave_;
        // Flag to enable autosaving to the temporary directory.
        bool tempAutosave_;
        // Flag to indicate if the current layout path can be written to.
        bool isWriteable_;
        // The current path to the layout in use.
        string_t currentPath_;
        // The file name (without extension) of the layout in use.
        string_view_t currentName_;
        // Directory to search when searching or displaying available layout files.
        string_t layoutsDir_;
        // Names of available layouts found.
        vector_t<string_t> layoutsFound_;
        // Path of the temporary autosave file.
        string_t tempPath_;
        // Buffer for temporary string operations.
        string_t writeBuffer_;
    };

}	// namespace
