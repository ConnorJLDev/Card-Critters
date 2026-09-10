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

#include "ImGuiInclude.h"

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

	void ArmatureBone::DisplayIMGUI()
	{
		ImGui::PushID(this);
		ImGui::Text("Name: ");
		ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ValueColor);
		ImGui::SameLine();
		ImGui::Text(GetName().c_str());
		ImGui::PopStyleColor();
		DisplayIMGUI3D();
		// Children
		if (!children_.empty())
		{
			constexpr ImGuiTreeNodeFlags nodeFlagsDefault{ ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth };

			ImGuiTreeNodeFlags flags{ nodeFlagsDefault };

			bool nodeOpen{ ImGui::TreeNodeEx("Children", flags)};

			// If the node is open, show the children and pop the node.
			if (nodeOpen)
			{
				for (auto& child : children_)
				{
					child->DisplayIMGUI();
				}
				ImGui::TreePop();
			}

		}
		ImGui::PopID();
	}

#pragma endregion Public Functions

}	// namespace
