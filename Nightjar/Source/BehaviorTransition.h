//------------------------------------------------------------------------------
//
// File Name:	BehaviorTransition.h
// Author(s):	Andrew Lamb (andrew.lamb)
// Course:		GAM250S26
// Project:		Card Critters
// Purpose:		Header for transition behavior class.
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
	class BehaviorTransition : public Behavior
	{
		// Public Constants, Enums, and Structs:
	public:
		// Common component data.
		COMPONENT_COMMON(BehaviorTransition);

		// Constructors/Destructors:
	public:
		BehaviorTransition(void);

		BehaviorTransition(const BehaviorTransition& other);

		~BehaviorTransition(void);

		// Public Static Functions:
	public:

		// Public Functions:
	public:
		/// <summary>
		/// Clone the BehaviorTransition.
		/// </summary>
		/// <returns>A clone of this BehaviorTransition.</returns>
		BehaviorTransition* Clone() const override;

		/// <summary>
		/// Read BehaviorTransition data from a stream.
		/// </summary>
		/// <param name="stream">The stream to read from.</param>
		void Read(StreamIn& stream) override;

		/// <summary>
		/// Writes the BehaviorTransition data into the stream.
		/// </summary>
		/// <param name="stream">The stream to write into.</param>
		void Write(StreamOut& stream) override;

		void SetNextScene(string_t scene, unsigned int index = 0);
		// Public Event Handlers
	public:
	
		// Private Constants, Enums, and Structs:
	private:

		enum States
		{
			cTransitionInvalid = -1,	// BehaviorTransition has not yet been initialized.
			cTransitionIdle,			// BehaviorTransition is idle.
			cTransitionFadeIn,			// 
			cTransitionFadeOut,
			cTransitionWaiting,
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

		// Private Static Variables:
	private:

		// Private Variables:
	private:
		string_t _nextScene;
        unsigned int _nextSceneIndex;
	};

}	// namespace
