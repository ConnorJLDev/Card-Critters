//------------------------------------------------------------------------------
//
// File Name:	BehaviorTransition.cpp
// Author(s):	Andrew Lamb (andrew.lamb)
// Course:		GAM250S26
// Project:		Card Critters
// Purpose:		Class for transition behavior.
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
#include "Animation.h"
#include "BehaviorTransition.h"
#include "SceneSystem.h"
#include "Scene.h"
#include "TurnSystem.h"

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

	BehaviorTransition::BehaviorTransition() : Behavior(cBehaviorTransition), _nextScene("NULL")
	{
	}

	BehaviorTransition::BehaviorTransition(const BehaviorTransition& other)
		: Behavior(other), _nextScene(other._nextScene)
	{
	}

	//--------------------------------------------------------------------------

	BehaviorTransition::~BehaviorTransition()
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

	BehaviorTransition* BehaviorTransition::Clone() const
	{
		return new BehaviorTransition(*this);
	}

	void BehaviorTransition::Read(StreamIn& stream)
	{
		UNREFERENCED_PARAMETER(stream);
	}

	void BehaviorTransition::Write(StreamOut& stream)
	{
		UNREFERENCED_PARAMETER(stream);
	}

	void BehaviorTransition::SetNextScene(string_t scene, unsigned int index)
	{
		_nextScene = scene;
        _nextSceneIndex = index;
	}

	void BehaviorTransition::DisplayIMGUI()
	{

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

	void BehaviorTransition::OnInit()
	{
		Animation* animation = GetParent()->Has(Animation);
		switch (GetStateCurrent())
		{
		case cTransitionIdle:
			SetStateNext(cTransitionFadeIn);
			break;
		case cTransitionFadeIn:
			if (animation != nullptr)
			{
				animation->SetSpeed(1);
				animation->SetPlaying(true);
			}
			break;
		case cTransitionFadeOut:
			if (animation != nullptr)
			{
				animation->SetSpeed(-1);
				animation->SetPlaying(true);
			}
			break;
		}
	}

	void BehaviorTransition::OnUpdate(float dt)
	{
		UNREFERENCED_PARAMETER(dt);
		Animation* animation = GetParent()->Has(Animation);
		switch (GetStateCurrent())
		{
		case cTransitionIdle:
			break;
		case cTransitionFadeIn:
			if (animation->IsDone())
			{
                if (_nextScene != "" && _nextScene != "NULL")
                {
                    TurnSystem::ClearParties();
                    SceneSystem::SetNext(_nextScene, _nextSceneIndex);
                    SetStateNext(cTransitionWaiting);
                 
                }
                else if (_nextScene == "")
                {
                    for (unsigned int i = 0; i < 5; i++)
                        SceneSystem::SetNext("", i);
                }

			}
			break;
		case cTransitionFadeOut:
			if (animation->IsDone())
				SceneSystem::SetNext("", 4);
			break;
		case cTransitionWaiting:
			if (SceneSystem::GetScene(0) != nullptr)
			{
				if (SceneSystem::GetScene(0)->GetName() == _nextScene)
				{
					SetStateNext(cTransitionFadeOut);
				}
			}
			break;
		}
	}

	void BehaviorTransition::OnExit()
	{
		switch (GetStateCurrent())
		{
		case cTransitionInvalid:
			break;
		case cTransitionIdle:
			break;
		}
	}

#pragma endregion Private Functions

}	// namespace
