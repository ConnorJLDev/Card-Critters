//------------------------------------------------------------------------------
//
// File Name:	Sound.cpp
// Author(s):	lambg
// Course:		GAM200F25
// Project:		Critter Cards
// Purpose:		Template class for a new object.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "AudioSystem.h"
#include "Sound.h"

#include "fmod.hpp"
#include "fmod_errors.h"

#include "Library.inl"
#include "LibraryStatic.inl"

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

	ENUM_MAPS_CLASS_SEQUENTIAL(Sound, AudioType, SoundEffect, Music);

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

	Sound::Sound()
		: NamedObject()
		, fmodSound_(nullptr)
		, filePath_()
		, type_(AudioType::None)
		, isLooping_(false)
		, isPaused_(false)
	{
	}

	Sound::Sound(string_view_arg name, AudioType typeInput)
		: NamedObject(name)
		, fmodSound_(nullptr)
		, filePath_(name)
		, type_(typeInput)
		, isLooping_(typeInput == AudioType::Music)
		, isPaused_(false)
	{
		fmodSound_ = AudioSystem::CreateSound(this);
	}

	Sound::Sound(const Sound& other)
		: NamedObject(other)
		, fmodSound_(other.fmodSound_ == nullptr ? nullptr : AudioSystem::CreateSound(other.GetName().c_str()))
		, filePath_(other.filePath_)
		, type_(other.type_)
		, isLooping_(other.isLooping_)
		, isPaused_(false)
	{
	}

	//--------------------------------------------------------------------------

	Sound::~Sound(void)
	{
		AudioSystem::ReleaseSound(fmodSound_);
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

	void Sound::Read(StreamIn& stream)
	{
		ReadName(stream);

		TraceInfo("Reading Sound: {}", GetName());

		stream.TryReadValue("IsLooping", isLooping_);

		if (stream.TryReadValue("FileName", filePath_))
		{
			fmodSound_ = AudioSystem::CreateSound(filePath_.c_str(), isLooping_);
		}

		stream.TryReadEnum("Type", ENUM_READ(AudioType), type_);
	}

	void Sound::Play()
	{
		isPaused_ = false;
		AudioSystem::PlaySound(this);
	}

	void Sound::Play(float pitch) const
	{
		AudioSystem::PlaySound(this, pitch);
	}

	void Sound::Pause()
	{
		isPaused_ = true;
		AudioSystem::PauseSound(this);
	}

	FMOD::Sound* Sound::GetFMODSound() const
	{
		return fmodSound_;
	}

	void Sound::SetFMODSound(FMOD::Sound* input)
	{
		fmodSound_ = input;
	}

	Sound::AudioType Sound::GetType() const
	{
		return type_;
	}

	void Sound::SetType(AudioType input)
	{
		type_ = input;
	}

	bool Sound::GetIsLooping() const
	{
		return isLooping_;
	}

	void Sound::SetIsLooping(bool input)
	{
		isLooping_ = input;
	}

	bool Sound::GetIsPaused() const
	{
		return isPaused_;
	}

	const string_t& Sound::GetFilePath() const
	{
		return filePath_;
	}

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

#pragma endregion Private Functions

	//--------------------------------------------------------------------------
	// Library Implementation:
	//--------------------------------------------------------------------------

#pragma region Library Implementation

	template SoundLibrary;

	INSTANTIATE_LIBRARY_IMGUI_SELECTOR(Sound);

#pragma endregion Library Implementation

}	// namespace