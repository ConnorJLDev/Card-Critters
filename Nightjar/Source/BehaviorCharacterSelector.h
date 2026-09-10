//------------------------------------------------------------------------------
//
// File Name:	BehaviorCharacterSelector.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Behavior for registering clicks on characters.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "BehaviorInteractable.h"

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
	class BehaviorCharacter;
	struct CollisionRecord;

	// Typedefs:

	// Class Definition:
	class BehaviorCharacterSelector : public BehaviorInteractable
	{
		// Public Constants, Enums, and Structs:
	public:
		COMPONENT_COMMON(BehaviorCharacterSelector);

		// Constructors/Destructors:
	public:
		BehaviorCharacterSelector(void);

		BehaviorCharacterSelector(const BehaviorCharacterSelector& other);

		~BehaviorCharacterSelector(void);

		// Public Static Functions:
	public:

		// Public Functions:
	public:
		/// <summary>
		/// Clone the BehaviorCharacterSelector.
		/// </summary>
		/// <returns>A clone of this BehaviorCharacterSelector.</returns>
		BehaviorCharacterSelector* Clone() const override;

		/// <summary>
		/// Read BehaviorCharacterSelector data from a stream.
		/// </summary>
		/// <param name="stream">The stream to read from.</param>
		void Read(StreamIn& stream) override;

		/// <summary>
		/// Writes the BehaviorCharacterSelector data into the stream.
		/// </summary>
		/// <param name="stream">The stream to write into.</param>
		void Write(StreamOut& stream) override;

		// Public Event Handlers
	public:
	
		// Private Constants, Enums, and Structs:
	private:

		enum BehaviorCharacterSelectorStates
		{
			cBehaviorCharacterSelectorInvalid = -1,	// BehaviorCharacterSelector has not yet been initialized.
			cBehaviorCharacterSelectorIdle,			// BehaviorCharacterSelector is idle.
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
		/// Callback when the mouse enters the collider.
		/// </summary>
		void OnHoverStart() override;

		/// <summary>
		/// Callback when the mouse exits the collider.
		/// </summary>
		void OnHoverEnd() override;

		/// <summary>
		/// Callback when a mouse button is pressed while in the collider.
		/// </summary>
		void OnClickStart() override;

		/// <summary>
		/// Callback when a mouse button is released while in the collider.
		/// </summary>
		/// <param name="contained">True if the click stayed entirely contained within the collider, false otherwise.</param>
		void OnClickEnd(bool contained) override;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		BehaviorCharacter* GetCharacter() const;

		// Private Static Variables:
	private:

		// Private Variables:
	private:
	};

}	// namespace
