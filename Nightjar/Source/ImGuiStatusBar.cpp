//------------------------------------------------------------------------------
//
// File Name:	ImGuiStatusBar.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Manages items on the ImGui status bar at the bottom of the
//              screen.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiStatusBar.h"

#include "ImGuiInclude.h"
#include "imgui_internal.h"

#include "ImGuiWindowBase.h"
#include "ImGuiWindows.h"

#include "SceneSystem.h"
#include "Scene.h"

//------------------------------------------------------------------------------
// Preprocessor Directives:
//------------------------------------------------------------------------------

#define NESTED_TABLE

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

    static constexpr ImGuiWindowFlags SidebarFlags = ImGuiWindowFlags_None | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

    //--------------------------------------------------------------------------
    // Private Static Variables:
    //--------------------------------------------------------------------------

    ImGuiStatusBar* ImGuiStatusBar::StatusBar = nullptr;

#define inst ImGuiStatusBar::StatusBar

    //--------------------------------------------------------------------------
    // Private Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Constructors/Destructors:
    //--------------------------------------------------------------------------

#pragma region Constructors

    ImGuiStatusBar::ImGuiStatusBar(void)
        : statuses_()
        , scenes_()
    {
        assert(inst == nullptr);
        inst = this;
    }

    //--------------------------------------------------------------------------

    ImGuiStatusBar::~ImGuiStatusBar(void)
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

    bool ImGuiStatusBar::Init(void)
    {
        inst->statuses_.Register("FPS", ImGuiStatusBar::DisplayFPS, ImGuiAlignedBar::Alignment::Left);
        inst->statuses_.Register("Center", ImGuiStatusBar::DisplayCenter, ImGuiAlignedBar::Alignment::Center);
        inst->statuses_.Register("Center2", ImGuiStatusBar::DisplayCenter, ImGuiAlignedBar::Alignment::Center);
        inst->statuses_.Register("Scenes", ImGuiStatusBar::DisplayOpenScenes, ImGuiAlignedBar::Alignment::Right);

        return true;
    }

    void ImGuiStatusBar::Exit(void)
    {
    }

    void ImGuiStatusBar::DisplayFPS(void)
    {
        ImGui::Text("FPS: % 5.1f", ImGui::GetIO().Framerate);
    }

    void ImGuiStatusBar::DisplayCenter(void)
    {
        ImGui::TextUnformatted(string_view_t("Center test!"));
    }

    void ImGuiStatusBar::DisplayOpenScenes(void)
    {
        inst->scenes_.clear();

        const unsigned slotCount = SceneSystem::GetSceneSlotCount();

        for (unsigned i = 0; i < slotCount; ++i)
        {
            Scene* scene = SceneSystem::GetScene(i);
            if (scene == nullptr)
            {
                continue;
            }
            if (!inst->scenes_.empty())
            {
                inst->scenes_ += ", ";
            }
            FORMAT_TO(inst->scenes_, "({})\"{}\"", i, scene->GetName());
        }

        ImGui::TextUnformatted(inst->scenes_);
    }

#pragma endregion Private Static Functions

    //--------------------------------------------------------------------------
    // Private Functions:
    //--------------------------------------------------------------------------

#pragma region Private Functions

    bool ImGuiStatusBar::Update(void)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        auto& style = ImGui::GetStyle();

        // WindowPadding FramePadding CellPadding
        const Vec2 oldWindowPadding = style.WindowPadding;
        style.WindowPadding = Vec2(0.0f, 0.0f);

        const float size = ImGui::GetTextLineHeight() + style.CellPadding.y * 2.0f;

        const bool began = ImGui::BeginViewportSideBar("##StatusBar", viewport, ImGuiDir_Down, size, SidebarFlags);

        style.WindowPadding = oldWindowPadding;

        if (began)
        {
            statuses_.Display();

            ImGui::End();
        }

        return began;
    }

#pragma endregion Private Functions

}	// namespace
