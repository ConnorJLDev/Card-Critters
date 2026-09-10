//------------------------------------------------------------------------------
//
// File Name:	BehaviorFloatingText.h
// Author(s):	jason.nguyen
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Floating combat text behavior that auto-destroys.
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
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	// Forward Declarations:
	class Entity;
	struct CollisionRecord;

	//------------------------------------------------------------------------------
	// Class Definition:
	//------------------------------------------------------------------------------

	class BehaviorFloatingText : public Behavior
	{
	public:
		COMPONENT_COMMON(BehaviorFloatingText);

		// Constructors/Destructors
	public:
		BehaviorFloatingText(void);
		BehaviorFloatingText(const BehaviorFloatingText& other);
		~BehaviorFloatingText(void);

		// Clone
	public:
		BehaviorFloatingText* Clone() const override;

		// Read from JSON
		void Read(StreamIn& stream) override;

		/// <summary>
		/// Writes the BehaviorFloatingText data into the stream.
		/// </summary>
		/// <param name="stream">The stream to write into.</param>
		void Write(StreamOut& stream) override;

	private:

		enum BehaviorFloatingTextStates
		{
			cBehaviorFloatingTextInvalid = -1,
			cBehaviorFloatingTextIdle,
		};

	private:
		// Behavior overrides
		void OnInit() override;
		void OnUpdate(float dt) override;
		void OnExit() override;

		void CollisionHandler(const CollisionRecord& record);

	private:
		// Settings
		float lifetime;
		float timer;
		float floatSpeed;
		bool fade;        
	};
}