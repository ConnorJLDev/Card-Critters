//------------------------------------------------------------------------------
//
// File Name:	IOSystem.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		System for unifying IO operations and directories.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

// Omitted to prevent recompilation due to changes of BaseSystem.
//#include "BaseSystem.h"

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
    class IOSystem
    {
        // Public Constants, Enums, and Structs:
    public:
        // Common directory for asset files.
#define DIR_ASSETS "Assets/"

        // Common directory for data files.
#define DIR_DATA "Data/"

        // Common directory for asset files.
        static constexpr string_view_t DirAssets{ DIR_ASSETS };

        // Common directory for data files.
        static constexpr string_view_t DirData{ DIR_DATA };

        // Callback type used when registering a file change callback.
        using FileChangeCallback = std::function<void(string_view_arg)>;

        // 
        enum class ItemType : unsigned
        {
            None            = 0,
            Directory       = 1 << 0,
            File            = 1 << 1,
            BlockFile       = 1 << 2,
            CharacterFile   = 1 << 3,
            FIFO            = 1 << 4,
            Socket          = 1 << 5,
            Symlink         = 1 << 6,
            Other           = 1 << 7,
        };

        // 
        enum class AccessType : unsigned
        {
            None        = 0,
            Readable    = 1 << 0,
            Writable    = 1 << 1,
        };

        // Constructors/Destructors:
    public:
        IOSystem(void) = delete;

        // Public Static Functions:
    public:
        /// <summary>
        /// Gets the instance of the IOSystem.
        /// </summary>
        /// <returns>A pointer to the system.</returns>
        static BaseSystem* GetInstance(void);

        /// <summary>
        /// Attempts to find a file with multiple possible extensions.
        /// </summary>
        /// <param name="name">The filepath to find. Modified to the first successful match if one is found.</param>
        /// <param name="extensions">The possible extensions to check for.</param>
        /// <returns>True if a file was found, false otherwise.</returns>
        static bool FindWithoutExtension(string_t& name, const std::ranges::range auto& extensions);

        /// <summary>
        /// Gets the base assets path.
        /// </summary>
        /// <returns>A reference to the base assets path.</returns>
        static const string_t& GetAssetsPath(void);

        /// <summary>
        /// Gets the directory path of a subdirectory within the assets directory.
        /// </summary>
        /// <param name="subdirectory">The name of the subdirectory.</param>
        /// <returns>The path to the subdirectory.</returns>
        static string_t GetAssetsPath(string_view_arg subdirectory);

        /// <summary>
        /// Gets the base data path.
        /// </summary>
        /// <returns>A reference to the base data path.</returns>
        static const string_t& GetDataPath(void);

        /// <summary>
        /// Gets the directory path of a subdirectory within the data directory.
        /// </summary>
        /// <param name="subdirectory">The name of the subdirectory.</param>
        /// <returns>The path to the subdirectory.</returns>
        static string_t GetDataPath(string_view_arg subdirectory);

        /// <summary>
        /// Gets the directory path to the executable.
        /// </summary>
        /// <returns>The path to the executable.</returns>
        static const string_t& GetExecutablePath(void);

        /// <summary>
        /// Gets the directory path to user data SPECIFIC to this game.
        /// </summary>
        /// <returns>The path to user data.</returns>
        static const string_t& GetUserDataPath(void);

        /// <summary>
        /// Gets the directory path to temporary file storage.
        /// </summary>
        /// <returns>The path to temporary file storage.</returns>
        static const string_t& GetTemporaryPath(void);

        /// <summary>
        /// Opens a stream to an object of type T with the specified name.
        /// </summary>
        /// <typeparam name="T">The type of the object.</typeparam>
        /// <param name="name">The name of the object.</param>
        /// <param name="success">Set to true if successful, false otherwise.</param>
        /// <returns>The stream to the requested object.</returns>
        template<typename T>
        static StreamIn OpenStreamIn(string_view_arg name, bool& success);

        /// <summary>
        /// Registers a callback function that is called whenever a change occurs within the game directories.
        /// </summary>
        /// <param name="callback">The callback function.</param>
        /// <returns>The ID of the registration, used for unregistration later.</returns>
        static CallbackID RegisterChangeCallback(const FileChangeCallback& callback);

        /// <summary>
        /// Unregisters a file change callback function.
        /// </summary>
        /// <param name="callbackID">The ID of the callback function to remove.</param>
        /// <returns>True if the callback was successfully removed, false otherwise.</returns>
        static bool UnregisterChangeCallback(const CallbackID& callbackID);

        /// <summary>
        /// Reads a given file as text into a string buffer.
        /// </summary>
        /// <param name="filename">The file to read from.</param>
        /// <param name="buffer">The buffer to load the file into.</param>
        /// <returns>True if the operation completed successfully, false otherwise.</returns>
        static bool ReadFileAsString(const string_t& filename, string_t& buffer);

        /// <summary>
        /// Checks if a file exists.
        /// </summary>
        /// <param name="path">The filepath to check.</param>
        /// <returns>True if the file exists, false otherwise.</returns>
        static bool FileExists(string_view_arg path);

        /// <summary>
        /// Ensures directories exist for the path specified, and creates them if necessary.
        /// </summary>
        /// <param name="path">The path to force existence of.</param>
        static void ForceDirectory(string_view_arg path);

        /// <summary>
        /// Recursively destroys a file or directory.
        /// </summary>
        /// <param name="path">The path to destroy.</param>
        /// <param name="preserveDirectories">If true, only files are destroyed, and all directories remain valid.</param>
        /// <param name="preserveRoot">If true, only subdirectories and files are destroyed, and the root directory remains valid.</param>
        static void Destroy(string_view_arg path, bool preserveDirectories, bool preserveRoot);

        /// <summary>
        /// Copies a file.
        /// </summary>
        /// <param name="from">The target path to copy from.</param>
        /// <param name="to">The destination path to copy to.</param>
        /// <param name="overwrite">Flag to allow overwriting existing files.</param>
        /// <returns>True if the copy was successful, false otherwise.</returns>
        static bool Copy(string_view_arg from, string_view_arg to, bool overwrite = false);

        /// <summary>
        /// Moves a file.
        /// </summary>
        /// <param name="from">The target path to move from.</param>
        /// <param name="to">The destination path to move to.</param>
        /// <param name="overwrite">Flag to allow overwriting existing files.</param>
        /// <returns>True if the move was successful, false otherwise.</returns>
        static bool Move(string_view_arg from, string_view_arg to, bool overwrite = false);

        /// <summary>
        /// Renames a file.
        /// </summary>
        /// <param name="from">The path of the file to be changed.</param>
        /// <param name="to">The new path of the file.</param>
        /// <returns>True if the rename was successful, false otherwise.</returns>
        static bool Rename(string_view_arg from, string_view_arg to);

        /// <summary>
        /// Validates a path's separators by converting backslashes to forward slashes.
        /// </summary>
        /// <param name="path">The path to validate.</param>
        static void ValidatePathSeparators(string_t& path);

        /// <summary>
        /// Validates a path's separators by converting backslashes to forward slashes.
        /// </summary>
        /// <param name="path">The path to validate.</param>
        /// <returns>A copy of the path with validated separators.</returns>
        static string_t ConvertPathSeparators(string_view_arg path);

        // Public Functions:
    public:

        // Public Event Handlers
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

}	// namespace
