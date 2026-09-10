//------------------------------------------------------------------------------
//
// File Name:	PersistenceSystem.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		System that manages persistent data, such as settings.
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
    class BaseSystem;
    class PersistenceEntry;
    class PersistenceGroup;

    // Typedefs:

    // Class Definition:
    class PersistenceSystem
    {
        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    private:
        PersistenceSystem(void) = delete;

        // Public Static Functions:
    public:
        /// <summary>
        /// Gets the instance of the PersistenceSystem.
        /// </summary>
        /// <returns>A pointer to the system.</returns>
        static BaseSystem* GetInstance(void);

        /// <summary>
        /// Gets a group by name.
        /// </summary>
        /// <param name="groupName">The name of the group to get.</param>
        /// <returns>A pointer to the group, or nullptr if not found.</returns>
        static PersistenceGroup* GetGroup(string_view_arg groupName);

        /// <summary>
        /// Creates a new group with the given name.
        /// </summary>
        /// <param name="groupName">The name of the new group.</param>
        /// <returns>A pointer to the newly created group, or nullptr if that name is already in use.</returns>
        static PersistenceGroup* CreateGroup(string_view_arg groupName);

        /// <summary>
        /// Destroys a group by name.
        /// </summary>
        /// <param name="groupName">The name of the group to destroy.</param>
        /// <returns>True if the group was found and destroyed, false otherwise.</returns>
        static bool DestroyGroup(string_view_arg groupName);

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
