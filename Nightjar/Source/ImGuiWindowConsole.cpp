//------------------------------------------------------------------------------
//
// File Name:	ImGuiWindowConsole.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		ImGui window that displays console output.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiWindowConsole.h"

#include "ImGuiInclude.h"

#include "ImGuiWindowBase.inl"

//------------------------------------------------------------------------------
// Preprocessor Directives:
//------------------------------------------------------------------------------

#define USE_MESSAGE_TABLE

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

    constexpr string_view_t ImGuiWindowConsole::WindowName = "Console";

    constexpr size_t ImGuiWindowConsole::InvalidSelectionIndex = static_cast<decltype(InvalidSelectionIndex)>(-1);

    ENUM_MAPS_CLASS_SEQUENTIAL(ImGuiWindowConsole, ColorSettings, Timestamp, Spacestamp);

    enum MessageColumn : int
    {
        Severity,
        Category,
        Message,
        Time,
        Frame,
#ifdef _DEBUG_TRACE
        SourceFile,
        SourceLine,
        SourceFunc,
#endif
        NUM_ENTRIES,
    };

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

    ImGuiWindowConsole::ImGuiWindowConsole(void)
        : ImGuiWindowBase(WindowName, ImGuiWindowFlags_MenuBar)
        , meta_()
        , colorsSeverities_({
            Vec4(0.6f, 0.6f, 0.6f, 1.0f), // Debug
            Vec4(0.6f, 0.6f, 0.6f, 1.0f), // Verbose
            Vec4(1.0f, 1.0f, 1.0f, 1.0f), // Info
            Vec4(1.0f, 1.0f, 0.0f, 1.0f), // Warning
            Vec4(1.0f, 0.0f, 0.0f, 1.0f), // Error
            })
        , colorsSettings_({
            Vec4(0.6f, 0.6f, 0.6f, 1.0f), // Timestamp
            Vec4(0.6f, 0.6f, 0.6f, 1.0f), // Spacestamp
            })
        , enableColor_(true)
        , enableAutoscroll_(true)
        , enableFilterSeverity_(true)
        , enableFilterCategory_(false)
        , enableFilterString_(false)
        , enableFilterRegex_(false)
        , filterDirty_(false)
        , filterSeverities_((1 << (int)Trace::Severity::Info) | (1 << (int)Trace::Severity::Warning) | (1 << (int)Trace::Severity::Error))
        , filterCategories_()
        , filterString_()
        , filterRegex_()
        , lastMessageCount_()
        , messageSelected_(static_cast<decltype(messageSelected_)>(-1))
    {
    }

    ImGuiWindowConsole::ImGuiWindowConsole(ImGuiWindowConsole&& other) noexcept
        : ImGuiWindowBase(std::move(other))
        , meta_(std::move(other.meta_))
        , colorsSeverities_(std::move(other.colorsSeverities_))
        , colorsSettings_(std::move(other.colorsSettings_))
        , enableColor_(std::move(other.enableColor_))
        , enableAutoscroll_(std::move(other.enableAutoscroll_))
        , enableFilterSeverity_(std::move(other.enableFilterSeverity_))
        , enableFilterCategory_(std::move(other.enableFilterCategory_))
        , enableFilterString_(std::move(other.enableFilterString_))
        , enableFilterRegex_(std::move(other.enableFilterRegex_))
        , filterDirty_(std::move(other.filterDirty_))
        , filterSeverities_(std::move(other.filterSeverities_))
        , filterCategories_(std::move(other.filterCategories_))
        , filterString_(std::move(other.filterString_))
        , filterRegex_(std::move(other.filterRegex_))
        , lastMessageCount_(std::move(other.lastMessageCount_))
        , messageSelected_(std::move(other.messageSelected_))
    {
    }

    ImGuiWindowConsole::MessageMetadata::MessageMetadata(size_t index_, const Trace::MessageData& messageInfo)
        : index(index_)
        , timestamp("00:00:00.0000")
        , framestamp("00000000")
    {
        {
            size_t time = static_cast<size_t>(messageInfo.time);
            size_t timeMs = static_cast<size_t>(messageInfo.time * 10000.0);
            timestamp.Format("{:02}:{:02}:{:02}:{:04}", time / 3600, (time / 60) % 60, time % 60, timeMs % 10000);
        }
        {
            framestamp.Format("{: 7}", messageInfo.frame);
        }
    }

    ImGuiWindowConsole::MessageMetadata::MessageMetadata(MessageMetadata&& other) noexcept
        : index(other.index)
        , timestamp(std::move(other.timestamp))
        , framestamp(std::move(other.framestamp))
    {
    }

    //--------------------------------------------------------------------------

    ImGuiWindowConsole::~ImGuiWindowConsole(void)
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

