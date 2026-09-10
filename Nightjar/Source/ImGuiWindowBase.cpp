//------------------------------------------------------------------------------
//
// File Name:	ImGuiWindowBase.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Base class for ImGui windows.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiWindowBase.h"

#include "ImGuiInclude.h"
#include "imgui_internal.h"

#include "ImGuiWindows.h"

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

    ImGuiWindowBase::WindowIndex_t ImGuiWindowBase::GlobalIndex(0);

    //--------------------------------------------------------------------------
    // Private Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Constructors/Destructors:
    //--------------------------------------------------------------------------

#pragma region Constructors

    ImGuiWindowBase::ImGuiWindowBase(string_view_arg name, int flags)
        : NamedObject(name)
        , internalName_()
        , pos_(0.0f, 0.0f)
        , size_(240.0f, 240.0f)
        , contentSize_()
        , flags_(flags)
        , dockID_(0)
        , index_(GlobalIndex++)
        , enableOpen_(true)
        , open_(true)
        , loaded_(false)
    {
        FORMAT_TO(internalName_, "{}##{}", name, index_);
    }

    ImGuiWindowBase::ImGuiWindowBase(ImGuiWindowBase&& other) noexcept
        : NamedObject(std::move(other))
        , internalName_(std::move(other.internalName_))
        , pos_(std::move(other.pos_))
        , size_(std::move(other.size_))
        , contentSize_(std::move(other.contentSize_))
        , flags_(std::move(other.flags_))
        , dockID_(std::exchange(other.dockID_, 0))
        , index_(std::move(other.index_))
        , enableOpen_(std::move(other.enableOpen_))
        , open_(std::move(other.open_))
        , loaded_(false)
    {
    }

    //--------------------------------------------------------------------------

    ImGuiWindowBase::~ImGuiWindowBase(void)
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

    bool ImGuiWindowBase::Display(void)
    {
        if (!open_)
        {
            return false;
        }

        ImGui::SetNextWindowPos(pos_, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(size_, ImGuiCond_FirstUseEver); // ImGuiCond_Once ImGuiCond_FirstUseEver

        const char* const windowName = internalName_.c_str();
        static_assert(std::is_same_v<decltype(flags_), ImGuiWindowFlags>);
        bool windowBegan = ImGui::Begin(windowName, enableOpen_ ? &open_ : nullptr, static_cast<ImGuiWindowFlags>(flags_));

        if (windowBegan)
        {
            pos_ = ImGui::GetWindowPos();
            size_ = ImGui::GetWindowSize();
            contentSize_ = ImGui::GetContentRegionAvail();
            static_assert(std::is_same_v<decltype(dockID_), ImGuiID>);
            dockID_ = ImGui::GetWindowDockID();

            DisplayImpl();
        }

        ImGui::End();

        return windowBegan;
    }

    bool ImGuiWindowBase::GetIsOpen(void) const
    {
        return open_;
    }

#pragma endregion Public Functions

    //--------------------------------------------------------------------------
    // Private Static Functions:
    //--------------------------------------------------------------------------

#pragma region Private Static Functions

#define REGISTER_IMGUI_WINDOW(window) { string_t(window::WindowName), []() { return new window(); } },
    // The following function is the unwrapped form of the macro on the next line, as preprocessor directives within macros are undefined behavior.
    //STATIC_UNORDERED_MAP_STRING_CLASS((std::function<ImGuiWindowBase* ()>), ImGuiWindowBase, WindowGenerators);
    unordered_map_string_t<std::function<ImGuiWindowBase*()>>& ImGuiWindowBase::WindowGenerators(void)
    {
        static unordered_map_string_t<std::function<ImGuiWindowBase*()>>& WindowGenerators_(StaticManagement::UnorderedMapWrapper<string_t, std::function<ImGuiWindowBase*()>, Utility::StringHash, std::equal_to<>>::Create(
            {
#include "ImGuiWindows.def"
            }
        )); return WindowGenerators_;
    };

    ImGuiWindowBase* ImGuiWindowBase::GenerateWindow(string_view_arg name, size_t index)
    {
        auto iter = WindowGenerators().find(name);
        if (iter == WindowGenerators().end())
        {
            return nullptr;
        }
        WindowIndex_t oldGlobal = GlobalIndex;
        if (index < InvalidWindowIndex)
        {
            GlobalIndex = static_cast<WindowIndex_t>(index);
        }
        ImGuiWindowBase* window = iter->second();
        if (index < InvalidWindowIndex)
        {
            GlobalIndex = MathMax(oldGlobal, GlobalIndex);
        }
        return window;
    }

    unsigned ImGuiWindowBase::ReadStringArray(string_view_arg value, std::function<void(string_view_arg)> func)
    {
        unsigned count = 0;
        size_t startSlice = 0;
        size_t endSlice = value.find_first_of(',');
        while (endSlice != string_view_t::npos)
        {
            func(value.substr(startSlice, endSlice - startSlice));
            ++count;
            startSlice = endSlice + 1;
            endSlice = value.find_first_of(',', startSlice);
        }
        endSlice = value.size();
        if (startSlice < endSlice)
        {
            func(value.substr(startSlice, endSlice - startSlice));
            ++count;
        }
        return count;
    }

    void ImGuiWindowBase::ReadFinish(void)
    {
    }

#pragma endregion Private Static Functions

    //--------------------------------------------------------------------------
    // Private Functions:
    //--------------------------------------------------------------------------

#pragma region Private Functions

    void ImGuiWindowBase::SetIndex(size_t index)
    {
        index_ = static_cast<WindowIndex_t>(index);
        FORMAT_SET(internalName_, "{}##{}", GetName(), index_);
    }

    void ImGuiWindowBase::Destroy(void)
    {
        const char* const windowName = internalName_.c_str();
        ImGuiWindow* window = ImGui::FindWindowByName(windowName);
        [[maybe_unused]]
        ImGuiID windowID = ImGui::GetID(windowName, windowName + internalName_.size());

        ImGui::ClearWindowSettings(windowName);

#ifdef IMGUI_HAS_DOCK
        ImGui::DockBuilderRemoveNode(windowID);
#endif

        if (window != nullptr)
        {
            window->StateStorage.Clear();
        }
    }

#pragma endregion Private Functions

}	// namespace
