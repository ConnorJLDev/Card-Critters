//------------------------------------------------------------------------------
//
// File Name:	ImGuiLayoutManager.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Manages saving, loading, and processing ImGui settings files.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiLayoutManager.h"
#include "ImGuiSystem.h"

#include "ImGuiInclude.h"
#include "imgui_internal.h"

#include "ImGuiWindowManager.h"
#include "ImGuiWindowBase.h"

#include "IOSystem.h"

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

    // File suffix for settings files.
    static constexpr string_view_t FileSuffix = ".ini";

    // Default subdirectory to store settings files in.
    static constexpr string_view_t DefaultSubDir = "Editor/Layouts/";
    // Default settings file name.
    static constexpr string_view_t DefaultLayoutName = "Default";

    // File name for the temporary file.
    static constexpr string_view_t TempName = "temp";

    enum SettingsFindResult : int
    {
        SFR_NotFound,       // File was not found.
        SFR_FoundAvailable, // File was found within the available settings directory.
        SFR_FoundItself,    // File was found because the name is the entire path to the file.
    };

    //--------------------------------------------------------------------------
    // Private Static Variables:
    //--------------------------------------------------------------------------

    ImGuiLayoutManager* ImGuiLayoutManager::Settings = nullptr;
#define inst ImGuiLayoutManager::Settings

    //--------------------------------------------------------------------------
    // Private Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Private Static Local Functions:
    //--------------------------------------------------------------------------

    /// <summary>
    /// Consistently formats a given name for scanning files for.
    /// </summary>
    /// <param name="name">The name of the setting file being searched for.</param>
    /// <param name="buffer">Buffer to store the formatted string to.</param>
    /// <param name="base">The base directory to start looking in.</param>
    /// <returns>A string view of the buffer that only has the name with a confirmed extension.</returns>
    static string_view_t ConfirmExtension(string_view_arg name, string_t& buffer, string_view_arg base)
    {
        const bool hasSuffix = name.ends_with(FileSuffix);
        const size_t apathSize = base.size();
        buffer = base;
        buffer += name;
        if (!hasSuffix)
        {
            buffer += FileSuffix;
        }
        IOSystem::ValidatePathSeparators(buffer);
        string_view_t tempName(buffer.c_str() + apathSize, buffer.size() - apathSize);
        return tempName;
    }

    //--------------------------------------------------------------------------
    // Constructors/Destructors:
    //--------------------------------------------------------------------------

