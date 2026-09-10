//------------------------------------------------------------------------------
//
// File Name:	CollisionRecord.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Record of data in a collision event.
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
	class Entity;
	class Collider;
	struct CollisionRecord;

	// Typedefs:
	typedef std::function<void(const CollisionRecord&)> CollisionEventHandler;

	// Struct Definition:
	struct CollisionRecord
	{
		friend Collider;
		// Public Constants, Enums, and Structs:
	public:

		// Constructors/Destructors:
	private:
		CollisionRecord();

	public:
		CollisionRecord& operator=(const CollisionRecord& other) = delete;

		// Public Static Functions:
	public:

		// Public Functions:
	public:

		// Public Event Handlers
	public:

		// Public Variables:
	public:
		Entity * entityUs, * entityThem;
		Vec2 contact, oldDestination, oldVelocity;

		// Private Constants, Enums, and Structs:
	private:

		// Private Static Functions:
	private:

		// Private Functions:
	private:

		// Private Static Variables:
	private:

		// Private Variables:
	private:
	};

}	// namespace
