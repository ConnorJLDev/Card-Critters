//------------------------------------------------------------------------------
//
// File Name:	ImGuiSprite.cpp
// Author(s):	Barend Brink (barend.brink), Connor Lariviere (connor.lariviere), Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		DEAR IMGUI system view for Sprite.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiInclude.h"

#include "Sprite.h"
#include "Texture.h"
#include "SpriteSource.h"

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

	void Sprite::DisplayIMGUI()
	{
        dirty_ |= SpriteSourceLibrary::DisplayIMGUISelector_S("Spritesource", spriteSource_);

		if (spriteSource_ != nullptr)
		{
            const Texture* texture = spriteSource_->GetTexture();
            IVec2 texSize = texture->GetDimensions();
            ImGui::LabelText("###SPRITE DATA", "Sprite Data", sizeof("SPRITE DATA"));
            ImGui::Text("Size: ");
            ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ValueColor);
            ImGui::SameLine();
            ImGui::Text("%d x %d", texSize.x, texSize.y);
            ImGui::PopStyleColor();
            //ImGui::Separator();
            //frame index of the sprites animation, may be inaccurate max frame count due to empty frames being counted
            ImGui::Text("Frame Index: ");
            ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ValueColor);
            ImGui::SameLine();
            ImGui::Text("%d/%d", GetFrameIndex() + 1, spriteSource_->GetFrameCount());
            ImGui::PopStyleColor();
            static constexpr Vec2 texPreviewSize(256.0f, 256.0f);
            ImGui::Image(texture, texPreviewSize);
            ImGui::Separator();
		}

        dirty_ |= ImGui::SliderInt("Zlayer", &zLayer_, Sprite::ZBounds.x, Sprite::ZBounds.y, "%d");

        dirty_ |= ImGui::ColorEdit4("Tint Color", &tint_.x);
	}

#pragma endregion Public Functions

}	// namespace
