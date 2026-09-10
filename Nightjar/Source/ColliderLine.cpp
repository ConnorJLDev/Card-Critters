//------------------------------------------------------------------------------
//
// File Name:	ColliderLine.cpp
// Author(s):	Jonathan Sandquist
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Collider Line component.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Collider.h"
#include "ColliderLine.h"
#include "ColliderCircle.h"
#include "Transform.h"
#include "Physics.h"
#include "Entity.h"

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

	ColliderLine::ColliderLine()
		: Collider(cColliderLine)
		, lineCount_(0)
		, lineSegments_()
	{
	}

	ColliderLine::ColliderLine(const ColliderLine& other)
		: Collider(other)
		, lineCount_(other.lineCount_)
		, lineSegments_(other.lineSegments_)
	{
	}

	//--------------------------------------------------------------------------

	ColliderLine::~ColliderLine()
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

	ColliderLine* ColliderLine::Clone(void) const
	{
		return new ColliderLine(*this);
	}

	void ColliderLine::Read(StreamIn& stream)
	{
		UNREFERENCED_PARAMETER(stream);
	}

	void ColliderLine::Write(StreamOut& stream)
	{
		UNREFERENCED_PARAMETER(stream);
	}

	void ColliderLine::AddLineSegment(const Vec2* p0, const Vec2* p1)
	{
		Segment lineSegment{ *p0, *p1 };
		lineSegments_.push_back(lineSegment);
		++lineCount_;
	}

	bool ColliderLine::IsCollidingPoint(const Collider* other) const
	{
		/*if (other == NULL)
		{
			return false;
		}*/
		assert(other != nullptr);
		const Entity* entityThem = other->GetParent();
		Physics* physicsThem = entityThem->Has(Physics);
		Transform* transformThem = entityThem->Has(Transform);
		const Vec2
			& translationOld = physicsThem->GetOldTranslation(),
			translationNew = transformThem->GetTranslation();
		Vec2 translationVector = translationNew - translationOld;
		Vec2 normalVector = glm::normalize(translationVector);
		for (unsigned i = 0; i < lineCount_; ++i)
		{
			const Segment* lineSegment = &lineSegments_[i];
			const Vec2
				* p0 = &lineSegment->point[0],
				* p1 = &lineSegment->point[1];
			Vec2 lineSegmentVector = *p1 - *p0;
			Vec2 lineSegmentNormal = Vec2(lineSegmentVector.y, -lineSegmentVector.x);
			lineSegmentNormal = glm::normalize(lineSegmentNormal);
			float NVec = glm::dot(lineSegmentNormal, translationVector);
			// Parallel
			if (NVec == 0)
			{
				continue;
			}
			// Points on same side of line
			float NP0 = glm::dot(lineSegmentNormal, (Vec2)*p0);
			float NOld = glm::dot(lineSegmentNormal, translationOld);
			float NNew = glm::dot(lineSegmentNormal, translationNew);
			if ((NOld <= NP0 && NNew < NP0) || (NOld >= NP0 && NNew > NP0))
			{
				continue;
			}
			// Get incident position
			Vec2 incidentPos;
			float incidentTime = (NP0 - NOld) / NVec;
			incidentPos = (translationVector * incidentTime) + translationOld;
			// Points beyond line segment endpoints
			Vec2 incidentP = incidentPos - *p0;
			if (glm::dot(lineSegmentVector, incidentP) < 0)
			{
				continue;
			}
			incidentP = incidentPos - *p1;
			if (glm::dot(lineSegmentVector, incidentP) > 0)
			{
				continue;
			}
			// Reflection
			Vec2 incidentVec, penetrationVec, reflectVec, reflectPos;
			incidentVec = translationNew - incidentPos;
			float Nincident = glm::dot(lineSegmentNormal, incidentVec);
			penetrationVec = lineSegmentNormal * Nincident;
			reflectVec = (penetrationVec * -2.0f) + incidentVec;
			reflectPos = incidentPos + reflectVec;
			transformThem->SetTranslation(reflectPos);
			float angleNew = atan2f(reflectVec.y, reflectVec.x);
			transformThem->SetRotation(angleNew);
			const Vec2 velocityOld = physicsThem->GetVelocity();
			float velocitySpeed = glm::length(velocityOld);
			Vec2 velocityNew = glm::normalize(reflectVec);
			velocityNew *= velocitySpeed;
			physicsThem->SetVelocity(velocityNew);
			CollisionRecordSetContact(other, incidentPos);
			const Transform* transformUs = GetParent()->Has(Transform);
			CollisionRecordSetOldDestination(other, transformUs->GetTranslation(), translationNew);
			const Physics* physicsUs = GetParent()->Has(Physics);
			Vec2 velocityUs(physicsUs == nullptr ? Vec2(0, 0) : physicsUs->GetVelocity());
			CollisionRecordSetOldVelocity(other, velocityUs, velocityOld);
			return true;
		}
		return false;
	}

	bool ColliderLine::Contains(const Vec2& point) const
	{
		UNREFERENCED_PARAMETER(point);
		return false;
	}

	bool ColliderLine::IsColliding(const ColliderLine* other) const
	{
		UNREFERENCED_PARAMETER(other);
		return false;
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

	bool ColliderLine::IsColliding(const Collider* other) const
	{
		switch (other->GetSubType())
		{
		case cColliderCircle:
			return IsCollidingPoint(static_cast<const ColliderCircle*>(other));
		case cColliderLine:
			return IsColliding(static_cast<const ColliderLine*>(other));
		case cColliderRect:
		default:
			return false;
		}
	}

#pragma endregion Private Functions

}	// namespace
