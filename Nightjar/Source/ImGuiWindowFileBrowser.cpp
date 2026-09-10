//------------------------------------------------------------------------------
//
// File Name:	ImGuiWindowFileBrowser.cpp
// Author(s):	Barend Brink (barend.brink), Connor Lariviere (connor.lariviere), Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		ImGui window that displays files within the game directory.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiWindowFileBrowser.h"

#include "ImGuiInclude.h"

#include "ImGuiWindowBase.inl"

#include "SceneSystem.h"
#include "Scene.h"
#include "EntityContainer.h"
#include "Entity.h"

#include <filesystem>

//------------------------------------------------------------------------------
// Preprocessor Directives:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

namespace fs = std::filesystem;

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

    constexpr string_view_t ImGuiWindowFileBrowser::WindowName = "File Browser";

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

    ImGuiWindowFileBrowser::ImGuiWindowFileBrowser(void)
        : ImGuiWindowBase(WindowName, 0/*ImGuiWindowFlags_MenuBar*/)
        , searchFilter_("")
        , files_()
        , startPath_("Data")
        , selectedFile_()
        , initialized_(false)
        , refresh_(false)
    {
    }

    ImGuiWindowFileBrowser::ImGuiWindowFileBrowser(ImGuiWindowFileBrowser&& other) noexcept
        : ImGuiWindowBase(std::move(other))
        , searchFilter_(std::move(other.searchFilter_))
        , files_(std::move(other.files_))
        , startPath_(std::move(other.startPath_))
        , selectedFile_(std::move(other.selectedFile_))
        , initialized_(std::move(other.initialized_))
        , refresh_(std::move(other.refresh_))
    {
    }

    //--------------------------------------------------------------------------

    ImGuiWindowFileBrowser::~ImGuiWindowFileBrowser(void)
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

    void ImGuiWindowFileBrowser::DisplayImpl(void)
    {
        EntityContainer* container;
        Scene* scene = SceneSystem::GetScene(0);
        if (scene == nullptr)
        {
            return;
        }
        container = scene->FindContainer();
        if (!container)
        {
            return;
        }

        searchFilter_.Draw("Search", 260.0f - 100.0f);

        //We check if there has been leaded entries into our "directory"
        if (!initialized_)
        {
            files_.clear();
            for (const auto& entry : fs::directory_iterator(startPath_)) //for every entry move forward
            {

                if (entry.is_regular_file() || entry.is_directory())
                {
                    std::string fname = entry.path().filename().string();
                    files_.push_back(CONVERT_STR_INL(fname));
                }

            }
            initialized_ = true;
        }

        ImGui::Text("Browsing: {%s}", startPath_.c_str()); //This updates as the start Path updates.
        ImGui::Separator();
        if (startPath_ != "Data")
            if (ImGui::Button("<- Back")) //Confirms and adds our old code from "add" buttom
            {
                startPath_ = "Data";
            }
        for (size_t i = 0; i < files_.size(); ++i) //Goes through an iterator for file  array and decides which is selected based on clicked. (lets us know what is selected)
        {
            const string_t& file = files_[i];
            string_t fullPath = startPath_ + "/" + file;
            bool isDir = fs::is_directory(fullPath);
            bool isSelected = (selectedFile_ == file);

            const char* name = file.c_str();
            if (!searchFilter_.PassFilter(name)) continue;

            // Color folders differently
            if (isDir)
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(50.0f, .9f, 50.0f, 1.0f));
            else
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.9f, 0.9f, 1.0f));

            if (ImGui::Selectable(name, isSelected))
                selectedFile_ = file;

            ImGui::PopStyleColor();

            // Handle double-click navigation
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && isDir)
            {
                startPath_ += "/" + selectedFile_;
                refresh_ = true;
            }
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && !isDir) //DO MORE STUFF HERE?
            {
                string_t fullPath2 = startPath_ + "/" + selectedFile_;

                if (startPath_.ends_with("Data/Entities"))
                {
                    Entity* entity = new Entity(fullPath2);
                    entity->Read(fullPath2);

                    container->Add(entity);
                    auto& ctx = ImGuiContextData::GetContext();
                    ctx.selectedEntity_ = entity;
                }
                else if (startPath_.ends_with("Data/Scenes"))
                {
                    //selectedEntity_ = nullptr;
                    //sSelectedIdx = -1;
                    //IMGUIisRestarting = true;
                    SceneSystem::SetNext(string_view_t(selectedFile_).substr(0, selectedFile_.size() - 5));

                }
            }
        }
        ImGui::Separator();

        if (ImGui::Button("Refresh") || refresh_) //Refreshes folder if we add more files! Found from youtube video as help!
        {
            files_.clear();
            for (const auto& entry : fs::directory_iterator(startPath_))
            {
                if (entry.is_regular_file() || entry.is_directory())
                {
                    std::string fname = entry.path().filename().string();
                    files_.push_back(CONVERT_STR_INL(fname));
                }
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Confirm") && !selectedFile_.empty()) //Confirms and adds our old code from "add" buttom
        {
            string_t fullPath = startPath_ + "/" + selectedFile_;

            Entity* entity = new Entity(fullPath);
            entity->Read(fullPath);

            container->Add(entity);
        }
        ImGui::SameLine();

    }

    ImGuiWindowBase::ReadError ImGuiWindowFileBrowser::ReadKeyValue([[maybe_unused]] string_view_arg key, [[maybe_unused]] string_view_arg value)
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

    void ImGuiWindowFileBrowser::ReadFinish(void)
    {
    }

    void ImGuiWindowFileBrowser::WriteAll([[maybe_unused]] string_t& buffer)
    {
    }

#pragma endregion Private Functions

}	// namespace
