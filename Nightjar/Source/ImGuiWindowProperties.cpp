//------------------------------------------------------------------------------
//
// File Name:	ImGuiWindowProperties.cpp
// Author(s):	Barend Brink (barend.brink), Connor Lariviere (connor.lariviere), Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		ImGui window that displays properties of a selected item.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiWindowProperties.h"

#include "ImGuiInclude.h"

#include "ImGuiWindowBase.inl"

#include "Entity.h"

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

    constexpr string_view_t ImGuiWindowProperties::WindowName = "Properties";

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

    ImGuiWindowProperties::ImGuiWindowProperties(void)
        : ImGuiWindowBase(WindowName, 0/*ImGuiWindowFlags_MenuBar*/)
    {
    }

    ImGuiWindowProperties::ImGuiWindowProperties(ImGuiWindowProperties&& other) noexcept
        : ImGuiWindowBase(std::move(other))
    {
    }

    //--------------------------------------------------------------------------

    ImGuiWindowProperties::~ImGuiWindowProperties(void)
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

    void ImGuiWindowProperties::DisplayImpl(void)
    {
        auto& ctx = ImGuiContextData::GetContext();
        Entity* entity = ctx.selectedEntity_;

        if (entity == nullptr || entity->IsDestroyed())
        {
            ImGui::Text("No entity selected.");
        }
        else
        {
            entity->DisplayIMGUI();
        }
    }

    ImGuiWindowBase::ReadError ImGuiWindowProperties::ReadKeyValue([[maybe_unused]] string_view_arg key, [[maybe_unused]] string_view_arg value)
    {
        //bool parseSuccess;
        //switch (READKEY_HASH(key))
        //{
        //READKEY_CHECK("Dummy")
        //    READKEY_VALUE(dummyValue_);
        //    return READKEY_RETURN;
        //default:
        //    break;
        //}
        return ReadError::InvalidKey;
    }

    void ImGuiWindowProperties::ReadFinish(void)
    {
    }

    void ImGuiWindowProperties::WriteAll([[maybe_unused]] string_t& buffer)
    {
    }

    //void ImGuiWindowProperties::DisplayMenuBar(void)
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
