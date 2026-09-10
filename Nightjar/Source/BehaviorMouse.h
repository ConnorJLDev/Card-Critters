//------------------------------------------------------------------------------
//
// File Name:	BehaviorMouse.h
// Author(s):	Barend Brink (barend.brink)
// Project:		Card Critters
// Course:		GAM200
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Behavior.h"

namespace Nightjar
{
	//------------------------------------------------------------------------------
	// Forward References:
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// Public Constants:
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// Public Typedefs:
	//------------------------------------------------------------------------------

	typedef struct CollisionRecord CollisionRecord;

	//------------------------------------------------------------------------------
	// Public Structures:
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// Public Variables:
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	class BehaviorMouse : public Behavior
	{
	public:
		// Common component data.
		COMPONENT_COMMON(BehaviorMouse);
		BehaviorMouse();
		BehaviorMouse(const BehaviorMouse& other);
		~BehaviorMouse();
		BehaviorMouse* Clone(void) const override;
		void Read(StreamIn& stream) override;

		/// <summary>
		/// Writes the BehaviorMouse data into the stream.
		/// </summary>
		/// <param name="stream">The stream to write into.</param>
		void Write(StreamOut& stream) override;

	protected:
		void OnInit() override;
		void OnUpdate(float dt) override;
		void OnExit() override;
	private:
		enum MouseStates
		{
			cMouseInvalid = -1,	// Mouse has not yet been initialized.
			cMouseIdle,			// Mouse is idle.
		};
	};

}
