//------------------------------------------------------------------------------
//
// File Name:	TemplateImGuiWindow.cpp
// Author(s):	
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		ImGui window that displays .
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "TemplateImGuiWindow.h"

#include "ImGuiInclude.h"

#include "ImGuiWindowBase.inl"

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

    constexpr string_view_t TemplateImGuiWindow::WindowName = "Template";

    //--------------------------------------------------------------------------
    // Private Static Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Private Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Constructors/Destructors:
    //--------------------------------------------------------------------------

#pragma region Constructors

    TemplateImGuiWindow::TemplateImGuiWindow(void)
        : ImGuiWindowBase(WindowName, 0/*ImGuiWindowFlags_MenuBar*/)
    {
    }

    TemplateImGuiWindow::TemplateImGuiWindow(TemplateImGuiWindow&& other) noexcept
        : ImGuiWindowBase(std::move(other))
    {
    }

    //--------------------------------------------------------------------------

    TemplateImGuiWindow::~TemplateImGuiWindow(void)
    {
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

#pragma endregion Private Static Functions

    //--------------------------------------------------------------------------
    // Private Functions:
    //--------------------------------------------------------------------------

#pragma region Private Functions

    void TemplateImGuiWindow::DisplayImpl(void)
    {
        //DisplayMenuBar();
    }

    bool TemplateImGuiWindow::ReadKeyValue([[maybe_unused]] string_view_arg key, [[maybe_unused]] string_view_arg value)
    {
        //switch (READKEY_HASH(key))
        //{
        //READKEY_CHECK("Dummy")
        //    TryParseValue(value, dummyValue_);
        //    return true;
        //default:
        //    break;
        //}
        return false;
    }

    void TemplateImGuiWindow::ReadFinish(void)
    {
    }

    void TemplateImGuiWindow::WriteAll([[maybe_unused]] string_t& buffer)
    {
    }

    //void TemplateImGuiWindow::DisplayMenuBar(void)
    //{
    //    if (!ImGui::BeginMenuBar())
    //    {
    //        return;
    //    }
    //
    //    if (ImGui::BeginMenu("Settings"))
    //    {
    //
    //
    //        ImGui::EndMenu();
    //    }
    //
    //    ImGui::EndMenuBar();
    //}

#pragma endregion Private Functions

}	// namespace
