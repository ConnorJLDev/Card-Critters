//------------------------------------------------------------------------------
//
// File Name:	Light.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class representing a light component.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Component.h"
#include "Light.h"
#include "Transform.h"
#include "Entity.h"

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
		  ReadKeyAmbient = "Ambient"
		, ReadKeyIndex = "Index"
		, ReadKeyColor = "Color"
		, ReadKeyAttnDist = "AttenuationDistance"
		, ReadKeyAttnAngle = "AttenuationAngle"
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

	Light::Light(void)
		: Component(TypeEnum::Light)
		, LightSource()
		, ambient_(false)
		, dirty_(true)
		, init_(0)
	{
	}

	Light::Light(const Light& other)
		: Component(other)
		, LightSource(other)
		, ambient_(other.ambient_)
		, dirty_(true)
		, init_(0)
	{
	}

	//--------------------------------------------------------------------------

	Light::~Light(void)
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

	Light* Light::Clone(void) const
	{
		return new Light(*this);
	}

	void Light::Read(StreamIn& stream)
	{
		stream.TryReadValue(ReadKeyAmbient, ambient_);
		stream.TryReadValue(ReadKeyIndex, index_);
		stream.TryReadValue(ReadKeyColor, color_);
		stream.TryReadValue(ReadKeyAttnDist, attenuationDistance_);
		stream.TryReadValue(ReadKeyAttnAngle, attenuationAngle_);
		dirty_ = true;
	}

	void Light::Write(StreamOut& stream)
	{
		stream.WriteValue(ReadKeyAmbient, ambient_);
		stream.WriteValue(ReadKeyIndex, index_);
		stream.WriteValue(ReadKeyColor, color_);
		stream.WriteValue(ReadKeyAttnDist, attenuationDistance_);
		stream.WriteValue(ReadKeyAttnAngle, attenuationAngle_);
	}

	void Light::Update(float)
	{
		static constexpr unsigned char InitCount = 8;
		bool initDirty = init_ < InitCount;

		if (initDirty)
		{
			++init_;
			dirty_ = true;
		}

		if (ambient_)
		{
			if (dirty_)
			{
				LightSource::SetAmbientLight(color_);
				dirty_ = false;
			}
			return;
		}

		Transform* transform = EntityHas(Transform);

		if (initDirty || transform->GetUpdated())
		{
			const Mat4& mtx = transform->GetMatrix();

			Vec3 scale, translation, skew;
			Vec4 perspective;
			Quat orientation;

			glm::decompose(mtx, scale, orientation, translation, skew, perspective);

			pos_ = translation;
			normal_ = orientation * Vec3(0, 0, -1);

			dirty_ = true;
		}

		if (dirty_)
		{
			UpdateGraphics();
			dirty_ = false;
		}
	}

	void Light::Render() const
	{
	}

	const Vec4& Light::GetColor() const
	{
		return color_;
	}

	void Light::SetColor(const Vec4& color)
	{
		color_ = color;
		dirty_ = true;
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
