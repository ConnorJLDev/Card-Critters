//------------------------------------------------------------------------------
//
// File Name:	AnimationCurve.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Object that represents an animation curve.
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

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	// Forward Declarations:
	struct KeyFrame;

	// Typedefs:

	// Class Definition:
	class AnimationCurve
	{
		friend class AnimationSequence;
		// Public Constants, Enums, and Structs:
	public:
		enum class CurveType : unsigned char
		{
			Constant,
			Stepped,
			Linear,
			Cubic,
			NUM_ENTRIES,
		};

		// Constructors/Destructors:
	public:
		AnimationCurve();

		~AnimationCurve();

		// Public Static Functions:
	public:
		//
		static AnimationCurve* Read(StreamIn& stream);

		// Public Functions:
	public:
		//
		float Evaluate(float time);

		//
		CurveType GetType() const;

		//
		void DisplayIMGUI(float timespan = 0);

		// Public Event Handlers
	public:

		// Private Constants, Enums, and Structs:
	private:
		static const StreamKey ReadKeyType, ReadKeyData, ReadKeyDuration, ReadKeyConstant;

		ENUM_MAPS_CLASS_SEQUENTIAL_DECLARE(CurveType)

		// Private Static Functions:
	private:

		// Private Functions:
	private:
		//
		void EvaluateNearestKeyFrames(float time, KeyFrame*& left, KeyFrame*& right);

		//
		template<typename T>
		void ReadKeyFrames(const vector_t<float>& raw);

		// Private Static Variables:
	private:

		// Private Variables:
	private:
		// The KeyFrames contained in this curve.
		vector_t<KeyFrame*> keyFrames_;

		// How long this curve runs for.
		float duration_;

		// The value returned if this curve is constant, or the default value if an error occurs while evaluating.
		float constant_;

		// The interpolation between KeyFrames.
		CurveType type_;
	};

}	// namespace
