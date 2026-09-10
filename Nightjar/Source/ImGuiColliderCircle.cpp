//------------------------------------------------------------------------------
//
// File Name:	ImGuiColliderCircle.cpp
// Author(s):	Barend Brink (barend.brink), Connor Lariviere (connor.lariviere), Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		DEAR IMGUI system view for ColliderCircle.
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
#include "ColliderCircle.h"

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

	void ColliderCircle::DisplayIMGUI()
	{
		Collider::DisplayIMGUI();
		ImGui::DragFloat("Radius", &radius_, 1.0f, 0.0f, std::numeric_limits<float>::infinity(), "%.1f");
	}

#pragma endregion Public Functions

}	// namespace
