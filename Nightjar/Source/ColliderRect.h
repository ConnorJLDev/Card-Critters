//------------------------------------------------------------------------------
//
// File Name:	ColliderRect.h
// Author(s):	Andrew Lamb
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Collider Rectangle component header.
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
	class ColliderCircle;

	// Typedefs:

	// Class Definition:
	class ColliderRect : public Collider
	{
		friend class ColliderCircle;

		// Public Constants, Enums, and Structs:
	public:
		// Common component data.
		COMPONENT_COMMON(ColliderRect);

		// Constructors/Destructors:
	public:
		ColliderRect();
	private:
		ColliderRect(const ColliderRect& other);
	public:
		~ColliderRect();

		// Public Static Functions:
	public:

		// Public Functions:
	public:
		//
		Collider* Clone(void) const override;

		//
		void Read(StreamIn& stream) override;

		/// <summary>
		/// Writes the ColliderRect data into the stream.
		/// </summary>
		/// <param name="stream">The stream to write into.</param>
		void Write(StreamOut& stream) override;

		//
		void Update(float dt) override;

		//
		void Render() const override;

		// Returns rectangle collider's width
		float GetWidth() const;

		// Sets rectangle collider's width
		void SetWidth(float widthInp);

		// Returns rectangle collider's length
		float GetHeight() const;

		// Sets rectangle collider's length
		void SetHeight(float lengthInp);

		/// <summary>
		/// Checks if the collider contains the point specified.
		/// </summary>
		/// <param name="point">The point to check.</param>
		/// <returns>True if the collider contains the point, false otherwise.</returns>
		bool Contains(const Vec2& point) const override;

		// Public Event Handlers
	public:

		// Protected functions
	protected:
		bool IsColliding(const ColliderCircle* other) const;

		// Private Constants, Enums, and Structs:
	private:
		static const StreamKey ReadKeySize;

		// Private Static Functions:
	private:

		// Private Functions:
	private:
		//Checks for collisions between rectangles and between the rectangle and another collider
		bool IsColliding(const ColliderRect* other) const;

		bool IsColliding(const Collider* other) const override;

		// Private Static Variables:
	private:

		// Private Variables:
	private:
		//Size of rectangle collider
		Vec2 size_;
		//Size of rectangle collider in world space
		Vec2 sizeWorld_;
		//Array of points
		Vec2 points[4];


	};

}	// namespace