//------------------------------------------------------------------------------
//
// File Name:	ImGuiWindowPerformance.cpp
// Author(s):	Barend Brink (barend.brink), Connor Lariviere (connor.lariviere), Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		ImGui window that displays performance information.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiWindowPerformance.h"

#include "ImGuiInclude.h"

#include "ImGuiWindowBase.inl"

#include "PlatformSystem.h"
#include "PerformanceSystem.h"
#include "PerformanceTracker.h"

#include "Engine.h"
#include "Systems.inl"

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

    constexpr string_view_t ImGuiWindowPerformance::WindowName = "Performance";

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

    ImGuiWindowPerformance::ImGuiWindowPerformance(void)
        : ImGuiWindowBase(WindowName, 0/*ImGuiWindowFlags_MenuBar*/)
        , headerAdvanced_(false)
        , headerGroups_()
        , groupSorts_()
    {
    }

    ImGuiWindowPerformance::ImGuiWindowPerformance(ImGuiWindowPerformance&& other) noexcept
        : ImGuiWindowBase(std::move(other))
        , headerAdvanced_(std::move(other.headerAdvanced_))
        , headerGroups_(std::move(other.headerGroups_))
        , groupSorts_(std::move(other.groupSorts_))
    {
    }

    ImGuiWindowPerformance::GroupSort::GroupSort(void)
        : source(PerformanceSource::CPU)
        , loop(EngineLoop::Update)
        , sortDir(SortDir::None)
    {
    }

    //--------------------------------------------------------------------------

    ImGuiWindowPerformance::~ImGuiWindowPerformance(void)
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

    void ImGuiWindowPerformance::DisplayImpl(void)
    {
        DisplayFrameTimings();

        FORMAT_MAKE(vsyncText, "VSync: {}", PlatformSystem::GetVSync() ? "On" : "Off");
        if (ImGui::Button(vsyncText.data()))
        {
            PlatformSystem::SetVSync(!PlatformSystem::GetVSync());
        }

        ImGui::SetNextItemOpen(headerAdvanced_, ImGuiCond_Once);
        headerAdvanced_ = ImGui::CollapsingHeader("Advanced");
        if (headerAdvanced_)
        {
            for (unsigned group = 0; group < static_cast<unsigned>(PerformanceGroup::NUM_ENTRIES); ++group)
            {
                DisplayTrackerTimingsGroup(static_cast<PerformanceGroup>(group));
            }
        }
    }

    void ImGuiWindowPerformance::DisplayFrameTimings(void)
    {
        auto& frameTimes = PerformanceSystem::GetFrameTime();

        // Display text info
        ImGui::Text("FPS: %.1f", 1.0f / std::chrono::duration_cast<secf_t>(frameTimes.stats.mean).count());
        ImGui::Text("Frame Time: %.2f ms", std::chrono::duration_cast<msecf_t>(frameTimes.FrontOrDefault()).count());
        ImGui::Separator();

        float (*valuesGetter)(void*, int) = [](void* data, int index)
        {
            const auto& tracker = *static_cast<const PerformanceTracker*>(data);
            return std::chrono::duration_cast<msecf_t>(tracker[static_cast<unsigned>(index)]).count();
        };

        ImGui::PlotLines("Frame Times (ms)", valuesGetter, &frameTimes, static_cast<int>(frameTimes.times.size()), 0, nullptr, 0.0f, 50.0f, Vec2(0, 80));

        {
            int temp = static_cast<int>(frameTimes.timeCapacity);
            if (ImGui::InputInt("Frame History Size", &temp))
            {
                frameTimes.SetCapacity(static_cast<unsigned>(temp));
            }
        }
    }

    void ImGuiWindowPerformance::DisplayTrackerTimingsDetailed([[maybe_unused]] ObjectID id)
    {
    }

    static constexpr ImGuiTableFlags flagsTable = ImGuiTableFlags_Resizable | ImGuiTableFlags_Hideable | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY;
    static constexpr array_t<float, PerformanceGroup::NUM_ENTRIES> verticalSizes = { { 40, 280, 80, 400, 400, 400, 400, 200, } };

    void ImGuiWindowPerformance::DisplayTrackerTimingsGroup(PerformanceGroup group)
    {
        static constexpr ImGuiTableFlags gflagsTable = /*ImGuiTableFlags_Resizable |*/ ImGuiTableFlags_Sortable | ImGuiTableFlags_SortTristate /*| ImGuiTableFlags_SortTristate*/ | ImGuiTableFlags_Hideable | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY;
        static constexpr unsigned char PreDataCol = 3;

        const unsigned groupIndex = static_cast<unsigned>(group);
        
        ImGui::PushID(static_cast<int>(groupIndex));
        ImGui::SetNextItemOpen(headerGroups_[groupIndex], ImGuiCond_Once);
        headerGroups_[groupIndex] = ImGui::CollapsingHeader(ENUM_WRITE_VALUE(PerformanceGroup, group).data());
        if (!headerGroups_[groupIndex])
        {
            ImGui::PopID();
            return;
        }

        const float lineHeight = ImGui::GetTextLineHeight() + ImGui::GetStyle().CellPadding.y * 2.0f;
        float vertSize = groupIndex != 0 ? (groupIndex != 1 ? verticalSizes[groupIndex] : lineHeight * (SystemMax + 1)) : (lineHeight * 2);

        if (!ImGui::BeginTable("TrackerGroupPerformance", static_cast<int>(PreDataCol + PerformanceTracker::SrcCatMax), gflagsTable, Vec2(0, vertSize + ImGui::GetStyle().FramePadding.y * 2.0f), 0))
        {
            ImGui::PopID();
            return;
        }
        // Set up header row with column names.
        ImGui::TableSetupScrollFreeze(0, 1);
        static const float timingWidth = ImGui::CalcTextSize("0000000.000").x;
        static constexpr ImGuiTableFlags
              flagsColumnBase = ImGuiTableColumnFlags_None | ImGuiTableColumnFlags_NoClip | ImGuiTableColumnFlags_WidthFixed
            , flagsColumnMeta = flagsColumnBase | ImGuiTableColumnFlags_NoSort
            , flagsColumnMetaHide = flagsColumnMeta | ImGuiTableColumnFlags_DefaultHide
            , flagsColumnInfo = flagsColumnBase
            ;
        ImGui::TableSetupColumn("Index",    flagsColumnMeta,        0.0f, static_cast<ImGuiID>(-1));
        ImGui::TableSetupColumn("ID",       flagsColumnMetaHide,    0.0f, static_cast<ImGuiID>(-2));
        ImGui::TableSetupColumn("Name",     flagsColumnMeta,        0.0f, static_cast<ImGuiID>(-3));

#define LoopSourceID(loop, source) static_cast<ImGuiID>(static_cast<unsigned>(EngineLoop::loop) * static_cast<unsigned>(PerformanceSource::NUM_ENTRIES) + static_cast<unsigned>(PerformanceSource::source))
//#define LoopSourceID(loop, source) static_cast<ImGuiID>((fnv1a_hash(#loop) & 0xFFF) | ((fnv1a_hash(#source) & 0xFFF) << 12))

        ImGui::TableSetupColumn("Update",       flagsColumnInfo,    timingWidth, LoopSourceID(Update, CPU));
        ImGui::TableSetupColumn("Update GFX",   flagsColumnInfo,    timingWidth, LoopSourceID(Update, Graphics));
        ImGui::TableSetupColumn("Render",       flagsColumnInfo,    timingWidth, LoopSourceID(Render, CPU));
        ImGui::TableSetupColumn("Render GFX",   flagsColumnInfo,    timingWidth, LoopSourceID(Render, Graphics));
        ImGui::TableHeadersRow();
        ImGui::TableNextRow();

        auto& sort = groupSorts_[groupIndex];

        ImGuiTableSortSpecs* sortSpecs = ImGui::TableGetSortSpecs();
        if (sortSpecs != nullptr && sortSpecs->SpecsDirty)
        {
            for (int i = 0; i < sortSpecs->SpecsCount; ++i)
            {
                const ImGuiTableColumnSortSpecs& spec = sortSpecs->Specs[i];
                const unsigned col = static_cast<unsigned>(MathMax(static_cast<int>(spec.ColumnUserID), 0));
                sort.source = static_cast<PerformanceSource>(col % static_cast<unsigned>(PerformanceSource::NUM_ENTRIES));
                sort.loop = static_cast<EngineLoop>(col / static_cast<unsigned>(PerformanceSource::NUM_ENTRIES));
                static constexpr array_t<GroupSort::SortDir, 3> dirs = 
                {
                    GroupSort::SortDir::Default, // ImGuiSortDirection_None
                    GroupSort::SortDir::Ascending, // ImGuiSortDirection_Ascending
                    GroupSort::SortDir::Descending, // ImGuiSortDirection_Descending
                };
                sort.sortDir = dirs[spec.SortDirection];
            }
            sortSpecs->SpecsDirty = false;
        }

        // Insert values for each row.
        size_t timesCount = PerformanceSystem::GetTrackerCount(group, sort.loop);
        for (size_t i = 0; i < timesCount; ++i)
        {
            auto& perf = PerformanceSystem::GetTrackerOrdered(group, sort.loop, i);
            ImGui::TableNextRow();
            if (ImGui::TableSetColumnIndex(0))
            {
                ImGui::Text("% 4d", i);
            }
            if (ImGui::TableSetColumnIndex(1))
            {
                ImGui::Text("% 8d", perf.object);
            }
            if (ImGui::TableSetColumnIndex(2))
            {
                ImGui::TextUnformatted(perf.name);
            }
            for (unsigned col = 0; col < PerformanceTracker::SrcCatMax; ++col)
            {
                if (ImGui::TableSetColumnIndex(static_cast<int>(PreDataCol + col)))
                {
                    ImGui::Text("% 11.3f", std::chrono::duration_cast<usecf_t>(perf.statsAll[col].mean).count());
                }
            }
        }

        ImGui::EndTable();

        ImGui::PopID();
    }

    void ImGuiWindowPerformance::DisplayTrackerTimingsAll(void)
    {
        const float lineHeight = ImGui::GetTextLineHeight() + ImGui::GetStyle().CellPadding.y * 2.0f;
        int columnIndex = 0;
        for (unsigned engineLoop = 0; engineLoop < static_cast<unsigned>(EngineLoop::NUM_ENTRIES); ++engineLoop)
        {
            for (unsigned source = 0; source < static_cast<unsigned>(PerformanceSource::NUM_ENTRIES); ++source, ++columnIndex)
            {
                ImGui::TableSetColumnIndex(columnIndex);
                ImGui::PushID(columnIndex);

                for (unsigned groupIndex = 0; groupIndex < static_cast<unsigned>(PerformanceGroup::NUM_ENTRIES); ++groupIndex)
                {
                    if (static_cast<PerformanceGroup>(groupIndex) == PerformanceGroup::ComponentType)
                    {
                        //ImGui::TextUnformatted(string_view_t("ComponentType category not implemented."));
                        continue;
                    }
                    ImGui::PushID(static_cast<int>(groupIndex));
                    float vertSize = groupIndex != 0 ? (groupIndex != 1 ? verticalSizes[groupIndex] : lineHeight * (SystemMax + 1)) : (lineHeight * 2);
                    if (!ImGui::BeginTable("DrawPerformance", 3, flagsTable, Vec2(0, vertSize), 0))
                    {
                        ImGui::PopID();
                        continue;
                    }
                    // Set up header row with column names.
                    ImGui::TableSetupScrollFreeze(0, 1);
                    static constexpr ImGuiTableFlags flagsColumn = ImGuiTableColumnFlags_WidthFixed;
                    ImGui::TableSetupColumn("Index", flagsColumn);
                    ImGui::TableSetupColumn("Name", flagsColumn);
                    ImGui::TableSetupColumn("Time", flagsColumn);
                    ImGui::TableHeadersRow();
                    // Insert values for each row.
                    size_t timesCount = PerformanceSystem::GetTrackerCount(static_cast<PerformanceGroup>(groupIndex), static_cast<EngineLoop>(engineLoop));
                    for (size_t i = 0; i < timesCount; ++i)
                    {
                        auto& perf = PerformanceSystem::GetTrackerOrdered(static_cast<PerformanceGroup>(groupIndex), static_cast<EngineLoop>(engineLoop), i);
                        ImGui::TableNextRow();
                        if (ImGui::TableSetColumnIndex(0))
                        {
                            ImGui::Text("% 4d", i);
                        }
                        if (ImGui::TableSetColumnIndex(1))
                        {
                            ImGui::TextUnformatted(perf.name);
                        }
                        if (ImGui::TableSetColumnIndex(2))
                        {
                            ImGui::Text("% 9.3f", std::chrono::duration_cast<usecd_t>(perf.statsAll[static_cast<unsigned>(columnIndex)].mean).count());
                        }
                    }

                    ImGui::EndTable();
                    ImGui::PopID();
                }

                ImGui::PopID();
            }
        }
    }

    ImGuiWindowBase::ReadError ImGuiWindowPerformance::ReadKeyValue([[maybe_unused]] string_view_arg key, [[maybe_unused]] string_view_arg value)
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

    void ImGuiWindowPerformance::ReadFinish(void)
    {
    }

    void ImGuiWindowPerformance::WriteAll([[maybe_unused]] string_t& buffer)
    {
    }

    //void ImGuiWindowPerformance::DisplayMenuBar(void)
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
