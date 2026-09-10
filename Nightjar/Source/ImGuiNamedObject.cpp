//------------------------------------------------------------------------------
//
// File Name:	ImGuiNamedObject.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		DEAR IMGUI system view for NamedObject.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiInclude.h"

#include "NamedObject.h"

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

	void NamedObject::DisplayIMGUI(bool editableName)
	{
		Object::DisplayIMGUI();
		if (!editableName)
		{
			ImGui::BeginDisabled();
		}
		constexpr ImGuiInputTextFlags flags = ImGuiInputTextFlags_CallbackCompletion;
		ImGui::InputText("Name", name_.data(), name_.size(), flags);
		if (!editableName)
		{
			ImGui::EndDisabled();
		}
	}

#pragma endregion Public Functions

}	// namespace
