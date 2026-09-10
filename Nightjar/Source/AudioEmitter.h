//------------------------------------------------------------------------------
//
// File Name:	AudioEmitter.h
// Author(s):	Andrew Lamb (andrew.lamb)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Audio Emitter component class header
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Component.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	// Forward Declarations:
	class Sound;

	// Typedefs:

	// Class Definition:
	class AudioEmitter : public Component
	{
		// Public Constants, Enums, and Structs:
	public:
		COMPONENT_COMMON(AudioEmitter);
		
		// Constructors/Destructors:
	public:
		AudioEmitter(void);
	private:
		AudioEmitter(const AudioEmitter& other);
	public:
		~AudioEmitter(void);

		// Public Static Functions:
	public:

		// Public Functions:
	public:
		/// <summary>
		/// Clone the AudioEmitter.
		/// </summary>
		/// <returns>A clone of this AudioEmitter.</returns>
		AudioEmitter* Clone() const override;
		
		/// <summary>
		/// Read AudioEmitter data from a stream.
		/// </summary>
		/// <param name="stream">The stream to read from.</param>
		void Read(StreamIn& stream) override;

		/// <summary>
		/// Writes the AudioEmitter data into the stream.
		/// </summary>
		/// <param name="stream">The stream to write into.</param>
		void Write(StreamOut& stream) override;

		/// <summary>
		/// Update this AudioEmitter.
		/// </summary>
		/// <param name="dt">The time elapsed since the previous tick.</param>
		void Update(float dt) override;
		
		/// <summary>
		/// Render this AudioEmitter.
		/// </summary>
		void Render() const override;

		//Emits audio
		void Play();

		//Sets the pitch to a random value within range
		void RandPitch(float min, float max);

		//Sets isPaused to true
		void Pause();

		//Returns volume float
		float GetVolume() const;

		//Sets volume float
		void SetVolume(float volume);

		//Returns pitch float
		float GetPitch() const;

		//Sets pitch float
		void SetPitch(float pitch);

		//Sets sound pointer
		void SetSound(const Sound* sound);

		void SetSound(string_view_arg sound);
		
		//Returns sound pointer
		const Sound* GetSound() const;

	

		//Returns isPaused
		bool GetIsPaused();

		// Public Event Handlers
	public:

		// Private Constants, Enums, and Structs:
	private:
		static const StreamKey ReadKeySound;

		// Private Static Functions:
	private:

		// Private Functions:
	private:

		// Private Static Variables:
	private:

		// Private Variables:
	private:
		const Sound* sound_;
		float pitch_;
		float volume_;
		bool isPaused_;
	};

}	// namespace
