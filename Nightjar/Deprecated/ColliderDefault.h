//------------------------------------------------------------------------------
//
// File Name:	ColliderDefault.h
// Author(s):	Andrew Lamb
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Collider Default component header.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "Collider.h"
#include "Stream.h"

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
	class ColliderDefault : public Collider
	{
		// Public Constants, Enums, and Structs:
	public:

		// Constructors/Destructors:
	public:
		ColliderDefault();
		ColliderDefault(const ColliderDefault& other);
		~ColliderDefault();
		ColliderDefault* Clone(void) const override;
		void Read(Nightjar::Stream& obj) override;
		// Public Static Functions:
	public:

		// Public Functions:
	public:

		// Public Event Handlers
	public:

		// Private Constants, Enums, and Structs:
	private:

		// Private Static Functions:
	private:

		// Private Functions:
	private:
		bool IsColliding(const ColliderDefault* other) const;

		bool IsColliding(const Collider* other) const override;

		// Private Static Variables:
	private:

		// Private Variables:
	private:
	};

}	// namespace