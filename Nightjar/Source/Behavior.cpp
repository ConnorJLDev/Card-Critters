//------------------------------------------------------------------------------
//
// File Name:	Behavior.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Base behavior component.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Behavior.h"
#include "Transform.h"
#include "Entity.h"
#include "EntityContainer.h"

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

	Behavior::Behavior(SubTypeEnum type)
		: Component(TypeEnum::Behavior)
		, stateCurr_(-1)
		, stateNext_(0)
		, type_(type)
	{
	}

	Behavior::Behavior(const Behavior& other)
		: Component(other)
		, stateCurr_(other.stateCurr_)
		, stateNext_(other.stateNext_)
		, type_(other.type_)
	{
	}

	//--------------------------------------------------------------------------

	Behavior::~Behavior(void)
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

	void Behavior::Update(float dt)
	{
		if (stateCurr_ != stateNext_)
		{
			if (stateCurr_ < 0)
			{
				OnStartup();
			}
			OnExit();
			stateCurr_ = stateNext_;
			OnInit();
		}
		OnUpdate(dt);
	}

	Behavior::SubTypeEnum Behavior::GetSubType() const
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
	// Protected Functions:
	//--------------------------------------------------------------------------

#pragma region Protected Functions

	void Behavior::OnStartup()
	{
	}

	void Behavior::SetStateNext(int state)
	{
		stateNext_ = state;
	}

	int Behavior::GetStateCurrent() const {
		return stateCurr_;
	}

	Entity* Behavior::GetOrMakeChildEntity(string_view_arg name)
	{
		Transform* transform = GetParent()->Has(Transform);
		Transform* child = transform->FindChild(name);
		Entity* childEnt;
		if (child == nullptr)
		{
			childEnt = new Entity(name);
			childEnt->Read(name);
			child = childEnt->Has(Transform);
			assert(child != nullptr);
			GetParent()->GetContainer()->Add(childEnt);
			child->SetParentTransform(transform);
		}
		else
		{
			childEnt = child->GetParent();
		}
		assert(childEnt != nullptr);
		return childEnt;
	}

#pragma endregion Protected Functions

}	// namespace
