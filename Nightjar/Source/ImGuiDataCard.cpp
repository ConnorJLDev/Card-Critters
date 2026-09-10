//------------------------------------------------------------------------------
//
// File Name:	ImGuiCard.cpp
// Author(s):	Barend Brink (barend.brink), Connor Lariviere (connor.lariviere), Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		DEAR IMGUI system view for Card.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiInclude.h"

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

	void DataCard::DisplayIMGUI()
	{
		FORMAT_MAKE(suffix, "##{}", GetGUID());
		string_t ProxyString = GetName();
		if (ImGui::CollapsingHeader(("Card" + suffix).c_str()))
		{
            ImGui::InputText("Name", ProxyString.data(), ProxyString.size());
			if (ProxyString != GetName())
			{
				SetName(ProxyString);
			}
			ImGui::Text("Type: %d", type);
			ImGui::DragInt(("Cost" + suffix).c_str(), &cost, 1, 0, 0, "%d");
			ImGui::DragInt(("Damage" + suffix).c_str(), &damage, 1, 0, 0, "%d");
			ImGui::DragInt(("Heal" + suffix).c_str(), &heal, 1, 0, 0, "%d");
			ImGui::DragInt(("Move" + suffix).c_str(), &move, 1, 0, 0, "%d");
			ImGui::DragInt(("Defense" + suffix).c_str(), &defense, 1, 0, 0, "%d");
		}
	}

#pragma endregion Public Functions

}	// namespace
