//------------------------------------------------------------------------------
//
// File Name:	AnimationKeyFrame.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Object that represents a key frame in an animation curve.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

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

	KeyFrame::KeyFrame(const float* data)
		: time(data[0])
		, value(data[1])
		, last_(nullptr)
		, lastFactor_(0)
	{
	}

	//--------------------------------------------------------------------------

	KeyFrame::~KeyFrame()
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

	float KeyFrame::Evaluate(const KeyFrame* next, float timeArg)
	{
		if (next != last_)
		{
			if (next != nullptr)
			{
				lastFactor_ = 1.0f / (next->time - time);
				if (!std::isfinite(lastFactor_))
				{
					TraceWarning("KeyFrame length not finite! [{},{}] ({})", time, next->time, next->time - time);
					lastFactor_ = 0;
				}
				OnNextChange_(next);
			}
			last_ = next;
		}
		if (next == nullptr)
		{
			return value;
		}
		const float timeNormalized((timeArg - time) * lastFactor_);
		return EvaluateNormalized_(next, timeNormalized);
	}

#pragma endregion Public Functions

	//--------------------------------------------------------------------------
	// Protected Static Functions:
	//--------------------------------------------------------------------------

#pragma region Protected Static Functions

#pragma endregion Protected Static Functions

	//--------------------------------------------------------------------------
	// Protected Functions:
	//--------------------------------------------------------------------------

#pragma region Protected Functions

	void KeyFrame::OnNextChange_(const KeyFrame* next)
	{
		UNREFERENCED_PARAMETER(next);
	}

#pragma endregion Protected Functions

}	// namespace
