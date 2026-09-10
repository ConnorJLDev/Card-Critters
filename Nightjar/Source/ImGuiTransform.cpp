//------------------------------------------------------------------------------
//
// File Name:	ImGuiTransform.cpp
// Author(s):	Barend Brink (barend.brink), Connor Lariviere (connor.lariviere), Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		DEAR IMGUI system view for Transform.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiInclude.h"

#include "Transform.h"

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

	void Transform::DisplayIMGUI()
	{
		{ // World transform
			ImGui::Text("World Transform");
			{
				Vec2 worldTranslation = GetTranslationWorld();
				ImGui::Text("Translation: ");
				ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ValueColor);
				ImGui::SameLine();
				ImGui::Text("%.1f, %.1f", worldTranslation.x, worldTranslation.y);
				ImGui::PopStyleColor();
			}
			{
				float worldRotation = GetRotationWorld();
				ImGui::Text("Rotation: ");
				ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ValueColor);
				ImGui::SameLine();
				ImGui::Text("%.1f", worldRotation);
				ImGui::PopStyleColor();
			}
			{
				Vec2 worldScale = GetScaleWorld();
				ImGui::Text("Scale: ");
				ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ValueColor);
				ImGui::SameLine();
				ImGui::Text("%.1f, %.1f", worldScale.x, worldScale.y);
				ImGui::PopStyleColor();
			}
		}
		ImGui::Checkbox("3D", &mode3D_);
		if (mode3D_)
		{
			DisplayIMGUI3D();
		}
		else
		{
			DisplayIMGUI2D();
		}
        if (IsDirty())
        {
            SetChildrenFinalDirty();
        }
	}

#pragma endregion Public Functions

}	// namespace
