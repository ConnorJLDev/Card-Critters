//------------------------------------------------------------------------------
//
// File Name:	SaveSystem.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		System for loading and saving data objects.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "BaseSystem.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    // Forward Declarations:
    class DataParty;

    // Typedefs:

    // Class Definition:
    class SaveSystem SYSTEM_FINAL : public BaseSystem
    {
        SYSTEM_COMMON(SaveSystem);

        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    private:
        SaveSystem(void);

        // All systems need a virtual destructor to have their destructor called 
        ~SaveSystem(void) override;

        // Public Static Functions:
    public:
        // Get the instance of the SaveSystem.
        static SaveSystem* GetInstance(void);

        //
        static bool LoadSavedParty(string_view_arg name);

        //
        static const string_t& GetCurrentSaveName(void);

        //
        static DataParty* GetCurrentParty(void);

        //
        static bool SaveCurrentParty(void);

        //
        static bool UserSaveExists(string_view_arg name);

        //
        static const DataParty* LoadPreset(string_view_arg name);

        //
        static void WipeAllSaves(void);

        // Public Functions:
    public:

        // Public Event Handlers
    public:

        // Private Constants, Enums, and Structs:
    private:
        static const string_view_t DirUserSave, DirPresetSave;

        // Private Static Functions:
    private:
        //
        static string_t UserSaveFilePath(string_view_arg name);

        // Private Functions:
    private:
        /// <summary>
        /// Initializes the system.
        /// </summary>
        /// <returns>True if initialization succeeded, false otherwise.</returns>
        bool Init(void) override;

        /// <summary>
        /// Exits and shuts down the system.
        /// </summary>
        void Exit(void) override;

        // Private Static Variables:
    private:
        static SaveSystem* instance;

        // Private Variables:
    private:
        string_t activeSaveName_;
        DataParty* activeParty_;
        DataParty* activePreset_;
    };

}	// namespace
