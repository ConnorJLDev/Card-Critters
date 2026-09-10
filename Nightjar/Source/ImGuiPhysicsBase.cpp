//------------------------------------------------------------------------------
//
// File Name:	ImGuiPhysicsBase.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		DEAR IMGUI system view for PhysicsBase.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiInclude.h"

#include "PhysicsBase.h"

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

	void PhysicsBase::DisplayIMGUI2D()
	{
		ImGui::DragFloat2("Velocity", &velocity2_.x, 1.0f, 0.0f, 0.0f, "%.1f");
		ImGui::DragFloat2("Acceleration", &acceleration2_.x, 1.0f, 0.0f, 0.0f, "%.1f");
		ImGui::DragFloat("Rot. Velocity", &rotationalVelocity2_, 1.0f, 0.0f, 0.0f, "%.1f");
	}

	void PhysicsBase::DisplayIMGUI3D()
	{
		ImGui::DragFloat3("Velocity", &velocity3_.x, 1.0f, 0.0f, 0.0f, "%.1f");
		ImGui::DragFloat3("Acceleration", &acceleration3_.x, 1.0f, 0.0f, 0.0f, "%.1f");
		ImGui::DragFloat3("Rot. Velocity", &rotationalVelocity3_.x, 1.0f, 0.0f, 0.0f, "%.1f");
	}

#pragma endregion Public Functions

}	// namespace
