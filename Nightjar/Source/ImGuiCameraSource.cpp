//------------------------------------------------------------------------------
//
// File Name:	ImGuiCameraSource.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		DEAR IMGUI system view for CameraSource.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiInclude.h"

#include "PlatformSystem.h"

#include "CameraSource.h"

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

	void CameraSource::DisplayIMGUI()
	{
		projectionDirty_ |= ImGui::CreateEnumSelector("Mode", mode_, ENUM_WRITE(Mode));
		if (ImGui::Checkbox("Respect Window Size", &respectWindowSize_))
		{
			if (respectWindowSize_)
			{
				orthoSize_ /= PlatformSystem::GetWindowSize();
			}
			else
			{
				orthoSize_ *= PlatformSystem::GetWindowSize();
			}
			projectionDirty_ = true;
		}
		if (respectWindowSize_)
		{
			projectionDirty_ |= ImGui::DragFloat2("Ortho Size", &orthoSize_.x, 0.01f, 0.0f, 0.0f, "%.3f");
		}
		else
		{
			projectionDirty_ |= ImGui::DragFloat2("Ortho Size", &orthoSize_.x, 1.0f, 0.0f, 0.0f, "%.1f");
		}
		switch (mode_)
		{
		case Mode::Orthographic:
		{
		}
		break;
		case Mode::Perspective:
		{
			projectionDirty_ |= ImGui::CreateDragRotation("FOV", perspFOV_, 1.0f, 0.0f, 90.0f, "%.1f");
			projectionDirty_ |= ImGui::DragFloat("Clip Distance Near", &clipDistanceNear_, 10.0f, 0.0f, 100000.0f, "%.3f", ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoRoundToFormat);
			projectionDirty_ |= ImGui::DragFloat("Clip Distance Far", &clipDistanceFar_, 10.0f, 0.0f, 100000.0f, "%.3f", ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoRoundToFormat);
		}
		break;
		default:
			assert(false);
			break;
		}
	}

#pragma endregion Public Functions

}	// namespace
