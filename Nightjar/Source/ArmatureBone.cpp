//------------------------------------------------------------------------------
//
// File Name:	ArmatureBone.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Armature bone encapsulator.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ArmatureBone.h"
#include "Armature.h"

#include "TransformBase.inl"

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

    typedef TransformBase<NamedObject, ArmatureBone> Base;

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

	ArmatureBone::ArmatureBone(void)
		: Base()
		, mtxBindInverse_()
		, mtxObjNrm_()
	{
	}

	ArmatureBone::ArmatureBone(const ArmatureBone& other)
		: Base(other)
		, mtxBindInverse_(other.mtxBindInverse_)
		, mtxObjNrm_(other.mtxObjNrm_)
	{
	}

	//--------------------------------------------------------------------------

	ArmatureBone::~ArmatureBone(void)
	{
	}

#pragma endregion Constructors

	//--------------------------------------------------------------------------
	// Public Static Functions:
	//--------------------------------------------------------------------------

#pragma region Public Static Functions

	ArmatureBone* ArmatureBone::Read(StreamIn& stream, const Armature* armature)
	{
		string_t name;

		if (!TryReadName(stream, name))
		{
			TraceWarning("Failed to read bone from {}|{}", stream.GetPath().c_str(), stream.GetHierarchy().c_str());
			return nullptr;
		}

		ArmatureBone* bone = new ArmatureBone();
		bone->SetName(name);

		if (stream.StreamHas(ReadKeyParent))
		{
			string_t parentName = stream.ReadString(ReadKeyParent);
			bone->SetParentTransform(armature->GetBone(parentName));
		}

		stream.TryReadValue(ReadKeyTranslation, bone->translation3_);
		stream.TryReadValue(ReadKeyRotation, bone->rotation3_);
		stream.TryReadValue(ReadKeyScale, bone->scale3_);

		bone->dirtyFlags_ = DirtyFlags::All;
		bone->finalDirty_ = true;

		bone->SetInverseBindMatrix();

		return bone;
	}

#pragma endregion Public Static Functions

	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

	void ArmatureBone::SetArmature(Armature* armature)
	{
		if (parent_ != nullptr)
		{
			const string_t& parentName = parent_->GetName();
			SetParentTransform(armature->GetBone(parentName));
		}
	}

	const Vec3& ArmatureBone::GetTranslation() const
	{
		return translation3_;
	}

	void ArmatureBone::SetTranslation(const Vec3& translation)
	{
        SetTranslation3D(translation);
	}

	const Vec3& ArmatureBone::GetRotation() const
	{
		return GetRotation3D();
	}

	void ArmatureBone::SetRotation(const Vec3& rotation)
	{
        SetRotation3D(rotation);
	}

	const Vec3& ArmatureBone::GetScale() const
	{
		return GetScale3D();
	}

	void ArmatureBone::SetScale(const Vec3& scale)
	{
        SetScale3D(scale);
	}

	const Mat4& ArmatureBone::GetMatrixFinal()
	{
		bool finalDirty = finalDirty_ || (bool)dirtyFlags_;
		Base::GetMatrixFinal();
		if (finalDirty)
		{
			mtxObjNrm_ = /*glm::transpose(*/glm::inverse(mtxFinal_)/*)*/;//parent_ == nullptr ? localNrm : parent_->GetMatrixObjectNormal() * localNrm;
		}

		return mtxFinal_;
	}

	const Mat3& ArmatureBone::GetMatrixObjectNormal()
	{
		return mtxObjNrm_;
	}

	const Mat4& ArmatureBone::GetInverseBindMatrix()
	{
		return mtxBindInverse_;
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

	const Mat4& ArmatureBone::SetInverseBindMatrix()
	{
		return mtxBindInverse_ = glm::inverse(GetMatrixFinal());
	}

#pragma endregion Private Functions

    template Base;

}	// namespace
