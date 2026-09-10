//------------------------------------------------------------------------------
//
// File Name:	ImGuiAnimationCurve.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		DEAR IMGUI system view for AnimationCurve.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiInclude.h"

#include "AnimationCurve.h"
#include "AnimationKeyFrame.h"

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

	void AnimationCurve::DisplayIMGUI(float timespan)
	{
		bool modified = true;
		// Draw curve
		{
			if (timespan <= 0)
			{
				timespan = duration_;
			}
			static array_t<float, 100> curveEval;
			static float evalMin, evalMax;
			if (modified)
			{
				evalMin = std::numeric_limits<float>::max();
				evalMax = std::numeric_limits<float>::min();
				float step = 0.01f * timespan;
				for (unsigned i = 0; i < curveEval.size(); ++i)
				{
					float eval = Evaluate(step * (float)i);
					curveEval[i] = eval;
					evalMin = std::min(eval, evalMin);
					evalMax = std::max(eval, evalMax);
				}
			}
			ImGui::PlotLines("Curve", curveEval.data(), (int)curveEval.size(), 0, nullptr, evalMin, evalMax, ImVec2(0, 80));
		}
	}

#pragma endregion Public Functions

}	// namespace
