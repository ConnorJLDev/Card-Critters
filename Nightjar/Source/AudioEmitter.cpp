//------------------------------------------------------------------------------
//
// File Name:	AudioEmitter.cpp
// Author(s):	Andrew Lamb (andrew.lamb)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Audio Emitter component class
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Component.h"
#include "AudioEmitter.h"
#include "Sound.h"
#include "AudioSystem.h"
#include "Random.h"

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

	constexpr StreamKey
		AudioEmitter::ReadKeySound = "Sound";

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

	AudioEmitter::AudioEmitter(void)
		: Component(TypeEnum::AudioEmitter)
		, sound_(nullptr)
		, pitch_(1.0f)
		, volume_(1.0f)
		, isPaused_(true)
	{
	}

	AudioEmitter::AudioEmitter(const AudioEmitter& other)
		: Component(other)
		, sound_(other.sound_)
		, pitch_(other.pitch_)
		, volume_(other.volume_)
		, isPaused_(true)
	{
	}

	//--------------------------------------------------------------------------

	AudioEmitter::~AudioEmitter(void)
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

	AudioEmitter* AudioEmitter::Clone(void) const
	{
		return new AudioEmitter(*this);
	}

	void AudioEmitter::Read(StreamIn& stream)
	{
		string_t fileName;
		if (stream.TryReadValue(ReadKeySound, fileName))
		{
			SetSound(fileName);
		}
	}

	void AudioEmitter::Write(StreamOut& stream)
	{
		stream.WriteValue<const NamedObject*>(ReadKeySound, sound_);
	}

	void AudioEmitter::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);
	}

	void AudioEmitter::Render() const
	{
	}

	void AudioEmitter::Play()
	{
		isPaused_ = false;
		sound_->Play(pitch_);
	}

	void AudioEmitter::RandPitch(float min, float max)
	{
		pitch_ = Random::Range(min, max);
	}

	void AudioEmitter::Pause()
	{
		isPaused_ = true;
		AudioSystem::PauseSound(sound_);
	}

	float AudioEmitter::GetVolume() const
	{
		return volume_;
	}

	void AudioEmitter::SetVolume(float volume)
	{
		volume_ = volume;
	}

	float AudioEmitter::GetPitch() const
	{
		return pitch_;
	}

	void AudioEmitter::SetPitch(float pitch)
	{
		pitch_ = pitch;
	}

	void AudioEmitter::SetSound(const Sound* sound)
	{
		sound_ = sound;
	}

	void AudioEmitter::SetSound(string_view_arg sound)
	{
		SetSound(SoundLibrary::Get_S(sound));
	}

	const Sound* AudioEmitter::GetSound() const
	{
		return sound_;
	}

	bool AudioEmitter::GetIsPaused()
	{
		return isPaused_;
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

}	// namespace
