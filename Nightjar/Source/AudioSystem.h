//------------------------------------------------------------------------------
//
// File Name:	AudioSystem.h
// Author(s):	Andrew Lamb
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		System that handles audio.
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

#include "fmod.hpp"
#include "Sound.h"

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
    class AudioSystem SYSTEM_FINAL : public BaseSystem
    {
        friend class Sound;

        SYSTEM_COMMON(AudioSystem);

        // Public Constants and Enums:
    public:

        // Constructors/Destructors:
    private:
        AudioSystem(void);

        // All systems need a virtual destructor to have their destructor called 
        ~AudioSystem(void) override;
    public:

        // Public Static Functions:
    public:
        //Creates sound and returns pointer. Input is file name.
        static FMOD::Sound* CreateSound(const char* input, bool isLooping = false);

        static FMOD::Sound* CreateSound(Sound* input);
        //Releases sound. Input is sound pointer.
        static void ReleaseSound(FMOD::Sound* soundInp);
        //Plays sound. Input is sound pointer.
        static void PlaySound(const Sound* soundInp);

        static void PlaySound(const Sound* soundInp, float pitch);

        static void PauseSound(const Sound* soundInp);
    public:
        // Get the instance of the AudioSystem.
        static BaseSystem* GetInstance(void);

        // Public Functions:
    public:
        static float GetVolumeSFX();

        static float GetVolumeMusic();

        static void SetVolumeSFX(float input);

        static void SetVolumeMusic(float input);

        static float GetPitchSFX();

        static float GetPitchMusic();

        static void SetPitchSFX(float input);

        static void SetPitchMusic(float input);

        static bool GetPausedSFX();
                    
        static bool GetPausedMusic();

        static void SetPausedSFX(bool input);

        static void SetPausedMusic(bool input);

        static float GetVolumeScale();

        // Public Event Handlers
    public:

        // Private Static Functions:
    protected:
        static void ERRCHECK(FMOD_RESULT checkResult);

        //static FMOD::Channel* GetChannel();

        // Private Functions:
    private:
        /// <summary>
        /// Initializes the system.
        /// </summary>
        /// <returns>True if initialization succeeded, false otherwise.</returns>
        bool Init(void) override;

        /// <summary>
        /// Updates the system.
        /// </summary>
        void Update(void) override;

        /// <summary>
        /// Exits and shuts down the system.
        /// </summary>
        void Exit(void) override;

        // Private Static Constants:
    private:

        // Private Static Variables:
        static AudioSystem* instance;
    private:

        // Private Variables:
    private:

    };

}	// namespace
