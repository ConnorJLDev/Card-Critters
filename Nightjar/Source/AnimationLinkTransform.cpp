//------------------------------------------------------------------------------
//
// File Name:	AnimationLinkTransform.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Interface between an animation component and transform components.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "AnimationLinkTransform.h"
#include "ArmatureBone.h"
#include "Component.h"
#include "Entity.h"
#include "Transform.h"
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

	ENUM_MAPS_CLASS(AnimationLinkTransform, Options, 
		TranslationExists,
		RotationExists,
		ScaleExists,
		TranslationIdentity,
		RotationIdentity,
		ScaleIdentity,
		TranslationUniform,
		RotationUniform,
		ScaleUniform,
		TranslationConst,
		RotationConst,
		ScaleConst,
		TranslationXExists,
		TranslationYExists,
		TranslationZExists,
		RotationXExists,
		RotationYExists,
		RotationZExists,
		ScaleXExists,
		ScaleYExists,
		ScaleZExists,
		TranslationXConst,
		TranslationYConst,
		TranslationZConst,
		RotationXConst,
		RotationYConst,
		RotationZConst,
		ScaleXConst,
		ScaleYConst,
		ScaleZConst
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

	AnimationLinkTransform::AnimationLinkTransform()
		: AnimationLink()
		, data_()
	{
	}

	//--------------------------------------------------------------------------

	AnimationLinkTransform::~AnimationLinkTransform()
	{
	}

#pragma endregion Constructors

	//--------------------------------------------------------------------------
	// Public Static Functions:
	//--------------------------------------------------------------------------

#pragma region Public Static Functions

	ENUM_IMPLEMENT_OPS(AnimationLinkTransform::Options);

#pragma endregion Public Static Functions

	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

	void AnimationLinkTransform::Start(Entity* entity)
	{
		Transform* transform = entity->Has(Transform);
		if (transform == nullptr)
		{
			TraceWarning("Entity doesn't have Transform!");
			return;
		}
		data_.Start(*transform);
	}

	void AnimationLinkTransform::Update(Entity* entity, const vector_t<float>& evaluated)
	{
		Transform* transform = entity->Has(Transform);
		if (transform == nullptr)
		{
			TraceWarning("Entity doesn't have Transform!");
			return;
		}
		Apply(*transform, evaluated);

	}

	void AnimationLinkTransform::Update(Particle* particle, const vector_t<float>& evaluated)
	{
		Apply(static_cast<TransformComponents&>(*particle), evaluated);
	}

#pragma endregion Public Functions

	//--------------------------------------------------------------------------
	// Private Static Functions:
	//--------------------------------------------------------------------------

#pragma region Private Static Functions

    template <typename T>
    void AnimationLinkTransform::Apply(T& transform, const vector_t<float>& evaluated) const
    {
        data_.Apply(transform, evaluated);
    }

    template <typename T>
    void AnimationLinkTransform::TransformData::Start(T& transform) const
    {
#define INIT_VALUE_OF_CURVE(enumIndex, axis) ((bool)(options & Options::enumIndex##Const) ? elementData[(unsigned int)Indices::enumIndex].constValue : orig.axis)
#define INIT_VALUE_OF_EVAL(enumIndex) INIT_VALUE_OF_CURVE(enumIndex##X, x), INIT_VALUE_OF_CURVE(enumIndex##Y, y), INIT_VALUE_OF_CURVE(enumIndex##Z, z)
#define INIT_VALUE_OF_TFM(tfm)\
		if ((bool)(options & Options::tfm##Exists))\
		{\
			const Vec3& orig(transform.Get##tfm##3D());\
			Vec3 eval(INIT_VALUE_OF_EVAL(tfm));\
			transform.Set##tfm##3D(eval);\
		}\

        INIT_VALUE_OF_TFM(Scale);
        INIT_VALUE_OF_TFM(Rotation);
        INIT_VALUE_OF_TFM(Translation);
#undef INIT_VALUE_OF_TFM
#undef INIT_VALUE_OF_EVAL
#undef INIT_VALUE_OF_CURVE
    }

    template <typename T>
    void AnimationLinkTransform::TransformData::Apply(T& transform, const vector_t<float>& evaluated) const
    {
#define ANIM_VALUE_OF_CURVE(enumIndex, axis) ((bool)(options & Options::enumIndex##Const) ? orig.axis : evaluated[elementData[(unsigned int)Indices::enumIndex].curveIndex])
#define ANIM_VALUE_OF_EVAL(enumIndex) ANIM_VALUE_OF_CURVE(enumIndex##X, x), ANIM_VALUE_OF_CURVE(enumIndex##Y, y), ANIM_VALUE_OF_CURVE(enumIndex##Z, z)
#define ANIM_VALUE_OF_TFM(tfm)\
		if ((bool)(options & Options::tfm##Exists) && !(bool)(options & Options::tfm##Const))\
		{\
			const Vec3& orig(transform.Get##tfm##3D());\
			Vec3 eval(ANIM_VALUE_OF_EVAL(tfm));\
			transform.Set##tfm##3D(eval);\
		}\

        ANIM_VALUE_OF_TFM(Scale);
        ANIM_VALUE_OF_TFM(Rotation);
        ANIM_VALUE_OF_TFM(Translation);
#undef ANIM_VALUE_OF_TFM
#undef ANIM_VALUE_OF_EVAL
#undef ANIM_VALUE_OF_CURVE
    }

    template void AnimationLinkTransform::TransformData::Start<ArmatureBone>(ArmatureBone&) const;
    template void AnimationLinkTransform::TransformData::Apply<ArmatureBone>(ArmatureBone&, const vector_t<float>&) const;

#pragma endregion Private Static Functions

	//--------------------------------------------------------------------------
	// Private Functions:
	//--------------------------------------------------------------------------

#pragma region Private Functions

	void AnimationLinkTransform::TransformData::Read(StreamIn& stream, const vector_t<float>& curveIndices, unsigned& curveCount)
	{
		stream.TryReadEnumFlags(ReadKeyFlags, ENUM_READ(Options), options);
#define CONST_CHECK(component, axis, index)\
		if ((bool)(options & Options::component##axis##Const)) { elementData[(unsigned int)Indices::component##axis].constValue = curveIndices[index]; }\
		else { elementData[(unsigned int)Indices::component##axis].curveIndex = (unsigned int)curveIndices[##index]; }
#define INDEX_COUNT(component, constVal)\
		if ((bool)(options & Options::component##XConst) && (bool)(options & Options::component##YConst) && (bool)(options & Options::component##ZConst))\
		{\
			options |= Options::component##Const;\
		}\
		else if ((bool)(options & Options::component##Const))\
		{\
			options |= Options::component##XConst | Options::component##YConst | Options::component##ZConst;\
		}\
		else if ((bool)(options & Options::component##Identity))\
		{\
			options |= Options::component##Const | Options::component##XConst | Options::component##YConst | Options::component##ZConst;\
		}\
		if (!(bool)(options & Options::component##Exists))\
		{\
			elementData[(unsigned int)Indices::component##X].Invalidate();\
			elementData[(unsigned int)Indices::component##Y].Invalidate();\
			elementData[(unsigned int)Indices::component##Z].Invalidate();\
		}\
		else if ((bool)(options & Options::component##Identity))\
		{\
			elementData[(unsigned int)Indices::component##X].constValue = constVal;\
			elementData[(unsigned int)Indices::component##Y].constValue = constVal;\
			elementData[(unsigned int)Indices::component##Z].constValue = constVal;\
		}\
		else if ((bool)(options & Options::component##Uniform))\
		{\
			CONST_CHECK(component, X, curveCount);\
			CONST_CHECK(component, Y, curveCount);\
			CONST_CHECK(component, Z, curveCount++);\
		}\
		else\
		{\
			CONST_CHECK(component, X, curveCount++);\
			CONST_CHECK(component, Y, curveCount++);\
			CONST_CHECK(component, Z, curveCount++);\
		}
		INDEX_COUNT(Scale, 1.0f);
		INDEX_COUNT(Rotation, 0.0f);
		INDEX_COUNT(Translation, 0.0f);
#undef INDEX_COUNT
	}

	unsigned AnimationLinkTransform::OnRead(StreamIn& stream, const vector_t<float>& curveIndices)
	{
		unsigned curveCount = 0;

		data_.Read(stream, curveIndices, curveCount);

		return curveCount;
	}

#pragma endregion Private Functions

}	// namespace