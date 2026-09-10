//------------------------------------------------------------------------------
//
// File Name:	ColliderCircle.cpp
// Author(s):	Jonathan Sandquist
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Collider Circle component.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Collider.h"
#include "ColliderCircle.h"
#include "ColliderRect.h"
#include "ColliderLine.h"
#include "Transform.h"
#include "Physics.h"
#include "Entity.h"
#include "GraphicsSystem.h"

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
		ColliderCircle::ReadKeyRadius = "Radius";

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

	ColliderCircle::ColliderCircle()
		: Collider(cColliderCircle)
		, radius_(1.0f)
	{
	}

	ColliderCircle::ColliderCircle(const ColliderCircle& other)
		: Collider(other)
		, radius_(other.radius_)
	{
	}

	//--------------------------------------------------------------------------

	ColliderCircle::~ColliderCircle()
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

	ColliderCircle* ColliderCircle::Clone(void) const
	{
		return new ColliderCircle(*this);
	}

	void ColliderCircle::Render() const
	{
		if (render_ || renderAll_)
		{
			Transform* transform = GetParent()->Has(Transform);
			if (transform != nullptr)
			{
				Vec2 radii = radius_ / transform->GetScale();
				GraphicsSystem::DrawOval(transform->GetMatrix(), radii);
			}
		}
	}

	void ColliderCircle::Read(StreamIn& stream)
	{
		stream.TryReadValue(ReadKeyRadius, radius_);
	}

	void ColliderCircle::Write(StreamOut& stream)
	{
		stream.WriteValue(ReadKeyRadius, radius_);
	}

	float ColliderCircle::GetRadius() const
	{
		return radius_;
	}

	void ColliderCircle::SetRadius(float radius)
	{
		radius_ = radius;
	}

	bool ColliderCircle::Contains(const Vec2& point) const
	{
		const Transform* transform = GetParent()->Has(Transform);
		const Vec2& translation = transform->GetTranslationWorld();
		const Vec2 diff{ translation - point };
		float distanceSquared = glm::dot(diff, diff);
		return radius_ * radius_ < distanceSquared;
	}

	bool ColliderCircle::IsColliding(const ColliderCircle* other) const
	{
		assert(other != nullptr);
		const Transform
			* transformUs = GetParent()->Has(Transform),
			* transformThem = other->GetParent()->Has(Transform);
		const Vec2
			translationUs = transformUs->GetTranslationWorld(),
			translationThem = transformThem->GetTranslationWorld();
		const float
			collisionDistance = radius_ + other->radius_,
			collisionDistanceSquared = collisionDistance * collisionDistance;
		float distanceSquared = glm::dot((translationThem - translationUs), (translationThem - translationUs));
		if (distanceSquared >= collisionDistanceSquared)
		{
			return false;
		}
		float ratio = radius_ / collisionDistance;
		Vec2 contact = translationThem - translationUs;
		contact = (contact * ratio) + translationUs;
		CollisionRecordSetContact(other, contact);
		CollisionRecordSetOldDestination(other, translationUs, translationThem);
		const Physics
			* physicsUs = GetParent()->Has(Physics),
			* physicsThem = other->GetParent()->Has(Physics);
		Vec2 velocityUs(physicsUs == nullptr ? Vec2(0, 0) : physicsUs->GetVelocity()),
			velocityThem(physicsThem == nullptr ? Vec2(0, 0) : physicsThem->GetVelocity());
		CollisionRecordSetOldVelocity(other, velocityUs, velocityThem);
		return true;
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

	bool ColliderCircle::IsColliding(const Collider* other) const
	{
		switch (other->GetSubType())
		{
		case cColliderCircle:
			return IsColliding(static_cast<const ColliderCircle*>(other));
		case cColliderLine:
			return static_cast<const ColliderLine*>(other)->IsCollidingPoint(this);
		case cColliderRect:
			return static_cast<const ColliderRect*>(other)->IsColliding(this);
		default:
			return false;
		}
	}

#pragma endregion Private Functions

}	// namespace
