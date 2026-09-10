//------------------------------------------------------------------------------
//
// File Name:	AnimationLink.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Interface between an animation component and other components.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "AnimationLink.h"
#include "Component.h"
#include "Entity.h"
#include "AnimationLinkTransform.h"
#include "AnimationLinkSprite.h"
#include "AnimationLinkMaterial.h"
#include "AnimationLinkModel.h"

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
	// Protected Constants:
	//--------------------------------------------------------------------------

	constexpr StreamKey
		  AnimationLink::ReadKeyType = "Type"
		, AnimationLink::ReadKeyFlags = "Flags"
		, AnimationLink::ReadKeyCurves = "Curves"
		;

	constexpr float AnimationLink::InvalidValue = std::nanf("");

#define KEYMAP(key) {#key, [](){ return new AnimationLink##key(); }}
    const STATIC_MAP_CLASS((string_t, std::function<AnimationLink* ()>), AnimationLink, KeysType,
        FOR_EACH_0_1_1_C(KEYMAP,
            Transform,
            Sprite,
            Material,
            Model
        )
    )
#undef KEYMAP

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

	AnimationLink::AnimationLink()
	{
	}

	//--------------------------------------------------------------------------

	AnimationLink::~AnimationLink()
	{
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

	AnimationLink* AnimationLink::Read(StreamIn& stream)
	{
		if (!stream.StreamHas(ReadKeyType))
		{
			TraceWarning("Failed to read animation link type!");
			return nullptr;
		}

		string_t typeName = stream.ReadString(ReadKeyType);
		const auto& map = KeysType();
		const auto type = map.find(typeName);

		if (type == map.end())
		{
			TraceWarning("Unexpected animation link type: {}", typeName.c_str());
			return nullptr;
		}

		AnimationLink* link = type->second();

		link->Read_(stream);

		return link;
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

#pragma region Protected Functions

	void AnimationLink::ElementData::Invalidate()
	{
		constValue = InvalidValue;
	}

	void AnimationLink::Read_(StreamIn& stream)
	{
		vector_t<float> curveIndices;
		if (stream.StreamHas(ReadKeyCurves))
		{
			stream.ReadVector(ReadKeyCurves, curveIndices);
		}
		unsigned curveValuesSize = OnRead(stream, curveIndices);
		if (curveValuesSize < curveIndices.size())
		{
			TraceWarning("Not enough curves read! {}/{}", curveValuesSize, curveIndices.size());
		}
		else if (curveValuesSize > curveIndices.size())
		{
			TraceWarning("Too many curves read! {}/{}", curveValuesSize, curveIndices.size());
		}
	}

#pragma endregion Protected Functions

}	// namespace