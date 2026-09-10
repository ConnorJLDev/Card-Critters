//------------------------------------------------------------------------------
//
// File Name:	ImGuiBehaviorCard.cpp
// Author(s):	Barend Brink (barend.brink), Connor Lariviere (connor.lariviere), Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		DEAR IMGUI system view for BehaviorCard.
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
#include "BehaviorCard.h"

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

	void BehaviorCard::DisplayIMGUI()
	{
		Behavior::DisplayIMGUI();

		ImGui::Text("Card group: %d", group_);

		if (data_ != nullptr)
		{
			data_->DisplayIMGUI();
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255));
			ImGui::Text("No card data attached!");
			ImGui::PopStyleColor();
		}
	}

#pragma endregion Public Functions

}	// namespace
