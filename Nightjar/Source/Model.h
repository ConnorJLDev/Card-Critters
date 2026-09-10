//------------------------------------------------------------------------------
//
// File Name:	Model.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Component for handling a model.
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
	class ModelData;

	// Typedefs:

	// Class Definition:
	class Model : public Component
	{
		// Public Constants, Enums, and Structs:
	public:
		// Common component data.
		COMPONENT_COMMON(Model);

		// Constructors/Destructors:
	public:
		Model(void);
	private:
		Model(const Model& other);
	public:
		~Model(void);

		// Public Static Functions:
	public:

		// Public Functions:
	public:
		/// <summary>
		/// Clone the Model.
		/// </summary>
		/// <returns>A clone of this Model.</returns>
		Model* Clone() const override;
		
		/// <summary>
		/// Read Model data from a stream.
		/// </summary>
		/// <param name="stream">The stream to read from.</param>
		void Read(StreamIn& stream) override;

		/// <summary>
		/// Writes the Model data into the stream.
		/// </summary>
		/// <param name="stream">The stream to write into.</param>
		void Write(StreamOut& stream) override;

		/// <summary>
		/// Update this Model.
		/// </summary>
		/// <param name="dt">The time elapsed since the previous tick.</param>
		void Update(float dt) override;
		
		/// <summary>
		/// Render this Model.
		/// </summary>
		void Render() const override;

		/// <summary>
		/// Gets the underlying model data.
		/// </summary>
		/// <returns>The underlying model data.</returns>
		ModelData* GetModelData() const;

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
		ModelData* model_;
	};

}	// namespace
