//------------------------------------------------------------------------------
//
// File Name:	PhysicsBase.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Base physics class for handling kinematics.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "PhysicsBase.h"
#include "TransformBase.h"

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
	// Protected Constants:
	//--------------------------------------------------------------------------

	constexpr StreamKey
		  PhysicsBase::ReadKeyVelocity = "Velocity"
		, PhysicsBase::ReadKeyAcceleration = "Acceleration"
		, PhysicsBase::ReadKeyRotationalVelocity = "RotationalVelocity"
		, PhysicsBase::ReadKeyDrag = "Drag"
		, PhysicsBase::ReadKeyRotationalDrag = "RotationalDrag"
		;

	//--------------------------------------------------------------------------
	// Protected Static Variables:
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// Protected Variables:
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// Constructors/Destructors:
	//--------------------------------------------------------------------------

#pragma region Constructors

    PhysicsBase::PhysicsBase()
        : velocity3_(0, 0, 0)
        , acceleration3_(0, 0, 0)
        , rotationalVelocity3_(0, 0, 0)
        , drag_(0)
        , rotationalDrag_(0)
    {
    }
    
    PhysicsBase::PhysicsBase(const PhysicsBase& other)
        : velocity3_(other.velocity3_)
        , acceleration3_(other.acceleration3_)
        , rotationalVelocity3_(other.rotationalVelocity3_)
        , drag_(other.drag_)
        , rotationalDrag_(other.rotationalDrag_)
    {
    }
    
    PhysicsBase::PhysicsBase(PhysicsBase&& other) noexcept
        : velocity3_(std::move(other.velocity3_))
        , acceleration3_(std::move(other.acceleration3_))
        , rotationalVelocity3_(std::move(other.rotationalVelocity3_))
        , drag_(std::move(other.drag_))
        , rotationalDrag_(std::move(other.rotationalDrag_))
    {
    }
    
    PhysicsBase& PhysicsBase::operator=(const PhysicsBase& other)
    {
        velocity3_ = other.velocity3_;
        acceleration3_ = other.acceleration3_;
        rotationalVelocity3_ = other.rotationalVelocity3_;
        drag_ = other.drag_;
        rotationalDrag_ = other.rotationalDrag_;
        return *this;
    }

    //--------------------------------------------------------------------------

    PhysicsBase::~PhysicsBase(void)
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

    void PhysicsBase::UpdateInternal(TransformComponents& transform, float dt)
    {
        float rotDrag = glm::exp(-rotationalDrag_ * dt);
        float drag = glm::exp(-drag_ * dt);
        rotationalVelocity3_ *= rotDrag;
        Vec3 rotation = transform.GetRotation3D();
        rotation += rotationalVelocity3_ * dt;
        transform.SetRotation3D(rotation);
        velocity3_ += acceleration3_ * dt;
        velocity3_ *= drag;
        Vec3 translation = transform.GetTranslation3D();
        translation += velocity3_ * dt;
        transform.SetTranslation3D(translation);
    }
    
    const Vec3& PhysicsBase::GetVelocity3D() const
    {
        return velocity3_;
    }
    
    void PhysicsBase::SetVelocity3D(const Vec3& velocity)
    {
        velocity3_ = velocity;
    }
    
    const Vec2& PhysicsBase::GetVelocity2D() const
    {
        return velocity2_;
    }
    
    void PhysicsBase::SetVelocity2D(const Vec2& velocity)
    {
        velocity2_ = velocity;
    }
    
    const Vec3& PhysicsBase::GetAcceleration3D() const
    {
        return acceleration3_;
    }
    
    void PhysicsBase::SetAcceleration3D(const Vec3& acceleration)
    {
        acceleration3_ = acceleration;
    }
    
    const Vec2& PhysicsBase::GetAcceleration2D() const
    {
        return acceleration2_;
    }
    
    void PhysicsBase::SetAcceleration2D(const Vec2& acceleration)
    {
        acceleration2_ = acceleration;
    }
    
    const Vec3& PhysicsBase::GetRotationalVelocity3D() const
    {
        return rotationalVelocity3_;
    }
    
    void PhysicsBase::SetRotationalVelocity3D(const Vec3& rotationalVelocity)
    {
        rotationalVelocity3_ = rotationalVelocity;
    }
    
    const float& PhysicsBase::GetRotationalVelocity2D() const
    {
        return rotationalVelocity2_;
    }
    
    void PhysicsBase::SetRotationalVelocity2D(const float& rotationalVelocity)
    {
        rotationalVelocity2_ = rotationalVelocity;
    }
    
    const float& PhysicsBase::GetDrag() const
    {
        return drag_;
    }

    void PhysicsBase::SetDrag(const float& drag)
    {
        drag_ = drag;
    }
    
    const float& PhysicsBase::GetRotationalDrag() const
    {
        return rotationalDrag_;
    }
    
    void PhysicsBase::SetRotationalDrag(const float& rotationalDrag)
    {
        rotationalDrag_ = rotationalDrag;
    }

#pragma endregion Public Functions

	//--------------------------------------------------------------------------
	// Protected Static Functions:
	//--------------------------------------------------------------------------

#pragma region Protected Static Functions

#pragma endregion Protected Static Functions

	//--------------------------------------------------------------------------
	// Protected Functions:
	//--------------------------------------------------------------------------

#pragma region Protected Functions

#pragma endregion Protected Functions

}	// namespace
