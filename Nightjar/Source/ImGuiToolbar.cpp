//------------------------------------------------------------------------------
//
// File Name:	ImGuiToolbar.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Manages items on the ImGui toolbar at the top of the screen.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiToolbar.h"

#include "ImGuiInclude.h"
#include "imgui_internal.h"

//------------------------------------------------------------------------------
// Preprocessor Directives:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    //--------------------------------------------------------------------------
    // Public Constants:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Public Static Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Public Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Private Constants:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Private Static Variables:
    //--------------------------------------------------------------------------

    ImGuiToolbar* ImGuiToolbar::Toolbar = nullptr;

#define inst ImGuiToolbar::Toolbar

    //--------------------------------------------------------------------------
    // Private Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Constructors/Destructors:
    //--------------------------------------------------------------------------

#pragma region Constructors

    ImGuiToolbar::ImGuiToolbar(void)
    {
        assert(inst == nullptr);
        inst = this;
    }

    //--------------------------------------------------------------------------

    ImGuiToolbar::~ImGuiToolbar(void)
    {
        assert(inst == this);
        inst = nullptr;
    }

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

#pragma endregion Public Static Functions

    //--------------------------------------------------------------------------
    // Public Functions:
    //--------------------------------------------------------------------------

#pragma region Public Functions

#pragma endregion Public Functions

    //--------------------------------------------------------------------------
    // Private Static Functions:
    //--------------------------------------------------------------------------

#pragma region Private Static Functions

    bool ImGuiToolbar::Init(void)
    {
        return true;
    }

    void ImGuiToolbar::Exit(void)
    {
    }

#pragma endregion Private Static Functions

    //--------------------------------------------------------------------------
    // Private Functions:
    //--------------------------------------------------------------------------

#pragma region Private Functions

    bool ImGuiToolbar::Update(void)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();

        //FramePadding
        const float size = ImGui::GetTextLineHeight() + ImGui::GetStyle().WindowPadding.y * 2.0f;

        if (!ImGui::BeginViewportSideBar("##Toolbar", viewport, ImGuiDir_Up, size, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
        {
            return false;
        }

        ImGui::End();

        return true;
    }

#pragma endregion Private Functions

}	// namespace
