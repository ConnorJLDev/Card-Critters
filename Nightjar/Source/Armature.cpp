//------------------------------------------------------------------------------
//
// File Name:	Armature.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Armature encapsulator.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Armature.h"
#include "ArmatureBone.h"

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

	constexpr StreamKey
		Armature::ReadKeyBones = "Bones";

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

	Armature::Armature(void)
		: NamedObject()
		, bones_()
	{
	}

	Armature::Armature(const Armature& other)
		: NamedObject(other)
		, bones_(other.bones_)
	{
		for (auto& [name, bone] : bones_)
		{
			bone = new ArmatureBone(*bone);
			bone->SetArmature(this);
		}
	}

	//--------------------------------------------------------------------------

	Armature::~Armature(void)
	{
        for (auto& [name, bone] : bones_)
        {
            SafeDeleteCheck(bone);
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

	void Armature::Read(StreamIn& stream)
	{
		ReadName(stream);

		TraceInfo("Reading armature: {}", GetName());

		if (stream.StreamHas(ReadKeyBones))
		{
			stream.ReadArrayObjects(ReadKeyBones, [this](StreamIn& stream, int index)
			{
				UNREFERENCED_PARAMETER(index);
				ArmatureBone* bone = ArmatureBone::Read(stream, this);
				if (bone != nullptr)
				{
					bones_[bone->GetName()] = bone;
				}
			});
			GetRootBone();
		}

	}

	ArmatureBone* Armature::GetBone(const string_t& name) const
	{
		auto iter = bones_.find(name);
		return iter != bones_.end() ? iter->second : nullptr;
	}

	ArmatureBone* Armature::GetRootBone() const
	{
		auto iter = std::find_if(bones_.begin(), bones_.end(), [](auto& kv) { return kv.second->GetParentTransform() == nullptr; });
		assert(iter != bones_.end()); // There should always be a bone with no parent. Otherwise, we have an infinite chain, which is invalid.
		return iter->second;
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

	template ArmatureLibrary;

	INSTANTIATE_LIBRARY_IMGUI_SELECTOR(Armature);

	IMPLEMENT_READ(Armature);

	IMPLEMENT_WRITE(Armature);

#pragma endregion Library Implementation

}	// namespace
