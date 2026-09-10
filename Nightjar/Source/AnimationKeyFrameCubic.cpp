//------------------------------------------------------------------------------
//
// File Name:	AnimationKeyFrameCubic.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Object that represents a cubic key frame in an animation curve.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include <cassert>
#include <cmath>

#include "AnimationKeyFrameCubic.h"
#include "Trace.h"
#include "Stream.h"

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

	KeyFrameCubic::KeyFrameCubic(const float* data)
		: KeyFrame(data)
		, cubicB(data[2])
		, cubicC(data[3])
		, cubicD(data[4])
	{
	}

	//--------------------------------------------------------------------------

	KeyFrameCubic::~KeyFrameCubic()
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

	float KeyFrameCubic::EvaluateNormalized_(const KeyFrame* next, float timeNormalized)
	{
		UNREFERENCED_PARAMETER(next);
#define cubicA this->value
#define timeNorm1 timeNormalized
		const float timeNorm2(timeNorm1 * timeNorm1), timeNorm3(timeNorm1 * timeNorm2);
		return cubicA + timeNorm1 * cubicB + timeNorm2 * cubicC + timeNorm3 * cubicD;
#undef timeNorm1
#undef cubicB
#undef cubicA
	}

#pragma endregion Private Functions

}	// namespace
