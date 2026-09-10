//------------------------------------------------------------------------------
//
// File Name:	ColliderRect.cpp
// Author(s):	Andrew Lamb
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Collider Rectangle component.
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
#include "ColliderLine.h"
#include "Transform.h"
#include "Physics.h"
#include "Entity.h"
#include "ColliderRect.h"
#include "GraphicsSystem.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{

#define width sizeWorld_.x
#define height sizeWorld_.y

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
		ColliderRect::ReadKeySize = "Size";

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

	ColliderRect::ColliderRect()
		: Collider(cColliderRect)
		, size_(1, 1)
		, sizeWorld_()
		, points()
	{
	}

	ColliderRect::ColliderRect(const ColliderRect& other)
		: Collider(other)
		, size_(other.size_)
		, sizeWorld_()
		, points()
	{
	}

	//--------------------------------------------------------------------------

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

	void ColliderRect::Update(float dt)
	{
		if (!CollisionEnabled())
			return;

		UNREFERENCED_PARAMETER(dt);
		const Transform* transformUs = GetParent()->Has(Transform);
		Vec2 translationUs = transformUs->GetTranslationWorld();
		sizeWorld_ = transformUs->GetScaleWorld() * size_;
		points[0] = { translationUs.x - (width / 2), translationUs.y + (height / 2) };
		points[1] = { translationUs.x + (width / 2), translationUs.y + (height / 2) };
		points[2] = { translationUs.x + (width / 2), translationUs.y - (height / 2) };
		points[3] = { translationUs.x - (width / 2), translationUs.y - (height / 2) };
	}

	void ColliderRect::Render() const
	{
		if (!CollisionEnabled())
			return;

		if (render_ || renderAll_)
		{
			Transform* transform = GetParent()->Has(Transform);
			if (transform != nullptr)
			{
				//Vec2 size = Vec2(width, height) / transform->GetScale();
				GraphicsSystem::DrawRect(transform->GetMatrix(), size_);
			}
		}
	}

	ColliderRect::~ColliderRect()
	{
	}

	Collider* ColliderRect::Clone(void) const
	{
		return new ColliderRect(*this);
	}

	void ColliderRect::Read(StreamIn& stream)
	{
		stream.TryReadValue(ReadKeySize, size_);
	}

	void ColliderRect::Write(StreamOut& stream)
	{
		stream.WriteValue(ReadKeySize, size_);
	}

	float ColliderRect::GetWidth() const
	{
		return size_.x;
	}

	void ColliderRect::SetWidth(float widthInp)
	{
		size_.x = widthInp;
	}

	float ColliderRect::GetHeight() const
	{
		return size_.y;
	}

	void ColliderRect::SetHeight(float lengthInp)
	{
		size_.y = lengthInp;
	}

	bool ColliderRect::Contains(const Vec2& point) const
	{
		if (!CollisionEnabled())
			return false;

		const Transform* transform = GetParent()->Has(Transform);
		const Vec2& translation = transform->GetTranslationWorld();
		// Take twice the absolute value of the difference to directly compare it to sizeWorld_
		const Vec2 diff2{ glm::abs(translation - point) * 2.0f };
		return (diff2.x < sizeWorld_.x) && (diff2.y < sizeWorld_.y);
	}

	bool ColliderRect::IsColliding(const ColliderRect* other) const
	{
		if (!CollisionEnabled())
			return false;

		bool result = false;
		assert(other != nullptr);
		const Transform* transformUs = GetParent()->Has(Transform);
		//const Transform* transformThem = (Transform*)other->GetParent()->ComponentGet(cTransform);
		Vec2 translationUs = transformUs->GetTranslationWorld();
		Vec2 translationThem = transformUs->GetTranslationWorld();
		//Potential TODO: contact and old velocity
		//const float collisionDistanceX = width + other->width;
		//const float collisionDistanceY = length + other->length;
		//float distanceSquared = (translationThem - translationUs) * (translationThem - translationUs);
		if ((translationUs.x + (width / 2) > translationThem.x - (other->width / 2) && translationUs.x - (width / 2) < translationThem.x + (other->width / 2)) &&
			((translationUs.y + (height / 2) > translationThem.y - (other->height / 2) && translationUs.y - (height / 2) < translationThem.y + (other->height / 2))))
			result = true;
		return result;
	}

	bool ColliderRect::IsColliding(const ColliderCircle* other) const
	{
		if (!CollisionEnabled())
			return false;

		assert(other != nullptr);
		const Transform* transformUs = GetParent()->Has(Transform);
		const Transform* transformThem = other->GetParent()->Has(Transform);
		Vec2 translationUs = transformUs->GetTranslationWorld();
		Vec2 translationThem = transformThem->GetTranslationWorld();

		//Early return if center of the circle collider is within the rectangle
		if ((translationUs.x + (width / 2) > translationThem.x - (other->GetRadius() / 2) && translationUs.x - (width / 2) < translationThem.x + (other->GetRadius() / 2)) &&
			((translationUs.y + (height / 2) > translationThem.y - (other->GetRadius() / 2) && translationUs.y - (height / 2) < translationThem.y + (other->GetRadius() / 2))))
		{
			TraceDebug("Colliding");
			return true;
		}
		
		Vec2 point1;
		Vec2 point2;
		Vec2 lineSegmentVector;
		Vec2 lineSegmentNormal;
		for (int i = 0; i < 4; i++) {
			point1 = points[i];
			if (i < 3)
				point2 = points[i + 1];
			else
				point2 = points[0];
			lineSegmentVector = point2 - point1;
			lineSegmentNormal = Vec2(lineSegmentVector.y, -lineSegmentVector.x);
			lineSegmentNormal = glm::normalize(lineSegmentNormal);

			if (glm::length(translationThem - point1) < other->GetRadius() || glm::length(translationThem - point2) < other->GetRadius())
				return true;


			Vec2 translationVector = lineSegmentNormal * other->GetRadius();

			float NVec = glm::dot(lineSegmentNormal, translationVector);
			
			float NP0 = glm::dot(lineSegmentNormal, point1);
			float NOld = glm::dot(lineSegmentNormal, translationThem);
			
			Vec2 translationNew = translationVector + translationThem;
			float NNew = glm::dot(lineSegmentNormal, translationNew);
			if ((NOld <= NP0 && NNew < NP0) || (NOld >= NP0 && NNew > NP0))
			{
				continue;
			}
			
			Vec2 incidentPos;
			float incidentTime = (NP0 - NOld) / NVec;
			incidentPos = (translationVector * incidentTime) + translationThem;
			Vec2 incidentP = incidentPos - point1;
			if (glm::dot(lineSegmentVector, incidentP) < 0)
			{
				continue;
			}

			incidentP = incidentPos - point2;
			if (glm::dot(lineSegmentVector, incidentP) > 0)
			{
				continue;
			}

			TraceDebug("Colliding");
			return true;
		}
		
		TraceDebug("Not colliding");
		return false;
	}

	bool ColliderRect::IsColliding(const Collider* other) const
	{
		switch (other->GetSubType())
		{
		case cColliderRect:
			return IsColliding((const ColliderRect*)other);
		case cColliderCircle:
			return IsColliding((const ColliderCircle*)other);
		case cColliderLine:
		default:
			return false;
		}
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
