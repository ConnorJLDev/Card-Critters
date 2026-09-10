//------------------------------------------------------------------------------
//
// File Name:	AnimationSequence.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Object that encapsulates an animation sequence.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

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
	class AnimationCurve;
	class AnimationLink;
	class AnimationSequence;

	// Typedefs:
	typedef LibraryStatic<AnimationSequence> AnimationSequenceLibrary;

	// Class Definition:
	class AnimationSequence : public NamedObject
	{
		friend class Animation;
		friend class Particle;

		// Public Constants, Enums, and Structs:
	public:
		DECLARE_READ_KEY(Animation);
		DECLARE_READ_DIR(DIR_DATA"Animations");

		// Constructors/Destructors:
	public:
		AnimationSequence();

		~AnimationSequence();

		// Public Static Functions:
	public:

		// Public Functions:
	public:
		//
		void Read(StreamIn& stream);

		//
		void UpdateGroups(float time, vector_t<float>& results) const;

		//
		void DisplayIMGUI();

		// Public Event Handlers
	public:

		// Private Constants, Enums, and Structs:
	private:
		static const StreamKey ReadKeyLoop, ReadKeyDuration, ReadKeyLinks, ReadKeyCurves;

		// Private Static Functions:
	private:

		// Private Functions:
	private:

		// Private Static Variables:
	private:

		// Private Variables:
	private:
		// How long the animation is.
		float duration_;

		// The default looping behavior.
		AnimationLoop loop_;

		// All curves used by this animation.
		vector_t<AnimationCurve*> curves_;

		// Mappings of curves to destination components.
		vector_t<AnimationLink*> links_;
	};

}	// namespace
