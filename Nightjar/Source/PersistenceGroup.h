//------------------------------------------------------------------------------
//
// File Name:	PersistenceGroup.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Describes a group of data entries within the persistence system.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "PersistenceEntry.h"

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
    class PersistenceGroup : public NamedObject
    {
        friend class PersistenceSystem_;

        DECLARE_READ_KEY(PersistenceGroup);

        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    public:
        PersistenceGroup(void);
        PersistenceGroup(string_view_arg name);
        PersistenceGroup(const PersistenceGroup& other);
        PersistenceGroup(PersistenceGroup&& other) noexcept;
        PersistenceGroup& operator=(const PersistenceGroup& other);
        PersistenceGroup& operator=(PersistenceGroup&& other) noexcept;

        ~PersistenceGroup(void);

        // Public Static Functions:
    public:

        // Public Functions:
    public:
        /// <summary>
        /// Reads persistent data entries from the file that this group's path points to.
        /// </summary>
        /// <returns>True if the file was found and successfully read from, false otherwise.</returns>
        bool Read(void);

        /// <summary>
        /// Writes persistent data entries to the file that this group's path points to.
        /// </summary>
        /// <returns>True if the file was found and successfully written to, false otherwise.</returns>
        bool Write(void);

        /// <summary>
        /// Deletes all entries from this group.
        /// </summary>
        void Clear(void);

        /// <summary>
        /// Gets the file path that this group is stored in.
        /// </summary>
        /// <returns>The file path that this group is stored in.</returns>
        string_view_t GetPath(void) const;
        
        /// <summary>
        /// Sets the file path that this group is stored in.
        /// </summary>
        /// <param name="path">The new file path to store this group in.</param>
        void SetPath(string_view_arg path);
        
        /// <summary>
        /// Gets an entry by name.
        /// </summary>
        /// <param name="entryName">The name of the entry to get.</param>
        /// <returns>A pointer to the entry, or nullptr if not found.</returns>
        PersistenceEntry* GetEntry(string_view_arg entryName);

        /// <summary>
        /// Creates a new entry with the given name.
        /// </summary>
        /// <param name="entryName">The name of the new entry.</param>
        /// <returns>A pointer to the newly created entry, or nullptr if that name is already in use.</returns>
        PersistenceEntry* CreateEntry(string_view_arg entryName);

        /// <summary>
        /// Destroys an entry by name.
        /// </summary>
        /// <param name="entryName">The name of the entry to destroy.</param>
        /// <returns>True if the entry was found and destroyed, false otherwise.</returns>
        bool DestroyEntry(string_view_arg entryName);

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
        // Path to where the persistent data is stored.
        string_t path_;
        // Container of the entries.
        unordered_map_string_t<PersistenceEntry> entries_;
    };

}	// namespace
