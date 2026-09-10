//------------------------------------------------------------------------------
//
// File Name:	ImGuiBehaviorCharacter.cpp
// Author(s):	Barend Brink (barend.brink), Connor Lariviere (connor.lariviere), Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		DEAR IMGUI system view for BehaviorCharacter.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiInclude.h"

#include "Behavior.h"
#include "BehaviorCharacter.h"
#include "DataCard.h"

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

	void BehaviorCharacter::DisplayIMGUI()
	{
		BehaviorHealth::DisplayIMGUI();

		ImGui::Text("Card Count:");
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
		ImGui::Text("%d", cards_.size());
		ImGui::PopStyleColor();
		for (auto& card : cards_)
		{
			card.DisplayIMGUI();
		}
	}

#pragma endregion Public Functions

}	// namespace
