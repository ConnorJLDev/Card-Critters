//------------------------------------------------------------------------------
//
// File Name:	AnimationLinkModel.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Interface between an animation component and model components.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "AnimationLinkModel.h"
#include "Component.h"
#include "Entity.h"
#include "Model.h"
#include "Armature.h"
#include "ArmatureBone.h"
#include "ModelData.h"
#include "Particle.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	//--------------------------------------------------------------------------
	// Public Constants:
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// Public Static Variables:
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// Public Variables:
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// Private Constants:
	//--------------------------------------------------------------------------

	constexpr StreamKey AnimationLinkModel::ReadKeyBones = "Bones";

	//--------------------------------------------------------------------------
	// Private Static Variables:
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// Private Variables:
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// Constructors/Destructors:
	//--------------------------------------------------------------------------

#pragma region Constructors

	AnimationLinkModel::AnimationLinkModel()
		: AnimationLink()
		, options_(cNone)
		, boneData_()
	{
	}

	//--------------------------------------------------------------------------

	AnimationLinkModel::~AnimationLinkModel()
	{
	}

#pragma endregion Constructors

	//--------------------------------------------------------------------------
	// Public Static Functions:
	//--------------------------------------------------------------------------

#pragma region Public Static Functions

	ENUM_IMPLEMENT_OPS(AnimationLinkModel::Options);

#pragma endregion Public Static Functions

	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

	void AnimationLinkModel::Start(Entity* entity)
	{
		Model* model = entity->Has(Model);
		if (model == nullptr)
		{
			TraceWarning("Entity doesn't have Model!");
			return;
		}
		Armature* armature = model->GetModelData()->GetArmature();
		for (const auto& data : boneData_)
		{
			ArmatureBone* bone = armature->GetBone(data.name);
			if (bone == nullptr)
			{
				TraceWarning("Bone \"{}\" not found for anim!", data.name);
				continue;
			}
			data.Start(*bone);
		}
	}

	void AnimationLinkModel::Update(Entity* entity, const vector_t<float>& evaluated)
	{
		Model* model = entity->Has(Model);
		if (model == nullptr)
		{
			TraceWarning("Entity doesn't have Model!");
			return;
		}
		Apply(model, evaluated);
	}

	void AnimationLinkModel::Update(Particle*, const vector_t<float>&)
	{
		assert(false);
	}

	void AnimationLinkModel::Apply(Model* model, const vector_t<float>& evaluated) const
	{
		Apply(model->GetModelData(), evaluated);
	}

	void AnimationLinkModel::Apply(ModelData* model, const vector_t<float>& evaluated) const
	{
		Apply(model->GetArmature(), evaluated);
	}

	void AnimationLinkModel::Apply(Armature* armature, const vector_t<float>& evaluated) const
	{
		for (const auto& data : boneData_)
		{
			ArmatureBone* bone = armature->GetBone(data.name);
			if (bone == nullptr)
			{
				TraceWarning("Bone \"{}\" not found for anim!", data.name);
				continue;
			}
			data.Apply(*bone, evaluated);
		}
	}

#pragma endregion Public Functions

	//--------------------------------------------------------------------------
	// Private Static Functions:
	//--------------------------------------------------------------------------

#pragma region Private Static Functions

#pragma endregion Private Static Functions

	//--------------------------------------------------------------------------
	// Private Functions:
	//--------------------------------------------------------------------------

#pragma region Private Functions

	unsigned AnimationLinkModel::OnRead(StreamIn& stream, const vector_t<float>& curveIndices)
	{
		unsigned curveCount = 0;
		if (stream.StreamHas(ReadKeyBones))
		{
			boneData_.clear();
			stream.ReadArrayObjects(ReadKeyBones, [this, &curveIndices, &curveCount](StreamIn& stream, int)
			{
				auto& data = boneData_.emplace_back();
				stream.ReadValue("Name", data.name);
				data.Read(stream, curveIndices, curveCount);
			});
		}

		return curveCount;
	}

#pragma endregion Private Functions

}	// namespace