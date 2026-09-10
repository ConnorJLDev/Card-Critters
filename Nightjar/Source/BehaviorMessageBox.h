//------------------------------------------------------------------------------
//
// File Name:	BehaviorMessageBox.h
// Author(s):	Jonathan
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
	class Sprite;
	class SpriteText;
	struct CollisionRecord;

	// Typedefs:

	// Class Definition:
	class BehaviorMessageBox : public BehaviorInteractable
	{
		// Public Constants, Enums, and Structs:
	public:
		// Common component data.
		COMPONENT_COMMON(BehaviorMessageBox);

		// Constructors/Destructors:
	public:
		BehaviorMessageBox(void);

		BehaviorMessageBox(const BehaviorMessageBox& other);

		~BehaviorMessageBox(void);

		// Public Static Functions:
	public:

		// Public Functions:
	public:
		/// <summary>
		/// Clone the BehaviorMessageBox.
		/// </summary>
		/// <returns>A clone of this BehaviorMessageBox.</returns>
		BehaviorMessageBox* Clone() const override;

		/// <summary>
		/// Read BehaviorMessageBox data from a stream.
		/// </summary>
		/// <param name="stream">The stream to read from.</param>
		void Read(StreamIn& stream) override;

		/// <summary>
		/// Writes the BehaviorMessageBox data into the stream.
		/// </summary>
		/// <param name="stream">The stream to write into.</param>
		void Write(StreamOut& stream) override;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="func"></param>
        void SetFinishCallback(std::function<void()> func);

		// Public Event Handlers
	public:
	
		// Private Constants, Enums, and Structs:
	private:

		enum BehaviorMessageBoxStates
		{
			cBehaviorMessageBoxInvalid = -1,	// BehaviorMessageBox has not yet been initialized.
			cBehaviorMessageBoxIdle,			// BehaviorMessageBox is idle.
		};

		// Private Static Functions:
	private:
		static const StreamKey ReadKeyMessages, ReadKeyOnMessagesEnd, ReadKeyFont;

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

		void UpdateMessageEndFunc();

		// Private Static Variables:
	private:

		// Private Variables:
	private:
		string_t fontName_;
		vector_t<string_t> messages_;
		string_t onMessageEndStr_;
		std::function<void()> onMessageEnd_;
		SpriteText* childText_;
		int messageIndex_;
        bool onMessageEndCustom_;
	};

}	// namespace
