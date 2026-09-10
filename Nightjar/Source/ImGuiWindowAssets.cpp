//------------------------------------------------------------------------------
//
// File Name:	ImGuiWindowAssets.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		ImGui window that displays asset information by type.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiWindowAssets.h"

#include "ImGuiInclude.h"

#include "ImGuiWindowBase.inl"

#include "Texture.h"
#include "AnimationSequence.h"
#include "Font.h"
#include "Mesh.h"

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

    constexpr string_view_t ImGuiWindowAssets::WindowName = "Assets";

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

    ImGuiWindowAssets::ImGuiWindowAssets(void)
        : ImGuiWindowBase(WindowName, 0/*ImGuiWindowFlags_MenuBar*/)
    {
    }

    ImGuiWindowAssets::ImGuiWindowAssets(ImGuiWindowAssets&& other) noexcept
        : ImGuiWindowBase(std::move(other))
    {
    }

    //--------------------------------------------------------------------------

    ImGuiWindowAssets::~ImGuiWindowAssets(void)
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

    void ImGuiWindowAssets::DisplayImpl(void)
    {
        Vec4 clearColor = GraphicsSystem::GetClearColor();
        if (ImGui::ColorEdit4("Clear Color", &clearColor.x))
        {
            GraphicsSystem::SetClearColor(clearColor);
        }

        AnimationSequenceLibrary::DisplayIMGUI_S();
        FontLibrary::DisplayIMGUI_S();
        MeshLibrary::DisplayIMGUI_S();
        TextureLibrary::DisplayIMGUI_S();
    }

    ImGuiWindowBase::ReadError ImGuiWindowAssets::ReadKeyValue([[maybe_unused]] string_view_arg key, [[maybe_unused]] string_view_arg value)
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

    void ImGuiWindowAssets::ReadFinish(void)
    {
    }

    void ImGuiWindowAssets::WriteAll([[maybe_unused]] string_t& buffer)
    {
    }

    //void ImGuiWindowAssets::DisplayMenuBar(void)
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
