//------------------------------------------------------------------------------
//
// File Name:	ImGuiBehaviorMessageBox.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Editor view for BehaviorMessageBox.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#ifdef _EDITOR

#include "ImGuiSystem.h"

#include "BehaviorMessageBox.h"

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

	void BehaviorMessageBox::DisplayIMGUI()
	{
		BehaviorInteractable::DisplayIMGUI();
	}

#pragma endregion Public Functions

}	// namespace

#endif
