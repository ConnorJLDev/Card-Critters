//------------------------------------------------------------------------------
//
// File Name:	BehaviorHealth.h
// Author(s):	Jason Nguyen (jason.nguyen)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Health behavior.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Transform.h"
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
	class BehaviorHealth : public Behavior
	{
		// Public Constants, Enums, and Structs:
	public:
		// Common component data.
		COMPONENT_COMMON(BehaviorHealth);

		// Constructors/Destructors:
	public:
		BehaviorHealth(SubTypeEnum type);

		BehaviorHealth(const BehaviorHealth& other);

		~BehaviorHealth(void) override;

		// Public Static Functions:
	public:

		// Public Functions:
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="stream"></param>
		void Read(StreamIn& stream) override;

		/// <summary>
		/// Writes the BehaviorHealth data into the stream.
		/// </summary>
		/// <param name="stream">The stream to write into.</param>
		void Write(StreamOut& stream) override;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="dt"></param>
		void Update(float dt) override;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="amount"></param>
		void ModifyHealth(int amount);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="amount"></param>
		void SetHealth(int amount);
		
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		int GetHealth() const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		int GetMaxHealth() const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		bool IsDead() const;

		// Public Event Handlers
	public:

		// Protected Constants, Enums, and Structs:
	protected:
		static const StreamKey ReadKeyHealth, ReadKeyHealthMax;
		static const StreamKey ChildHealthText, ChildHealthBarBG, ChildHealthBarFill, ChildHealthBarBorder;

		// Protected Static Functions:
	protected:

		// Protected Functions:
	protected:
		/// <summary>
		/// Creates/updates child entities.
		/// </summary>
		void UpdateChildren(void);
		void UpdateHealthText(Transform* parentTransform);
		void UpdateHealthBarBackground(Transform* parentTransform);
		void UpdateHealthBarFill(Transform* parentTransform);
		void UpdateHealthBarBorder(Transform* parentTransform);
		// Protected Static Variables:
	protected:

		// Protected Variables:
	protected:
		int health;
		int healthMax;
		int healthPrev;
		float displayedHealthRatio;
	};

}	// namespace
