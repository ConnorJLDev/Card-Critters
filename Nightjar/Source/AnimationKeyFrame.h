//------------------------------------------------------------------------------
//
// File Name:	AnimationKeyFrame.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Object that represents a key frame in an animation curve.
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

	// Typedefs:

	// Struct Definition:
	struct KeyFrame
	{
		// Public Constants, Enums, and Structs:
	public:
		static const constexpr unsigned Size = 2;

		// Constructors/Destructors:
	public:
		KeyFrame(const float* data);

		virtual ~KeyFrame() = 0;

		// Public Static Functions:
	public:

		// Public Functions:
	public:

		//
		float Evaluate(const KeyFrame* next, float time);

		//
		virtual void DisplayIMGUI();

		// Public Event Handlers
	public:

		// Public Variables:
	public:
		float time, value;

		// Protected Constants, Enums, and Structs:
	protected:

		// Protected Static Functions:
	protected:

		// Protected Functions:
	protected:
		//
		virtual float EvaluateNormalized_(const KeyFrame* next, float timeNormalized) = 0;

		//
		virtual void OnNextChange_(const KeyFrame* next);

		// Protected Static Variables:
	protected:

		// Protected Variables:
	protected:
		//
		const KeyFrame* last_;
		//
		float lastFactor_;
	};

}	// namespace
