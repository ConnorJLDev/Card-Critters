//------------------------------------------------------------------------------
//
// File Name:	Collider.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Collider component.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Component.h"
#include "Entity.h"
#include "CollisionRecord.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	// Forward Declarations:

	// Typedefs:

	// Class Definition:
	class Collider : public Component
	{
		friend class ImGuiSystem;
		friend class StreamIn;

		// Public Constants, Enums, and Structs:
	public:
		// Common component data.
		COMPONENT_COMMON(Collider);

		enum SubTypeEnum : char
		{
			cColliderCircle,
			cColliderLine,
			cColliderRect,
		};

		// Constructors/Destructors:
	public:
		Collider(SubTypeEnum type);
	protected:
		Collider(const Collider& other);
	public:
		virtual ~Collider();

		// Public Static Functions:
	public:

		// Public Functions:
	public:
		/// <summary>
		/// Gets the subtype of the Collider.
		/// </summary>
		/// <returns>The subtype of the Collider.</returns>
		SubTypeEnum GetSubType() const;

		// Check if two Entities are colliding.
		// (Hint: Refer to the project instructions for implementation suggestions.)
		// (Hint: Make sure to call the handler for both colliders, passing the 
		//	  parent Entity pointers in the correct order!)
		// Params:
		//	 collider = Pointer to the first Collider component.
		//	 other = Pointer to the second Collider component.
		void Check(const Collider* other) const;

		/// <summary>
		/// Checks if the collider contains the point specified.
		/// </summary>
		/// <param name="point">The point to check.</param>
		/// <returns>True if the collider contains the point, false otherwise.</returns>
		virtual bool Contains(const Vec2& point) const = 0;

		// Set the collision event handler for when two entities collide.
		// (Hint: This allows other components, such as Behaviors, to respond to collision events.)
		// (Note: It is acceptable for the handler to be NULL.  This allows an existing handler to be removed.)
		// Params:
		//	 collider = Pointer to the Collider component.
		//	 handler = Pointer to the collision event handler (may be NULL).
		void SetCollisionHandler(const CollisionEventHandler& handler);

		// Public Event Handlers
	public:


		bool CollisionEnabled() const;
		void SetEnabled(bool enabled);
		// Protected Functions
	protected:
		void CollisionRecordSetContact(const Collider* other, const Vec2& contact) const;
		void CollisionRecordSetOldDestination(const Collider* other, const Vec2& oldDestinationUs, const Vec2& oldDestinationThem) const;
		void CollisionRecordSetOldVelocity(const Collider* other, const Vec2& oldVelocityUs, const Vec2& oldVelocityThem) const;

		// Private Constants, Enums, and Structs:
	private:

		// Private Static Functions:
	private:

		// Private Functions:
	private:
		virtual bool IsColliding(const Collider* other) const = 0;

		void CollisionRecordSetEntities(const Collider* other) const;

		// Private Static Variables:
	protected:
		static bool renderAll_;

		// Private Variables:
	private:
		// The type of collider used by this component.
		SubTypeEnum type_;
	protected:
		bool render_;
	private:
		// Pointer to a function that handles collisions between entities.
		CollisionEventHandler handler_;
		bool collision_toggle = true;
		CollisionRecord* collisionRecord_;
	};

}	// namespace