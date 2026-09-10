//------------------------------------------------------------------------------
//
// File Name:	Behavior.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Base behavior component.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Component.h"

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
	class Behavior : public Component
	{
		// Public Constants, Enums, and Structs:
	public:
		// Common component data.
		COMPONENT_COMMON(Behavior);

		enum SubTypeEnum
		{
			cBehaviorInvalid = -1,
#define REGISTER_BEHAVIOR(behaviorName) c##behaviorName,
#include "Behaviors.def"
#undef REGISTER_BEHAVIOR
			cBehaviorNUM_ENTRIES,
		};

		// Constructors/Destructors:
	public:
		Behavior(SubTypeEnum type);
	protected:
		Behavior(const Behavior& other);
	public:
		virtual ~Behavior(void);

		// Public Static Functions:
	public:

		// Public Functions:
	public:
		/// <summary>
		/// Update this Behavior.
		/// </summary>
		/// <param name="dt">The time elapsed since the previous tick.</param>
		void Update(float dt) override;

		/// <summary>
		/// Gets the subtype of the Behavior.
		/// </summary>
		/// <returns>The subtype of the Behavior.</returns>
		SubTypeEnum GetSubType() const;

		// Public Event Handlers
	public:

		// Private Constants, Enums, and Structs:
	private:

		// Private Static Functions:
	private:

		// Private Functions:
	protected:
		// Behavior Finite-State Machine (FSM) function pointers.
		virtual void OnInit() = 0;
		virtual void OnUpdate(float dt) = 0;
		virtual void OnExit() = 0;

		/// <summary>
		/// Called only on the first frame this behavior is updated (typically from an Invalid state to an Idle state).
		/// </summary>
		virtual void OnStartup(void);

		/// <summary>
		/// Sets the next state for this Behavior.
		/// </summary>
		/// <param name="state">The next state for this Behavior.</param>
		void SetStateNext(int state);

		/// <summary>
		/// Gets the current state of this Behavior.
		/// </summary>
		/// <returns>The current state of this Behavior.</returns>
		int GetStateCurrent() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		Entity* GetOrMakeChildEntity(string_view_arg name);

		// Private Static Variables:
	private:

		// Private Variables:
	private:
		// Behavior Finite-State Machine (FSM) state variables.
		int stateCurr_, stateNext_;

		SubTypeEnum type_;

	};

}	// namespace
