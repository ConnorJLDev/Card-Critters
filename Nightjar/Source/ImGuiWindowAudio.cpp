//------------------------------------------------------------------------------
//
// File Name:	ImGuiWindowAudio.cpp
// Author(s):	Barend Brink (barend.brink), Connor Lariviere (connor.lariviere), Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		ImGui window that displays audio information and controls.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiWindowAudio.h"

#include "ImGuiInclude.h"

#include "ImGuiWindowBase.inl"

#include "AudioSystem.h"

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

    constexpr string_view_t ImGuiWindowAudio::WindowName = "Audio";

    //--------------------------------------------------------------------------
    // Private Static Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Private Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Constructors/Destructors:
    //--------------------------------------------------------------------------

#pragma region Constructors

    ImGuiWindowAudio::ImGuiWindowAudio(void)
        : ImGuiWindowBase(WindowName, 0/*ImGuiWindowFlags_MenuBar*/)
    {
    }

    ImGuiWindowAudio::ImGuiWindowAudio(ImGuiWindowAudio&& other) noexcept
        : ImGuiWindowBase(std::move(other))
    {
    }

    //--------------------------------------------------------------------------

    ImGuiWindowAudio::~ImGuiWindowAudio(void)
    {
    }

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

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

    void ImGuiWindowAudio::DisplayImpl(void)
    {
        auto* as = static_cast<AudioSystem*>(AudioSystem::GetInstance());

        ImGui::Text("Music:");

        bool pauseEdit = as->GetPausedMusic();
        if (ImGui::Checkbox("Pause Music Channels", &pauseEdit))
        {
            as->SetPausedMusic(pauseEdit);
        }

        float volumeEdit = as->GetVolumeMusic();
        if (ImGui::SliderFloat("Music Volume", &volumeEdit, .0f, 1.f))
        {
            as->SetVolumeMusic(volumeEdit);
        }

        float pitchEdit = as->GetPitchMusic();
        if (ImGui::SliderFloat("Music Pitch", &pitchEdit, .1f, 2.f))
        {
            as->SetPitchMusic(pitchEdit);
        }

        ImGui::Text("SFX:");

        pauseEdit = as->GetPausedSFX();
        if (ImGui::Checkbox("Pause SFX Channels", &pauseEdit))
        {
            as->SetPausedSFX(pauseEdit);
        }

        volumeEdit = as->GetVolumeSFX();
        if (ImGui::SliderFloat("SFX Volume", &volumeEdit, .0f, 1.f))
        {
            as->SetVolumeSFX(volumeEdit);
        }

        pitchEdit = as->GetPitchSFX();
        if (ImGui::SliderFloat("SFX Pitch", &pitchEdit, .1f, 2.f))
        {
            as->SetPitchSFX(pitchEdit);
        }

        //ImGui::Text("Volume:\nMusic: %.1f\nSFX: %.1f", as->GetVolumeMusic(), as->GetVolumeSFX());
        //ImGui::Text("Pitch:\nMusic: %.2f\nSFX: %.2f", as->GetPitchMusic(), as->GetPitchSFX());
    }

    ImGuiWindowBase::ReadError ImGuiWindowAudio::ReadKeyValue([[maybe_unused]] string_view_arg key, [[maybe_unused]] string_view_arg value)
    {
        //bool parseSuccess;
        //switch (READKEY_HASH(key))
        //{
        //READKEY_CHECK("Dummy")
        //    READKEY_VALUE(dummyValue_);
        //    return READKEY_RETURN;
        //default:
        //    break;
        //}
        return ReadError::InvalidKey;
    }

    void ImGuiWindowAudio::ReadFinish(void)
    {
    }

    void ImGuiWindowAudio::WriteAll([[maybe_unused]] string_t& buffer)
    {
    }

    //void ImGuiWindowAudio::DisplayMenuBar(void)
    //{
    //    if (!ImGui::BeginMenuBar())
    //    {
    //        return;
    //    }
    //
    //    if (ImGui::BeginMenu("Settings"))
    //    {
    //
    //
    //        ImGui::EndMenu();
    //    }
    //
    //    ImGui::EndMenuBar();
    //}

#pragma endregion Private Functions

}	// namespace
