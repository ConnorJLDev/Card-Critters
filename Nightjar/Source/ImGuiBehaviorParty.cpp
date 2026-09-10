//------------------------------------------------------------------------------
//
// File Name:	ImGuiBehaviorParty.cpp
// Author(s):	Barend Brink (barend.brink), Connor Lariviere (connor.lariviere), Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		DEAR IMGUI system view for BehaviorParty.
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
#include "BehaviorParty.h"
#include "Entity.h"

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

	void BehaviorParty::DisplayIMGUI()
	{
		Behavior::DisplayIMGUI();
		ImGui::Text("Party Members:");
		for (const auto& member : characters_)
		{
			ImGui::Text("%s", member->GetParent()->GetName().c_str());
		}
	}

#pragma endregion Public Functions

}	// namespace
