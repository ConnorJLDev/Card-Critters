//------------------------------------------------------------------------------
//
// File Name:	ImGuiModel.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		DEAR IMGUI system view for Model.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiSystem.h"

#include "Model.h"
#include "ModelData.h"
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

	void Model::DisplayIMGUI()
	{
		Armature* armature = model_->GetArmature();
		if (armature != nullptr)
		{
			armature->DisplayIMGUI();
		}
	}

#pragma endregion Public Functions

}	// namespace
