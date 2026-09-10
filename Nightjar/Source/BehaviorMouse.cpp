//------------------------------------------------------------------------------
//
// File Name:	BehaviorMouse.cpp
// Author(s):	Barend Brink (barend.brink)
// Project:		Card Critters
// Course:		GAM200
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "BehaviorMouse.h"
#include "Entity.h"
#include "Transform.h"
#include "Sprite.h"
#include "InputSystem.h"
#include "Scene.h"

namespace Nightjar
{
	//------------------------------------------------------------------------------
	// Private Constants:
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// Private Structures:
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// Public Variables:
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// Private Function Declarations:
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	BehaviorMouse::BehaviorMouse() : Behavior(cBehaviorMouse)
	{

	}

	BehaviorMouse::BehaviorMouse(const BehaviorMouse& other) : Behavior(other)
	{

	}

	BehaviorMouse::~BehaviorMouse()
	{

	}

	BehaviorMouse* BehaviorMouse::Clone() const
	{
		return new BehaviorMouse(*this);
	}

	void BehaviorMouse::Read(StreamIn&)
	{
	}

	void BehaviorMouse::Write(StreamOut&)
	{
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	void BehaviorMouse::OnInit()
	{
		switch (GetStateCurrent())
		{
		case cMouseIdle:
			break;
		}
	}

	void BehaviorMouse::OnUpdate(float)
	{
		const Vec2& mousePos = GetParent()->GetScene()->GetPointingPosition();

		Transform* transform = EntityHas(Transform);
		if (transform != nullptr)
		{
			transform->SetTranslation(mousePos);
		}

		Sprite* sprite = EntityHas(Sprite);

#ifdef _DEBUG
		B_Interaction lmouse = InputSystem::GetMouseInput(B_Mouse::Left);
		if (lmouse == B_Interaction::Idle)
		{
			sprite->SetTint(Vec3(1, 1, 1));
		}
		else if (lmouse == B_Interaction::Triggered)
		{
			sprite->SetTint(Vec3(0, 0, 0));
		}
		else if (lmouse == B_Interaction::Held)
		{
			sprite->SetTint(Vec3(0, 1, 0));
		}
		else if (lmouse == B_Interaction::Released)
		{
			sprite->SetTint(Vec3(0, 0, 0));
		}
#else
        if (sprite != nullptr)
        {
            sprite->SetZLayer(-3621);
        }
#endif
		/*
		switch (GetStateCurrent())
		{
		case cMouseIdle:
			break;
		}
		*/
	}

	void BehaviorMouse::OnExit()
	{
		switch (GetStateCurrent())
		{
		case cMouseIdle:
			break;
		}
	}

}
