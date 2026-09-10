//------------------------------------------------------------------------------
//
// File Name:	BehaviorCredits.h
// Author(s):	$username$
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Template class for a new behavior.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Behavior.h"

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
	struct CollisionRecord;

	// Typedefs:

	// Class Definition:
	class BehaviorCredits : public Behavior
	{
		// Public Constants, Enums, and Structs:
	public:
		// Common component data.
		COMPONENT_COMMON(BehaviorCredits);

		// Constructors/Destructors:
	public:
		BehaviorCredits(void);

		BehaviorCredits(const BehaviorCredits& other);

		~BehaviorCredits(void);

		// Public Static Functions:
	public:

		// Public Functions:
	public:
		/// <summary>
		/// Clone the BehaviorCredits.
		/// </summary>
		/// <returns>A clone of this BehaviorCredits.</returns>
		BehaviorCredits* Clone() const override;

		/// <summary>
		/// Read BehaviorCredits data from a stream.
		/// </summary>
		/// <param name="stream">The stream to read from.</param>
		void Read(StreamIn& stream) override;

		/// <summary>
		/// Writes the BehaviorCredits data into the stream.
		/// </summary>
		/// <param name="stream">The stream to write into.</param>
		void Write(StreamOut& stream) override;

		// Public Event Handlers
	public:
	
		// Private Constants, Enums, and Structs:
	private:

		enum class States
		{
			cInvalid = -1,	// BehaviorCredits has not yet been initialized.
			cIdle,			// BehaviorCredits is idle.
		};

		// Private Static Functions:
	private:

		// Private Functions:
	private:
		/// <summary>
		/// Called on initializing/entering a new state.
		/// </summary>
		void OnInit() override;

		/// <summary>
		/// Called when the base behavior updates.
		/// </summary>
		/// <param name="dt">The time elapsed since the previous tick.</param>
		void OnUpdate(float dt) override;

		/// <summary>
		/// Called on exiting an old state.
		/// </summary>
		void OnExit() override;

		/// <summary>
		/// Callback for when the parent entity of this behavior experiences a collision.
		/// </summary>
		/// <param name="record">The record of the collision.</param>
		void CollisionHandler(const CollisionRecord& record);

		// Private Static Variables:
	private:

		// Private Variables:
	private:
	};

}	// namespace
