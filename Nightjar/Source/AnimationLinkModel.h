//------------------------------------------------------------------------------
//
// File Name:	AnimationLinkModel.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Interface between an animation component and model components.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "AnimationLink.h"
#include "AnimationLinkTransform.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	// Forward Declarations:
	class Model;
	class ModelData;
	class Armature;

	// Typedefs:

	// Class Definition:
	class AnimationLinkModel : public AnimationLink
	{
		// Public Constants, Enums, and Structs:
	public:
		enum Options : unsigned int
		{
			cNone = 0,
		};

		// Constructors/Destructors:
	public:
		AnimationLinkModel();

		~AnimationLinkModel();

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
		void Apply(Model* model, const vector_t<float>& evaluated) const;

		//
		void Apply(ModelData* model, const vector_t<float>& evaluated) const;

		//
		void Apply(Armature* armature, const vector_t<float>& evaluated) const;

		// Public Event Handlers
	public:

		// Private Constants, Enums, and Structs:
	private:
		enum Indices
		{
			cElementCount,
		};

		struct BoneData : public AnimationLinkTransform::TransformData
		{
			string_t name;
		};

		static const StreamKey ReadKeyBones;

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
		vector_t<BoneData> boneData_;
	};

}	// namespace
