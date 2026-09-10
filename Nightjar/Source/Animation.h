//------------------------------------------------------------------------------
//
// File Name:	Animation.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Component for handling animations.
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
#include "AnimationLoop.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	// Forward Declarations:
	class AnimationSequence;

	// Typedefs:

	// Class Definition:
	class Animation : public Component
	{
		friend class ImGuiSystem;

		// Public Constants, Enums, and Structs:
	public:
		// Common component data.
		COMPONENT_COMMON(Animation);

		// Constructors/Destructors:
	public:
		Animation();
	private:
		Animation(const Animation& other);
	public:
		~Animation();

		// Public Static Functions:
	public:

		// Public Functions:
	public:
		/// <summary>
		/// Clone the Animation.
		/// </summary>
		/// <returns>A clone of this Animation.</returns>
		Animation* Clone() const override;

		/// <summary>
		/// Read Animation data from a stream.
		/// </summary>
		/// <param name="stream">The stream to read from.</param>
		void Read(StreamIn& stream) override;

		/// <summary>
		/// Writes the Animation data into the stream.
		/// </summary>
		/// <param name="stream">The stream to write into.</param>
		void Write(StreamOut& stream) override;

		/// <summary>
		/// Update this Animation.
		/// </summary>
		/// <param name="dt">The time elapsed since the previous tick.</param>
		void Update(float dt) override;

		/// <summary>
		/// Checks if the Animation is playing.
		/// </summary>
		/// <returns>True if the animation is playing, false otherwise.</returns>
		bool GetPlaying() const;

		/// <summary>
		/// Sets the playing state of the animation.
		/// </summary>
		/// <param name="playing">True to play the animation, false to stop the animation.</param>
		/// <param name="fromStart">If "playing" is true, then setting this to true will make the animation start at the beginning.</param>
		void SetPlaying(bool playing, bool fromStart = false);

		/// <summary>
		/// Gets the looping behavior of the animation.
		/// </summary>
		/// <returns>The current looping behavior.</returns>
		AnimationLoop GetLoop();

		/// <summary>
		/// Sets the looping behavior of the animation.
		/// </summary>
		/// <param name="loop">The new looping behavior.</param>
		/// <returns>The old looping behavior.</returns>
		AnimationLoop SetLoop(AnimationLoop loop);

		/// <summary>
		/// Gets the speed of the animation.
		/// </summary>
		/// <returns>The current speed.</returns>
		float GetSpeed() const;

		/// <summary>
		/// Sets the speed of the animation.
		/// </summary>
		/// <param name="speed">The new speed.</param>
		/// <returns>The old speed.</returns>
		float SetSpeed(float speed);

		/// <summary>
		/// Gets the time of the animation.
		/// </summary>
		/// <returns>The current time.</returns>
		float GetTime() const;

		/// <summary>
		/// Gets the normalized time of the animation.
		/// </summary>
		/// <returns>The current normalized time.</returns>
		float GetTimeNormalized() const;

		/// <summary>
		/// Sets the time of the animation.
		/// </summary>
		/// <param name="time">The new time.</param>
		/// <returns>The old time.</returns>
		float SetTime(float time);

		/// <summary>
		/// Gets the duration of the animation.
		/// </summary>
		/// <returns>The current duration.</returns>
		float GetDuration() const;

		/// <summary>
		/// Sets the duration of the animation.
		/// </summary>
		/// <param name="duration">The new duration.</param>
		/// <returns>The old duration.</returns>
		float SetDuration(float duration);

		/// <summary>
		/// Gets the sequence of the animation.
		/// </summary>
		/// <returns>The current sequence.</returns>
		const AnimationSequence* GetSequence() const;

		/// <summary>
		/// Sets the sequence of the animation.
		/// </summary>
		/// <param name="sequence">The new sequence.</param>
		/// <returns>The old sequence.</returns>
		const AnimationSequence* SetSequence(const AnimationSequence* sequence, bool updateParams = true);

		/// <summary>
		/// Sets the sequence of the animation.
		/// </summary>
		/// <param name="sequenceName">The name of the new sequence.</param>
		/// <returns>The old sequence.</returns>
		const AnimationSequence* SetSequence(string_view_arg sequenceName, bool updateParams = true, bool setIfNull = true);

		/// <summary>
		/// Checks if the Animation is mirrored.
		/// </summary>
		/// <returns>True if the animation is mirrored, false otherwise.</returns>
		bool GetMirrored() const;

		/// <summary>
		/// Sets the mirror status of the animation.
		/// </summary>
		/// <param name="mirror">The new mirror status.</param>
		/// <returns>The old mirror status.</returns>
		bool SetMirrored(bool mirror);

		/// <summary>
		/// Checks if the Animation has completed.
		/// </summary>
		/// <returns>True if the animation has finished, false otherwise.</returns>
		bool IsDone() const;

		// Public Event Handlers
	public:

		// Private Constants, Enums, and Structs:
	private:
		// Keys for reading from a stream.
		static const StreamKey ReadKeySequence, ReadKeyDuration, ReadKeySpeed, ReadKeyLoop, ReadKeyTime;

		// Private Static Functions:
	private:

		// Private Functions:
	private:
		//
		void StartLinks();

		//
		void UpdateLinks();

		// Private Static Variables:
	private:

		// Private Variables:
	private:
		// The sequence being referenced.
		const AnimationSequence* sequence_;

		// Temporary container for evaluated curve values.
		vector_t<float> curvesEvaluated_;

		// The active time in the animation.
		float time_;

		// How long the animation is.
		float duration_;

		// How fast the animation is.
		float speed_;

		// True if the animation is running; false if the animation has stopped.
		bool isRunning_;

		// True if the animation is running in reverse; false if the animation is running normally.
		bool isMirrored_;

		// True if the end of the animation sequence has been reached, false otherwise.
		bool isDone_;

		// How the animation behaves when it reaches the end of its duration.
		AnimationLoop loopAction_;
	};

}	// namespace
