//------------------------------------------------------------------------------
//
// File Name:	Camera.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Component for handling a camera.
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
#include "CameraSource.h"

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
	class Camera : public Component, public CameraSource
	{
		// Public Constants, Enums, and Structs:
	public:
		// Common component data.
		COMPONENT_COMMON(Camera);

		// Constructors/Destructors:
	public:
		Camera(void);
	private:
		Camera(const Camera& other);
	public:
		~Camera(void);

		// Public Static Functions:
	public:

		// Public Functions:
	public:
		/// <summary>
		/// Clone the Camera.
		/// </summary>
		/// <returns>A clone of this Camera.</returns>
		Camera* Clone() const override;
		
		/// <summary>
		/// Read Camera data from a stream.
		/// </summary>
		/// <param name="stream">The stream to read from.</param>
		void Read(StreamIn& stream) override;

		/// <summary>
		/// Writes the Camera data into the stream.
		/// </summary>
		/// <param name="stream">The stream to write into.</param>
		void Write(StreamOut& stream) override;

		/// <summary>
		/// Update this Camera.
		/// </summary>
		/// <param name="dt">The time elapsed since the previous tick.</param>
		void Update(float dt) override;
		
		/// <summary>
		/// Render this Camera.
		/// </summary>
		void Render() const override;

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
	};

}	// namespace
