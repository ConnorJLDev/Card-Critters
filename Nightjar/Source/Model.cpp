//------------------------------------------------------------------------------
//
// File Name:	Model.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Component for handling a model.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Component.h"
#include "Model.h"
#include "Entity.h"
#include "GraphicsSystem.h"
#include "Armature.h"
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"
#include "Animation.h"
#include "ModelData.h"

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

	static constexpr StreamKey
		  ReadKeyModel = "Model"
		;

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

	Model::Model(void)
		: Component(TypeEnum::Model)
		, model_(nullptr)
	{
	}

	Model::Model(const Model& other)
		: Component(other)
		, model_(new ModelData(*other.model_))
	{
	}

	//--------------------------------------------------------------------------

	Model::~Model(void)
	{
		SafeDeleteCheck(model_);
	}

#pragma endregion Constructors

	//--------------------------------------------------------------------------
	// Public Static Functions:
	//--------------------------------------------------------------------------

#pragma region Public Static Functions

#pragma endregion Public Static Functions

	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

	Model* Model::Clone(void) const
	{
		return new Model(*this);
	}

	void Model::Read(StreamIn& stream)
	{
		string_t modelName;
		if (stream.TryReadValue(ReadKeyModel, modelName))
		{
			const ModelData* model = ModelLibrary::Get_S(modelName);
			assert(model != nullptr);
			model_ = new ModelData(*model);
		}
	}

	void Model::Write(StreamOut& stream)
	{
		UNREFERENCED_PARAMETER(stream);
	}

	void Model::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		assert(model_ != nullptr);
		model_->UpdateMatrices();
	}

	void Model::Render() const
	{
		assert(model_ != nullptr);
		Transform* transform = EntityHas(Transform);
		assert(transform != nullptr);
		model_->Render(transform->GetMatrix());
	}

	ModelData* Model::GetModelData() const
	{
		return model_;
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

#pragma endregion Private Functions

}	// namespace
