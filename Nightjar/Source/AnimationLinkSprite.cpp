//------------------------------------------------------------------------------
//
// File Name:	AnimationLinkSprite.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Interface between an animation component and sprite components.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "AnimationLinkSprite.h"
#include "Component.h"
#include "Entity.h"
#include "Sprite.h"
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

	ENUM_MAPS_CLASS(AnimationLinkSprite, Options,
		FrameIndexExists,
		RedExists,
		GreenExists,
		BlueExists,
		AlphaExists,
		FrameIndexConst,
		RedConst,
		GreenConst,
		BlueConst,
		AlphaConst
	);

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

	AnimationLinkSprite::AnimationLinkSprite()
		: AnimationLink()
		, options_(Options::None)
		, elementData_()
	{
	}

	//--------------------------------------------------------------------------

	AnimationLinkSprite::~AnimationLinkSprite()
	{
	}

#pragma endregion Constructors

	//--------------------------------------------------------------------------
	// Public Static Functions:
	//--------------------------------------------------------------------------

#pragma region Public Static Functions

	ENUM_IMPLEMENT_OPS(AnimationLinkSprite::Options);

#pragma endregion Public Static Functions

	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

	void AnimationLinkSprite::Start(Entity*)
	{
	}

	void AnimationLinkSprite::Update(Entity* entity, const vector_t<float>& evaluated)
	{
		Sprite* sprite = entity->Has(Sprite);
		if (sprite == nullptr)
		{
			TraceWarning("Entity doesn't have Sprite!");
			return;
		}
#define ANIM_VALUE_OF(enumIndex) evaluated[elementData_[(unsigned int)Indices::enumIndex].curveIndex]
		if ((bool)(options_ & Options::FrameIndexExists))
		{
			sprite->SetFrameIndex(static_cast<unsigned int>(ANIM_VALUE_OF(FrameIndex)));
		}
		if ((bool)(options_ & (Options::RedExists | Options::GreenExists | Options::BlueExists | Options::AlphaExists)))
		{
			Vec4 tint = sprite->GetTint();
#define SET_COMPONENT(eName, compName) if ((bool)(options_ & Options::eName##Exists)) { tint.compName = ANIM_VALUE_OF(eName); }
			SET_COMPONENT(Red, r);
			SET_COMPONENT(Green, g);
			SET_COMPONENT(Blue, b);
			SET_COMPONENT(Alpha, a);
#undef SET_COMPONENT
			sprite->SetTint(tint);
		}
	}

	void AnimationLinkSprite::Update(Particle*, const vector_t<float>&)
	{
		assert(false);
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

	unsigned AnimationLinkSprite::OnRead(StreamIn& stream, const vector_t<float>& curveIndices)
	{
		stream.TryReadEnumFlags(ReadKeyFlags, ENUM_READ(Options), options_);

		unsigned curveCount = 0;
#define INDEX_COUNT(eName)\
		if (!(bool)(options_ & Options::eName##Exists)) { elementData_[(unsigned int)Indices::eName].Invalidate(); }\
		else if ((bool)(options_ & Options::eName##Const)) { elementData_[(unsigned int)Indices::eName].constValue = curveIndices[curveCount++]; }\
		else { elementData_[(unsigned int)Indices::eName].curveIndex = static_cast<unsigned>(curveIndices[curveCount++]); }
		INDEX_COUNT(FrameIndex);
		INDEX_COUNT(Red);
		INDEX_COUNT(Green);
		INDEX_COUNT(Blue);
		INDEX_COUNT(Alpha);
#undef INDEX_COUNT
		return curveCount;
	}

#pragma endregion Private Functions

}	// namespace