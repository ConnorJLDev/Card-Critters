//------------------------------------------------------------------------------
//
// File Name:	AnimationLinkMaterial.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Interface between an animation component and materials.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "AnimationLinkMaterial.h"
#include "Component.h"
#include "Entity.h"
#include "Material.h"
#include "Model.h"
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

	static constexpr StreamKey ReadKeyProperties = "Properties";

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

	AnimationLinkMaterial::AnimationLinkMaterial()
		: AnimationLink()
		, options_(cNone)
		, name_()
		, elementData_()
		, properties_()
	{
	}

	//--------------------------------------------------------------------------

	AnimationLinkMaterial::~AnimationLinkMaterial()
	{
	}

#pragma endregion Constructors

	//--------------------------------------------------------------------------
	// Public Static Functions:
	//--------------------------------------------------------------------------

#pragma region Public Static Functions

	ENUM_IMPLEMENT_OPS(AnimationLinkMaterial::Options);

#pragma endregion Public Static Functions

	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

	void AnimationLinkMaterial::Start(Entity* entity)
	{
		Model* model = entity->Has(Model);
		if (model == nullptr)
		{
			TraceWarning("Entity doesn't have Model!");
			return;
		}
#ifdef _DEBUG
		Material* material = model->GetModelData()->GetMaterial(name_);
		assert(material != nullptr);
#endif
	}

	void AnimationLinkMaterial::Update(Entity* entity, const vector_t<float>& evaluated)
	{
		Model* model = entity->Has(Model);
		if (model == nullptr)
		{
			TraceWarning("Entity doesn't have Model!");
			return;
		}
		ModelData* modelData = model->GetModelData();
		Apply(modelData, evaluated);
	}

	void AnimationLinkMaterial::Update(Particle* particle, const vector_t<float>& evaluated)
	{
		Apply(particle->GetMaterial(), evaluated);
	}

	void AnimationLinkMaterial::Apply(ModelData* model, const vector_t<float>& evaluated)
	{
		assert(model != nullptr);
		Material* material = model->GetMaterial(name_);
		Apply(material, evaluated);
	}

	void AnimationLinkMaterial::Apply(Material* material, const vector_t<float>& evaluated)
	{
		assert(material != nullptr);
		for (const auto& prop : properties_)
		{
			prop.Apply(material, evaluated);
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

	void AnimationLinkMaterial::PropertyData::Apply(Material* material, const vector_t<float>& evaluated) const
	{
		union
		{
			Vec4 evalSingle;
			IVec4 evalInt;
			UVec4 evalUInt;
		};
		for (unsigned char i = 0; i < curvesCount; ++i)
		{
			evalSingle[i] = curvesConst[i] ? elementData[i].constValue : evaluated[elementData[i].curveIndex];
		}
		switch (typeBase)
		{
		case DataType::Single:
			break;
		case DataType::Int:
			evalInt = evalSingle;
			break;
		case DataType::UInt:
			evalUInt = evalSingle;
			break;
		default:
			TraceError("Unhandled type base \"{}\" in animation for material \"{}\"", ENUM_WRITE_VALUE(DataType, typeBase), material->GetName());
			assert(false);
			return;
		}
		switch (type)
		{
#define SET_UNIFORM(typeEnum, type, container) case DataType::typeEnum: material->SetUniform(name, reinterpret_cast<type*>(&container)); break
			SET_UNIFORM(Single, float, evalSingle);
			SET_UNIFORM(Vec2, Vec2, evalSingle);
			SET_UNIFORM(Vec3, Vec3, evalSingle);
			SET_UNIFORM(Vec4, Vec4, evalSingle);
			SET_UNIFORM(Int, int, evalInt);
			SET_UNIFORM(IVec2, IVec2, evalInt);
			SET_UNIFORM(IVec3, IVec3, evalInt);
			SET_UNIFORM(IVec4, IVec4, evalInt);
			SET_UNIFORM(UInt, unsigned int, evalUInt);
			SET_UNIFORM(UVec2, UVec2, evalUInt);
			SET_UNIFORM(UVec3, UVec3, evalUInt);
			SET_UNIFORM(UVec4, UVec4, evalUInt);
		default:
			TraceError("Unhandled type \"{}\" in animation for material \"{}\"", ENUM_WRITE_VALUE(DataType, typeBase), material->GetName());
			assert(false);
			return;
		}
	}

	unsigned AnimationLinkMaterial::OnRead(StreamIn& stream, const vector_t<float>& curveIndices)
	{
		unsigned curveCount = 0;
		stream.TryReadValue("Name", name_);
		if (stream.StreamHas(ReadKeyProperties))
		{
			properties_.clear();
			vector_t<bool> curveConsts;
			stream.ReadArrayObjects(ReadKeyProperties, [this, &curveIndices, &curveCount, &curveConsts](StreamIn& stream, int)
			{
				auto& data = properties_.emplace_back();
				stream.ReadValue("Name", data.name);
				data.type = stream.ReadEnum<DataType>("Type", ENUM_READ(DataType));
                DataTypeInfo typeInfo = DataTypeInfos[(unsigned char)data.type];
				data.curvesCount = typeInfo.elementCount;
				assert(data.curvesCount > 0);
				data.typeBase = typeInfo.typeBase;
				data.curvesOffset = (unsigned char)curveCount;
				curveCount += data.curvesCount;
				curveConsts.clear();
				stream.ReadVector("Const", curveConsts);
				assert(curveConsts.size() == data.curvesCount);
				for (unsigned char i = 0; i < data.curvesCount; ++i)
				{
					const bool& curveConst = data.curvesConst[i] = curveConsts[i];
					if (curveConst)
					{
						data.elementData[i].constValue = curveIndices[(size_t)data.curvesOffset + i];
					}
					else
					{
						data.elementData[i].curveIndex = (unsigned)curveIndices[(size_t)data.curvesOffset + i];
					}
				}
			});
		}

		return curveCount;
	}

#pragma endregion Private Functions

}	// namespace
