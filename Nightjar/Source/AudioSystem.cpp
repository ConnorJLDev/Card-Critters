//------------------------------------------------------------------------------
//
// File Name:	AudioSystem.cpp
// Author(s):	Andrew Lamb
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		System that handles audio.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "AudioSystem.h"
#include "BaseSystem.h"
#include "fmod.hpp"
#include "fmod_errors.h"
#include "Sound.h"
#include "InputSystem.h"
#include "PlatformSystem.h"

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
    //const float MAX_VOLUME = 1.f;
    //--------------------------------------------------------------------------
    // Private Static Variables:
    //--------------------------------------------------------------------------

    AudioSystem* AudioSystem::instance = nullptr;

    //--------------------------------------------------------------------------
    // Private Variables:
    //--------------------------------------------------------------------------

    FMOD::System* SoundSystem = 0;
    FMOD::ChannelGroup* MasterChannelGroup = 0;
    FMOD::ChannelGroup* SFXChannelGroup = 0;
    FMOD::ChannelGroup* MusicChannelGroup = 0;
    FMOD::Channel* SFXChannel = 0;
    FMOD::Channel* MusicChannel = 0;
    static float global_volume = .6f;
    static float volume_scale = .1f;

    static bool focusLast = false;
    static bool focusSFXWasPaused = false;
    static bool focusMusicWasPaused = false;

    //--------------------------------------------------------------------------
    // Constructors/Destructors:
    //--------------------------------------------------------------------------

