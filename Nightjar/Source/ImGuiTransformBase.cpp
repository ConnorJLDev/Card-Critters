//------------------------------------------------------------------------------
//
// File Name:	ImGuiTransformBase.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		DEAR IMGUI system view for TransformBase.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiInclude.h"

#include "TransformBase.h"

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

	void TransformComponents::DisplayIMGUI2D()
	{
		static bool uniform = false;
		dirtyFlags_ |= ImGui::DragFloat2("Translation", &translation2_.x, 10.0f, 0.0f, 0.0f, "%.1f") ? DirtyFlags::Translation : DirtyFlags::None;
        dirtyFlags_ |= ImGui::CreateDragRotation("Rotation", rotation2_, 1.0f, 0.0f, 0.0f, "%.1f") ? DirtyFlags::Rotation : DirtyFlags::None;
		bool scaleModified = false;
		if (uniform)
		{
			float ratio = scale2_.y / scale2_.x;
			if (std::isnan(ratio) || std::isinf(ratio))
			{
				ratio = 0.0f;
			}
			scaleModified |= ImGui::DragFloat("Scale", &scale2_.x, 10.0f, 0.0f, 0.0f, "%.1f");
			scaleModified |= ImGui::DragFloat("Ratio", &ratio, 0.05f, 0.0f, 0.0f, "%.3f", 0);
			scale2_.y = ratio * scale2_.x;
		}
		else
		{
			scaleModified |= ImGui::DragFloat2("Scale", &scale2_.x, 10.0f, 0.0f, 0.0f, "%.1f");
		}
		if (ImGui::Button("Flip X"))
		{
			scale2_.x *= -1;
			scaleModified = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Flip Y"))
		{
			scale2_.y *= -1;
			scaleModified = true;
		}
		ImGui::SameLine();
		ImGui::Checkbox("Uniform Scale", &uniform);

		if (scaleModified)
		{
			dirtyFlags_ |= DirtyFlags::Scale;
		}
	}

	void TransformComponents::DisplayIMGUI3D()
	{
		dirtyFlags_ |= ImGui::DragFloat3("Translation", &translation3_.x, 10.0f, 0.0f, 0.0f, "%.1f") ? DirtyFlags::Translation : DirtyFlags::None;
        dirtyFlags_ |= ImGui::CreateDragRotation("Rotation", rotation3_, 1.0f, 0.0f, 0.0f, "%.1f") ? DirtyFlags::Rotation : DirtyFlags::None;
		dirtyFlags_ |= ImGui::DragFloat3("Scale", &scale3_.x, 0.1f, 0.0f, 0.0f, "%.1f") ? DirtyFlags::Scale : DirtyFlags::None;
	}

#pragma endregion Public Functions

}	// namespace
