//------------------------------------------------------------------------------
//
// File Name:	AnimationCurve.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Object that represents an animation curve.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "AnimationCurve.h"
#include "AnimationKeyFrame.h"
#include "AnimationKeyFrameStepped.h"
#include "AnimationKeyFrameLinear.h"
#include "AnimationKeyFrameCubic.h"

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
		  AnimationCurve::ReadKeyType = "Type"
		, AnimationCurve::ReadKeyData = "Data"
		, AnimationCurve::ReadKeyDuration = "Duration"
		, AnimationCurve::ReadKeyConstant = "Constant"
		;

	ENUM_MAPS_CLASS_SEQUENTIAL(AnimationCurve, CurveType, Constant, Stepped, Linear, Cubic);

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

	AnimationCurve::AnimationCurve()
		: keyFrames_()
		, duration_(0)
		, constant_(0)
		, type_(CurveType::Constant)
	{
	}

	AnimationCurve::~AnimationCurve()
	{
		for (auto kf : keyFrames_)
		{
			delete kf;
		}
	}

#pragma endregion Constructors

	//--------------------------------------------------------------------------
	// Public Static Functions:
	//--------------------------------------------------------------------------

#pragma region Public Static Functions

	AnimationCurve* AnimationCurve::Read(StreamIn& stream)
	{
		if (!stream.StreamHas(ReadKeyType))
		{
			TraceWarning("Failed to read curve type!");
			return nullptr;
		}

		AnimationCurve* curve = new AnimationCurve();

		stream.TryReadEnum(ReadKeyType, ENUM_READ(CurveType), curve->type_);
		const auto& type = curve->type_;

		if (type != CurveType::Constant && stream.StreamHas(ReadKeyData))
		{
			vector_t<float> raw;
			stream.ReadVector(ReadKeyData, raw);
			switch (type)
			{
			case CurveType::Stepped:
				curve->ReadKeyFrames<KeyFrameStepped>(raw);
				break;
			case CurveType::Linear:
				curve->ReadKeyFrames<KeyFrameLinear>(raw);
				break;
			case CurveType::Cubic:
				curve->ReadKeyFrames<KeyFrameCubic>(raw);
				break;
			case CurveType::Constant:
			default:
				TraceWarning("Unexpected keyframe type to read data: {}", ENUM_WRITE_VALUE(CurveType, type));
				break;
			}
		}

		if (stream.StreamHas(ReadKeyConstant))
		{
			curve->constant_ = stream.ReadFloat(ReadKeyConstant);
		}

		if (stream.StreamHas(ReadKeyDuration))
		{
			curve->duration_ = stream.ReadFloat(ReadKeyDuration);
		}
		else if (curve->keyFrames_.size() > 0)
		{
			curve->duration_ = curve->keyFrames_.back()->time;
		}

		return curve;
	}

#pragma endregion Public Static Functions

	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

	float AnimationCurve::Evaluate(float time)
	{
		if (time > duration_)
		{
			time = duration_;
		}
		else if (time < 0)
		{
			time = 0;
		}
		switch (type_)
		{
		case CurveType::Constant:
			return constant_;
		case CurveType::Stepped:
		case CurveType::Linear:
		case CurveType::Cubic:
			KeyFrame* left, * right;
			EvaluateNearestKeyFrames(time, left, right);
			if (left == nullptr)
			{
				if (right == nullptr)
				{
					TraceWarning("Non-const curve ({:d}) has no keyframes!", type_);
					return constant_;
				}
				return right->value;
			}
			return left->Evaluate(right, time);
		default:
			TraceWarning("Unexpected curve type: {:d}", type_);
			return constant_;
		}
	}

	AnimationCurve::CurveType AnimationCurve::GetType() const
	{
		return type_;
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

	void AnimationCurve::EvaluateNearestKeyFrames(float time, KeyFrame*& left, KeyFrame*& right)
	{
		for (auto it = keyFrames_.begin(); it != keyFrames_.end(); ++it)
		{
			if ((*it)->time > time)
			{
				right = *it;
				left = it == keyFrames_.begin() ? nullptr : *--it;
				return;
			}
		}
		right = nullptr;
		left = keyFrames_.empty() ? nullptr : keyFrames_.back();
	}

	template<typename T>
	void AnimationCurve::ReadKeyFrames(const vector_t<float>& raw)
	{
		if (raw.size() % T::Size)
		{
			TraceWarning("Keyframe data is not a multiple of {}: {}", T::Size, raw.size());
		}
		const size_t limit = raw.size() - T::Size + 1;
		for (size_t i = 0; i < limit; i += T::Size)
		{
			keyFrames_.emplace_back(new T(&raw[i]));
		}
	}

#pragma endregion Private Functions

}	// namespace
