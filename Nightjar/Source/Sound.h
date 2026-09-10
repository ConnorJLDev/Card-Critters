//------------------------------------------------------------------------------
//
// File Name:	Sound.h
// Author(s):	Andrew Lamb
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Sound class header
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

namespace FMOD
{
	class Sound;
}

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	// Forward Declarations:
	class Sound;

	// Typedefs:
	typedef LibraryStatic<Sound> SoundLibrary;

	// Class Definition:
	class Sound : public NamedObject
	{
		// Public Constants and Enums:
	public:
		DECLARE_READ_KEY(Sound);
		DECLARE_READ_DIR(DIR_DATA"Sounds");

		enum class AudioType
		{
			None = -1,
			SoundEffect = 0,
			Music,
			NUM_ENTRIES,
		};

		// Constructors/Destructors:
	public:
		Sound(void);

		Sound(string_view_arg name, AudioType typeInput = AudioType::SoundEffect);

		Sound(const Sound& other);

		~Sound(void);

		// Public Static Functions:
	public:

		// Public Functions:
	public:

		void Read(StreamIn& stream);

		void Play();

		void Play(float pitch = 1.f) const;

		void Pause();

		FMOD::Sound* GetFMODSound() const;

		void SetFMODSound(FMOD::Sound* input);

		AudioType GetType() const;

		void SetType(AudioType input);

		bool GetIsLooping() const;

		void SetIsLooping(bool input);

		bool GetIsPaused() const;

		const string_t& GetFilePath() const;

		// Private Functions:
	private:

		// Private Constants:
	private:
		ENUM_MAPS_CLASS_SEQUENTIAL_DECLARE(AudioType);

		// Private Static Variables:
	private:

		// Private Variables:
	private:
		FMOD::Sound* fmodSound_;
		string_t filePath_;
		AudioType type_;
		bool isLooping_;
		bool isPaused_;
	};

}	// namespace