//------------------------------------------------------------------------------
//
// File Name:	Animation.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Component for handling animations.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Entity.h"
#include "AnimationCurve.h"
#include "Animation.h"
#include "AnimationLink.h"
#include "AnimationSequence.h"

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
		Animation::ReadKeySequence = "Sequence",
		Animation::ReadKeyDuration = "Duration",
		Animation::ReadKeySpeed = "Speed",
		Animation::ReadKeyLoop = "Loop",
		Animation::ReadKeyTime = "Time";

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

	Animation::Animation()
		: Component(TypeEnum::Animation)
		, sequence_(nullptr)
		, curvesEvaluated_()
		, time_(0.0f)
		, duration_(0.0f)
		, speed_(1.0f)
		, isRunning_(false)
		, isMirrored_(false)
		, isDone_(false)
		, loopAction_(AnimationLoop::Stop)
	{
	}

	Animation::Animation(const Animation& other)
		: Component(other)
		, sequence_(other.sequence_)
		, curvesEvaluated_()
		, time_(other.time_)
		, duration_(other.duration_)
		, speed_(other.speed_)
		, isRunning_(other.isRunning_)
		, isMirrored_(other.isMirrored_)
		, isDone_(other.isDone_)
		, loopAction_(other.loopAction_)
	{
	}

	//--------------------------------------------------------------------------

	Animation::~Animation()
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

	Animation* Animation::Clone() const
	{
		return new Animation(*this);
	}

	void Animation::Read(StreamIn& stream)
	{
		if (stream.StreamHas(ReadKeySequence))
		{
			string_t seqName = stream.ReadString(ReadKeySequence);
			sequence_ = AnimationSequenceLibrary::Get_S(seqName);
			if (sequence_ != nullptr)
			{
				duration_ = sequence_->duration_;
				loopAction_ = sequence_->loop_;
			}
			else
			{
				TraceWarning("Failed to find animation sequence {}", seqName);
			}
		}
		stream.TryReadValue(ReadKeyTime, time_);

		isRunning_ = true;
	}

	void Animation::Write(StreamOut& stream)
	{
		stream.WriteValue(ReadKeySequence, sequence_);
	}

	void Animation::Update(float dt)
	{
		isDone_ = false;
		if (!isRunning_)
		{
			return;
		}
		time_ += (isMirrored_ ? -speed_ : speed_) * dt;
		if (time_ >= duration_)
		{
			switch (loopAction_)
			{
			case AnimationLoop::Mirror:
			case AnimationLoop::MirrorOnce:
				if (!isMirrored_)
				{
					isMirrored_ = true;
					time_ = duration_ - (time_ - duration_);
				}
				break;
			case AnimationLoop::Stop:
				time_ = duration_;
				isDone_ = true;
				break;
			case AnimationLoop::Loop:
				time_ -= duration_;
				break;
			default:
				assert(false);
				break;
			}
		}
		else if (time_ <= 0)
		{
			switch (loopAction_)
			{
			case AnimationLoop::Mirror:
				if (isMirrored_)
				{
					isMirrored_ = false;
					time_ = -time_;
				}
				break;
			case AnimationLoop::MirrorOnce:
				if (isMirrored_)
				{
					isMirrored_ = false;
					time_ = 0;
					isDone_ = true;
					isRunning_ = false;
				}
				break;
			case AnimationLoop::Stop:
				time_ = 0;
				isDone_ = true;
				break;
			case AnimationLoop::Loop:
				time_ = 0;
				break;
			default:
				assert(false);
				break;
			}
		}
		UpdateLinks();
		isRunning_ &= !isDone_;
	}

	bool Animation::GetPlaying() const
	{
		return isRunning_;
	}

	void Animation::SetPlaying(bool playing, bool fromStart)
	{
		if (playing)
		{
			if (fromStart)
			{
				time_ = speed_ >= 0.0f ? 0 : duration_;
			}
			isDone_ = false;
			isRunning_ = true;
			StartLinks();
		}
		else
		{
			isDone_ = true;
			isRunning_ = false;
		}
	}

	AnimationLoop Animation::GetLoop()
	{
		return loopAction_;
	}

	AnimationLoop Animation::SetLoop(AnimationLoop loop)
	{
		// If we're changing from a mirrored loop to non-mirrored, reset the reverse flag.
		if ((loopAction_ == AnimationLoop::Mirror || loopAction_ == AnimationLoop::MirrorOnce) && !(loop == AnimationLoop::Mirror || loop == AnimationLoop::MirrorOnce))
		{
			isMirrored_ = false;
		}
		std::swap(loop, loopAction_);
		return loop;
	}

	float Animation::GetSpeed() const
	{
		return speed_;
	}

	float Animation::SetSpeed(float speed)
	{
		std::swap(speed, speed_);
		return speed;
	}

	float Animation::GetTime() const
	{
		return time_;
	}

	float Animation::GetTimeNormalized() const
	{
		return time_ / duration_;
	}

	float Animation::SetTime(float time)
	{
		std::swap(time, time_);
		return time;
	}

	float Animation::GetDuration() const
	{
		return duration_;
	}

	float Animation::SetDuration(float duration)
	{
		std::swap(duration, duration_);
		return duration;
	}

	const AnimationSequence* Animation::GetSequence() const
	{
		return sequence_;
	}

	const AnimationSequence* Animation::SetSequence(const AnimationSequence* sequence, bool updateParams)
	{
		std::swap(sequence, sequence_);
		if (updateParams && sequence_ != nullptr)
		{
			duration_ = sequence_->duration_;
			loopAction_ = sequence_->loop_;
			if (sequence != sequence_)
			{
				time_ = 0.0f;
			}
		}
		return sequence;
	}

	const AnimationSequence* Animation::SetSequence(string_view_arg sequenceName, bool updateParams, bool setIfNull)
	{
		const AnimationSequence* sequence = AnimationSequenceLibrary::Get_S(sequenceName);
		if (sequence != nullptr || setIfNull)
		{
			return SetSequence(sequence, updateParams);
		}
		else
		{
			return nullptr;
		}
	}

	bool Animation::GetMirrored() const
	{
		return isMirrored_;
	}

	bool Animation::SetMirrored(bool mirror)
	{
		std::swap(mirror, isMirrored_);
		return mirror;
	}

	bool Animation::IsDone() const
	{
		return isDone_;
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

	void Animation::StartLinks()
	{
		if (sequence_ != nullptr)
		{
			sequence_->UpdateGroups(time_, curvesEvaluated_);
			Entity* parent = GetParent();
			for (auto animLink : sequence_->links_)
			{
				animLink->Start(parent);
				animLink->Update(parent, curvesEvaluated_);
			}
		}
	}

	void Animation::UpdateLinks()
	{
		if (sequence_ != nullptr)
		{
			sequence_->UpdateGroups(time_, curvesEvaluated_);
			Entity* parent = GetParent();
			for (auto animLink : sequence_->links_)
			{
				animLink->Update(parent, curvesEvaluated_);
			}
		}
	}

#pragma endregion Private Functions

}	// namespace
