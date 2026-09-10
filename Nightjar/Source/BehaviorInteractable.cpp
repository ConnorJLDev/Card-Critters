//------------------------------------------------------------------------------
//
// File Name:	BehaviorInteractable.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Base behavior for an entity that interacts with the mouse.
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
#include "BehaviorInteractable.h"

#include "Transform.h"
#include "Sprite.h"
#include "EntityContainer.h"

#include "InputSystem.h"
#include "Scene.h"

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

	BehaviorInteractable::BehaviorInteractable(SubTypeEnum type)
		: Behavior(type)
		, hovered_(false)
		, clicked_(false)
		, clickContained_(false)
	{
	}

	BehaviorInteractable::BehaviorInteractable(const BehaviorInteractable& other)
		: Behavior(other)
		, hovered_(other.hovered_)
		, clicked_(other.clicked_)
		, clickContained_(other.clickContained_)
	{
	}

	//--------------------------------------------------------------------------

	BehaviorInteractable::~BehaviorInteractable()
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

	void BehaviorInteractable::Update(float dt)
	{
        // If the scene is paused, don't update any interactions.
        if (dt <= 0.0f)
        {
            Behavior::Update(dt);
            return;
        }

		// We are hovered if the mouse position is within our collider.
		const Collider* collider = GetParent()->Has(Collider);
		assert(collider != nullptr);
		const Vec2& mousePos = GetParent()->GetScene()->GetPointingPosition();
		bool hoveredCurrent = collider->Contains(mousePos);

		// We are clicked if the mouse is down and we are hovered.
		B_Interaction leftMouseInput = InputSystem::GetMouseInput(B_Mouse::Left);
		clicked_ = (bool)(leftMouseInput & B_Interaction::FlagHeld) && hovered_;

		if (hoveredCurrent)
		{
			if (!hovered_)
			{
				// If we are hovering and we weren't before, then we have started hover.
				hovered_ = hoveredCurrent;
				OnHoverStart();
			}
			if (leftMouseInput == B_Interaction::Triggered)
			{
				// If we are hovering and we have just started a click, then we have started a click.
				clickContained_ = true;
				OnClickStart();
			}
			else if (leftMouseInput == B_Interaction::Released)
			{
				// If we are hovering and we have just stopped a click, then we have ended a click.
				OnClickEnd(clickContained_);
			}
		}
		else if (hovered_)
		{
			// If we are not hovering and we were before, then we have ended hover.
			hovered_ = hoveredCurrent;
			OnHoverEnd();
		}
		// Changes have been processed, move values to "last" or "previous" frame variables.
		hovered_ = hoveredCurrent;
		// If a click has begun, check if it has stayed contained in the collider for the whole click.
		// This is after the release is checked, because clicked becomes false at that time.
		clickContained_ &= clicked_;

		Behavior::Update(dt);
	}

	bool BehaviorInteractable::IsHovered() const
	{
		return hovered_;
	}

	bool BehaviorInteractable::IsClicked() const
	{
		return clicked_;
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

	void BehaviorInteractable::OnHoverStart()
	{
	}

	void BehaviorInteractable::OnHoverEnd()
	{
	}

	void BehaviorInteractable::OnClickStart()
	{
	}

	void BehaviorInteractable::OnClickEnd(bool)
	{
	}

#pragma endregion Private Functions

}	// namespace
