//------------------------------------------------------------------------------
//
// File Name:	PhysicsBase.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Base physics class for handling kinematics.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	// Forward Declarations:
    struct TransformComponents;

	// Typedefs:

	// Class Definition:
	class PhysicsBase
	{
		// Public Constants, Enums, and Structs:
	public:

		// Constructors/Destructors:
	public:
		PhysicsBase();

		PhysicsBase(const PhysicsBase& other);

		PhysicsBase(PhysicsBase&& other) noexcept;

		PhysicsBase& operator=(const PhysicsBase& other);

		~PhysicsBase(void);

		// Public Static Functions:
	public:

		// Public Functions:
	public:
		//
		void UpdateInternal(TransformComponents& transform, float dt);

		//
		const Vec3& GetVelocity3D() const;

		//
		void SetVelocity3D(const Vec3& velocity);

		//
		const Vec2& GetVelocity2D() const;

		//
		void SetVelocity2D(const Vec2& velocity);

		//
		const Vec3& GetAcceleration3D() const;

		//
		void SetAcceleration3D(const Vec3& acceleration);

		//
		const Vec2& GetAcceleration2D() const;

		//
		void SetAcceleration2D(const Vec2& acceleration);

		//
		const Vec3& GetRotationalVelocity3D() const;

		//
		void SetRotationalVelocity3D(const Vec3& rotationalVelocity);

		//
		const float& GetRotationalVelocity2D() const;

		//
		void SetRotationalVelocity2D(const float& rotationalVelocity);

		//
		const float& GetDrag() const;

		//
		void SetDrag(const float& drag);

		//
		const float& GetRotationalDrag() const;

		//
		void SetRotationalDrag(const float& rotationalDrag);

		//
		void DisplayIMGUI2D();

		//
		void DisplayIMGUI3D();

		// Public Event Handlers
	public:

		// Protected Constants, Enums, and Structs:
	protected:
        //
        static const StreamKey ReadKeyVelocity, ReadKeyAcceleration, ReadKeyRotationalVelocity, ReadKeyDrag, ReadKeyRotationalDrag;

		// Protected Static Functions:
	protected:

		// Protected Functions:
	protected:

		// Protected Static Variables:
	protected:

		// Protected Variables:
	protected:
		union
		{
			// The physics's 2D velocity.
			Vec2 velocity2_;
			// The physics's 3D velocity.
			Vec3 velocity3_;
		};
		union
		{
			// The physics's 2D acceleration.
			Vec2 acceleration2_;
			// The physics's 3D acceleration.
			Vec3 acceleration3_;
		};
		union
		{
			struct
			{
				Vec2 _;
				// The physics's 2D rotational velocity.
				float rotationalVelocity2_;
			};
			// The physics's 3D rotational velocity as XYZ Euler angles.
			Vec3 rotationalVelocity3_;
		};
		// The physics's drag applied to velocity.
		float drag_;
		// The physics's drag applied to rotational velocity.
		float rotationalDrag_;
	};

}	// namespace