#pragma region Constructors

    ImGuiLayoutManager::ImGuiLayoutManager(void)
        : autosave_(true)
        , tempAutosave_()
        , isWriteable_(true)
        , currentPath_()
        , currentName_()
        , layoutsDir_()
        , layoutsFound_()
        , tempPath_()
    {
        assert(inst == nullptr);
        inst = this;
    }

    //--------------------------------------------------------------------------

    ImGuiLayoutManager::~ImGuiLayoutManager(void)
    {
        assert(inst == this);
        inst = nullptr;
    }

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

    bool ImGuiLayoutManager::GetAutosave(void)
    {
        return inst->autosave_;
    }

    bool ImGuiLayoutManager::SetAutosave(bool enabled)
    {
        inst->autosave_ = enabled;
        auto& io = ImGui::GetIO();
        if (enabled && IOSystem::FileExists(inst->currentPath_))
        {
            io.IniFilename = inst->currentPath_.c_str();
            return true;
        }
        else if (inst->tempAutosave_)
        {
            io.IniFilename = inst->tempPath_.c_str();
        }
        else
        {
            io.IniFilename = nullptr;
        }
        return false;
    }

    bool ImGuiLayoutManager::GetTempAutosave(void)
    {
        return inst->tempAutosave_;
    }

    void ImGuiLayoutManager::SetTempAutosave(bool enabled)
    {
        inst->tempAutosave_ = enabled;
        if (!inst->autosave_)
        {
            auto& io = ImGui::GetIO();
            if (inst->tempAutosave_)
            {
                io.IniFilename = inst->tempPath_.c_str();
            }
            else
            {
                io.IniFilename = nullptr;
            }
        }
    }

    bool ImGuiLayoutManager::LoadLayout(string_view_arg name)
    {
        // If the name is empty, load the current settings.
        if (name.empty())
        {
            bool valid = IOSystem::FileExists(inst->currentPath_);
            if (valid)
            {
                ImGui::LoadIniSettingsFromDisk(inst->currentPath_.c_str());
            }
            return valid;
        }
        // Else, find the named settings and load them if possible.
        auto [settingsPath, foundCode] = inst->GetSettings(name);
        switch (foundCode)
        {
        case SFR_NotFound:
            return false;
        case SFR_FoundAvailable:
        case SFR_FoundItself:
            inst->currentPath_ = settingsPath;
            inst->SetActiveSettings();
            break;
        default:
            assert(false);
            return false;
        }
        ImGui::LoadIniSettingsFromDisk(inst->currentPath_.c_str());
        return true;
    }

    bool ImGuiLayoutManager::SaveLayout(void)
    {
        const bool valid = IOSystem::FileExists(inst->currentPath_);
        if (valid)
        {
            ImGui::SaveIniSettingsToDisk(inst->currentPath_.c_str());
        }
        return valid;
    }

    bool ImGuiLayoutManager::CreateLayout(string_view_arg name, bool setActive, string_view_arg copyFrom)
    {
        // Name cannot be empty.
        if (name.empty())
        {
            return false;
        }
        string_t tempStr;
        ConfirmExtension(name, tempStr, inst->layoutsDir_);
        // File must not exist.
        if (IOSystem::FileExists(tempStr))
        {
            return false;
        }
        bool success;
        if (copyFrom.empty())
        {
            ImGui::SaveIniSettingsToDisk(tempStr.c_str());
            success = true;
        }
        else
        {
            auto [settingsPath, foundCode] = inst->GetSettings(copyFrom);
            switch (foundCode)
            {
            case SFR_NotFound:
                success = false;
                break;
            case SFR_FoundAvailable:
            case SFR_FoundItself:
                success = IOSystem::Copy(settingsPath, tempStr, false);
                break;
            default:
                assert(false);
                success = false;
                break;
            }
        }
        if (setActive && success)
        {
            inst->currentPath_ = tempStr;
            inst->SetActiveSettings();
        }
        return success;
    }

    bool ImGuiLayoutManager::RenameLayout(string_view_arg name)
    {
        // Name cannot be empty.
        if (name.empty())
        {
            return false;
        }
        string_t tempStr;
        ConfirmExtension(name, tempStr, inst->layoutsDir_);
        // File must not exist.
        if (IOSystem::FileExists(tempStr))
        {
            return false;
        }
        // Rename.
        bool success = IOSystem::Rename(inst->currentPath_, tempStr);
        if (success)
        {
            inst->currentPath_ = tempStr;
            inst->SetActiveSettings();
        }
        return success;
    }

    bool ImGuiLayoutManager::RenameLayout(string_view_arg from, string_view_arg to)
    {
        // Name cannot be empty.
        if (from.empty() || to.empty())
        {
            return false;
        }
        string_t tempStr;
        ConfirmExtension(to, tempStr, inst->layoutsDir_);
        // File must not exist.
        if (IOSystem::FileExists(tempStr))
        {
            return false;
        }
        auto [settingsPath, foundCode] = inst->GetSettings(from);
        switch (foundCode)
        {
        case SFR_NotFound:
            return false;
        case SFR_FoundAvailable:
        case SFR_FoundItself:
            break;
        default:
            assert(false);
            return false;
        }
        // Rename.
        return IOSystem::Rename(settingsPath, tempStr);
    }

    bool ImGuiLayoutManager::DestroyLayout(string_view_arg name)
    {
        auto [settingsPath, foundCode] = inst->GetSettings(name);
        switch (foundCode)
        {
        case SFR_NotFound:
            return false;
        case SFR_FoundAvailable:
        case SFR_FoundItself:
            break;
        default:
            assert(false);
            return false;
        }
        IOSystem::Destroy(settingsPath, true, false);
        return true;
    }

    string_view_t ImGuiLayoutManager::GetLayoutPath(void)
    {
        return inst->currentPath_;
    }

    string_view_t ImGuiLayoutManager::GetLayoutName(void)
    {
        return inst->currentName_;
    }

    string_view_t ImGuiLayoutManager::GetLayoutDirectory(void)
    {
        return inst->layoutsDir_;
    }

    void ImGuiLayoutManager::SetLayoutDirectory(string_view_arg directory)
    {
        inst->layoutsDir_ = directory;
        IOSystem::ForceDirectory(inst->layoutsDir_);
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

    bool ImGuiLayoutManager::Init(void)
    {
        {
            const string_view_t name = ImGuiSystem::GetInstance()->GetName();
            ImGuiSettingsHandler settingsHandler;
            settingsHandler.TypeName = name.data();
            settingsHandler.TypeHash = ImHashStr(name.data());
            settingsHandler.ClearAllFn = CallbackSettingsClearAll;
            settingsHandler.ApplyAllFn = CallbackSettingsApplyAll;
            settingsHandler.ReadInitFn = CallbackSettingsReadInit;
            settingsHandler.ReadOpenFn = CallbackSettingsReadOpen;
            settingsHandler.ReadLineFn = CallbackSettingsReadLine;
            settingsHandler.WriteAllFn = CallbackSettingsWriteAll;
            settingsHandler.UserData = nullptr;
            ImGui::AddSettingsHandler(&settingsHandler);
        }

        {
            inst->tempPath_ = IOSystem::GetTemporaryPath();
            inst->tempPath_ += TempName;
            inst->tempPath_ += FileSuffix;
        }

        {
            inst->layoutsDir_ = IOSystem::GetUserDataPath();
            inst->layoutsDir_ += DefaultSubDir;
            IOSystem::ForceDirectory(inst->layoutsDir_);
            string_t& curPath = inst->currentPath_;
            curPath = DefaultSubDir;
            curPath += DefaultLayoutName;
            curPath += FileSuffix;
            inst->isWriteable_ = false;
            inst->SetActiveSettings();
        }

        return true;
    }

    void ImGuiLayoutManager::Exit(void)
    {
    }

    void ImGuiLayoutManager::CallbackSettingsClearAll([[maybe_unused]] ImGuiContext* context, [[maybe_unused]] ImGuiSettingsHandler* handler)
    {
    }

    void ImGuiLayoutManager::CallbackSettingsReadInit([[maybe_unused]] ImGuiContext* context, [[maybe_unused]] ImGuiSettingsHandler* handler)
    {
        TraceVerbose("ImGui settings read initialization: \"{}\"", handler->TypeName);
    }

    void* ImGuiLayoutManager::CallbackSettingsReadOpen([[maybe_unused]] ImGuiContext* context, ImGuiSettingsHandler* handler, const char* name)
    {
        TraceVerbose("ImGui settings read opening: \"{}\" \"{}\"", handler->TypeName, name);
        const string_view_t nameView(name);

        if (nameView == ImGuiWindowManager::DockActiveWindowsName)
        {
            handler->UserData = &ImGuiWindowManager::Windows->windowsActive_;
            return (void*)true;
        }

        const size_t nameBaseEnd(nameView.find_first_of('#'));

        const string_view_t nameBase(name, nameBaseEnd);

        const string_view_t nameIndex(nameView.substr(nameBaseEnd + 2));

        size_t index = std::numeric_limits<size_t>::max();

        TryParseValue(nameIndex, index);

        // Supposed to be "window" but typoed and found it funny.
        ImGuiWindowBase* wubdiw = ImGuiWindowBase::GenerateWindow(nameBase, index);

        bool success = wubdiw != nullptr;

        if (success)
        {
            TraceInfo("Opening window: \"{}\"", name);
            ImGuiWindowManager::Windows->windows_.push_back(wubdiw);
            wubdiw->loaded_ = true;
            handler->UserData = wubdiw;
        }
        else
        {
            TraceWarning("Failed to identify window name while reading: \"{}\"", name);
            handler->UserData = nullptr;
        }

        return (void*)success;
    }

    void ImGuiLayoutManager::CallbackSettingsReadLine([[maybe_unused]] ImGuiContext* context, ImGuiSettingsHandler* handler, [[maybe_unused]] void* entry, const char* line)
    {
        if (handler->UserData == nullptr)
        {
            return;
        }

        const string_view_t lineView(line);

        if (lineView.empty())
        {
            return;
        }

        const size_t index(lineView.find_first_of('='));
        const string_view_t lineKey(lineView.substr(0, index)), lineValue(lineView.substr(index + 1));

        if (handler->UserData == &ImGuiWindowManager::Windows->windowsActive_)
        {
            if (lineKey != ImGuiWindowManager::DockWindowKeyName)
            {
                TraceWarning("Unknown line window \"{}\" when reading \"{}\"", lineView, ImGuiWindowManager::DockActiveWindowsName);
                return;
            }
            unsigned int dockID = static_cast<unsigned int>(ImGuiWindowManager::Windows->windowsActive_.size());
            ImGuiWindowManager::Windows->windowsActive_.emplace(dockID, lineValue);
            return;
        }

        TraceVerbose("ImGui settings reading window data: \"{}\"", lineView);

        ImGuiWindowBase* window = static_cast<ImGuiWindowBase*>(handler->UserData);

        using ReadError = ImGuiWindowBase::ReadError;
        ReadError readError = window->ReadKeyValue(lineKey, lineValue);
        switch (readError)
        {
        case ReadError::None:
            break;
        case ReadError::InvalidKey:
            TraceWarning("Unknown key \"{}\" when reading \"{}\"", lineKey, window->internalName_);
            break;
        case ReadError::InvalidValue:
            TraceWarning("Invalid value \"{}\" for key \"{}\" when reading \"{}\"", lineValue, lineKey, window->internalName_);
            break;
        default:
            assert(false);
            TraceWarning("Unknown error when reading \"{}\"", lineKey, window->internalName_);
            break;
        }
    }

    void ImGuiLayoutManager::CallbackSettingsApplyAll([[maybe_unused]] ImGuiContext* context, [[maybe_unused]] ImGuiSettingsHandler* handler)
    {
        for (ImGuiWindowBase* window : ImGuiWindowManager::Windows->windows_)
        {
            window->ReadFinish();
        }
    }

    void ImGuiLayoutManager::CallbackSettingsWriteAll([[maybe_unused]] ImGuiContext* context, ImGuiSettingsHandler* handler, ImGuiTextBuffer* buffer)
    {
        string_t& bufferStr = ImGuiWindowManager::Windows->windowsWriteBuffer_;

        {
            std::erase_if(ImGuiWindowManager::Windows->windowsActive_, [](const auto& iter)
            {
                ImGuiDockNode* node = ImGui::DockBuilderGetNode(iter.first);
                ImGuiWindow* window = ImGui::FindWindowByName(iter.second.c_str());
                return node == nullptr || window == nullptr || window->DockNode != node || !window->DockIsActive;
            });
            // Write the active window in each docker node.
            bufferStr.clear();
            for (const auto& [dockId, name] : ImGuiWindowManager::Windows->windowsActive_)
            {
                FORMAT_TO(bufferStr, "{}={}\n", ImGuiWindowManager::DockWindowKeyName, name);
            }
            bufferStr += '\0'; // Guarantee null termination.
            buffer->appendf("[%s][%s]\n%s\n", handler->TypeName, ImGuiWindowManager::DockActiveWindowsName.data(), bufferStr.c_str());
        }

        for (ImGuiWindowBase* window : ImGuiWindowManager::Windows->windows_)
        {
            bufferStr.clear();
            window->WriteAll(bufferStr);
            bufferStr += '\0'; // Guarantee null termination.

            buffer->appendf("[%s][%s]\n%s\n", handler->TypeName, window->internalName_.c_str(), bufferStr.c_str());
        }
    }

#pragma endregion Private Static Functions

    //--------------------------------------------------------------------------
    // Private Functions:
    //--------------------------------------------------------------------------

#pragma region Private Functions

    void ImGuiLayoutManager::SetActiveSettings(void)
    {
        const char*& ioPath = ImGui::GetIO().IniFilename;
        if (autosave_ && isWriteable_)
        {
            ioPath = currentPath_.c_str();
        }
        else if (tempAutosave_)
        {
            ioPath = tempPath_.c_str();
        }
        else
        {
            ioPath = nullptr;
        }
        const size_t pathSize = currentPath_.find_last_of('/');
        currentName_ = string_view_t(currentPath_.c_str() + pathSize, currentPath_.size() - pathSize - FileSuffix.size());
    }

    std::pair<string_view_t, int> ImGuiLayoutManager::GetSettings(string_view_arg name)
    {
        const bool hasSuffix = name.ends_with(FileSuffix);
        const size_t apathSize = layoutsDir_.size();
        string_t& tempStr = writeBuffer_;
        tempStr = layoutsDir_;
        tempStr += name;
        if (!hasSuffix)
        {
            tempStr += FileSuffix;
        }
        IOSystem::ValidatePathSeparators(tempStr);
        string_view_t tempName(tempStr.c_str() + apathSize, tempStr.size() - apathSize);
        // First look in availableSettings...
        if (IOSystem::FileExists(tempStr))
        {
            return std::pair<string_view_t, int>(tempStr, SFR_FoundAvailable);
        }
        // Then look for just the name.
        else if (IOSystem::FileExists(tempName))
        {
            return std::pair<string_view_t, int>(tempName, SFR_FoundItself);
        }
        return std::pair<string_view_t, int>("", SFR_NotFound);
    }

#pragma endregion Private Functions

}	// namespace
