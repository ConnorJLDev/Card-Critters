//------------------------------------------------------------------------------
//
// File Name:	ModelData.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Model encapsulator.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ModelData.h"
#include "GraphicsSystem.h"
#include "Armature.h"
#include "ArmatureBone.h"
#include "Mesh.h"
#include "Material.h"
#include "Shader.h"

#include "Library.inl"
#include "LibraryStatic.inl"

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
		  ReadKeySubmeshes = "Submeshes"
		, ReadKeyMeshes = "Meshes"
		, ReadKeyMaterials = "Materials"
		, ReadKeyBoneBindings = "BoneBindings"
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

	ModelData::ModelData(void)
		: NamedObject()
		, armature_(nullptr)
		, submeshes_()
		, boneNames_()
		, materials_()
		, meshes_()
		, boneMtxs_()
		, boneNormalMtxs_()
		, dirty_(true)
	{
	}

	ModelData::ModelData(const ModelData& other)
		: NamedObject(other)
		, armature_(GraphicsSystem::BuildArmature(other.armature_))
		, submeshes_(other.submeshes_)
		, boneNames_(other.boneNames_)
		, materials_()
		, meshes_(other.meshes_)
		, boneMtxs_()
		, boneNormalMtxs_()
		, dirty_(true)
	{
		for (auto& [materialName, otherMaterial] : other.materials_)
		{
			Material* materialCopy = GraphicsSystem::BuildMaterial(otherMaterial);
			materials_[materialName] = materialCopy;
			for (auto& submesh : submeshes_ | std::views::filter([otherMaterial](Submesh& submesh) { return submesh.material == otherMaterial; }))
			{
				submesh.material = materialCopy;
			}
		}
	}

	//--------------------------------------------------------------------------

	ModelData::~ModelData(void)
	{
		for (auto& [materialName, material] : materials_)
		{
			GraphicsSystem::DestroyMaterial(material);
		}
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

	void ModelData::Read(StreamIn& stream)
	{
		assert(NamedObject::ReadName(stream));

		string_t prefix = "MODEL<" + GetName() + ">";

		if (/*!stream.TryReadValue(Armature::ReadKey, armature_) && */stream.StepInto(Armature::ReadKey))
		{
			armature_ = new Armature();
			armature_->Read(stream);
			armature_->SetName(prefix + armature_->GetName());
			ArmatureLibrary::GetInstance()->Add(armature_);
			stream.StepBack();
		}
		
		if (stream.StreamHas(ReadKeyMeshes))
		{
			stream.ReadArrayObjects(ReadKeyMeshes, [this, prefix](StreamIn& stream, int index)
			{
				UNREFERENCED_PARAMETER(index);
				Mesh* mesh = new Mesh();
				mesh->Read(stream);
				meshes_.emplace(mesh->GetName(), mesh);
				mesh->SetName(prefix + mesh->GetName());
				MeshLibrary::GetInstance()->Add(mesh);
			});
		}
		
		if (stream.StreamHas(ReadKeyMaterials))
		{
			stream.ReadArrayObjects(ReadKeyMaterials, [this, prefix](StreamIn& stream, int index)
			{
				UNREFERENCED_PARAMETER(index);
				Material* material = new Material();
				material->Read(stream);
				materials_.emplace(material->GetName(), material);
				material->SetName(prefix + material->GetName());
				MaterialLibrary::GetInstance()->Add(material);
			});
		}
		
		if (stream.StreamHas(ReadKeySubmeshes))
		{
			submeshes_.clear();
			submeshes_.reserve((unsigned)stream.GetArraySize(ReadKeySubmeshes));
			stream.ReadArrayObjects(ReadKeySubmeshes, [this, prefix](StreamIn& stream, int index)
			{
				UNREFERENCED_PARAMETER(index);
				Submesh& submesh = submeshes_.emplace_back();
				string_t meshName, materialName;
				// Get submesh mesh.
				stream.ReadValue(Mesh::ReadKey, meshName);
				auto meshIter = meshes_.find(meshName);
				submesh.mesh = meshIter == meshes_.end() ? GraphicsSystem::GetMesh(meshName) : meshIter->second;
				meshes_[meshName] = submesh.mesh;
				// Get submesh material.
				stream.ReadValue(Material::ReadKey, materialName);
				auto materialIter = materials_.find(materialName);
				submesh.material = materialIter == materials_.end() ? GraphicsSystem::BuildMaterial(materialName) : materialIter->second;
				materials_[materialName] = submesh.material;
			});
		}

		if (stream.StreamHas(ReadKeyBoneBindings))
		{
			boneNames_.clear();
			stream.ReadVector(ReadKeyBoneBindings, boneNames_);
		}

		dirty_ = true;
	}

	void ModelData::Render(const Mat4& transform)
	{
		for (const auto& submesh : submeshes_)
		{
			GraphicsSystem::DrawCall(transform, *submesh.mesh, *submesh.material);
		}
	}

	Armature* ModelData::GetArmature() const
	{
		return armature_;
	}

	Material* ModelData::GetMaterial(const string_t& name) const
	{
		return materials_.at(name);
	}

	const Mesh* ModelData::GetMesh(const string_t& name) const
	{
		return meshes_.at(name);
	}

	void ModelData::UpdateMatrices()
	{
		size_t count = boneNames_.size();
		if (boneMtxs_.size() < count || boneNormalMtxs_.size() < count)
		{
			boneMtxs_.resize(count);
			boneNormalMtxs_.resize(count);
			vector_t<Mat4> bindMtxs{};
			bindMtxs.resize(count);
			for (size_t i = 0; i < count; ++i)
			{
				bindMtxs[i] = armature_->GetBone(boneNames_[i])->GetInverseBindMatrix();
			}
			for (auto& [materialName, material] : materials_)
			{
				constexpr Mat4 identity = glm::identity<Mat4>();
				material->SetUniform("bonesBindInv[0]", bindMtxs.data(), count, 1);
				material->SetUniform("bones[0]", identity);
				material->SetUniform("bonesNrm[0]", identity);
				material->SetUniform("bonesBindInv[0]", identity);
			}
		}
		for (size_t i = 0; i < count; ++i)
		{
			ArmatureBone* bone = armature_->GetBone(boneNames_[i]);
			boneMtxs_[i] = bone->GetMatrixFinal();
			boneNormalMtxs_[i] = bone->GetMatrixObjectNormal();
		}
		if (count <= 0)
		{
			return;
		}
		for (auto& [materialName, material] : materials_)
		{
			material->SetUniform("bones[0]", boneMtxs_.data(), count, 1);
			material->SetUniform("bonesNrm[0]", boneNormalMtxs_.data(), count, 1);
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

#pragma endregion Private Functions

	//--------------------------------------------------------------------------
	// Library Implementation:
	//--------------------------------------------------------------------------

#pragma region Library Implementation

	template ModelLibrary;

	INSTANTIATE_LIBRARY_IMGUI_SELECTOR(ModelData);

#pragma endregion Library Implementation

}	// namespace
