//------------------------------------------------------------------------------
//
// File Name:	BehaviorAudio.cpp
// Author(s):	$username$
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Behavior for entities with that only play audio
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Entity.h"
#include "Collider.h"
#include "BehaviorAudio.h"
#include "AudioEmitter.h"

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

	BehaviorAudio::BehaviorAudio()
		: Behavior(cBehaviorAudio)
	{
	}

	BehaviorAudio::BehaviorAudio(const BehaviorAudio& other)
		: Behavior(other)
	{
	}

	//--------------------------------------------------------------------------

	BehaviorAudio::~BehaviorAudio()
	{
		AudioEmitter* audioEmitter = GetParent()->Has(AudioEmitter);
		if (audioEmitter)
		{
			audioEmitter->Pause();
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
	void BehaviorAudio::DisplayIMGUI(void) { return; };

	BehaviorAudio* BehaviorAudio::Clone() const
	{
		return new BehaviorAudio(*this);
	}

	void BehaviorAudio::Read(StreamIn& stream)
	{
		UNREFERENCED_PARAMETER(stream);
	}

	void BehaviorAudio::Write(StreamOut& stream)
	{
		UNREFERENCED_PARAMETER(stream);
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

	void BehaviorAudio::OnInit()
	{
		switch (GetStateCurrent())
		{
		case cBehaviorAudioIdle:
			AudioEmitter* audioEmitter = GetParent()->Has(AudioEmitter);
			if (audioEmitter)
			{
				audioEmitter->Play();
			}
			break;
		}
		Collider* collider = GetParent()->Has(Collider);
		if (collider != nullptr)
		{
			collider->SetCollisionHandler([this](const CollisionRecord& record) { CollisionHandler(record); });
		}
	}

	void BehaviorAudio::OnUpdate(float dt)
	{
		UNREFERENCED_PARAMETER(dt);
		switch (GetStateCurrent())
		{
		case cBehaviorAudioIdle:
			break;
		}
	}

	void BehaviorAudio::OnExit()
	{
		switch (GetStateCurrent())
		{
		case cBehaviorAudioIdle:
			break;
		}
	}

	void BehaviorAudio::CollisionHandler(const CollisionRecord& record)
	{
		UNREFERENCED_PARAMETER(record);
	}

#pragma endregion Private Functions

}	// namespace
