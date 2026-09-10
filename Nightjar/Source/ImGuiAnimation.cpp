//------------------------------------------------------------------------------
//
// File Name:	ImGuiAnimation.cpp
// Author(s):	Barend Brink (barend.brink), Connor Lariviere (connor.lariviere), Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		DEAR IMGUI system view for Animation.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiInclude.h"

#include "Animation.h"
#include "AnimationSequence.h"

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

	void Animation::DisplayIMGUI()
	{
		if (AnimationSequenceLibrary::DisplayIMGUISelector_S("Sequence", sequence_))
		{
			// If it changed, update our parameters.
			SetSequence(sequence_, true);
		}

		if (sequence_ != nullptr)
		{
			// Box for editing the speed of animation.
			ImGui::SliderFloat("Speed", &speed_, 0.0f, 15.0f, "%.2f");

			// Box for editing the current time of the animation.
			ImGui::SliderFloat("Time", &time_, 0.0f, duration_, "%.2f");

			// Box for editing the duration of the animation.
			ImGui::InputFloat("Duration", &duration_);

			// Check box for is playing.
			ImGui::Checkbox("Is Playing", &isRunning_);

			// Check box for mirrored.
			ImGui::Checkbox("Mirrored", &isMirrored_);

			// Enum selector for loop method.
			ImGui::CreateEnumSelector("Loop Method", loopAction_, ENUM_WRITE(AnimationLoop));
		}
		else
		{
			// If we have no sequence, display red text.
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
			ImGui::Text("NO ANIMATION SEQUENCE");
			ImGui::PopStyleColor();
		}
	}

#pragma endregion Public Functions

}	// namespace
