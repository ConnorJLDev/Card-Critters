//------------------------------------------------------------------------------
//
// File Name:	BehaviorSceneTransitionTimer.cpp
// Author(s):	barend.brink
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Template class for a new behavior.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Entity.h"
#include "Transform.h"
#include "Sprite.h"
#include "Collider.h"
#include "BehaviorSceneTransitionTimer.h"
#include "SceneSystem.h"
#include "TurnSystem.h"
#include "Scene.h"
#include <chrono>

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
        BehaviorSceneTransitionTimer::ReadKeyTime = "Time"
        , BehaviorSceneTransitionTimer::ReadKeyTimerEnd = "TimerEnd"
        , BehaviorSceneTransitionTimer::ReadKeyNextSceneName = "NextSceneName"
        ;

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

	BehaviorSceneTransitionTimer::BehaviorSceneTransitionTimer()
		: Behavior(cBehaviorSceneTransitionTimer),
        Time(0),
        TimerEnd(1),
        NextSceneName()
	{
	}

	BehaviorSceneTransitionTimer::BehaviorSceneTransitionTimer(const BehaviorSceneTransitionTimer& other)
		: Behavior(other),
        Time(other.Time),
        TimerEnd(other.TimerEnd),
        NextSceneName(other.NextSceneName)
	{
	}

	//--------------------------------------------------------------------------

	BehaviorSceneTransitionTimer::~BehaviorSceneTransitionTimer()
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

	BehaviorSceneTransitionTimer* BehaviorSceneTransitionTimer::Clone() const
	{
		return new BehaviorSceneTransitionTimer(*this);
	}

    void BehaviorSceneTransitionTimer::Read(StreamIn& stream)
    {
        stream.TryReadValue(ReadKeyTime, Time);
        stream.TryReadValue(ReadKeyTimerEnd, TimerEnd);
        stream.TryReadValue(ReadKeyNextSceneName, NextSceneName);
    }

    void BehaviorSceneTransitionTimer::Write(StreamOut& stream)
    {
        stream.WriteValue(ReadKeyTime, Time);
        stream.WriteValue(ReadKeyTimerEnd, TimerEnd);
        stream.WriteValue(ReadKeyNextSceneName, NextSceneName);
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

    void BehaviorSceneTransitionTimer::OnStartup()
    {
    }

	void BehaviorSceneTransitionTimer::OnInit()
	{
        Time = 0;

		switch (GetStateCurrent())
		{
		case States::cIdle:
			break;
		}
		Collider* collider = GetParent()->Has(Collider);
		if (collider != nullptr)
		{
			collider->SetCollisionHandler([this](const CollisionRecord& record) { CollisionHandler(record); });
		}
	}

	void BehaviorSceneTransitionTimer::OnUpdate(float dt)
	{
		Time += dt;

        if (Time >= TimerEnd)
        {
            OnTimerEnd();
        }
	}

	void BehaviorSceneTransitionTimer::OnExit()
	{
		switch (GetStateCurrent())
		{
		case States::cInvalid:
			break;
		case States::cIdle:
			break;
		}
	}

	void BehaviorSceneTransitionTimer::CollisionHandler(const CollisionRecord& record)
	{
        UNREFERENCED_PARAMETER(record);
	}

    void BehaviorSceneTransitionTimer::OnTimerEnd()
    {
        if (SceneSystem::GetScene(0)->GetName() != "MainMenu")
        {
            TurnSystem::TransitionScene(NextSceneName, 0);
        }
    }

    void BehaviorSceneTransitionTimer::DisplayIMGUI()
    {
    }

#pragma endregion Private Functions

}	// namespace
