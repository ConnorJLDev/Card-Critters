//------------------------------------------------------------------------------
//
// File Name:	ImGuiArmature.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		DEAR IMGUI system view for Armature.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiSystem.h"

#include "Armature.h"
#include "ArmatureBone.h"

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

	void Armature::DisplayIMGUI()
	{
		ArmatureBone* rootBone = GetRootBone();
		rootBone->DisplayIMGUI();
	}

#pragma endregion Public Functions

}	// namespace