#define GET_COLOR_SEVERITY_VAL(value) colorsSeverities_[static_cast<uint_fast8_t>(value)]
#define GET_COLOR_SEVERITY(name) GET_COLOR_SEVERITY_VAL(Trace::Severity::name)
#define GET_COLOR_SETTINGS_VAL(value) colorsSettings_[static_cast<uint_fast8_t>(value)]
#define GET_COLOR_SETTINGS(name) GET_COLOR_SETTINGS_VAL(ColorSettings::name)

#pragma endregion Private Static Functions

    //--------------------------------------------------------------------------
    // Private Functions:
    //--------------------------------------------------------------------------

#pragma region Private Functions

    void ImGuiWindowConsole::DisplayImpl(void)
    {
        DisplayMenuBar();
        DisplayMessages();
        if (messageSelected_ != InvalidSelectionIndex)
        {
            DisplayMessageDetails();
        }
    }

    ImGuiWindowBase::ReadError ImGuiWindowConsole::ReadKeyValue(string_view_arg key, string_view_arg value)
    {
        bool parseSuccess;
        switch (READKEY_HASH(key))
        {
        READKEY_CHECK("ColorTimestamp")
            READKEY_COLOR(GET_COLOR_SETTINGS(Timestamp));
            return READKEY_RETURN;
        READKEY_CHECK("ColorSpacestamp")
            READKEY_COLOR(GET_COLOR_SETTINGS(Spacestamp));
            return READKEY_RETURN;
        READKEY_CHECK("ColorDebug")
            READKEY_COLOR(GET_COLOR_SEVERITY(Debug));
            return READKEY_RETURN;
        READKEY_CHECK("ColorVerbose")
            READKEY_COLOR(GET_COLOR_SEVERITY(Verbose));
            return READKEY_RETURN;
        READKEY_CHECK("ColorInfo")
            READKEY_COLOR(GET_COLOR_SEVERITY(Info));
            return READKEY_RETURN;
        READKEY_CHECK("ColorWarning")
            READKEY_COLOR(GET_COLOR_SEVERITY(Warning));
            return READKEY_RETURN;
        READKEY_CHECK("ColorError")
            READKEY_COLOR(GET_COLOR_SEVERITY(Error));
            return READKEY_RETURN;
        READKEY_CHECK("EnableAutoscroll")
            READKEY_VALUE(enableAutoscroll_);
            return READKEY_RETURN;
        READKEY_CHECK("EnableColor")
            READKEY_VALUE(enableColor_);
            return READKEY_RETURN;
        READKEY_CHECK("EnableFilterSeverity")
            READKEY_VALUE(enableFilterSeverity_);
            return READKEY_RETURN;
        READKEY_CHECK("EnableFilterCategory")
            READKEY_VALUE(enableFilterCategory_);
            return READKEY_RETURN;
        READKEY_CHECK("EnableFilterString")
            READKEY_VALUE(enableFilterString_);
            return READKEY_RETURN;
        READKEY_CHECK("EnableFilterRegex")
            READKEY_VALUE(enableFilterRegex_);
            return READKEY_RETURN;
        READKEY_CHECK("FilterSeverities")
            READKEY_VALUE(filterSeverities_);
            return READKEY_RETURN;
        READKEY_CHECK("FilterCategories")
            ReadStringArray(value, [this](string_view_arg val)
            {
                if (!val.empty())
                {
                    filterCategories_.emplace(val, true);
                }
            });
            return ReadError::None;
        READKEY_CHECK("FilterString")
            filterString_ = value;
            return ReadError::None;
        default:
            break;
        }
        return ReadError::InvalidKey;
    }

    void ImGuiWindowConsole::ReadFinish(void)
    {
        if (enableFilterRegex_)
        {
            filterRegex_ = filterString_;
        }
    }

    void ImGuiWindowConsole::WriteAll(string_t& buffer)
    {
#define WRITE_BOOL_C(name) WRITE_BOOL("Enable" #name , enable##name##_)
#define WRITE_COLOR_C(name, var) WRITE_COLOR("Color" #name, var)
#define WRITE_COLOR_SEVERITY(name) WRITE_COLOR_C(name, GET_COLOR_SEVERITY(name))
#define WRITE_COLOR_SETTING(name) WRITE_COLOR_C(name, GET_COLOR_SETTINGS(name))

        WRITE_BOOL_C(Autoscroll);
        WRITE_BOOL_C(Color);

        WRITE_COLOR_SETTING(Timestamp);
        WRITE_COLOR_SETTING(Spacestamp);
        WRITE_COLOR_SEVERITY(Debug);
        WRITE_COLOR_SEVERITY(Verbose);
        WRITE_COLOR_SEVERITY(Info);
        WRITE_COLOR_SEVERITY(Warning);
        WRITE_COLOR_SEVERITY(Error);

        WRITE_BOOL_C(FilterSeverity);
        WRITE_BOOL_C(FilterCategory);
        WRITE_BOOL_C(FilterString);
        WRITE_BOOL_C(FilterRegex);
        WRITE_VALUE("FilterSeverities", filterSeverities_);
        {
            buffer += "FilterCategories=";
            bool added = false;
            for (const auto& [catName, enabled] : filterCategories_)
            {
                if (enabled)
                {
                    if (added)
                    {
                        buffer += ',';
                    }
                    added = true;
                    buffer += catName;
                }
            }
            buffer += '\n';
        }
        WRITE_STRING("FilterString", filterString_);
    }

    void ImGuiWindowConsole::DisplayMenuBar(void)
    {
        if (!ImGui::BeginMenuBar())
        {
            return;
        }

        if (ImGui::BeginMenu("Settings"))
        {
            ImGui::Checkbox("Autoscroll", &enableAutoscroll_);
            ImGui::CreateHoverTooltip("Enable automatically scrolling to the bottom when a new message appears.");

            ImGui::Checkbox("Text Colors", &enableColor_);
            ImGui::CreateHoverTooltip("Enable text coloration.");

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Filters"))
        {
            filterDirty_ |= ImGui::Checkbox("Filter Severities", &enableFilterSeverity_);
            ImGui::CreateHoverTooltip("Enable filtering messages by severity level.");

            if (ImGui::BeginMenu("Severities"))
            {
                bool severityChanged = false;
                for (unsigned int i = 0; i < static_cast<unsigned int>(Trace::Severity::NUM_ENTRIES); ++i)
                {
                    severityChanged |= ImGui::CheckboxFlags(ENUM_WRITE_VALUE(Trace::Severity, i).data(), &filterSeverities_, 1u << i);
                }

                filterDirty_ |= severityChanged && enableFilterSeverity_;

                ImGui::EndMenu();
            }

            filterDirty_ |= ImGui::Checkbox("Filter Categories", &enableFilterCategory_);
            ImGui::CreateHoverTooltip("Enable filtering messages by category name.");

            if (ImGui::BeginMenu("Categories"))
            {
                bool categoryChanged = false;
                for (auto& [catName, enabled] : filterCategories_)
                {
                    categoryChanged |= ImGui::Checkbox(catName.data(), &enabled);
                }

                filterDirty_ |= categoryChanged && enableFilterCategory_;

                ImGui::EndMenu();
            }

            filterDirty_ |= ImGui::Checkbox("Filter String", &enableFilterString_);
            ImGui::CreateHoverTooltip("Enable filtering messages by searching for a string within them.");

            filterDirty_ |= ImGui::Checkbox("Filter Regex", &enableFilterRegex_);
            ImGui::CreateHoverTooltip("Enable filtering messages by using the filter string as a regular expression. Requires filter string to be enabled.");

            filterDirty_ |= ImGui::InputText("Filter", &filterString_, ImGuiInputTextFlags_CallbackCompletion) && enableFilterString_;

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Colors"))
        {
            for (uint_fast16_t i = 0; i < colorsSettings_.size(); ++i)
            {
                ImGui::ColorEdit4(ENUM_WRITE_VALUE(ColorSettings, i).data(), &colorsSettings_[i].r);
            }

            ImGui::Separator();

            for (uint_fast16_t i = 0; i < colorsSeverities_.size(); ++i)
            {
                ImGui::ColorEdit4(ENUM_WRITE_VALUE(Trace::Severity, i).data(), &colorsSeverities_[i].r);
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    void ImGuiWindowConsole::DisplayMessages(void)
    {
#ifdef USE_MESSAGE_TABLE
        static constexpr ImGuiTableFlags flagsTable = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY;
        if (!ImGui::BeginTable("MessagesScroll", MessageColumn::NUM_ENTRIES, flagsTable, contentSize_, 0))
        {
            return;
        }
        ImGui::TableSetupScrollFreeze(0, 1);
        static constexpr ImGuiTableFlags flagsColumn = ImGuiTableColumnFlags_WidthFixed;
        ImGui::TableSetupColumn("Severity", flagsColumn);
        ImGui::TableSetupColumn("Category", flagsColumn);
        ImGui::TableSetupColumn("Message", flagsColumn);
        ImGui::TableSetupColumn("Timestamp", flagsColumn);
        ImGui::TableSetupColumn("Framestamp", flagsColumn);
#ifdef _DEBUG_TRACE
        ImGui::TableSetupColumn("Source File", flagsColumn);
        ImGui::TableSetupColumn("Line #", flagsColumn);
        ImGui::TableSetupColumn("Source Function", flagsColumn);
#endif
        ImGui::TableHeadersRow();
#else
        if (!ImGui::BeginChild("MessagesScroll", contentSize_))
        {
            return;
        }

        // Wrap messages.
        ImGui::PushTextWrapPos();
#endif

        const auto& messages = Trace::GetHistory();
        size_t historySize = messages.size();
        bool historySizeUpdated = lastMessageCount_ != historySize;

        /*if (meta_.capacity() < messages.capacity())
        {
            meta_.reserve(messages.capacity());
        }*/
        for (; lastMessageCount_ < historySize; ++lastMessageCount_)
        {
            meta_.emplace_back(lastMessageCount_, messages[lastMessageCount_]);
            if (filterCategories_.find(messages[lastMessageCount_].category) == filterCategories_.end())
            {
                filterCategories_.emplace(messages[lastMessageCount_].category, !enableFilterCategory_);
            }
        }
        lastMessageCount_ = historySize;

        bool rowNotSelected = true;
        bool rowAwaitingSelect = rowNotSelected;
        size_t i = 0;

        auto DoRender = [this, &rowAwaitingSelect, &rowNotSelected, &i](string_view_arg text, const Vec4& color)
        {
            if (rowAwaitingSelect)
            {
                ImVec2 pos = ImGui::GetCursorPos();
                rowAwaitingSelect = false;
                bool rowSelected = messageSelected_ == static_cast<decltype(messageSelected_)>(i);
                if (ImGui::Selectable("###TABLE_SELECTION", rowSelected, ImGuiSelectableFlags_SpanAllColumns))
                {
                    messageSelected_ = static_cast<decltype(messageSelected_)>(i);
                    rowNotSelected = false;
                }
                ImGui::SetCursorPos(pos);
            }
            if (enableColor_)
            {
                ImGui::PushStyleColor(ImGuiCol_Text, color);
                ImGui::TextUnformatted(text);
                ImGui::PopStyleColor();
            }
            else
            {
                ImGui::TextUnformatted(text);
            }
        };

        ImGuiListClipper clipper;
        clipper.Begin(static_cast<int>(meta_.size()));

        while (clipper.Step())
        {
            decltype(i) endpoint = static_cast<decltype(i)>(clipper.DisplayEnd);
            for (i = static_cast<decltype(i)>(clipper.DisplayStart); i < endpoint; ++i)
            {
                const auto& meta = meta_[i];
                const auto& messageInfo = messages[meta.index];
#ifdef _DEBUG_TRACE
                const auto& source = messageInfo.source;
#endif

                ImGui::PushID(reinterpret_cast<void*>(meta.index));
    #ifdef USE_MESSAGE_TABLE
                ImGui::TableNextRow();

                for (int j = 0; j < MessageColumn::NUM_ENTRIES; ++j)
                {
                    if (!ImGui::TableSetColumnIndex(j))
                    {
                        continue;
                    }
                    switch (j)
                    {
                    case MessageColumn::Severity:
                        DoRender(ENUM_WRITE_VALUE(Trace::Severity, messageInfo.severity), GET_COLOR_SEVERITY_VAL(messageInfo.severity));
                        break;
                    case MessageColumn::Category:
                        DoRender(messageInfo.category, GET_COLOR_SEVERITY_VAL(messageInfo.severity));
                        break;
                    case MessageColumn::Message:
                        DoRender(messageInfo.message, GET_COLOR_SEVERITY_VAL(messageInfo.severity));
                        break;
                    case MessageColumn::Time:
                        DoRender(meta.timestamp, GET_COLOR_SETTINGS(Timestamp));
                        break;
                    case MessageColumn::Frame:
                        DoRender(meta.framestamp, GET_COLOR_SETTINGS(Timestamp));
                        break;
    #ifdef _DEBUG_TRACE
                    case MessageColumn::SourceFile:
                        DoRender(source.GetSourceFile(), GET_COLOR_SETTINGS(Spacestamp));
                        break;
                    case MessageColumn::SourceLine:
                        DoRender(source.GetSourceLineStr(), GET_COLOR_SETTINGS(Spacestamp));
                        break;
                    case MessageColumn::SourceFunc:
                        DoRender(source.GetSourceFunction(), GET_COLOR_SETTINGS(Spacestamp));
                        break;
    #endif
                    }
                }

                rowAwaitingSelect = rowNotSelected;
    #endif
                ImGui::PopID();
            }
        }

        clipper.End();

#ifndef USE_MESSAGE_TABLE
        // Done wrapping messages.
        ImGui::PopTextWrapPos();
#endif

        if (enableAutoscroll_ && historySizeUpdated)
        {
            ImGui::SetScrollHereY(1.0f);
        }

#ifdef USE_MESSAGE_TABLE
        ImGui::EndTable();
#else
        ImGui::EndChild();
#endif
    }

    void ImGuiWindowConsole::DisplayMessageDetails(void)
    {
    }

#pragma endregion Private Functions

}	// namespace