#pragma region Constructors

    AudioSystem::AudioSystem(void)
        : BaseSystem("AudioSystem", LoopFlags::HasUpdate)
    {
        // Raise an assert if this system has already been created.
        assert(instance == nullptr);

        // Store this system's instance for use by static functions.
        instance = this;
    }

    //--------------------------------------------------------------------------

    AudioSystem::~AudioSystem(void)
    {
    }

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

    BaseSystem* AudioSystem::GetInstance()
    {
        return instance != nullptr ? instance : new AudioSystem();
    }

    float AudioSystem::GetVolumeSFX()
    {
        float volume = 0.f;
        SFXChannelGroup->getVolume(&volume);
        return volume / volume_scale;
    }

    float AudioSystem::GetVolumeMusic()
    {
        float volume = 0.f;
        MusicChannelGroup->getVolume(&volume);
        return volume / volume_scale;
    }

    void AudioSystem::SetVolumeSFX(float input)
    {
        //input *= MAX_VOLUME;
        SFXChannelGroup->setVolume(volume_scale * std::clamp(input, 0.f, 1.f));
    }

    void AudioSystem::SetVolumeMusic(float input)
    {
        //input *= MAX_VOLUME;
        MusicChannelGroup->setVolume(volume_scale * std::clamp(input, 0.f, 1.f));
    }

    float AudioSystem::GetPitchSFX()
    {
        float result = 0.f;
        SFXChannelGroup->getPitch(&result);
        return result;
    }

    float AudioSystem::GetPitchMusic()
    {
        float result = 0.f;
        MusicChannelGroup->getPitch(&result);
        return result;
    }

    void AudioSystem::SetPitchSFX(float input)
    {
        SFXChannelGroup->setPitch(std::clamp(input, 0.1f, 2.f));
    }

    void AudioSystem::SetPitchMusic(float input)
    {
        MusicChannelGroup->setPitch(std::clamp(input, 0.1f, 2.f));
    }

    bool AudioSystem::GetPausedSFX()
    {
        bool paused;
        SFXChannelGroup->getPaused(&paused);
        return paused;
    }

    bool AudioSystem::GetPausedMusic()
    {
        bool paused;
        MusicChannelGroup->getPaused(&paused);
        return paused;
    }

    void AudioSystem::SetPausedSFX(bool input)
    {
        SFXChannelGroup->setPaused(input);
    }

    void AudioSystem::SetPausedMusic(bool input)
    {
        MusicChannelGroup->setPaused(input);
    }

    float AudioSystem::GetVolumeScale()
    {
        return volume_scale;
    }

    FMOD::Sound* AudioSystem::CreateSound(const char* input, bool isLooping)
    {
        FMOD_RESULT result;
        FMOD::Sound* sound;
        FORMAT_MAKE(pathName, "Assets/Sounds/{}", input);
        if (isLooping)
        {
            result = SoundSystem->createSound(pathName.c_str(), FMOD_LOOP_NORMAL, nullptr, &sound);
        }
        else
        {
            result = SoundSystem->createSound(pathName.c_str(), FMOD_2D, nullptr, &sound);
        }
        ERRCHECK(result);
        return sound;
    }

    FMOD::Sound* AudioSystem::CreateSound(Sound* input)
    {
        FMOD_RESULT result;
        FMOD::Sound* sound;
        FORMAT_MAKE(pathName, "Assets/Sounds/{}", input->GetFilePath());
        if (input->GetIsLooping())
        {
            result = SoundSystem->createSound(pathName.c_str(), FMOD_LOOP_NORMAL, nullptr, &sound);
        }
        else
        {
            result = SoundSystem->createSound(pathName.c_str(), FMOD_2D, nullptr, &sound);
        }
        ERRCHECK(result);
        return sound;
    }

    void AudioSystem::ReleaseSound(FMOD::Sound* soundInp)
    {
        FMOD_RESULT result;
        if (soundInp) {
            result = soundInp->release();
            ERRCHECK(result);
        }
    }

    void AudioSystem::PlaySound(const Sound* soundInp)
    {
        PlaySound(soundInp, 1.f);
    }

    void AudioSystem::PlaySound(const Sound* soundInp, float pitch)
    {
        FMOD_RESULT result;
        if (soundInp->GetType() == Sound::AudioType::Music)
        {
            result = SoundSystem->playSound(soundInp->GetFMODSound(), MusicChannelGroup, soundInp->GetIsPaused(), &MusicChannel);
            MusicChannel->setPitch(pitch);
        }
        else
        {
            result = SoundSystem->playSound(soundInp->GetFMODSound(), SFXChannelGroup, soundInp->GetIsPaused(), &SFXChannel);
            SFXChannel->setPitch(pitch);
        }
        ERRCHECK(result);
    }

    void AudioSystem::PauseSound(const Sound* soundInp)
    {
        FMOD_RESULT result;
        if (soundInp->GetType() == Sound::AudioType::Music)
        {
            result = MusicChannel->stop();
        }
        else
        {
            result = SFXChannel->stop();
        }
        ERRCHECK(result);
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

    void AudioSystem::ERRCHECK(FMOD_RESULT checkResult)
    {
        if (checkResult != FMOD_OK)
        {
            TraceError("FMOD error: ({}) {}", static_cast<int>(checkResult), FMOD_ErrorString(checkResult));
        }
    }

#pragma endregion Private Static Functions

    //--------------------------------------------------------------------------
    // Private Functions:
    //--------------------------------------------------------------------------

#pragma region Private Functions

    bool AudioSystem::Init(void)
    {
        SoundLibrary::Init_S();

        FMOD_RESULT result;
        result = FMOD::System_Create(&SoundSystem, FMOD_VERSION);
        ERRCHECK(result);
        result = SoundSystem->init(32, FMOD_INIT_NORMAL, nullptr);
        ERRCHECK(result);
        
        result = SoundSystem->getMasterChannelGroup(&MasterChannelGroup);
        ERRCHECK(result);

        result = SoundSystem->createChannelGroup("SFXChannelGroup", &SFXChannelGroup);
        ERRCHECK(result);
        result = SoundSystem->createChannelGroup("MusicChannelGroup", &MusicChannelGroup);
        ERRCHECK(result);
        //TraceInfo("Number of channels: {}", SoundSystem->channels);
        SFXChannel = 0;
        MusicChannel = 0;
        result = SFXChannel->setChannelGroup(SFXChannelGroup);
        ERRCHECK(result);
        result = MusicChannel->setChannelGroup(MusicChannelGroup);
        ERRCHECK(result);
        SetVolumeSFX(1.f);
        SetVolumeMusic(1.f);
        SetPitchSFX(1.f);
        SetPitchMusic(1.f);
        // Return true if the initialization completed successfully.
        SetVolumeSFX(global_volume);
        SetVolumeMusic(global_volume);
        return true;
    }

    void AudioSystem::Update(void)
    {
        bool isFocused = !PlatformSystem::GetIsIconified() && PlatformSystem::GetIsFocused();
        if (focusLast != isFocused)
        {
            if (isFocused)
            {
                SetPausedSFX(focusSFXWasPaused);
                SetPausedMusic(focusMusicWasPaused);
            }
            else
            {
                focusSFXWasPaused = GetPausedSFX();
                focusMusicWasPaused = GetPausedMusic();
                SetPausedSFX(true);
                SetPausedMusic(true);
            }
            focusLast = isFocused;
        }

#ifdef _DEBUG
        if (InputSystem::KeyIsHeld(B_Keyboard::LeftShift))
        {
            if (InputSystem::KeyIsTriggered(B_Keyboard::Minus))
            {
                SetPitchSFX(GetPitchSFX() - 0.1f);
                SetPitchMusic(GetPitchMusic() - 0.1f);
                //TraceInfo("Pitch: {:.1f}", GetPitchSFX());
            }
            if (InputSystem::KeyIsTriggered(B_Keyboard::Equal))
            {
                SetPitchSFX(GetPitchSFX() + 0.1f);
                SetPitchMusic(GetPitchMusic() + 0.1f);
                //TraceInfo("Pitch: {:.1f}", GetPitchSFX());
            }
        }
        else
        {
            if (InputSystem::KeyIsTriggered(B_Keyboard::Minus))
            {
                SetVolumeSFX(GetVolumeSFX() - 0.1f);
                SetVolumeMusic(GetVolumeMusic() - 0.1f);
                //TraceInfo("Volume: {:.1f}", GetVolumeSFX());
            }
            if (InputSystem::KeyIsTriggered(B_Keyboard::Equal))
            {
                SetVolumeSFX(GetVolumeSFX() + 0.1f);
                SetVolumeMusic(GetVolumeMusic() + 0.1f);
                //TraceInfo("Volume: {:.1f}", GetVolumeSFX());
            }
        }
        
        if (InputSystem::KeyIsTriggered(B_Keyboard::Space))
        {
            SetPausedSFX(!GetPausedSFX());
            SetPausedMusic(!GetPausedMusic());
            //TraceInfo("Paused: {}", GetPausedSFX());
        }
#endif
        FMOD_RESULT result = SoundSystem->update();
        ERRCHECK(result);
    }

    void AudioSystem::Exit(void)
    {
        SoundLibrary::Shutdown_S();

        if (SoundSystem)
        {
            FMOD_RESULT result;
            result = SoundSystem->close();
            ERRCHECK(result);
            result = SoundSystem->release();
            ERRCHECK(result);
        }
    }

#pragma endregion Private Functions

}	// namespace
