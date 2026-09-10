//------------------------------------------------------------------------------
//
// File Name:	Transform.cpp
// Author(s):	Andrew Lamb (andrew.lamb)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Transform component.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Transform.h"
#include "Entity.h"

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

	static constexpr StreamKey ReadKeyMode3D = "3D";

	typedef TransformBase<Component, Transform> Base;

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

	Transform::Transform()
		: Base(TypeEnum::Transform)
		, mode3D_(false)
        , updated_(true)
        , updatedLast_(true)
	{
	}

	Transform::Transform(const Transform& other)
		: Base(other)
		, mode3D_(false)
        , updated_(true)
        , updatedLast_(true)
	{
	}

	//--------------------------------------------------------------------------

	Transform::~Transform()
	{
		for (auto& child : children_)
		{
			child->parent_ = parent_;
		}
		if (parent_ != nullptr)
		{
			parent_->RemoveChild(this);
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

	Transform* Transform::Clone() const
	{
		return new Transform(*this);
	}

	void Transform::Read(StreamIn& stream)
	{
		stream.TryReadValue(ReadKeyMode3D, mode3D_);
		if (mode3D_)
		{
			stream.TryReadValue(ReadKeyTranslation, translation3_);
			stream.TryReadValue(ReadKeyRotation, rotation3_);
			stream.TryReadValue(ReadKeyScale, scale3_);
		}
		else
		{
			stream.TryReadValue(ReadKeyTranslation, "Position", translation2_);
			stream.TryReadValue(ReadKeyRotation, rotation2_);
			stream.TryReadValue(ReadKeyScale, scale2_);
		}
		string_t parentName;
		if (stream.TryReadValue(ReadKeyParent, parentName) && !parentName.empty())
		{
			Entity* componentEntity = GetParent();
			Entity* parentEntity = componentEntity->FindEntity(parentName);
			parent_ = parentEntity->Has(Transform);
			if (parent_ == nullptr)
			{
				TraceWarning("Failed to find parent \"{}\" for transform of \"{}\"", parentName, componentEntity->GetName());
			}
		}

		dirtyFlags_ = DirtyFlags::All;
	}

	void Transform::Write(StreamOut& stream)
	{
		if (mode3D_)
		{
			stream.WriteValue(ReadKeyMode3D, mode3D_);
			stream.WriteValue(ReadKeyTranslation, translation3_);
			stream.WriteValue(ReadKeyRotation, rotation3_);
			stream.WriteValue(ReadKeyScale, scale3_);
		}
		else
		{
			stream.WriteValue(ReadKeyTranslation, translation2_);
			stream.WriteValue(ReadKeyRotation, rotation2_);
			stream.WriteValue(ReadKeyScale, scale2_);
		}
		stream.WriteValue(ReadKeyParent, parent_ == nullptr ? string_t() : parent_->GetParent()->GetName());
	}

	void Transform::Update(float)
	{
		updatedLast_ = updated_;
		updated_ = false;
	}

	const Mat4& Transform::GetMatrix()
	{
		updated_ = finalDirty_ || (bool)dirtyFlags_;
		return GetMatrixFinal();
	}

	const Vec2& Transform::GetTranslation() const
	{
		return translation2_;
	}

	Vec2 Transform::GetTranslationWorld() const
	{
		Vec4 transformTemp{ translation2_, 0, 1 };
		transformTemp = GetParentMatrix() * transformTemp;
		return Vec2{ transformTemp };
	}

	void Transform::SetTranslation(const Vec2& translation)
	{
        SetTranslation2D(translation);
	}

	float Transform::GetRotation() const
	{
		return glm::degrees(rotation2_);
	}

	float Transform::GetRotationWorld() const
	{
		return glm::degrees(parent_ != nullptr ? glm::radians(parent_->GetRotationWorld()) + rotation2_ : rotation2_);
	}

	void Transform::SetRotation(float rotation)
	{
        SetRotation2D(rotation);
	}

	const Vec2& Transform::GetScale() const
	{
		return scale2_;
	}

	Vec2 Transform::GetScaleWorld() const
	{
		return parent_ != nullptr ? parent_->GetScaleWorld() * scale2_ : scale2_;
	}

	void Transform::SetScale(const Vec2& scale)
	{
        SetScale2D(scale);
	}

	bool Transform::IsChild(const Transform* other) const
	{
		const Transform* parent = other->parent_;
		while (parent != nullptr && parent != this)
		{
			parent = parent->parent_;
		}
		return parent != nullptr;
	}

	Transform* Transform::FindChild(string_view_arg name) const
	{
		/*size_t subnameSize = std::min(name.find('/'), name.size());
		string_view_t subname(name.data(), subnameSize);
		ChildIter_t childIter = std::find_if(children_.begin(), children_.end(), [subname](Transform* child) { return child->EntityNamed(subname); });
		if (childIter != children_.end())
		{
			Transform* child = *childIter;
			return subnameSize == name.size() ? child : child->FindChild(string_view_t(name.data() + (subnameSize + 1), name.size() - (subnameSize + 1)));
		}*/

		// Breadth-first search
		// Immediate children
		for (auto& child : children_)
		{
			if (child->EntityNamed(name))
			{
				return child;
			}
		}
		// Recursion
		for (auto& child : children_)
		{
			Transform* found = child->FindChild(name);
			if (found != nullptr)
			{
				return found;
			}
		}
		return nullptr;
	}

	bool Transform::GetUpdated() const
	{
		return updatedLast_ || updated_ || finalDirty_ || (bool)dirtyFlags_;
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

    template Base;

}	// namespace
