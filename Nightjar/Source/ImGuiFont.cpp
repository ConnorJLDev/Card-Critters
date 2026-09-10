//------------------------------------------------------------------------------
//
// File Name:	ImGuiFont.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		DEAR IMGUI system view for Font.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiInclude.h"

#include "Font.h"

#include TEXT_INCLUDE

#include "Texture.h"

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

	void Font::DisplayIMGUI()
	{
		NamedObject::DisplayIMGUI(true);

		if (tss_ != nullptr)
		{
			const Texture* texture = tss_->texture;
			IVec2 texSize = texture->GetDimensions();

			ImGui::Text("Size: ");
			ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ValueColor);
			ImGui::SameLine();
			ImGui::Text("%d x %d", texSize.x, texSize.y);
			ImGui::PopStyleColor();

			ImGui::Image(texture, { 256.0f, 256.0f });
		}
		else
		{
			ImGui::Text("No TSS.");
		}

		{
			auto familyName = face_->family_name;
			ImGui::Text("Family Name: ");
			ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ValueColor);
			ImGui::SameLine();
			ImGui::Text("%s", familyName);
			ImGui::PopStyleColor();

			auto styleName = face_->style_name;
			ImGui::Text("Style Name: ");
			ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ValueColor);
			ImGui::SameLine();
			ImGui::Text("%s", styleName);
			ImGui::PopStyleColor();

			int faceCount = face_->num_faces;
			ImGui::Text("Face Count: ");
			ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ValueColor);
			ImGui::SameLine();
			ImGui::Text("%d", faceCount);
			ImGui::PopStyleColor();
			
			int glyphCount = face_->num_glyphs;
			ImGui::Text("Glyph Count: ");
			ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ValueColor);
			ImGui::SameLine();
			ImGui::Text("%d", glyphCount);
			ImGui::PopStyleColor();

			int fixedCount = face_->num_fixed_sizes;
			ImGui::Text("Fixed Count: ");
			ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ValueColor);
			ImGui::SameLine();
			ImGui::Text("%d", fixedCount);
			ImGui::PopStyleColor();

			int charmapCount = face_->num_charmaps;
			ImGui::Text("CharMap Count: ");
			ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ValueColor);
			ImGui::SameLine();
			ImGui::Text("%d", charmapCount);
			ImGui::PopStyleColor();

			size_t faceFlags = (size_t)face_->face_flags;
			ImGui::Text("Face Flags: ");
			ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ValueColor);
			bool facePrinted = false;
			static constexpr array_t<string_view_t, 19> faceFlagNames{
				"SCALABLE",
				"FIXED_SIZES",
				"FIXED_WIDTH",
				"SFNT",
				"HORIZONTAL",
				"VERTICAL",
				"KERNING",
				"FAST_GLYPHS",
				"MULTIPLE_MASTERS",
				"GLYPH_NAMES",
				"EXTERNAL_STREAM",
				"HINTER",
				"CID_KEYED",
				"TRICKY",
				"COLOR",
				"VARIATION",
				"SVG",
				"SBIX",
				"SBIX_OVERLAY",
			};
			for (unsigned i = 0; i < faceFlagNames.size(); ++i)
			{
				if (faceFlags & (1ull << i))
				{
					ImGui::SameLine();
					if (!facePrinted)
					{
						facePrinted = true;
						ImGui::Text("%s", faceFlagNames[i].data());
					}
					else
					{
						ImGui::Text(", %s", faceFlagNames[i].data());
					}
				}
			}
			ImGui::PopStyleColor();

			size_t styleFlags = (size_t)face_->style_flags;
			ImGui::Text("Style Flags: ");
			ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ValueColor);
			bool stylePrinted = false;
			static constexpr array_t<string_view_t, 2> styleFlagNames{
				"ITALIC",
				"BOLD",
			};
			for (unsigned i = 0; i < styleFlagNames.size(); ++i)
			{
				if (styleFlags & (1ull << i))
				{
					ImGui::SameLine();
					if (!stylePrinted)
					{
						stylePrinted = true;
						ImGui::Text("%s", styleFlagNames[i].data());
					}
					else
					{
						ImGui::Text(", %s", styleFlagNames[i].data());
					}
				}
			}
			ImGui::PopStyleColor();

			//auto sizes = face_->available_sizes;
		}

	}

#pragma endregion Public Functions

}	// namespace
