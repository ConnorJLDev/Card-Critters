//------------------------------------------------------------------------------
//
// File Name:	Physics.cpp
// Author(s):	Andrew Lamb (andrew.lamb), Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Physics component.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Entity.h"
#include "Physics.h"
#include "Transform.h"

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

	Physics::Physics()
		: Component(TypeEnum::Physics)
        , PhysicsBase()
		, oldTranslation_()
	{
	}

	Physics::Physics(const Physics& other)
        : Component(other)
        , PhysicsBase(other)
		, oldTranslation_(other.oldTranslation_)
	{
	}

	//--------------------------------------------------------------------------

	Physics::~Physics() {
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

	Physics* Physics::Clone() const
	{
		return new Physics(*this);
	}

	void Physics::Read(StreamIn& stream)
	{
		stream.TryReadValue(ReadKeyVelocity, velocity2_);
		stream.TryReadValue(ReadKeyAcceleration, acceleration2_);
		stream.TryReadValue(ReadKeyRotationalVelocity, rotationalVelocity2_);
		stream.TryReadValue(ReadKeyDrag, drag_);
	}

	void Physics::Write(StreamOut& stream)
	{
		stream.WriteValue(ReadKeyVelocity, velocity2_);
		stream.WriteValue(ReadKeyAcceleration, acceleration2_);
		stream.WriteValue(ReadKeyRotationalVelocity, rotationalVelocity2_);
		stream.WriteValue(ReadKeyDrag, drag_);
	}

	void Physics::Update(float dt)
	{
		Transform* transform = EntityHas(Transform);
		assert(transform != nullptr);
		oldTranslation_ = transform->GetTranslation();
		UpdateInternal(*transform, dt);
	}

	const Vec2& Physics::GetAcceleration() const
	{
		return GetAcceleration2D();
	}

	const Vec2& Physics::GetVelocity() const
	{
		return GetVelocity2D();
	}

	float Physics::GetRotationalVelocity() const
	{
		return GetRotationalVelocity2D();
	}

	const Vec2& Physics::GetOldTranslation() const
	{
		return oldTranslation_;
	}

	void Physics::SetAcceleration(const Vec2& acceleration)
	{
		SetAcceleration2D(acceleration);
	}

	void Physics::SetVelocity(const Vec2& velocity)
	{
		SetVelocity2D(velocity);
	}

	void Physics::SetRotationalVelocity(float rotationalVelocity)
	{
		SetRotationalVelocity2D(rotationalVelocity);
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

#pragma endregion Private Functions

}	// namespace
