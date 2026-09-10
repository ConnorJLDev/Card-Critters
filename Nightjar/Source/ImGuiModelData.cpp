//------------------------------------------------------------------------------
//
// File Name:	ImGuiModelData.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		DEAR IMGUI system view for ModelData.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiInclude.h"

#include "ModelData.h"
#include "Armature.h"

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

	void ModelData::DisplayIMGUI()
	{
		NamedObject::DisplayIMGUI();
		if (armature_ != nullptr)
		{
			armature_->DisplayIMGUI();
		}
	}

#pragma endregion Public Functions

}	// namespace
