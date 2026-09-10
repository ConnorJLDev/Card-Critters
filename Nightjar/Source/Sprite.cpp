//------------------------------------------------------------------------------
//
// File Name:	Sprite.cpp
// Author(s):	Jason Nguyen (jason.nguyen), Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Sprite component implementation.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Material.h"
#include "Entity.h"
#include "Sprite.h"
#include "Mesh.h"
#include "SpriteSource.h"
#include "Font.h"
#include "Texture.h"
#include "Sampler.h"
#include "Transform.h"
#include "GraphicsSystem.h"
#include "Shader.h"
#include "Component.h"
#include "EntityContainer.h"
#include "SceneSystem.h"

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
		Sprite::ReadKeyAlpha = "Alpha",
		Sprite::ReadKeyTint = "Tint",
		Sprite::ReadKeyFrameIndex = "FrameIndex",
		Sprite::ReadKeyZLayer = "ZLayer",
		Sprite::ReadKeyText = "Text",
		Sprite::ReadKeySpriteSource = "SpriteSource",
		Sprite::ReadKeyFont = "Font",
		Sprite::ReadKeyTextAlignment = "TextAlignment";

	constexpr IVec2 Sprite::ZBounds(-20, 20);
	constexpr Vec2 Sprite::ZBoundsFactor(1.0f / static_cast<float>(ZBounds.y - ZBounds.x), -static_cast<float>(ZBounds.x) / static_cast<float>(ZBounds.y - ZBounds.x));

	//--------------------------------------------------------------------------	
	// Private Static Variables:
	//--------------------------------------------------------------------------

    const SpriteSource* Sprite::DebugSpriteSource = nullptr;

	//--------------------------------------------------------------------------	
	// Private Variables:
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------	
	// Constructors/Destructors:
	//--------------------------------------------------------------------------

#pragma region Constructors

	Sprite::Sprite()
		: Component(TypeEnum::Sprite)
		, spriteSource_(nullptr)
		, material_(GraphicsSystem::BuildMaterial("Sprite"))
		, frameIndex_(0)
		, tint_(1, 1, 1, 1)
		, zLayer_(5)
        , dirty_(true)
	{
	}

	Sprite::Sprite(const Sprite& other)
		: Component(other)
		, spriteSource_(other.spriteSource_)
		, material_(GraphicsSystem::BuildMaterial(other.material_))
		, frameIndex_(other.frameIndex_)
		, tint_(other.tint_)
		, zLayer_(other.zLayer_)
        , dirty_(true)
	{
	}

	//--------------------------------------------------------------------------

	Sprite::~Sprite()
	{
		GraphicsSystem::DestroyMaterial(material_);
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

	Sprite* Sprite::Clone(void) const
	{
		return new Sprite(*this);
	}

	void Sprite::Read(StreamIn& stream)
	{
		stream.TryReadValue(ReadKeyTint, tint_);
		stream.TryReadValue(ReadKeyAlpha, tint_.a);
		stream.TryReadValue(ReadKeyFrameIndex, frameIndex_);
		stream.TryReadValue(ReadKeyZLayer, zLayer_);
		stream.TryReadValue(ReadKeySpriteSource, spriteSource_);
	}

	void Sprite::Write(StreamOut& stream)
	{
		stream.WriteValue(ReadKeySpriteSource, spriteSource_);
		stream.WriteValue(ReadKeyFrameIndex, frameIndex_);
		stream.WriteValue(ReadKeyTint, tint_);
		stream.WriteValue(ReadKeyZLayer, zLayer_);
	}

	void Sprite::Render() const
	{
		Transform* transform = EntityHas(Transform);
		assert(transform != nullptr);

		const SpriteSource* ss = spriteSource_;
		if (ss == nullptr)
		{
			if (DebugSpriteSource == nullptr)
			{
				DebugSpriteSource = SpriteSourceLibrary::Get_S("DEBUG");
			}
			ss = DebugSpriteSource;
            dirty_ = true;
		}
        const Mesh* mesh = ss->GetMesh();
        if (dirty_)
        {
            Vec2 uv;
            ss->GetFrameUV(frameIndex_, uv);

            const Texture* texture = ss->GetTexture();
            const Sampler* sampler = ss->GetSampler();

            material_->SetTextureSampler("inTexture", texture, sampler);
            material_->SetUniform("colorTint", tint_);
            material_->SetUniform("texCoordScale", ss->GetFrameUVSize());
            material_->SetUniform("texCoordOffset", uv);
            material_->SetUniform("alphaClip", 0.01f);
            material_->SetUniform("zLayer", static_cast<float>(zLayer_) * ZBoundsFactor.x + ZBoundsFactor.y);
            dirty_ = false;
        }

		GraphicsSystem::DrawCall(transform->GetMatrix(), *mesh, *material_);
	}

	float Sprite::GetAlpha() const
	{
		return tint_.a;
	}

	void Sprite::SetAlpha(float newAlpha)
	{
		tint_.a = MathClamp(newAlpha, 0.0f, 1.0f);
        dirty_ = true;
	}

	const Vec4& Sprite::GetTint() const
	{
		return tint_;
	}

	void Sprite::SetTint(const Vec4& color)
	{
		tint_ = color;
        dirty_ = true;
	}

	void Sprite::SetTint(const Vec3& color)
	{
		tint_ = Vec4(color, tint_.a);
	}

	unsigned int Sprite::GetFrameIndex() const
	{
		return frameIndex_;
	}

	void Sprite::SetFrameIndex(unsigned int newFrameIndex)
	{
		if (spriteSource_ != nullptr)
		{
			unsigned int frameCount = spriteSource_->GetFrameCount();
			if (newFrameIndex < frameCount)
			{
				frameIndex_ = newFrameIndex;
			}
			else
			{
				frameIndex_ = frameCount > 0 ? frameCount - 1 : 0;
				TraceWarning("Sprite::SetFrameIndex: frame index adjusted to {}", frameIndex_);
			}
		}
		else
		{
			frameIndex_ = newFrameIndex;
		}

		TraceInfo("SpriteSetFrame: frame index = {}", frameIndex_);
        dirty_ = true;
	}

	const SpriteSource* Sprite::GetSpriteSource() const
	{
		return spriteSource_;
	}

	void Sprite::SetSpriteSource(const string_t& spriteSourceName)
	{
		SetSpriteSource(SpriteSourceLibrary::Get_S(spriteSourceName));
        dirty_ = true;
	}

	void Sprite::SetSpriteSource(const SpriteSource* spriteSource)
	{
		spriteSource_ = spriteSource;
        dirty_ = true;
	}

	int Sprite::GetZlayer() const
	{
		return zLayer_;
	}

	void Sprite::SetZLayer(int layer)
	{
		zLayer_ = layer;
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
