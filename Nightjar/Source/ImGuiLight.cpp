//------------------------------------------------------------------------------
//
// File Name:	ImGuiLight.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		DEAR IMGUI system view for Light.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiInclude.h"

#include "Light.h"

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

	void Light::DisplayIMGUI()
	{
        if (ImGui::Checkbox("Ambient", &ambient_))
        {
            dirty_ = true;
        }
		if (!ambient_)
		{
			dirty_ |= LightSource::DisplayIMGUITransform();
			dirty_ |= LightSource::DisplayIMGUIIndex();
		}
		dirty_ |= LightSource::DisplayIMGUIColor();
		if (!ambient_)
		{
			ImGui::Separator();
			dirty_ |= LightSource::DisplayIMGUIAttenuation();
		}
	}

#pragma endregion Public Functions

}	// namespace
