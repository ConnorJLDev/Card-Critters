//------------------------------------------------------------------------------
//
// File Name:	ColliderDefault.cpp
// Author(s):	Andrew Lamb
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Collider Default component.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <assert.h>

#include "Collider.h"
#include "ColliderDefault.h"
#include "ColliderCircle.h"
#include "ColliderRect.h"
#include "ColliderLine.h"
#include "Stream.h"
#include "Transform.h"
#include "Physics.h"
#include "Entity.h"

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

namespace Nightjar
{

	ColliderDefault::ColliderDefault()
		: Collider(cColliderDefault)
	{
	}

	ColliderDefault::ColliderDefault(const ColliderDefault& other)
		: Collider(other)
	{
	}

	ColliderDefault::~ColliderDefault()
	{
	}

	ColliderDefault* ColliderDefault::Clone(void) const
	{
		return new ColliderDefault(*this);
	}

	void ColliderDefault::Read(Nightjar::Stream& obj)
	{
		UNREFERENCED_PARAMETER(obj);
	}

	bool ColliderDefault::IsColliding(const ColliderDefault* other) const
	{
		UNREFERENCED_PARAMETER(other);
		return false;
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	bool ColliderDefault::IsColliding(const Collider* other) const
	{
		switch (other->GetSubType())
		{
		case cColliderCircle:
			return false;
		case cColliderLine:
			return false;
		case cColliderRect:
			return false;
		case cColliderDefault:
			return false;
		case cColliderNone:
		default:
			return false;
		}
	}

	//------------------------------------------------------------------------------
}