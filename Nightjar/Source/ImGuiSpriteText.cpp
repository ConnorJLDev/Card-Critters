//------------------------------------------------------------------------------
//
// File Name:	ImGuiSpriteText.cpp
// Author(s):	Barend Brink (barend.brink), Connor Lariviere (connor.lariviere), Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		DEAR IMGUI system view for SpriteText.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiInclude.h"

#include "SpriteText.h"
#include "Font.h"

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

	void SpriteText::DisplayIMGUI(void)
	{
        static constexpr ImGuiInputTextFlags flags = ImGuiInputTextFlags_None /*| ImGuiInputTextFlags_CallbackCompletion*/;
        if (ImGui::InputTextMultiline("Text", &text_, Vec2(), flags))
        {
            UpdateTextMeta();
            UpdateTextWidth();
        }

        dirty_ |= ImGui::SliderInt("Zlayer", &zLayer_, ZBounds.x, ZBounds.y, "%d");

        dirty_ |= ImGui::ColorEdit4("Tint Color", &tint_.x);
        dirty_ |= ImGui::ColorEdit4("Border Color", &textBorderColor_.x);
        dirty_ |= ImGui::DragFloat("Border Strength", &textBorderStrength_, 0.1f);
		ImGui::Separator();

        if (ImGui::DragFloat("Alignment", &textAlignment_, 0.05f, -1.0f, 1.0f))
        {
            UpdateTextWidth();
        }

        const Font* font = font_;
        if (FontLibrary::DisplayIMGUISelector_S("Font", font))
        {
            SetFont(font);
        }
	}

#pragma endregion Public Functions

}	// namespace
