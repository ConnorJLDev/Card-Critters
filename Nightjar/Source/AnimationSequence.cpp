//------------------------------------------------------------------------------
//
// File Name:	AnimationSequence.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Object that encapsulates an animation sequence.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "AnimationCurve.h"
#include "AnimationSequence.h"
#include "AnimationLink.h"
#include "AnimationLoop.h"

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

	constexpr StreamKey
		AnimationSequence::ReadKeyLoop = "Loop",
		AnimationSequence::ReadKeyDuration = "Duration",
		AnimationSequence::ReadKeyLinks = "Links",
		AnimationSequence::ReadKeyCurves = "Curves";

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

	AnimationSequence::AnimationSequence()
		: NamedObject()
		, duration_(0)
		, loop_(AnimationLoop::Stop)
	{
	}

	//--------------------------------------------------------------------------

	AnimationSequence::~AnimationSequence()
	{
        for (auto& link : links_)
        {
            SafeDeleteCheck(link);
        }
        for (auto& curve : curves_)
        {
            SafeDeleteCheck(curve);
        }
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

	void AnimationSequence::Read(StreamIn& stream)
	{
		ReadName(stream);

		TraceInfo("Reading animation: {}", GetName());

		stream.TryReadEnum(ReadKeyLoop, ENUM_READ(AnimationLoop), loop_);

		stream.TryReadValue(ReadKeyDuration, duration_);

		if (stream.StreamHas(ReadKeyLinks))
		{
			links_.clear();
			stream.ReadArrayObjects(ReadKeyLinks, [this](StreamIn& stream, int index)
			{
				UNREFERENCED_PARAMETER(index);
				AnimationLink* link = AnimationLink::Read(stream);
				if (link != nullptr)
				{
					links_.push_back(link);
				}
			});
		}

		if (stream.StreamHas(ReadKeyCurves))
		{
			curves_.clear();
			stream.ReadArrayObjects(ReadKeyCurves, [this](StreamIn& stream, int index)
			{
				UNREFERENCED_PARAMETER(index);
				AnimationCurve* curve = AnimationCurve::Read(stream);
				if (curve != nullptr)
				{
					curves_.push_back(curve);
				}
			});
		}
	}

	void AnimationSequence::UpdateGroups(float time, vector_t<float>& results) const
	{
		if (results.size() < curves_.size())
		{
			results.resize(curves_.size());
		}
		for (size_t i = 0; i < curves_.size(); ++i)
		{
			results[i] = curves_[i]->Evaluate(time);
		}
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

	template AnimationSequenceLibrary;

	INSTANTIATE_LIBRARY_IMGUI_SELECTOR(AnimationSequence);

	IMPLEMENT_READ(AnimationSequence);

	IMPLEMENT_WRITE(AnimationSequence);

#pragma endregion Library Implementation

}	// namespace
