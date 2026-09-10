//------------------------------------------------------------------------------
//
// File Name:	ImGuiLibrary.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Base DEAR IMGUI system view for libraries.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include <filesystem>

#include "ImGuiInclude.h"

#include "Library.h"

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

	template<typename T>
	void Library<T>::DisplayIMGUI()
	{
		static T* current{ nullptr };
		DisplayIMGUISelector(T::ReadKey, current);
		if (current != nullptr)
		{
			current->DisplayIMGUI();
		}
	}

	template<typename T>
	template<typename TPtr, std::enable_if_t<std::is_same_v<std::decay_t<TPtr>, T>, bool>>
	bool Library<T>::DisplayIMGUISelector(string_view_arg name, TPtr*& current)
	{
		bool changed = false;

		static constexpr string_view_t emptyName = "Select...";

        const string_t itemCurrentName = current == nullptr ? string_t(emptyName) : current->GetName();

		if (ImGui::BeginCombo(name.data(), itemCurrentName.c_str()))
		{
			// First, display loaded items.
			for (unsigned i = 0; i < Container<T>::Count(); ++i)
			{
				T* item = Container<T>::GetFromIndex(i);
				const string_t& itemName = item->GetName();
				ImGuiSelectableFlags flags{};
				if (item == current)
				{
					flags |= ImGuiSelectableFlags_Disabled;
				}
				if (ImGui::Selectable(itemName.c_str(), false, flags))
				{
					current = item;
					changed = true;
				}
			}

			ImGui::Separator();

			// Then, display all files under the item's read directory that aren't loaded.
			for (const auto& entry : std::filesystem::recursive_directory_iterator(T::ReadDir))
			{
				if (!entry.is_regular_file())
				{
					continue;
				}
				auto filePath = entry.path().relative_path();
				//string_t fileStem = filePath.stem().string();
                std::string _temp = filePath.filename().string();
				string_t fileName(CONVERT_STR(_temp));
				if (fileName.ends_with(".json"))
				{
					fileName = fileName.substr(0, fileName.size() - (_countof(".json") - 1));
				}
                _temp = filePath.parent_path().string();
				string_t dirPathParent(CONVERT_STR(_temp));
				StringReplaceAll(dirPathParent, "\\", "/");
				string_t dirPathParentTruncated;
                if (dirPathParent.starts_with(T::ReadDir))
                {
                    constexpr unsigned ReadDirSizeGtr = (unsigned)T::ReadDir.size() + 1;
                    unsigned substrOffset = std::min((unsigned)dirPathParent.size(), ReadDirSizeGtr);
                    dirPathParentTruncated = dirPathParent.substr(substrOffset);
                }
                else
                {
                    dirPathParentTruncated = dirPathParent;
                }
				string_t fileNameFinal = dirPathParentTruncated.empty() ? fileName : (dirPathParentTruncated + "/" + fileName);
				T* item{ nullptr };
				if (Container<T>::Find(fileNameFinal, item))
				{
					continue;
				}
				ImGuiSelectableFlags flags{};
				if (ImGui::Selectable(fileNameFinal.c_str(), false, flags))
				{
					current = Get_(fileNameFinal);
					changed = true;
				}
			}
			ImGui::EndCombo();
		}

		return changed;
	}

#pragma endregion Public Functions

}	// namespace
