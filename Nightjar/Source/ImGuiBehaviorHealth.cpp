//------------------------------------------------------------------------------
//
// File Name:	ImGuiBehaviorHealth.cpp
// Author(s):	Barend Brink (barend.brink), Connor Lariviere (connor.lariviere), Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		DEAR IMGUI system view for BehaviorHealth.
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
#include "BehaviorHealth.h"

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

	void BehaviorHealth::DisplayIMGUI()
	{
		Behavior::DisplayIMGUI();
		ImGui::DragInt("Health", &health, 1, 0, 0, "%d");
	}

#pragma endregion Public Functions

}	// namespace
