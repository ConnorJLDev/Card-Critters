//------------------------------------------------------------------------------
//
// File Name:	AnimationLinkMaterial.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Interface between an animation component and materials.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "AnimationLink.h"

#include "DataTypes.h"

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
	class Material;

	// Typedefs:

	// Class Definition:
	class AnimationLinkMaterial : public AnimationLink
	{
		// Public Constants, Enums, and Structs:
	public:
		enum Options
		{
			cNone = 0,
		};

		// Constructors/Destructors:
	public:
		AnimationLinkMaterial();

		~AnimationLinkMaterial();

		// Public Static Functions:
	public:
		ENUM_DECLARE_OPS(Options);

		// Public Functions:
	public:
		//
		void Start(Entity* entity) override;

		//
		void Update(Entity* entity, const vector_t<float>& evaluated) override;

		//
		void Update(Particle* particle, const vector_t<float>& evaluated) override;

		//
		void Apply(ModelData* model, const vector_t<float>& evaluated);

		//
		void Apply(Material* material, const vector_t<float>& evaluated);

		// Public Event Handlers
	public:

		// Private Constants, Enums, and Structs:
	private:
		enum Indices
		{
			cElementCount = 4,
		};

		struct PropertyData
		{
			string_t name;
			ElementData elementData[cElementCount];
			bool curvesConst[cElementCount];
			DataType type;
			DataType typeBase;
			unsigned char curvesOffset;
			unsigned char curvesCount;

			void Apply(Material* material, const vector_t<float>& evaluated) const;
		};

		// Private Static Functions:
	private:

		// Private Functions:
	private:
		//
		unsigned OnRead(StreamIn& stream, const vector_t<float>& curveIndices) override;

		// Private Static Variables:
	private:

		// Private Variables:
	private:
		Options options_;
		string_t name_;
		ElementData elementData_[cElementCount];
		vector_t<PropertyData> properties_;
	};

}	// namespace
