//------------------------------------------------------------------------------
//
// File Name:	ImGuiEntity.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		DEAR IMGUI system view for Entity.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiInclude.h"

#include "Entity.h"
#include "ComponentRegistry.h"

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

	void Entity::DisplayIMGUI()
	{
		NamedObject::DisplayIMGUI();

		vector_t<Component*> toRemove{};
		for (Component*& component : components_)
		{
			string_view_arg typeName{ component->GetReadKey() };
			ImGui::PushID(typeName.data());
			// Button for destroying the component.
			bool kill = ImGui::Button("X");
			ImGui::SameLine();
			// The header for displaying the component data.
			bool active = ImGui::CollapsingHeader(typeName.data());
			if (kill)
			{
				toRemove.push_back(component);
			}
			else if (active)
			{
				component->DisplayIMGUI();
			}
			ImGui::PopID();
		}
		// Remove components marked for removal.
		for (Component*& component : toRemove)
		{
			ComponentRemove(component);
		}
		// Dropdown for adding a new component
		ImGui::Separator();
		if (ImGui::BeginCombo("Add Component##componentList", "Select..."))
		{
			for (const auto& componentEntry : ComponentRegistry::Entries)
			{
				ImGuiSelectableFlags flags{};
				if (ComponentGet(static_cast<Component::TypeEnum>(componentEntry.typeId)) != nullptr)
				{
					flags |= ImGuiSelectableFlags_Disabled;
				}
				if (ImGui::Selectable(componentEntry.typeName.data(), false, flags))
				{
					Component* component = componentEntry.generator();
					ComponentAdd(component);
				}
			}
			ImGui::EndCombo();
		}
	}

#pragma endregion Public Functions

}	// namespace
