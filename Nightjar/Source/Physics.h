//------------------------------------------------------------------------------
//
// File Name:	Physics.h
// Author(s):	Andrew Lamb (andrew.lamb), Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Physics component header.
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

#include "PhysicsBase.h"

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
	class Physics : public Component, public PhysicsBase
	{
		// Public Constants, Enums, and Structs:
	public:
		// Common component data.
		COMPONENT_COMMON(Physics);

		// Constructors/Destructors:
	public:
		Physics();
	private:
		Physics(const Physics& other);
	public:
		~Physics();

		// Public Static Functions:
	public:

		// Public Functions:
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		Physics* Clone() const override;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="stream"></param>
		void Read(StreamIn& stream) override;

		/// <summary>
		/// Writes the Physics data into the stream.
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
		/// <returns></returns>
		const Vec2& GetAcceleration() const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const Vec2& GetVelocity() const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		float GetRotationalVelocity() const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const Vec2& GetOldTranslation() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="acceleration"></param>
		void SetAcceleration(const Vec2& acceleration);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="velocity"></param>
		void SetVelocity(const Vec2& velocity);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rotationalVelocity"></param>
		void SetRotationalVelocity(float rotationalVelocity);

		// Public Event Handlers
	public:

		// Private Constants, Enums, and Structs:
	private:

		// Private Static Functions:
	private:

		// Private Functions:
	private:

		// Private Static Variables:
	private:

		// Private Variables:
	private:
		Vec2 oldTranslation_;
	};

}	// namespace
