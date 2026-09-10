//------------------------------------------------------------------------------
//
// File Name:	ImGuiAnimationSequence.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		DEAR IMGUI system view for AnimationSequence.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiInclude.h"

#include "AnimationSequence.h"
#include "AnimationLink.h"
#include "AnimationCurve.h"

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

	void AnimationSequence::DisplayIMGUI()
	{
		// Duration
		ImGui::InputFloat("Duration", &duration_);
		// Looping method
		ImGui::CreateEnumSelector("Loop", loop_, ENUM_WRITE(AnimationLoop));
		// Links
		if (ImGui::CollapsingHeader("Links"))
		{
			for (auto& link : links_)
			{
				ImGui::PushID(&link);
				link->DisplayIMGUI();
				ImGui::PopID();
			}
		}
		// Curves
		if (ImGui::CollapsingHeader("Curves"))
		{
			for (auto& curve : curves_)
			{
				ImGui::PushID(&curve);
				curve->DisplayIMGUI(duration_);
				ImGui::PopID();
			}
		}
	}

#pragma endregion Public Functions

}	// namespace
