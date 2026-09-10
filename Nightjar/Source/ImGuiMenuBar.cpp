//------------------------------------------------------------------------------
//
// File Name:	ImGuiMenuBar.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Manages items and menus on the ImGui main menu bar.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiMenuBar.h"

#include "ImGuiInclude.h"
#include "imgui_internal.h"

#include "ImGuiWindowBase.h"
#include "ImGuiWindows.h"
#include "ImGuiWindowManager.h"

#include "SceneSystem.h"
#include "Scene.h"

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

    ImGuiMenuBar* ImGuiMenuBar::MenuBar = nullptr;

#define inst ImGuiMenuBar::MenuBar

    //--------------------------------------------------------------------------
    // Private Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Constructors/Destructors:
    //--------------------------------------------------------------------------

#pragma region Constructors

    ImGuiMenuBar::ImGuiMenuBar(void)
    {
        assert(inst == nullptr);
        inst = this;
    }

    //--------------------------------------------------------------------------

    ImGuiMenuBar::~ImGuiMenuBar(void)
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

    bool ImGuiMenuBar::Init(void)
    {
        inst->menus_.emplace_back("Windows", ImGuiMenuBar::WindowsMenu, true);

        return true;
    }

    void ImGuiMenuBar::Exit(void)
    {
    }

    void ImGuiMenuBar::WindowsMenu(void)
    {
#define REGISTER_IMGUI_WINDOW(name)\
        if (ImGui::MenuItem(name::WindowName.data()))\
        {\
            TraceVerbose("Opening \"{}\" window.", name::WindowName);\
            ImGuiWindowBase* window = ImGuiWindowBase::GenerateWindow(name::WindowName);\
            if (window != nullptr)\
            {\
                ImGuiWindowManager::Windows->windows_.push_back(window);\
                TraceInfo("Opened \"{}\" window.", name::WindowName);\
            }\
            else\
            {\
                TraceWarning("Failed to open \"{}\" window!", name::WindowName); \
            }\
        }\

#include "ImGuiWindows.def"

    }

#pragma endregion Private Static Functions

    //--------------------------------------------------------------------------
    // Private Functions:
    //--------------------------------------------------------------------------

#pragma region Private Functions

    bool ImGuiMenuBar::Update(void)
    {
        // Create main menu bar.
        if (!ImGui::BeginMainMenuBar())
        {
            // If creation failed, return.
            return false;
        }

        for (auto& menuEntry : menus_)
        {
            if (ImGui::BeginMenu(menuEntry.name.data(), menuEntry.active))
            {
                menuEntry.callback();

                ImGui::EndMenu();
            }
        }

        ImGui::Separator();



        ImGui::EndMainMenuBar();

        return true;
    }

#pragma endregion Private Functions

}	// namespace
