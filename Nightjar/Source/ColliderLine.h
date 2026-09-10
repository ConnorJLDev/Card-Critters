//------------------------------------------------------------------------------
//
// File Name:	ColliderLine.h
// Author(s):	Jonathan Sandquist
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Collider Line component header.
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
// 
//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	// Forward Declarations:
	class ColliderCircle;

	// Typedefs:

	// Class Definition:
	class ColliderLine : public Collider
	{
		// Public Constants, Enums, and Structs:
	public:
		// Common component data.
		COMPONENT_COMMON(ColliderLine);

		// Constructors/Destructors:
	public:
		ColliderLine();
	private:
		ColliderLine(const ColliderLine& other);
	public:
		~ColliderLine();
		ColliderLine* Clone(void) const override;

		// Public Static Functions:
	public:

		// Public Functions:
	public:
		// Read the properties of a ColliderLine component from a file.
		// (NOTE: First read an integer indicating the number of lines to be read.)
		// (NOTE: For each line, read P0 and P1 using StreamReadVector2D.)
		// (HINT: Call ColliderLineAddLineSegment() to add each line.)
		// Params:
		//	 collider = Pointer to the Collider component.
		//	 stream = Pointer to the data stream used for reading.
		//void Read(StreamIn& stream) override;

		// Add a line segment to the line collider's line segment list.
		// Params:
		//	 collider = Pointer to the line collider component.
		//	 p0 = The line segment's starting position.
		//	 p1 = The line segment's ending position.
		void AddLineSegment(const glm::vec2* p0, const glm::vec2* p1);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="obj"></param>
		void Read(StreamIn& obj) override;

		/// <summary>
		/// Writes the ColliderLine data into the stream.
		/// </summary>
		/// <param name="stream">The stream to write into.</param>
		void Write(StreamOut& stream) override;

		// Check for collision between a line collider and a circle collider.
		// Params:
		//	 collider1 = Pointer to the line collider component.
		//	 collider2 = Pointer to the circle collider component.
		// Returns:
		//	 If the pointers are valid,
		//	   then return the results of a line-circle collision check,
		//	   else return false.
		bool IsCollidingPoint(const Collider* other) const;

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
		struct Segment
		{
			// A single line segment (P0 and P1).
			glm::vec2 point[2];
		};

		// Private Static Functions:
	private:

		// Private Functions:
	private:
		bool IsColliding(const ColliderLine* other) const;

		bool IsColliding(const Collider* other) const override;

		// Private Static Variables:
	private:

		// Private Variables:
	private:
		// The number of line segments in the list.
		unsigned int		lineCount_;

		// The individual line segments.
		// (NOTE: Make sure to allocate enough memory for all line segments!)
		vector_t<Segment> lineSegments_;
	};

}	// namespace