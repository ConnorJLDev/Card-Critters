//------------------------------------------------------------------------------
//
// File Name:	ImGuiAlignedBar.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Horizontally aligns and displays ImGui content within a bar.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiAlignedBar.h"

#include "ImGuiInclude.h"
#include "imgui_internal.h"

#include "CallbackWrapper.inl"

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
    
    static constexpr string_view_t
          TableMainName = "##StatusBarMainTable"
        , TableContentName = "##StatusBarContentTable"
        , TablePadName = "##StatusBarPadTable"
        , ColumnLeftName = "##StatusBarLeft"
        , ColumnCenterName = "##StatusBarCenter"
        , ColumnRightName = "##StatusBarRight"
        ;

    static constexpr array_t<string_view_t, ImGuiAlignedBar::Alignment::NUM_ENTRIES> ColumnMainNames =
    {
        "##StatusBarMainLeft",
        "##StatusBarMainCenter",
        "##StatusBarMainRight",
    };

    static constexpr ImGuiTableFlags
          TableMainFlags =      ImGuiTableFlags_None | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_NoClip | ImGuiTableFlags_SizingStretchSame
        , TableContentFlags =   ImGuiTableFlags_None | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_NoClip | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoHostExtendX
        , TablePadFlags =       ImGuiTableFlags_None | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_NoClip | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoHostExtendX
        ;

    static constexpr ImGuiTableColumnFlags
          ColumnMainFlags =     ImGuiTableColumnFlags_None | ImGuiTableColumnFlags_NoResize | ImGuiTableColumnFlags_NoReorder | ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_NoHeaderLabel | ImGuiTableColumnFlags_NoHeaderWidth | ImGuiTableColumnFlags_IndentDisable
        , ColumnContentFlags =  ColumnMainFlags | ImGuiTableColumnFlags_WidthFixed
        , ColumnPadFlags =      ColumnMainFlags | ImGuiTableColumnFlags_WidthStretch
        ;

    using EntryBase_t = CallbackWrapper<void>;

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

    ImGuiAlignedBar::ImGuiAlignedBar(void)
        : entries_()
        , counts_({0, 0, 0})
        , dirty_(true)
    {
    }

    ImGuiAlignedBar::EntryConfig::EntryConfig(void)
        : active()
        , alignment()
        , colIndex()
        , sortIndex()
    {
    }

    ImGuiAlignedBar::EntryConfig::EntryConfig(bool active_, Alignment alignment_, int sortIndex_)
        : active(active_)
        , alignment(alignment_)
        , colIndex()
        , sortIndex(sortIndex_)
    {
    }

    ImGuiAlignedBar::Entry::Entry(string_view_arg name_, const std::function<void(void)>& func, bool active_, Alignment alignment_, int sortIndex_)
        : EntryBase_t(func)
        , EntryConfig(active_, alignment_, sortIndex_)
        , name(name_)
    {
    }

    ImGuiAlignedBar::Entry::Entry(Entry&& other) noexcept
        : EntryBase_t(std::move(other))
        , EntryConfig(std::move(other))
        , name(std::move(other.name))
    {
    }

    ImGuiAlignedBar::Entry& ImGuiAlignedBar::Entry::operator=(Entry&& other) noexcept
    {
        if (&other != this)
        {
            EntryBase_t::operator=(std::move(other));
            EntryConfig::operator=(std::move(other));
            name = std::move(other.name);
        }
        return *this;
    }

    //--------------------------------------------------------------------------

    ImGuiAlignedBar::~ImGuiAlignedBar(void)
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

    CallbackID ImGuiAlignedBar::Register(string_view_arg name, const std::function<void(void)>& callback, Alignment alignment, int priority, bool active)
    {
        if (GetEntryConfig(name) != nullptr)
        {
            return 0;
        }

        auto& entry = entries_.emplace_back(name, callback, active, alignment, priority);
        ++counts_[ENUM_VAL(entry.alignment)];
        dirty_ = true;
        return entry.GetGUID();
    }

    bool ImGuiAlignedBar::Unregister(string_view_arg name)
    {
        auto count = std::erase_if(entries_, [this, name](const auto& entry)
        {
            bool found = entry.name == name;
            if (found)
            {
                --counts_[ENUM_VAL(entry.alignment)];
            }
            return found;
        });
        dirty_ |= static_cast<bool>(count);
        return static_cast<bool>(count);
    }

    bool ImGuiAlignedBar::Unregister(CallbackID id)
    {
        auto count = std::erase_if(entries_, [this, id](const auto& entry)
        {
            bool found = entry.GetGUID() == id;
            if (found)
            {
                --counts_[ENUM_VAL(entry.alignment)];
            }
            return found;
        });
        dirty_ |= static_cast<bool>(count);
        return static_cast<bool>(count);
    }

    ImGuiAlignedBar::EntryConfig* ImGuiAlignedBar::GetEntryConfig(string_view_arg name)
    {
        auto iter = std::find_if(entries_.begin(), entries_.end(), [name](const auto& entry)
        {
            return entry.name == name;
        });

        return iter != entries_.end() ? &*iter : nullptr;
    }

    ImGuiAlignedBar::EntryConfig* ImGuiAlignedBar::GetEntryConfig(CallbackID id)
    {
        auto iter = std::find_if(entries_.begin(), entries_.end(), [id](const auto& entry)
        {
            return entry.GetGUID() == id;
        });

        return iter != entries_.end() ? &*iter : nullptr;
    }

    bool ImGuiAlignedBar::Display(const Vec2& size)
    {
        auto CheckColumnMainNames = [](void) consteval
        {
            for (const auto& entry : ColumnMainNames)
            {
                if (entry.empty())
                {
                    return false;
                }
            }
            return true;
        };
        static_assert(CheckColumnMainNames(), "Column names must not be empty!");

        if (dirty_)
        {
            Refresh();
        }

        if (counts_[ENUM_NUM(Alignment::Center)] > 0)
        {
            return DisplayCentered<false>(size);
        }
        else
        {
            return DisplayRegular<true>(size);
        }
        //return false;
    }

    void ImGuiAlignedBar::Refresh(void)
    {
        std::sort(entries_.begin(), entries_.end(), [](const auto& a, const auto& b)
        {
            return std::tie(a.alignment, a.sortIndex) < std::tie(b.alignment, b.sortIndex);
        });
        dirty_ = false;
    }

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

    template <bool nested>
    bool ImGuiAlignedBar::DisplayCentered(const Vec2& size) const
    {
        auto& style = ImGui::GetStyle();

        const Vec2 oldCellPadding = style.CellPadding;
        style.CellPadding = Vec2(0.0f, 0.0f);

        // Set up the main table.
        {
            array_t<bool, Alignment::NUM_ENTRIES> alignActive;
            for (unsigned i = 0; i < static_cast<unsigned>(Alignment::NUM_ENTRIES); ++i)
            {
                alignActive[i] = counts_[i] > 0;
            }
            if (alignActive[ENUM_VAL(Alignment::Center)] && (alignActive[ENUM_VAL(Alignment::Right)] || alignActive[ENUM_VAL(Alignment::Left)]))
            {
                alignActive[ENUM_VAL(Alignment::Left)] = true;
                alignActive[ENUM_VAL(Alignment::Right)] = true;
            }

            if (!ImGui::BeginTable(TableMainName.data(), static_cast<int>(Alignment::NUM_ENTRIES), TableMainFlags, size))
            {
                style.CellPadding = oldCellPadding;
                return false;
            }

            for (unsigned i = 0; i < static_cast<unsigned>(Alignment::NUM_ENTRIES); ++i)
            {
                ImGuiTableColumnFlags flags = alignActive[i] ? ColumnPadFlags : (ColumnPadFlags | ImGuiTableColumnFlags_Disabled);
                ImGui::TableSetupColumn(ColumnMainNames[i].data(), flags, 0.0f, i + 1);
            }

            //ImGui::TableHeadersRow();
            ImGui::TableNextRow();
        }

        if constexpr (!nested)
        {
            style.CellPadding = oldCellPadding;
        }

        unsigned entryIndex = 0;
        for (unsigned alignIndex = 0; alignIndex < static_cast<unsigned>(Alignment::NUM_ENTRIES); ++alignIndex)
        {
            const unsigned itemCount = counts_[alignIndex];
            if (itemCount <= 0)
            {
                continue;
            }
            if (!ImGui::TableSetColumnIndex(static_cast<int>(alignIndex)))
            {
                entryIndex += itemCount;
                continue;
            }

            ImGui::PushID(static_cast<int>(alignIndex));

            const Alignment alignCurrent = static_cast<Alignment>(alignIndex);
            const bool centered = alignIndex - 1u < static_cast<unsigned>(Alignment::NUM_ENTRIES) - 2u;
            bool tableBegan;

            if constexpr (nested)
            {
                style.CellPadding = Vec2(0.0f, 0.0f);

                const int colPadCount = 2 + static_cast<int>(centered);
                tableBegan = ImGui::BeginTable(TablePadName.data(), colPadCount, TablePadFlags);

                style.CellPadding = oldCellPadding;
            }
            else
            {
                const int colCount = static_cast<int>(itemCount) + 1 + static_cast<int>(centered);
                tableBegan = ImGui::BeginTable(TableContentName.data(), colCount, TableContentFlags);
            }
            if (!tableBegan)
            {
                entryIndex += itemCount;
                ImGui::PopID();
                continue;
            }

            unsigned char curCol = 0;
            // Sets up padding column on left of table.
            if (centered || alignCurrent == Alignment::Right)
            {
                ImGui::TableSetupColumn(ColumnLeftName.data(), ColumnPadFlags, 0.0f, static_cast<ImGuiID>(-1));
                curCol = 1;
            }
            // If nested, set up content table column.
            if constexpr (nested)
            {
                ImGui::TableSetupColumn(ColumnCenterName.data(), ColumnContentFlags, 0.0f, static_cast<ImGuiID>(-2));
            }
            // If not nested, set up entry columns.
            else
            {
                SetupRange(entryIndex, entryIndex + itemCount, curCol);
            }
            // Sets up padding column on right of table.
            if (centered || alignCurrent == Alignment::Left)
            {
                ImGui::TableSetupColumn(ColumnRightName.data(), ColumnPadFlags, 0.0f, static_cast<ImGuiID>(-3));
            }

            //ImGui::TableHeadersRow();
            ImGui::TableNextRow();

            // If nested, display entry table.
            if constexpr (nested)
            {
                if (ImGui::TableSetColumnIndex(curCol))
                {
                    const int colCount = static_cast<int>(itemCount);
                    if (ImGui::BeginTable(TableContentName.data(), colCount, TableContentFlags))
                    {
                        curCol = 0;
                        SetupRange(entryIndex, entryIndex + itemCount, curCol);

                        //ImGui::TableHeadersRow();
                        ImGui::TableNextRow();

                        DisplayRange(entryIndex, entryIndex + itemCount);

                        ImGui::EndTable();
                    }
                }
            }
            // If not nested, display entry columns.
            else
            {
                DisplayRange(entryIndex, entryIndex + itemCount);
            }

            ImGui::EndTable();

            entryIndex += itemCount;

            ImGui::PopID();
        }

        ImGui::EndTable();

        return true;
    }

    template bool ImGuiAlignedBar::DisplayCentered<false>(const Vec2&) const;
    template bool ImGuiAlignedBar::DisplayCentered<true>(const Vec2&) const;

    template <bool nested>
    bool ImGuiAlignedBar::DisplayRegular(const Vec2& size) const
    {
        if constexpr (nested)
        {
            // Set up main table.
            {
                auto& style = ImGui::GetStyle();

                const Vec2 oldCellPadding = style.CellPadding;
                style.CellPadding = Vec2(0.0f, 0.0f);

                bool tableBegan = ImGui::BeginTable(TableMainName.data(), static_cast<int>(Alignment::NUM_ENTRIES), TableMainFlags, size);

                style.CellPadding = oldCellPadding;

                if (!tableBegan)
                {
                    return false;
                }

                for (unsigned i = 0; i < static_cast<unsigned>(Alignment::NUM_ENTRIES); ++i)
                {
                    const bool centered = i - 1u < static_cast<unsigned>(Alignment::NUM_ENTRIES) - 2u;
                    const bool active = centered ? true : (counts_[i] > 0);
                    const ImGuiTableColumnFlags flags = (active ? ImGuiTableColumnFlags_None : ImGuiTableColumnFlags_Disabled) | (centered ? ColumnPadFlags : ColumnContentFlags);
                    ImGui::TableSetupColumn(ColumnMainNames[i].data(), flags, 0.0f, i + 1);
                }
            }

            //ImGui::TableHeadersRow();
            ImGui::TableNextRow();

            unsigned entryIndex = 0;
            for (unsigned alignIndex = 0; alignIndex < static_cast<unsigned>(Alignment::NUM_ENTRIES); ++alignIndex)
            {
                const unsigned itemCount = counts_[alignIndex];
                if (itemCount <= 0 || !ImGui::TableSetColumnIndex(static_cast<int>(alignIndex)))
                {
                    entryIndex += itemCount;
                    continue;
                }

                ImGui::PushID(static_cast<int>(alignIndex));

                if (!ImGui::BeginTable(TableContentName.data(), static_cast<int>(itemCount), TableContentFlags))
                {
                    entryIndex += itemCount;
                    ImGui::PopID();
                    continue;
                }

                unsigned char curCol = 0;
                SetupRange(entryIndex, entryIndex + itemCount, curCol);

                //ImGui::TableHeadersRow();
                ImGui::TableNextRow();

                DisplayRange(entryIndex, entryIndex + itemCount);

                ImGui::EndTable();

                entryIndex += itemCount;

                ImGui::PopID();
            }

        }
        else
        {
            const unsigned
                  countL = counts_[ENUM_VAL(Alignment::Left)]
                , countR = counts_[ENUM_VAL(Alignment::Right)]
                ;
            const unsigned itemCount = countL + 1 + countR;
            if (!ImGui::BeginTable(TableContentName.data(), static_cast<int>(itemCount), TableContentFlags, size))
            {
                return false;
            }

            unsigned char curCol = 0;
            SetupRange(0, countL, curCol);
            ImGui::TableSetupColumn(ColumnCenterName.data(), ColumnPadFlags, 0.0f, static_cast<ImGuiID>(-2));
            ++curCol;
            SetupRange(countL, static_cast<unsigned>(entries_.size()), curCol);
            assert(curCol == itemCount && "Column counts must match!");

            //ImGui::TableHeadersRow();
            ImGui::TableNextRow();

            DisplayRange(0, static_cast<unsigned>(entries_.size()));
        }

        ImGui::EndTable();

        return true;
    }

    template bool ImGuiAlignedBar::DisplayRegular<false>(const Vec2&) const;
    template bool ImGuiAlignedBar::DisplayRegular<true>(const Vec2&) const;

    void ImGuiAlignedBar::SetupRange(unsigned indexBegin, unsigned indexEnd, unsigned char& indexColumn) const
    {
        for (unsigned i = indexBegin; i < indexEnd; ++i, ++indexColumn)
        {
            auto& entry = entries_[i];
            entry.colIndex = indexColumn;
            const ImGuiTableColumnFlags hiddenFlag = entry.active ? ImGuiTableColumnFlags_None : ImGuiTableColumnFlags_Disabled;
            ImGui::TableSetupColumn(entry.name.data(), ColumnContentFlags | hiddenFlag, 0.0f, static_cast<ImGuiID>(entry.GetGUID()));
        }
    }

    void ImGuiAlignedBar::DisplayRange(unsigned indexBegin, unsigned indexEnd) const
    {
        for (unsigned i = indexBegin; i < indexEnd; ++i)
        {
            auto& entry = entries_[i];
            if (entry.active && ImGui::TableSetColumnIndex(entry.colIndex))
            {
                entry();
            }
        }
    }

#pragma endregion Private Functions

}	// namespace
