//------------------------------------------------------------------------------
//
// File Name:	ImGuiSpriteSource.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		DEAR IMGUI system view for SpriteSource.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiInclude.h"

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

	void SpriteSource::DisplayIMGUI()
	{
		NamedObject::DisplayIMGUI(true);

		if (texture_ != nullptr)
		{
			IVec2 texSize = texture_->GetDimensions();

			ImGui::Text("Size: ");
			ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ValueColor);
			ImGui::SameLine();
			ImGui::Text("%d x %d", texSize.x, texSize.y);
			ImGui::PopStyleColor();

			ImGui::Image(texture_, { 256.0f, 256.0f });
		}
		else
		{
			ImGui::Text("No texture.");
		}
	}

#pragma endregion Public Functions

}	// namespace
