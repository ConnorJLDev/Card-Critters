//------------------------------------------------------------------------------
//
// File Name:	BehaviorInteractable.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Base behavior for an entity that interacts with the mouse.
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

	// Typedefs:

	// Class Definition:
	class BehaviorInteractable : public Behavior
	{
		// Public Constants, Enums, and Structs:
	public:
		// Common component data.
		COMPONENT_COMMON(BehaviorInteractable);

		// Constructors/Destructors:
	public:
		BehaviorInteractable(SubTypeEnum type);

		BehaviorInteractable(const BehaviorInteractable& other);

		~BehaviorInteractable(void);

		// Public Static Functions:
	public:

		// Public Functions:
	public:

		/// <summary>
		/// Updates the interactable status and the underlying behavior states.
		/// </summary>
		/// <param name="dt"></param>
		void Update(float dt) override;

		/// <summary>
		/// Checks if the mouse is hovering over the collider.
		/// </summary>
		/// <returns>True if the mouse is within the collider, false otherwise.</returns>
		bool IsHovered() const;

		/// <summary>
		/// Checks if a mouse button is down while hovering over the collider.
		/// </summary>
		/// <returns>True if a mouse button is down while within the collider, false otherwise.</returns>
		bool IsClicked() const;

		// Public Event Handlers
	public:
	
		// Private Constants, Enums, and Structs:
	private:

		// Private Static Functions:
	private:

		// Private Functions:
	private:
		/// <summary>
		/// Callback when the mouse enters the collider.
		/// </summary>
		virtual void OnHoverStart();

		/// <summary>
		/// Callback when the mouse exits the collider.
		/// </summary>
		virtual void OnHoverEnd();

		/// <summary>
		/// Callback when a mouse button is pressed while in the collider.
		/// </summary>
		virtual void OnClickStart();

		/// <summary>
		/// Callback when a mouse button is released while in the collider.
		/// </summary>
		/// <param name="contained">True if the click stayed entirely contained within the collider, false otherwise.</param>
		virtual void OnClickEnd(bool contained);

		// Private Static Variables:
	private:

		// Private Variables:
	private:
		// Stores if we are hovered this frame.
		bool hovered_;
		// Stores if we are clicked this frame.
		bool clicked_;
		// Stores if a click has been entirely contained in the collider (trigger to release).
		bool clickContained_;
	};

}	// namespace
