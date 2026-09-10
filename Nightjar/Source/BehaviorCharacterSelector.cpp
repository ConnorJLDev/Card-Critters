//------------------------------------------------------------------------------
//
// File Name:	BehaviorCharacterSelector.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Behavior for registering clicks on characters.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Entity.h"
#include "EntityContainer.h"
#include "Collider.h"
#include "Transform.h"
#include "BehaviorCharacterSelector.h"
#include "BehaviorCharacter.h"
#include "BehaviorParty.h"
#include "BehaviorHand.h"
#include "BehaviorCard.h"
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

	BehaviorCharacterSelector::BehaviorCharacterSelector()
		: BehaviorInteractable(cBehaviorCharacterSelector)
	{
	}

	BehaviorCharacterSelector::BehaviorCharacterSelector(const BehaviorCharacterSelector& other)
		: BehaviorInteractable(other)
	{
	}

	//--------------------------------------------------------------------------

	BehaviorCharacterSelector::~BehaviorCharacterSelector()
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

	BehaviorCharacterSelector* BehaviorCharacterSelector::Clone() const
	{
		return new BehaviorCharacterSelector(*this);
	}

	void BehaviorCharacterSelector::Read(StreamIn& stream)
	{
		UNREFERENCED_PARAMETER(stream);
	}

	void BehaviorCharacterSelector::Write(StreamOut& stream)
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

	void BehaviorCharacterSelector::OnInit()
	{
		switch (GetStateCurrent())
		{
		case cBehaviorCharacterSelectorIdle:
			break;
		}
	}

	void BehaviorCharacterSelector::OnUpdate(float dt)
	{
		UNREFERENCED_PARAMETER(dt);
		switch (GetStateCurrent())
		{
		case cBehaviorCharacterSelectorIdle:
			break;
		}
	}

	void BehaviorCharacterSelector::OnExit()
	{
		switch (GetStateCurrent())
		{
		case cBehaviorCharacterSelectorIdle:
			break;
		}
	}

	void BehaviorCharacterSelector::OnHoverStart()
	{
		// If character is active, hover arrow over it.
		BehaviorParty* party = TurnSystem::ActiveParty();
		if (party == nullptr)
		{
			TraceWarning("CharacterSelector failed to get active party for hovering.");
			return;
		}
		BehaviorCharacter* parent = GetCharacter();
		assert(parent != nullptr);
		party->hand_->SetPartyTarget(parent);
	}

	void BehaviorCharacterSelector::OnHoverEnd()
	{
	}

	void BehaviorCharacterSelector::OnClickStart()
	{
	}

	void BehaviorCharacterSelector::OnClickEnd(bool contained)
	{
		if (contained)
		{
			// If character is active, set it as the target.
			BehaviorParty* party = TurnSystem::ActiveParty();
			if (party == nullptr)
			{
				TraceWarning("CharacterSelector failed to get active party for clicking.");
				return;
			}
			BehaviorHand* hand = party->GetHand();
			BehaviorCard* activeCard = hand->GetActiveCard();
			if (activeCard != nullptr)
			{
				activeCard->Use();
			}
		}
	}

	BehaviorCharacter* BehaviorCharacterSelector::GetCharacter() const
	{
		return GetParent()->Has(Transform)->GetParentTransform()->GetParent()->HasSub(Behavior, BehaviorCharacter);
	}

#pragma endregion Private Functions

}	// namespace
