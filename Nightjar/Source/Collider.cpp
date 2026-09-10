//------------------------------------------------------------------------------
//
// File Name:	Collider.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Collider component.
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
#include "Entity.h"

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

namespace Nightjar
{
	bool Collider::renderAll_ = false;

	Collider::Collider(SubTypeEnum type)
		: Component(TypeEnum::Collider)
		, type_(type)
		, render_(false)
		, handler_(nullptr)
		, collisionRecord_(new CollisionRecord())
	{
	}

	Collider::Collider(const Collider& other)
		: Component(other)
		, type_(other.type_)
		, render_(false)
		, handler_(other.handler_)
		, collisionRecord_(new CollisionRecord())
	{
	}

	Collider::~Collider()
	{
		if (collisionRecord_ != nullptr)
		{
			delete collisionRecord_;
			collisionRecord_ = NULL;
		}
	}

	Collider::SubTypeEnum Collider::GetSubType() const
	{
		return type_;
	}

	void Collider::Check(const Collider* other) const
	{
		if (!IsColliding(other))
		{
			return;
		}
		CollisionRecordSetEntities(other);
		CollisionEventHandler
			handlerUs = handler_,
			handlerThem = other->handler_;
		if (handlerUs != nullptr)
		{
			handlerUs(*collisionRecord_);
		}
		if (handlerThem != nullptr)
		{
			handlerThem(*other->collisionRecord_);
		}
	}

	void Collider::CollisionRecordSetEntities(const Collider* other) const
	{
		collisionRecord_->entityUs = GetParent();
		collisionRecord_->entityThem = other->GetParent();
		other->collisionRecord_->entityUs = other->GetParent();
		other->collisionRecord_->entityThem = GetParent();
	}

	void Collider::SetCollisionHandler(const CollisionEventHandler& handler)
	{
		handler_ = handler;
	}

	bool Collider::CollisionEnabled() const
	{
		return collision_toggle;
	}

	void Collider::SetEnabled(bool enabled)
	{
		collision_toggle = enabled;
	}

	void Collider::CollisionRecordSetContact(const Collider* other, const Vec2& contact) const
	{
		collisionRecord_->contact = contact;
		other->collisionRecord_->contact = contact;
	}

	void Collider::CollisionRecordSetOldDestination(const Collider* other, const Vec2& oldDestinationUs, const Vec2& oldDestinationThem) const
	{
		collisionRecord_->oldDestination = oldDestinationUs;
		other->collisionRecord_->oldDestination = oldDestinationThem;
	}

	void Collider::CollisionRecordSetOldVelocity(const Collider* other, const Vec2& oldVelocityUs, const Vec2& oldVelocityThem) const
	{
		collisionRecord_->oldVelocity = oldVelocityUs;
		other->collisionRecord_->oldVelocity = oldVelocityThem;
	}

	//------------------------------------------------------------------------------
}