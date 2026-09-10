//------------------------------------------------------------------------------
//
// File Name:	ImGuiWindowTestMemory.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		ImGui window used for testing memory page functionality.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiWindowTestMemory.h"

#include "ImGuiInclude.h"

#include "ImGuiWindowBase.inl"

#include "MemoryPage.h"
#include "MemoryException.h"

//------------------------------------------------------------------------------
// Preprocessor Directives:
//------------------------------------------------------------------------------

#define pageC static_cast<MemoryPage*>(page_)
#define configC static_cast<MemoryPage::Config*>(config_)

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

    constexpr string_view_t ImGuiWindowTestMemory::WindowName = "Test:Memory";
    
    static constexpr string_view_t
          KeyPageSize = "PageSize"
        , KeyPageRequest = "PageBlockRequestSize"
        , KeyPageRawOffset = "PageRawOffset"
        , KeyPageRawHori = "PageRawHorizontal"
        , KeyPageRawVert = "PageRawVertical"
        , KeyPageConfigOpen = "PageConfigOpen"
        , KeyPageConfigBlockOpen = "PageConfigBlockOpen"
        , KeyPageStatsOpen = "PageStatsOpen"
        , KeyPageBlocksOpen = "PageBlocksOpen"
        , KeyPageRawOpen = "PageRawOpen"
        , KeyConfigBlockType = "ConfigBlockType"
        , KeyConfigHeaderOrganization = "ConfigHeaderOrganization"
        , KeyConfigHeaderFlags = "ConfigHeaderFlags"
        , KeyConfigDebugHeaders = "ConfigDebugHeaders"
        , KeyConfigDebugPatterns = "ConfigDebugPatterns"
        , KeyConfigPadSize = "ConfigPadSize"
        , KeyConfigFixedAlignment = "ConfigFixedAlignment"
        , KeyConfigFixedSize = "ConfigFixedSize"
        , KeyConfigFixedDelayedInitialization = "ConfigFixedDelayedInitialization"
        , KeyConfigVariableAbsorbNeighboringEmpty = "ConfigVariableAbsorbNeighboringEmpty"
        , KeyConfigVariableGroupPlanarHeaders = "ConfigVariableGroupPlanarHeaders"
        , KeyConfigVariableSortPlanarHeaders = "ConfigVariableSortPlanarHeaders"
        ;

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

    ImGuiWindowTestMemory::ImGuiWindowTestMemory(void)
        : ImGuiWindowBase(WindowName, 0/*ImGuiWindowFlags_MenuBar*/)
        , size_(4096)
        , config_(new MemoryPage::Config())
        , page_(nullptr)
        , dummyAddresses_()
        , blockRequest_(8)
        , rawOffset_(0)
        , rawHorizontal_(0x10)
        , rawVertical_(1)
        , blockSelection_(-1)
        , headerConfig_(false)
        , headerConfigBlock_(false)
        , headerStats_(false)
        , headerRaw_(false)
    {
        assert(config_ != nullptr);
    }

    ImGuiWindowTestMemory::ImGuiWindowTestMemory(ImGuiWindowTestMemory&& other) noexcept
        : ImGuiWindowBase(std::move(other))
        , size_(std::move(other.size_))
        , config_(std::exchange(other.config_, nullptr))
        , page_(std::exchange(other.page_, nullptr))
        , dummyAddresses_(std::move(other.dummyAddresses_))
        , blockRequest_(std::move(other.blockRequest_))
        , rawOffset_(std::move(other.rawOffset_))
        , rawHorizontal_(std::move(other.rawHorizontal_))
        , rawVertical_(std::move(other.rawVertical_))
        , blockSelection_(std::move(other.blockSelection_))
        , headerConfig_(std::move(other.headerConfig_))
        , headerConfigBlock_(std::move(other.headerConfigBlock_))
        , headerStats_(std::move(other.headerStats_))
        , headerRaw_(std::move(other.headerRaw_))
    {
    }

    //--------------------------------------------------------------------------

    ImGuiWindowTestMemory::~ImGuiWindowTestMemory(void)
    {
        SafeDeleteCheck(page_);
        SafeDeleteCheck(config_);
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

    void ImGuiWindowTestMemory::DisplayImpl(void)
    {
        if (config_ == nullptr)
        {
            config_ = new MemoryPage::Config();
            if (config_ == nullptr)
            {
                ImGui::TextUnformatted(string_view_t("Memory page configuration failed to initialize!"));
                return;
            }
        }

        DisplayConfig();

        if (page_ == nullptr)
        {
            if (ImGui::Button("Create Page"))
            {
                dummyAddresses_.clear();
                page_ = new MemoryPage(*configC, size_);
            }
        }
        else
        {
            if (ImGui::Button("Destroy Page"))
            {
                SafeDelete(page_);
                dummyAddresses_.clear();
            }
        }

        if (page_ != nullptr)
        {
            DisplayStats();
            DisplayBlockInterface();
            DisplayBlocks();
            DisplayRaw();
        }
    }

    void ImGuiWindowTestMemory::DisplayConfig(void)
    {
        ImGui::SetNextItemOpen(headerConfig_, ImGuiCond_Once);
        headerConfig_ = ImGui::CollapsingHeader("Config");
        if (!headerConfig_)
        {
            return;
        }
        ImGui::BeginDisabled(page_ != nullptr);
        {
            int tempSize = static_cast<int>(size_);
            if (ImGui::InputInt("Size", &tempSize))
            {
                size_ = static_cast<size_t>(tempSize);
            }
        }
        ImGui::Separator();
        MemoryPage::Config* const config = configC;
        ImGui::CreateEnumSelector("Block Type", config->type, ENUM_WRITE(MemoryPage::BlockType));
        ImGui::CreateEnumSelector("Block Header Organization", config->headerOrganization, ENUM_WRITE(MemoryPage::BlockHeaderOrganization));
        ImGui::CreateEnumFlagSelector("Header Flags", config->headerFlags, ENUM_WRITE(MemoryPage::HeaderFlags));
        {
            bool tempFlag = config->debugHeaders;
            if (ImGui::Checkbox("Debug Headers", &tempFlag))
            {
                config->debugHeaders = tempFlag;
            }
        }
        {
            bool tempFlag = config->debugPatterns;
            if (ImGui::Checkbox("Debug Patterns", &tempFlag))
            {
                config->debugPatterns = tempFlag;
            }
        }
        {
            int tempPad = static_cast<int>(config->padSize);
            if (ImGui::InputInt("Pad Size", &tempPad))
            {
                config->padSize = static_cast<decltype(config->padSize)>(tempPad);
            }
        }
        ImGui::EndDisabled();
        ImGui::Separator();
        switch (config->type)
        {
        case MemoryPage::BlockType::Fixed:
            ImGui::SetNextItemOpen(headerConfigBlock_, ImGuiCond_Once);
            headerConfigBlock_ = ImGui::CollapsingHeader("Fixed block configuration###BlockConfig");
            if (headerConfigBlock_)
            {
                ImGui::BeginDisabled(page_ != nullptr);
                {
                    int tempInt = static_cast<int>(config->fixed.size);
                    if (ImGui::InputInt("Fixed Size", &tempInt))
                    {
                        config->fixed.size = static_cast<decltype(config->fixed.size)>(tempInt);
                    }
                }
                {
                    int tempInt = static_cast<int>(config->fixed.alignment);
                    if (ImGui::InputInt("Fixed Alignment", &tempInt))
                    {
                        config->fixed.alignment = static_cast<decltype(config->fixed.alignment)>(tempInt);
                    }
                }
                {
                    int tempInt = static_cast<int>(config->fixed.delayedInitialization);
                    if (ImGui::InputInt("Fixed Delayed Initialization", &tempInt))
                    {
                        config->fixed.delayedInitialization = static_cast<decltype(config->fixed.delayedInitialization)>(tempInt);
                    }
                }
                ImGui::EndDisabled();
            }
            break;
        case MemoryPage::BlockType::Variable:
            ImGui::SetNextItemOpen(headerConfigBlock_, ImGuiCond_Once);
            headerConfigBlock_ = ImGui::CollapsingHeader("Variable block configuration###BlockConfig");
            if (headerConfigBlock_)
            {
                ImGui::BeginDisabled(page_ != nullptr);
                {
                    int tempInt = static_cast<int>(config->variable.absorbNeighboringEmpty);
                    if (ImGui::InputInt("Absorb Neighboring Empty", &tempInt))
                    {
                        config->variable.absorbNeighboringEmpty = static_cast<decltype(config->variable.absorbNeighboringEmpty)>(tempInt);
                    }
                }
                {
                    bool tempFlag = config->variable.groupPlanarHeaders;
                    if (ImGui::Checkbox("Group Planar Headers", &tempFlag))
                    {
                        config->variable.groupPlanarHeaders = tempFlag;
                    }
                }
                {
                    bool tempFlag = config->variable.sortPlanarHeaders;
                    if (ImGui::Checkbox("Sort Planar Headers", &tempFlag))
                    {
                        config->variable.sortPlanarHeaders = tempFlag;
                    }
                }
                ImGui::EndDisabled();
            }
            break;
        default:
            ImGui::Text("Unknown block type: %d", config->type);
            break;
        }
        ImGui::Separator();
    }

    void ImGuiWindowTestMemory::DisplayStats(void)
    {
        ImGui::SetNextItemOpen(headerStats_, ImGuiCond_Once);
        headerStats_ = ImGui::CollapsingHeader("Stats");
        if (!headerStats_)
        {
            return;
        }

        const MemoryPage* const page = pageC;

        assert(page != nullptr);

        ImGui::Text("Size: %d\nAvailable: %d\nTotal blocks: %d\nFree blocks: %d\nOwning: %d", page->size_, page->freeSpace_, page->blockCount_, page->freeBlockCount_, page->owning_);

        const MemoryPage::BlockInfo& blockInfo = page->blockInfo_;

        ImGui::Text("size: %d", blockInfo.header.size);
        ImGui::Text("alignment: %d", blockInfo.header.alignment);
        ImGui::Text("interleavedBlockOffset: %d", blockInfo.header.interleavedBlockOffset);
        ImGui::Text("offsetLabel: %d", blockInfo.header.offsetLabel);
        ImGui::Text("offsetDeallocFuncPtr: %d", blockInfo.header.offsetDeallocFuncPtr);
        ImGui::Text("offsetAllocNum: %d", blockInfo.header.offsetAllocNum);
        ImGui::Text("offsetAllocCount: %d", blockInfo.header.offsetAllocCount);
        ImGui::Text("offsetIntendedSize: %d", blockInfo.header.offsetIntendedSize);
        ImGui::Text("offsetUseFlag: %d", blockInfo.header.offsetUseFlag);

    }

    void ImGuiWindowTestMemory::DisplayBlockInterface(void)
    {
        assert(page_ != nullptr);

        {
            int tempInt = static_cast<int>(blockRequest_);
            if (ImGui::InputInt("Block Request Size", &tempInt))
            {
                blockRequest_ = static_cast<decltype(blockRequest_)>(tempInt);
            }
        }

        if (ImGui::Button("Create Block"))
        {
            try
            {
                auto block = pageC->FirstFit(blockRequest_, configC->fixed.alignment);
                void* allocated = pageC->Allocate(block, blockRequest_, configC->fixed.alignment);
                if (allocated != nullptr)
                {
                    TraceInfo("Allocated!");
                    dummyAddresses_.emplace_back(allocated, blockRequest_);
                }
                else
                {
                    TraceError("Failed to allocate!");
                }
            }
            catch (MemoryException ex)
            {
                TraceError("MemoryException ({}): {}", ex.code_str(), ex.what());
            }
        }

    }

    void ImGuiWindowTestMemory::DisplayBlocks(void)
    {
        ImGui::SetNextItemOpen(headerBlocks_, ImGuiCond_Once);
        headerBlocks_ = ImGui::CollapsingHeader("Blocks");
        if (!headerBlocks_)
        {
            return;
        }

        assert(page_ != nullptr);

        static constexpr ImGuiTableFlags flagsTable = ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY;
        if (!ImGui::BeginTable("Addresses", 3, flagsTable, contentSize_, 0))
        {
            return;
        }
        ImGui::TableSetupScrollFreeze(0, 1);
        static constexpr ImGuiTableFlags flagsColumn = ImGuiTableColumnFlags_WidthFixed;
        ImGui::TableSetupColumn("Index", flagsColumn);
        ImGui::TableSetupColumn("Address", flagsColumn);
        ImGui::TableSetupColumn("Size", flagsColumn);
        ImGui::TableHeadersRow();

        size_t i = 0;

        ImGuiListClipper clipper;
        clipper.Begin(static_cast<int>(dummyAddresses_.size()));

        while (clipper.Step())
        {
            decltype(i) endpoint = static_cast<decltype(i)>(clipper.DisplayEnd);
            for (i = static_cast<decltype(i)>(clipper.DisplayStart); i < endpoint; ++i)
            {
                const auto& [addr, size] = dummyAddresses_[i];

                ImGui::PushID(addr);
                ImGui::TableNextRow();

                if (ImGui::TableSetColumnIndex(0))
                {
                    ImGui::Text("%ull", i);
                }

                if (ImGui::TableSetColumnIndex(1))
                {
                    ImGui::Text("0x%016X", reinterpret_cast<size_t>(addr));
                }

                if (ImGui::TableSetColumnIndex(2))
                {
                    ImGui::Text("%ull", size);
                }

                ImGui::PopID();
            }
        }

        clipper.End();

        ImGui::EndTable();
    }

    void ImGuiWindowTestMemory::DisplayRaw(void)
    {
        ImGui::SetNextItemOpen(headerRaw_, ImGuiCond_Once);
        headerRaw_ = ImGui::CollapsingHeader("Raw");
        if (!headerRaw_)
        {
            return;
        }

        {
            int tempInt = static_cast<int>(rawOffset_);
            if (ImGui::InputInt("Offset", &tempInt, 0x01, 0x10, ImGuiInputTextFlags_CharsHexadecimal))
            {
                rawOffset_ = static_cast<decltype(rawOffset_)>(tempInt);
            }
        }
        {
            IVec2 tempInt2 = IVec2(static_cast<int>(rawHorizontal_), static_cast<int>(rawVertical_));
            if (ImGui::InputInt2("View", &tempInt2.x, ImGuiInputTextFlags_CharsHexadecimal))
            {
                rawHorizontal_ = static_cast<decltype(rawHorizontal_)>(tempInt2.x);
                rawVertical_ = static_cast<decltype(rawVertical_)>(tempInt2.y);
            }
        }

        static constexpr ImGuiTableFlags flagsTable = ImGuiTableFlags_Hideable | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY /*| ImGuiTableFlags_Borders*/;
        const float lineHeight = ImGui::GetTextLineHeight() + ImGui::GetStyle().CellPadding.y * 2.0f;
        if (!ImGui::BeginTable("Raw", 3, flagsTable, Vec2(0, static_cast<float>(rawVertical_ + 1) * lineHeight), 0))
        {
            return;
        }
        ImGui::TableSetupScrollFreeze(0, 1);
        static constexpr ImGuiTableFlags flagsColumn = ImGuiTableColumnFlags_WidthFixed;
        ImGui::TableSetupColumn("Address", flagsColumn);
        ImGui::TableSetupColumn("Offset", flagsColumn);
        ImGui::TableSetupColumn("Data", flagsColumn);
        ImGui::TableHeadersRow();

        size_t row = 0;

        ImGuiListClipper clipper;
        clipper.Begin(static_cast<int>((pageC->size_ / rawHorizontal_) + 1));

        static string_t& rawBuffer = StaticManagement::StringWrapper::Create(64);

        rawBuffer.resize(rawHorizontal_ * 2);

        // Type used for page pointers.
        using ct = const unsigned char;

        ct* const pageStart = reinterpret_cast<ct*>(pageC->page_);
        ct* const pageEnd = pageStart + pageC->GetSize();

        while (clipper.Step())
        {
            decltype(row) endpoint = static_cast<decltype(row)>(clipper.DisplayEnd);
            for (row = static_cast<decltype(row)>(clipper.DisplayStart); row < endpoint; ++row)
            {
                size_t offset = row * rawHorizontal_;
                ct* const addr = pageStart + offset;

                ImGui::TableNextRow();

                if (ImGui::TableSetColumnIndex(0))
                {
                    ImGui::Text("0x%016X", reinterpret_cast<size_t>(addr));
                }

                if (ImGui::TableSetColumnIndex(1))
                {
                    ImGui::Text("0x%08X", offset);
                }

                if (ImGui::TableSetColumnIndex(2))
                {
                    // Iterate through each byte and set the buffer string manually.
                    // std::format and std::format_to work, but they are much slower
                    // (40x debug, 3x release), and there could be a lot of data to parse.

                    unsigned i = static_cast<unsigned>(-1);
                    ct* const iterEnd = MathMin(addr + rawHorizontal_, pageEnd);
                    for (ct* iter = addr; iter < iterEnd; ++iter)
                    {
                        // Offset for the alpha characters accounting for the offset of 10.
                        static constexpr unsigned char AlphaOffset = 'A' - 10;
                        // Alpha threshold for the high nibble to avoid requiring shift for comparison.
                        static constexpr unsigned char Threshold0 = 10 << 4;
                        // High nibble.
                        const unsigned val0 = static_cast<unsigned>(*iter & 0xF0);
                        rawBuffer[++i] = (val0 >= Threshold0 ? AlphaOffset : '0') + static_cast<unsigned char>(val0 >> 4);
                        // Low nibble.
                        const unsigned val1 = static_cast<unsigned>(*iter & 0x0F);
                        rawBuffer[++i] = (val1 >= 10 ? AlphaOffset : '0') + static_cast<unsigned char>(val1);
                    }
                    ImGui::TextUnformatted(string_view_t(rawBuffer.c_str(), i + 1));
                }
            }
        }

        clipper.End();

        ImGui::EndTable();
    }

    ImGuiWindowBase::ReadError ImGuiWindowTestMemory::ReadKeyValue(string_view_arg key, string_view_arg value)
    {
        bool parseSuccess;
        switch (READKEY_HASH(key))
        {
        READKEY_CHECK(KeyPageSize)
            READKEY_VALUE(size_);
            return READKEY_RETURN;
        READKEY_CHECK(KeyPageRequest)
            READKEY_VALUE(blockRequest_);
            return READKEY_RETURN;
        READKEY_CHECK(KeyPageRawOffset)
            READKEY_VALUE(rawOffset_);
            return READKEY_RETURN;
        READKEY_CHECK(KeyPageRawHori)
            READKEY_VALUE(rawHorizontal_);
            return READKEY_RETURN;
        READKEY_CHECK(KeyPageRawVert)
            READKEY_VALUE(rawVertical_);
            return READKEY_RETURN;
        READKEY_CHECK(KeyPageConfigOpen)
            READKEY_BITFIELD(headerConfig_);
            return READKEY_RETURN;
        READKEY_CHECK(KeyPageConfigBlockOpen)
            READKEY_BITFIELD(headerConfigBlock_);
            return READKEY_RETURN;
        READKEY_CHECK(KeyPageStatsOpen)
            READKEY_BITFIELD(headerStats_);
            return READKEY_RETURN;
        READKEY_CHECK(KeyPageBlocksOpen)
            READKEY_BITFIELD(headerBlocks_);
            return READKEY_RETURN;
        READKEY_CHECK(KeyPageRawOpen)
            READKEY_BITFIELD(headerRaw_);
            return READKEY_RETURN;
        READKEY_CHECK(KeyConfigBlockType)
            READKEY_ENUM(MemoryPage::BlockType, configC->type);
            return READKEY_RETURN;
        READKEY_CHECK(KeyConfigHeaderOrganization)
            READKEY_ENUM(MemoryPage::BlockHeaderOrganization, configC->headerOrganization);
            return READKEY_RETURN;
        READKEY_CHECK(KeyConfigHeaderFlags)
            ReadStringArray(value, [this, key](string_view_arg val)
            {
                auto iter = ENUM_READ(MemoryPage::HeaderFlags).find(val);
                bool valid = iter != ENUM_READ(MemoryPage::HeaderFlags).end();
                if (valid)
                {
                    configC->headerFlags |= iter->second;
                }
                else
                {
                    TraceWarning("Unknown flag \"{}\" for enum MemoryPage::HeaderFlags of key \"{}\" in window \"{}\"", val, key, WindowName);
                }
            });
            return ReadError::None;
        READKEY_CHECK(KeyConfigDebugHeaders)
            READKEY_BITFIELD(configC->debugHeaders);
            return READKEY_RETURN;
        READKEY_CHECK(KeyConfigDebugPatterns)
            READKEY_BITFIELD(configC->debugPatterns);
            return READKEY_RETURN;
        READKEY_CHECK(KeyConfigPadSize)
            READKEY_VALUE(configC->padSize);
            return READKEY_RETURN;
        // Fixed
        READKEY_CHECK(KeyConfigFixedSize)
            READKEY_VALUE(configC->fixed.size);
            return READKEY_RETURN;
        READKEY_CHECK(KeyConfigFixedAlignment)
            READKEY_VALUE(configC->fixed.alignment);
            return READKEY_RETURN;
        READKEY_CHECK(KeyConfigFixedDelayedInitialization)
            READKEY_VALUE(configC->fixed.delayedInitialization);
            return READKEY_RETURN;
        // Variable
        READKEY_CHECK(KeyConfigVariableAbsorbNeighboringEmpty)
            READKEY_VALUE(configC->variable.absorbNeighboringEmpty);
            return READKEY_RETURN;
        READKEY_CHECK(KeyConfigVariableGroupPlanarHeaders)
            READKEY_VALUE(configC->variable.groupPlanarHeaders);
            return READKEY_RETURN;
        READKEY_CHECK(KeyConfigVariableSortPlanarHeaders)
            READKEY_VALUE(configC->variable.sortPlanarHeaders);
            return READKEY_RETURN;
        default:
            break;
        }
        return ReadError::InvalidKey;
    }

    void ImGuiWindowTestMemory::ReadFinish(void)
    {
    }

    void ImGuiWindowTestMemory::WriteAll(string_t& buffer)
    {
        WRITE_VALUE(KeyPageSize, size_);
        WRITE_VALUE(KeyPageRequest, blockRequest_);
        WRITE_VALUE(KeyPageRawOffset, rawOffset_);
        WRITE_VALUE(KeyPageRawHori, rawHorizontal_);
        WRITE_VALUE(KeyPageRawVert, rawVertical_);
        WRITE_BOOL(KeyPageConfigOpen, headerConfig_);
        WRITE_BOOL(KeyPageConfigBlockOpen, headerConfigBlock_);
        WRITE_BOOL(KeyPageStatsOpen, headerStats_);
        WRITE_BOOL(KeyPageBlocksOpen, headerBlocks_);
        WRITE_BOOL(KeyPageRawOpen, headerRaw_);
        WRITE_VALUE(KeyConfigBlockType, ENUM_WRITE_VALUE(MemoryPage::BlockType, configC->type));
        WRITE_VALUE(KeyConfigHeaderOrganization, ENUM_WRITE_VALUE(MemoryPage::BlockHeaderOrganization, configC->headerOrganization));
        //WRITE_VALUE(KeyConfigHeaderFlags, configC->headerFlags);
        {
            buffer += KeyConfigHeaderFlags;
            buffer += '=';
            bool added = false;
            for (const auto& [val, ename] : ENUM_WRITE(MemoryPage::HeaderFlags))
            {
                if (static_cast<bool>(configC->headerFlags & val))
                {
                    if (added)
                    {
                        buffer += ',';
                    }
                    added = true;
                    buffer += ename;
                }
            }
            buffer += '\n';
        }
        WRITE_BOOL(KeyConfigDebugHeaders, configC->debugHeaders);
        WRITE_BOOL(KeyConfigDebugPatterns, configC->debugPatterns);
        WRITE_VALUE(KeyConfigPadSize, configC->padSize);
        switch (configC->type)
        {
        case MemoryPage::BlockType::Fixed:
            WRITE_VALUE(KeyConfigFixedSize, configC->fixed.size);
            WRITE_VALUE(KeyConfigFixedAlignment, configC->fixed.alignment);
            WRITE_VALUE(KeyConfigFixedDelayedInitialization, configC->fixed.delayedInitialization);
            break;
        case MemoryPage::BlockType::Variable:
            WRITE_VALUE(KeyConfigVariableAbsorbNeighboringEmpty, configC->variable.absorbNeighboringEmpty);
            WRITE_BOOL(KeyConfigVariableGroupPlanarHeaders, configC->variable.groupPlanarHeaders);
            WRITE_BOOL(KeyConfigVariableSortPlanarHeaders, configC->variable.sortPlanarHeaders);
            break;
        default:
            TraceWarning("Window \"{}\" error when writing: Unknown block type: {}", internalName_, configC->type);
            break;
        }
    }

    void ImGuiWindowTestMemory::DisplayMenuBar(void)
    {
        if (!ImGui::BeginMenuBar())
        {
            return;
        }
    
        if (ImGui::BeginMenu("Settings"))
        {
    
    
            ImGui::EndMenu();
        }
    
        ImGui::EndMenuBar();
    }

#pragma endregion Private Functions

}	// namespace
