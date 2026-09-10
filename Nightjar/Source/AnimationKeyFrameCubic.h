//------------------------------------------------------------------------------
//
// File Name:	AnimationKeyFrameCubic.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Object that represents a cubic key frame in an animation curve.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "AnimationKeyFrame.h"

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
	struct KeyFrameCubic : KeyFrame
	{
		// Public Constants, Enums, and Structs:
	public:
		static const constexpr unsigned Size = 5;

		// Constructors/Destructors:
	public:
		KeyFrameCubic(const float* data);

		~KeyFrameCubic();

		// Public Static Functions:
	public:

		// Public Functions:
	public:

		// Public Event Handlers
	public:

		// Public Variables
	public:
		//
		float cubicB, cubicC, cubicD;

		// Private Constants, Enums, and Structs:
	private:

		// Private Static Functions:
	private:

		// Private Functions:
	private:
		//
		float EvaluateNormalized_(const KeyFrame* next, float timeNormalized) override;

		// Private Static Variables:
	private:

		// Private Variables:
	private:
		//
	};

}	// namespace
