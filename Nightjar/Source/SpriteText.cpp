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
#include "SpriteText.h"
#include "TextMesh.h"
#include "SpriteSource.h"
#include "Font.h"
#include "Texture.h"
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
		  SpriteText::ReadKeyAlpha = "Alpha"
		, SpriteText::ReadKeyTint = "Tint"
		, SpriteText::ReadKeyZLayer = "ZLayer"
		, SpriteText::ReadKeyText = "Text"
		, SpriteText::ReadKeyFont = "Font"
		, SpriteText::ReadKeyTextAlignment = "TextAlignment"
		, SpriteText::ReadKeyBorderColor = "BorderColor"
		, SpriteText::ReadKeyBorderStrength = "BorderStrength"
        ;

	constexpr IVec2 SpriteText::ZBounds(-20, 20);
	constexpr Vec2 SpriteText::ZBoundsFactor(1.0f / static_cast<float>(ZBounds.y - ZBounds.x), -static_cast<float>(ZBounds.x) / static_cast<float>(ZBounds.y - ZBounds.x));

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

	SpriteText::SpriteText()
		: Component(TypeEnum::SpriteText)
		, font_(nullptr)
		, material_(nullptr)
		, textMesh_(new TextMesh("SpriteText", ""))
		, text_()
		, tint_(1, 1, 1, 1)
		, textBorderColor_(0, 0, 0, 1)
		, textBorderStrength_(5.0f)
		, textWidth_()
		, zLayer_(5)
		, textAlignment_(0.0f)
        , dirty_(true)
	{
	}

	SpriteText::SpriteText(const SpriteText& other)
		: Component(other)
		, font_(other.font_)
		, material_(GraphicsSystem::BuildMaterial(other.material_))
		, textMesh_(new TextMesh("SpriteText", other.text_))
		, text_(other.text_)
		, tint_(other.tint_)
		, textBorderColor_(other.textBorderColor_)
		, textBorderStrength_(other.textBorderStrength_)
		, textWidth_(other.textWidth_)
		, zLayer_(other.zLayer_)
		, textAlignment_(other.textAlignment_)
        , dirty_(true)
	{
	}

	//--------------------------------------------------------------------------

	SpriteText::~SpriteText()
	{
		GraphicsSystem::DestroyMaterial(material_);
        SafeDeleteCheck(textMesh_);
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

	SpriteText* SpriteText::Clone(void) const
	{
		return new SpriteText(*this);
	}

	void SpriteText::Read(StreamIn& stream)
	{
		stream.TryReadValue(ReadKeyTint, tint_);
		stream.TryReadValue(ReadKeyAlpha, tint_.a);
        stream.TryReadValue(ReadKeyBorderColor, textBorderColor_);
        stream.TryReadValue(ReadKeyBorderStrength, textBorderStrength_);
		stream.TryReadValue(ReadKeyZLayer, zLayer_);
		if (stream.TryReadValue(ReadKeyText, text_))
		{
			textMesh_->SetText(text_);
			UpdateTextWidth();
		}
		if (stream.TryReadValue(ReadKeyFont, font_))
		{
			SetFont(font_);
		}
		stream.TryReadValue(ReadKeyTextAlignment, textAlignment_);
        dirty_ = true;
	}

	void SpriteText::Write(StreamOut& stream)
	{
		stream.WriteValue(ReadKeyTint, tint_);
        stream.WriteValue(ReadKeyBorderColor, textBorderColor_);
        stream.WriteValue(ReadKeyBorderStrength, textBorderStrength_);
		stream.WriteValue(ReadKeyZLayer, zLayer_);
		stream.WriteValue(ReadKeyFont, font_);
		stream.WriteValue(ReadKeyText, text_);
		stream.WriteValue(ReadKeyTextAlignment, textAlignment_);
	}

	void SpriteText::Update(float)
	{
	}

	void SpriteText::Render() const
	{
        assert(textMesh_ != nullptr);

        assert((font_ == nullptr && material_ == nullptr) || (font_ != nullptr && material_ != nullptr));

        if (text_.empty() || (font_ == nullptr && material_ == nullptr))
        {
            return;
        }

		//auto* mat = font_->tss_->material;
		auto* mat = material_;

        if (dirty_)
        {
            mat->SetUniform("textColor", tint_);
            mat->SetUniform("borderStrength", textBorderStrength_);
            mat->SetUniform("borderColor", textBorderColor_);
            mat->SetUniform("zLayer", static_cast<float>(zLayer_) * ZBoundsFactor.x + ZBoundsFactor.y);
            dirty_ = false;
        }

        Transform* transform = EntityHas(Transform);
        assert(transform != nullptr);

		Mat4 tfm = transform->GetMatrix();

		tfm = glm::translate(tfm, Vec3(textWidth_, 0, 0));

		GraphicsSystem::DrawCall(tfm, *textMesh_, *mat);
	}

	float SpriteText::GetAlpha() const
	{
		return tint_.a;
	}

	void SpriteText::SetAlpha(float newAlpha)
	{
		tint_.a = MathClamp(newAlpha, 0.0f, 1.0f);
        dirty_ = true;
	}

	const Vec4& SpriteText::GetTint() const
	{
		return tint_;
	}

	void SpriteText::SetTint(const Vec4& color)
	{
		tint_ = color;
        dirty_ = true;
	}

	void SpriteText::SetTint(const Vec3& color)
	{
		tint_ = Vec4(color, tint_.a);
        dirty_ = true;
	}

	const Font* SpriteText::GetFont()
	{
		return font_;
	}

	void SpriteText::SetFont(const string_t& fontName)
	{
		SetFont(FontLibrary::Get_S(fontName));
	}

	void SpriteText::SetFont(const Font* font)
	{
		font_ = font;
        if (material_ != nullptr)
        {
            GraphicsSystem::DestroyMaterial(material_);
        }
		material_ = font_ == nullptr ? nullptr : GraphicsSystem::BuildMaterial(font_->tss_->material);
        dirty_ = true;
	}

	const string_t& SpriteText::GetText() const
	{
		return text_;
	}

	void SpriteText::SetText(string_view_arg newText)
	{
		text_ = newText;
        UpdateTextMeta();
        UpdateTextWidth();
	}

	float SpriteText::GetTextBorderStrength(void) const
	{
		return textBorderStrength_;
	}

	void SpriteText::SetTextBorderStrength(float strength)
	{
		textBorderStrength_ = strength;
        dirty_ = true;
	}

	const Vec4& SpriteText::GetTextBorderColor(void) const
	{
		return textBorderColor_;
	}

	void SpriteText::SetTextBorderColor(const Vec4& color)
	{
		textBorderColor_ = color;
        dirty_ = true;
	}

	int SpriteText::GetZlayer() const
	{
		return zLayer_;
	}

	void SpriteText::SetZLayer(int layer)
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

    void SpriteText::UpdateTextMeta(void)
    {
        textMesh_->SetText(text_);
    }

    void SpriteText::UpdateTextWidth(void)
	{
		size_t strStart = 0;
		size_t strLen = text_.find('\n', strStart);
		size_t maxLen = 0;
		while (strLen != string_t::npos)
		{
			maxLen = MathMax(strLen - strStart, maxLen);
			strStart = strLen + 1;
			strLen = text_.find('\n', strStart);
		}
		maxLen = MathMax(text_.size() - strStart, maxLen);
		float align = (textAlignment_ - 1.0f) * 0.5f;
		textWidth_ = ((float)maxLen - (1.0f - MathAbs(textAlignment_))) * 0.1f * align;
	}

#pragma endregion Private Functions

}	// namespace
