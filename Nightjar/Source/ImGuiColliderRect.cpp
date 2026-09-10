//------------------------------------------------------------------------------
//
// File Name:	ImGuiColliderRect.cpp
// Author(s):	Barend Brink (barend.brink), Connor Lariviere (connor.lariviere), Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		DEAR IMGUI system view for ColliderRect.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiInclude.h"

#include "Collider.h"
#include "ColliderRect.h"

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

	void ColliderRect::DisplayIMGUI()
	{
		Collider::DisplayIMGUI();
		ImGui::DragFloat2("Size", &size_.x, 1.0f, 0.0f, std::numeric_limits<float>::infinity(), "%.1f");
	}

#pragma endregion Public Functions

}	// namespace
