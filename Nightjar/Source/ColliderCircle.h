//------------------------------------------------------------------------------
//
// File Name:	ColliderCircle.h
// Author(s):	Jonathan Sandquist
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Collider Circle component header.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "Collider.h"

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
	class ColliderCircle : public Collider
	{
		// Public Constants, Enums, and Structs:
	public:
		// Common component data.
		COMPONENT_COMMON(ColliderCircle);

		// Constructors/Destructors:
	public:
		ColliderCircle();
	private:
		ColliderCircle(const ColliderCircle& other);
	public:
		~ColliderCircle();

		// Public Static Functions:
	public:

		// Public Functions:
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name=""></param>
		/// <returns></returns>
		ColliderCircle* Clone(void) const override;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="stream"></param>
		void Read(StreamIn& stream) override;

		/// <summary>
		/// Writes the ColliderCircle data into the stream.
		/// </summary>
		/// <param name="stream">The stream to write into.</param>
		void Write(StreamOut& stream) override;

		//
		void Render() const override;

		// Get the circle collider's radius.
		// Params:
		//	 collider = Pointer to the circle collider component.
		// Returns:
		//	 If the pointer is valid,
		//	   then return the circle collider's radius,
		//	   else return 0.0f.
		float GetRadius() const;

		// Set the circle collider's radius.
		// Params:
		//	 collider = Pointer to the circle collider component.
		//   radius = the circle collider's new radius.
		void SetRadius(float radius);

		/// <summary>
		/// Checks if the collider contains the point specified.
		/// </summary>
		/// <param name="point">The point to check.</param>
		/// <returns>True if the collider contains the point, false otherwise.</returns>
		bool Contains(const Vec2& point) const override;

		// Public Event Handlers
	public:

		// Private Constants, Enums, and Structs:
	private:
		static const StreamKey ReadKeyRadius;

		// Private Static Functions:
	private:

		// Private Functions:
	private:
		// Check for collision between two circle colliders.
		// Params:
		//	 collider1 = Pointer to the first circle collider component.
		//	 collider2 = Pointer to the second circle collider component.
		bool IsColliding(const ColliderCircle* other) const;

		bool IsColliding(const Collider* other) const override;

		// Private Static Variables:
	private:

		// Private Variables:
	private:
		// Radius of the circle collider.
		float radius_;
	};

}	// namespace