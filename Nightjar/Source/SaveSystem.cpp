//------------------------------------------------------------------------------
//
// File Name:	SaveSystem.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		System for loading and saving data objects.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "SaveSystem.h"
#include "BaseSystem.h"

#include "DataParty.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    //--------------------------------------------------------------------------
    // Public Constants:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Public Static Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Public Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Private Constants:
    //--------------------------------------------------------------------------

    constexpr string_view_t 
          SaveSystem::DirUserSave = "Saves/"
        , SaveSystem::DirPresetSave = DIR_DATA"Saves/";

    //--------------------------------------------------------------------------
    // Private Static Variables:
    //--------------------------------------------------------------------------

    SaveSystem* SaveSystem::instance = nullptr;

    //--------------------------------------------------------------------------
    // Private Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Constructors/Destructors:
    //--------------------------------------------------------------------------

#pragma region Constructors

    SaveSystem::SaveSystem(void)
        : BaseSystem("SaveSystem", LoopFlags::None)
        , activeSaveName_("TEST")
        , activeParty_()
        , activePreset_()
    {
        // Raise an assert if this system has already been created.
        assert(instance == nullptr);

        // Store this system's instance for use by static functions.
        instance = this;
    }

    //--------------------------------------------------------------------------

    SaveSystem::~SaveSystem(void)
    {
    }

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

    SaveSystem* SaveSystem::GetInstance(void)
    {
        return instance != nullptr ? instance : new SaveSystem();
    }

    bool SaveSystem::LoadSavedParty(string_view_arg name)
    {
        instance->activeSaveName_ = name;
        string_t savePath = UserSaveFilePath(name);
        if (!IOSystem::FileExists(savePath))
        {
            TraceWarning("Failed to load save: {}", savePath);
            return false;
        }
        StreamIn stream{ savePath };
        if (!stream.IsValid())
        {
            TraceWarning("Failed to load save: {}", savePath);
            return false;
        }
        TraceInfo("Loaded save: {}", savePath);
        instance->activeParty_->Read(stream);
        return true;
    }

    const string_t& SaveSystem::GetCurrentSaveName(void)
    {
        return instance->activeSaveName_;
    }

    DataParty* SaveSystem::GetCurrentParty(void)
    {
        return instance->activeParty_;
    }

    bool SaveSystem::SaveCurrentParty(void)
    {
        string_t savePath = UserSaveFilePath(instance->activeSaveName_);
        StreamOut stream{ savePath };
        if (!stream.IsValid())
        {
            TraceWarning("Failed to save to: {}", savePath);
            return false;
        }
        instance->activeParty_->Write(stream);
        stream.Write();
        TraceInfo("Saved to: {}", savePath);
        return true;
    }

    bool SaveSystem::UserSaveExists(string_view_arg name)
    {
        return IOSystem::FileExists(UserSaveFilePath(name));
    }

    const DataParty* SaveSystem::LoadPreset(string_view_arg name)
    {
        string_t savePath = IOSystem::GetDataPath("Saves") + string_t(name) + ".json";
        if (!IOSystem::FileExists(savePath))
        {
            TraceWarning("Failed to load preset: {}", savePath);
            return nullptr;
        }
        StreamIn stream{ savePath };
        if (!stream.IsValid())
        {
            TraceWarning("Failed to load preset: {}", savePath);
            return nullptr;
        }
        instance->activePreset_->Read(stream);
        TraceInfo("Loaded preset: {}", savePath);
        return instance->activePreset_;
    }

    void SaveSystem::WipeAllSaves(void)
    {
        string_t saveDir = IOSystem::GetUserDataPath() + string_t(DirUserSave);
        IOSystem::Destroy(saveDir, false, true);
        TraceWarning("All saves wiped!");
    }

#pragma endregion Public Static Functions

    //--------------------------------------------------------------------------
    // Public Functions:
    //--------------------------------------------------------------------------

#pragma region Public Functions

#pragma endregion Public Functions

    //--------------------------------------------------------------------------
    // Private Static Functions:
    //--------------------------------------------------------------------------

#pragma region Private Static Functions

#pragma endregion Private Static Functions

    //--------------------------------------------------------------------------
    // Private Functions:
    //--------------------------------------------------------------------------

#pragma region Private Functions

    string_t SaveSystem::UserSaveFilePath(string_view_arg name)
    {
        return IOSystem::GetUserDataPath() + string_t(DirUserSave) + string_t(name) + ".json";
    }

    bool SaveSystem::Init(void)
    {
        IOSystem::ForceDirectory(IOSystem::GetUserDataPath() + string_t(DirUserSave));

        activeParty_ = new DataParty();
        activePreset_ = new DataParty();

        LoadSavedParty(activeSaveName_);

        // Return true if the initialization completed successfully.
        return true;
    }

    void SaveSystem::Exit(void)
    {
        SafeDeleteCheck(activeParty_);
        SafeDeleteCheck(activePreset_);
    }

#pragma endregion Private Functions

}	// namespace
