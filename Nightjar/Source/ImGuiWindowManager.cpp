//------------------------------------------------------------------------------
//
// File Name:	ImGuiWindowManager.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Manages ImGui window creation, destruction, and access.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiWindowManager.h"
#include "ImGuiSystem.h"
#include "BaseSystem.h"

#include "ImGuiInclude.h"
#include "imgui_internal.h"

#include "ImGuiWindowBase.h"
#include "ImGuiWindows.h"

#include "PerformanceSystem.h"

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

    constexpr string_view_t ImGuiWindowManager::DockActiveWindowsName = "DockedActiveWindows";

    constexpr string_view_t ImGuiWindowManager::DockWindowKeyName = "Window";

    //--------------------------------------------------------------------------
    // Private Static Variables:
    //--------------------------------------------------------------------------

    ImGuiWindowManager* ImGuiWindowManager::Windows = nullptr;
#define inst ImGuiWindowManager::Windows

    //--------------------------------------------------------------------------
    // Private Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Constructors/Destructors:
    //--------------------------------------------------------------------------

#pragma region Constructors

    ImGuiWindowManager::ImGuiWindowManager(void)
        : windows_()
        , windowsActive_()
        , windowsWriteBuffer_()
    {
        assert(inst == nullptr);
        inst = this;
    }

    //--------------------------------------------------------------------------

    ImGuiWindowManager::~ImGuiWindowManager(void)
    {
        for (auto& window : windows_)
        {
            SafeDelete(window);
        }
        assert(inst == this);
        inst = nullptr;
    }

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

    ImGuiWindowBase* ImGuiWindowManager::CreateWindow([[maybe_unused]] string_view_arg windowName)
    {
        return nullptr;
    }

    ImGuiWindowBase* ImGuiWindowManager::GetWindow([[maybe_unused]] string_view_arg windowName)
    {
        return nullptr;
    }

    ImGuiWindowBase* ImGuiWindowManager::GetWindow([[maybe_unused]] WindowIndex_t index)
    {
        return nullptr;
    }

    bool ImGuiWindowManager::DestroyWindow([[maybe_unused]] ImGuiWindowBase* window)
    {
        return false;
    }

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

    bool ImGuiWindowManager::Init(void)
    {
        PerformanceSystem::SetTrackerCount(PerformanceGroup::ImGuiWindow, 0);

        return true;
    }

    void ImGuiWindowManager::Exit(void)
    {
    }

    void ImGuiWindowManager::InitWindowSelection(void)
    {
        // Initialize window dock selection.

        for (auto* window : inst->windows_)
        {
            window->Display();
        }
        vector_t<string_t> windowsActiveNames{};
        windowsActiveNames.reserve(inst->windowsActive_.size());
        for (const auto& [dockId, name] : inst->windowsActive_)
        {
            ImGui::SetWindowFocus(name.c_str());
            TraceInfo("Focused window \"{}\"", name);
            windowsActiveNames.emplace_back(name);
        }
        ImGui::SetWindowFocus(nullptr);
        inst->windowsActive_.clear();
        for (auto& name : windowsActiveNames)
        {
            ImGuiWindow* window = ImGui::FindWindowByName(name.c_str());
            if (window != nullptr && window->DockIsActive && window->DockNode != nullptr)
            {
                inst->windowsActive_[window->DockNode->ID] = name;
            }
        }
    }

    ImGuiWindowBase* ImGuiWindowManager::CreateWindow([[maybe_unused]] string_view_arg windowName, [[maybe_unused]] WindowIndex_t index)
    {
        return nullptr;
    }

#pragma endregion Private Static Functions

    //--------------------------------------------------------------------------
    // Private Functions:
    //--------------------------------------------------------------------------

#pragma region Private Functions

    void ImGuiWindowManager::UpdateWindows(void)
    {
        auto* sys = ImGuiSystem::GetInstance();

        unsigned windowIndex = 4;
        PerformanceSystem::SetTrackerCount(PerformanceGroup::ImGuiWindow, EngineLoop::Update, windows_.size() + windowIndex);
        Stopwatch stopwatch{};
        stopwatch.Start();

        // Display all windows.
        for (auto* window : windows_)
        {
            bool displayed = window->Display();
            if (displayed)
            {
                windowsActive_[window->dockID_] = window->internalName_;
            }
            auto time = stopwatch.Lap<nsec_t>();
            PerformanceSystem::AssignTrackerOrder(*window, PerformanceGroup::ImGuiWindow, EngineLoop::Update, windowIndex++);
            auto& tracker = PerformanceSystem::GetTracker(*window, *sys);
            tracker.Push(time, EngineLoop::Update, PerformanceSource::CPU);
        }

        // Destroy all windows that have closed.
        std::erase_if(windows_, [](const auto& window)
        {
            bool closed = !window->GetIsOpen();
            if (closed)
            {
                window->Destroy();
                delete window;
            }
            return closed;
        });
    }

    void ImGuiWindowManager::CleanUpInactiveWindows(void)
    {
        unordered_set_t<string_t, StringHash, std::equal_to<>> used;
        for (auto* window : windows_)
        {
            used.insert(window->internalName_);
        }
        auto* context = ImGui::GetCurrentContext();
        for (ImGuiWindow* window : context->Windows)
        {
            if (used.find(window->Name) == used.end())
            {
                TraceWarning("Removing unused window \"{}\"!", window->Name);

                [[maybe_unused]]
                ImGuiID windowID = ImGui::GetID(window->Name);

                ImGui::ClearWindowSettings(window->Name);

#ifdef IMGUI_HAS_DOCK
                ImGui::DockBuilderRemoveNode(windowID);
#endif

                window->StateStorage.Clear();
            }
            else
            {
                TraceInfo("Validated window \"{}\"", window->Name);
            }
        }
    }

    ImGuiWindowManager::WindowIndex_t ImGuiWindowManager::GetLowestUnusedWindowIndex(void)
    {
        set_t<WindowIndex_t> foundIndices{};
        for (const auto* window : windows_)
        {
            foundIndices.insert(window->index_);
        }
        WindowIndex_t index = 0;
        while (foundIndices.contains(index))
        {
            ++index;
        }
        return index;
    }

#pragma endregion Private Functions

}	// namespace
