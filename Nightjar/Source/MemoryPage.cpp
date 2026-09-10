//------------------------------------------------------------------------------
//
// File Name:	MemoryPage.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class that represents a contiguous region of memory, split into
//              MemoryBlocks.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "MemoryPage.h"
#include "MemoryException.h"

//------------------------------------------------------------------------------
// Preprocessor Directives:
//------------------------------------------------------------------------------

#define pageC_ reinterpret_cast<char*>(page_)

#define blockType_ config_.type
#define headerOrg_ config_.headerOrganization
#define headerFlags_ config_.headerFlags
#define padSize_ config_.padSize

#define fixedSize_ config_.fixed.size
#define fixedAlignment_ config_.fixed.alignment
#define fixedDelayedInitialization_ config_.fixed.delayedInitialization

#define variableAbsorbNeighboringEmpty_ config_.variable.absorbNeighboringEmpty
#define variableGroupPlanarHeaders_ config_.variable.groupPlanarHeaders
#define variableSortPlanarHeaders_ config_.variable.sortPlanarHeaders


#define headerSize_ blockInfo_.header.size
#define headerAlignment_ blockInfo_.header.alignment
#define headerInterleavedBlockOffset_ blockInfo_.header.interleavedBlockOffset

#define fixedBlockFinalSize_ blockInfo_.fixed.blockFinalSize
#define fixedAlignSize_ blockInfo_.fixed.alignSize
#define fixedStartOffset_ blockInfo_.fixed.startOffset
#define fixedStartAlignment_ blockInfo_.fixed.startAlignment
#define fixedHeaderAlignSize_ blockInfo_.fixed.headerAlignSize
#define fixedPlanarHeaderOffset_ blockInfo_.fixed.planarHeaderOffset
#define fixedBlockLast_ blockInfo_.fixed.blockLast

#define variableUnallocStart_ blockInfo_.variable.unallocStart
#define variableMinAlloc_ blockInfo_.variable.minAlloc

// Calculates amount of padding to meet alignment using modulo.
#define AlignOp(input, align, alignPad) ((alignPad) - (((input) + (alignPad)) % (align)))
// Calculates amount of padding to meet alignment using bitwise logic (requires powers of 2).
#define AlignOpMask(input, align, alignMask) (((align) - ((input) & (alignMask))) & (alignMask))
// Pads the input to meet alignment using bitwise logic (requires powers of 2).
#define AlignedOpMask(input, alignMask) (((input) + (alignMask)) & ~(alignMask))

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

    struct PATTERN
    {
        static constexpr unsigned char
              UNALLOCATED = 0xAA    // New memory that has never been allocated.
            , ALLOCATED = 0xBB      // Memory that has been allocated and is about to be made accessible.
            , DEALLOCATED = 0xCC    // Memory that has been deallocated and is now free.
            , PADDING = 0xDD        // Bytes added for padding requirements.
            , ALIGNMENT = 0xEE      // Bytes added for alignment requirements.
            ;
    };

    static constexpr size_t InvalidFree = std::numeric_limits<size_t>::max();
    constexpr MemoryPage::HeaderSize_t MemoryPage::InvalidHeaderOffset = std::numeric_limits<HeaderSize_t>::max();

    using HeaderLabel_t = const char*;
    using HeaderDeallocFuncPtr_t = void(*)(void*);
    using HeaderAllocNum_t = uint32_t;
    using HeaderAllocCount_t = uint32_t;
    using HeaderIntendedSize_t = uint32_t;
    using HeaderUseFlag_t = bool;
    static constexpr HeaderUseFlag_t HeaderUseFlagInit = false;

    using BitMap_t = size_t;
    static constexpr size_t
          BitMapSize = sizeof(BitMap_t)
        , BitMapSizeBits = 8 * BitMapSize
        , BitMapPackPadOffset = BitMapSizeBits - 1
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

    MemoryPage::MemoryPage(const Config& config, size_t size)
        : page_(std::malloc(size))
        , size_(size)
        , freeSpace_(size)
        , freeBlockCount_(0)
        , blockCount_(0)
        , owning_(true)
        , config_(config)
        , blockInfo_(config_, size_)
        , freeStart_(InvalidFree)
    {
        if (page_ == nullptr)
        {
            throw MemoryException(MemoryException::ErrorCode::NoMemory, "Failed to create memory page!");
        }
        Initialize();
    }

    MemoryPage::MemoryPage(const Config& config, void* page, size_t size)
        : page_(page)
        , size_(size)
        , freeSpace_(size)
        , freeBlockCount_(0)
        , blockCount_(0)
        , owning_(false)
        , config_(config)
        , blockInfo_(config_, size_)
        , freeStart_(InvalidFree)
    {
        Initialize();
    }

    MemoryPage::MemoryPage(MemoryPage&& other) noexcept
        : page_(std::exchange(other.page_, nullptr))
        , size_(other.size_)
        , freeSpace_(std::move(other.freeSpace_))
        , freeBlockCount_(std::move(other.freeBlockCount_))
        , blockCount_(std::move(other.blockCount_))
        , owning_(std::exchange(other.owning_, false))
        , config_(std::move(other.config_))
        , blockInfo_(std::move(other.blockInfo_))
        , freeStart_(std::move(other.freeStart_))
    {
    }

    MemoryPage::BlockInfo::BlockInfo(void)
        : header()
    {
    }

    MemoryPage::BlockInfo::BlockInfo(const Config& config, [[maybe_unused]] size_t size)
        : header(config)
    {
    }

    MemoryPage::BlockInfo::HeaderMeta::HeaderMeta(void)
        : size(0)
        , alignment(1)
        , interleavedBlockOffset(InvalidHeaderOffset)
        , offsetLabel(InvalidHeaderOffset)
        , offsetDeallocFuncPtr(InvalidHeaderOffset)
        , offsetAllocNum(InvalidHeaderOffset)
        , offsetAllocCount(InvalidHeaderOffset)
        , offsetIntendedSize(InvalidHeaderOffset)
        , offsetUseFlag(InvalidHeaderOffset)
    {
    }

    MemoryPage::BlockInfo::HeaderMeta::HeaderMeta(const Config& config)
        : HeaderMeta()
    {
        // If variable block types are being used, the header must always know the block size.
        if (config.type == BlockType::Variable)
        {
            size += static_cast<HeaderSize_t>(sizeof(BlockSize_t));
            alignment = MathMax(alignment, static_cast<HeaderSize_t>(alignof(BlockSize_t)));
            // If variable block headers are planar, the header must also always know the block offset.
            if (config.headerOrganization == BlockHeaderOrganization::Planar)
            {
                size += static_cast<HeaderSize_t>(sizeof(BlockSize_t));
                alignment = MathMax(alignment, static_cast<HeaderSize_t>(alignof(BlockSize_t)));
            }
        }

#define ProcessHeaderFlag(enumVal, type)\
            if (static_cast<bool>(config.headerFlags & HeaderFlags::enumVal))\
            {\
                static constexpr HeaderSize_t\
                      tsize = static_cast<HeaderSize_t>(sizeof(type))\
                    , talign = static_cast<HeaderSize_t>(alignof(type))\
                    , padOffset = talign - 1\
                    ;\
                HeaderSize_t pad = static_cast<HeaderSize_t>(padOffset - ((size + padOffset) % talign));\
                offset##enumVal = static_cast<HeaderSize_t>(size + pad);\
                size += tsize + pad;\
                alignment = MathMax(alignment, talign);\
            }\

        ProcessHeaderFlag(Label, HeaderLabel_t);
        ProcessHeaderFlag(DeallocFuncPtr, HeaderDeallocFuncPtr_t);
        ProcessHeaderFlag(AllocNum, HeaderAllocNum_t);
        ProcessHeaderFlag(AllocCount, HeaderAllocCount_t);
        ProcessHeaderFlag(IntendedSize, HeaderIntendedSize_t);
        ProcessHeaderFlag(UseFlag, HeaderUseFlag_t);
#undef ProcessHeaderFlag
        // If headers are packed together in Planar, then their size must align.
        if (config.headerOrganization == BlockHeaderOrganization::Planar)
        {
            HeaderSize_t padOffset = static_cast<HeaderSize_t>(alignment - 1);
            HeaderSize_t pad = static_cast<HeaderSize_t>(AlignOp(size, alignment, padOffset));
            size += pad;
        }
    }

    //--------------------------------------------------------------------------

    MemoryPage::~MemoryPage(void)
    {
        if (owning_)
        {
            std::free(page_);
        }
        page_ = nullptr;
    }

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

    ENUM_IMPLEMENT_OPS(MemoryPage::HeaderFlags);

    ENUM_MAPS_CLASS_SEQUENTIAL(MemoryPage, BlockType, Fixed, Variable);
    ENUM_MAPS_CLASS_SEQUENTIAL(MemoryPage, BlockHeaderOrganization, Interleaved, Planar);
    ENUM_MAPS_CLASS(MemoryPage, HeaderFlags,
        Label,
        DeallocFuncPtr,
        AllocNum,
        AllocCount,
        IntendedSize,
        UseFlag
        );

#pragma endregion Public Static Functions

    //--------------------------------------------------------------------------
    // Public Functions:
    //--------------------------------------------------------------------------

#pragma region Public Functions

    MemoryBlock MemoryPage::FirstFit(size_t size, size_t alignment)
    {
        switch (blockType_)
        {
        case BlockType::Fixed:
        {
            // Every block is fixed.
            if (size <= fixedSize_ && fixedAlignment_ % alignment == 0)
            {
                // If we have a free block, send it.
                if (freeStart_ != InvalidFree)
                {
                    return MemoryBlock(freeStart_, fixedSize_, 0, (freeStart_ - fixedStartOffset_) / fixedBlockFinalSize_);
                }
                // If we could initialize free blocks, do so.
                else if (fixedBlockLast_ < blockCount_)
                {
                    InitializeFixedBlocks(fixedDelayedInitialization_);
                    assert(freeStart_ != InvalidFree && "We should always have free blocks after initializing them!");
                    return MemoryBlock(freeStart_, fixedSize_, 0, (freeStart_ - fixedStartOffset_) / fixedBlockFinalSize_);
                }
            }
        }
            break;
        case BlockType::Variable:
        {
            // TODO: Utilize planar headers.
            const size_t addr = reinterpret_cast<size_t>(page_);
            const size_t alignOffset = static_cast<size_t>(alignment - 1);
            // Iterate through the free list to find a suitable block.
            for (FreeListIterator iter(*this); iter != FreeListIterator::end; ++iter)
            {
                BlockSize_t freeSize = iter.Header().GetBlockSize();
                // Check if this block has enough space before doing alignment checks.
                if (freeSize >= size)
                {
                    size_t align = AlignOp(addr + iter.DataOffset(), alignment, alignOffset);
                    freeSize -= align;
                    if (freeSize >= size)
                    {
                        return MemoryBlock(iter.DataOffset(), freeSize, align, iter.HeaderOffset());
                    }
                }
            }
            // If no block was found, check if the unallocated space supports the request.
            size_t paddedStart = variableUnallocStart_ + padSize_;
            size_t align = AlignOp(addr + paddedStart, alignment, alignOffset);
            size_t alignedStart = paddedStart + align;
            size_t paddedEnd = alignedStart + size + padSize_;
            size_t availEnd = GetVariableUnallocatedSpaceEnd();
            if (paddedEnd <= availEnd)
            {
                return MemoryBlock(variableUnallocStart_, availEnd - alignedStart, align + padSize_, variableUnallocStart_ - headerInterleavedBlockOffset_);
            }
        }
            break;
        default:
            assert(false);
            throw MemoryException(MemoryException::ErrorCode::Generic, "Invalid block type!");
        }
        return MemoryBlock(InvalidFree, InvalidFree, InvalidFree, InvalidFree);
    }

    MemoryBlock MemoryPage::BestFit(size_t size, size_t alignment)
    {
        MemoryBlock bestFit;
        switch (blockType_)
        {
        case BlockType::Fixed:
            bestFit = FirstFit(size, alignment);
            break;
        case BlockType::Variable:
        {
            // TODO: Utilize planar headers.
            const size_t addr = reinterpret_cast<size_t>(page_);
            const size_t alignOffset = static_cast<size_t>(alignment - 1);
            // Iterate through the free list to find a suitable block.
            bestFit = MemoryBlock(InvalidFree, InvalidFree, InvalidFree, InvalidFree);
            for (FreeListIterator iter(*this); iter != FreeListIterator::end; ++iter)
            {
                BlockSize_t freeSize = iter.Header().GetBlockSize();
                // Check if this block has enough space before doing alignment checks.
                if (freeSize >= size)
                {
                    size_t align = AlignOp(addr + iter.DataOffset(), alignment, alignOffset);
                    freeSize -= align;
                    if (freeSize >= size)
                    {
                        if (freeSize < bestFit.size)
                        {
                            bestFit = MemoryBlock(iter.DataOffset(), freeSize, align, iter.HeaderOffset());
                        }
                    }
                }
            }
            // If no block was found, check if the unallocated space supports the request.
            size_t paddedStart = variableUnallocStart_ + padSize_;
            size_t align = AlignOp(addr + paddedStart, alignment, alignOffset);
            size_t alignedStart = paddedStart + align;
            size_t paddedEnd = alignedStart + size + padSize_;
            size_t availEnd = GetVariableUnallocatedSpaceEnd();
            if (paddedEnd <= availEnd)
            {
                BlockSize_t freeSize = availEnd - alignedStart - padSize_;
                if (freeSize < bestFit.size)
                {
                    bestFit = MemoryBlock(variableUnallocStart_, availEnd - alignedStart, align + padSize_, variableUnallocStart_ - headerInterleavedBlockOffset_);
                }
            }
        }
            break;
        default:
            assert(false);
            throw MemoryException(MemoryException::ErrorCode::Generic, "Invalid block type!");
        }
        return bestFit;
    }

    void* MemoryPage::Allocate(MemoryBlock block, size_t size, [[maybe_unused]] size_t alignment)
    {
        if (block.size < size + block.alignPad)
        {
            assert(false);
            throw MemoryException(MemoryException::ErrorCode::Generic, "Block must be able to support size and alignment on allocation!");
        }
        void* addr = nullptr;
        switch (blockType_)
        {
        case BlockType::Fixed:
        {
            assert(block.size == fixedSize_);
            addr = reinterpret_cast<char*>(page_) + block.offset;
            HeaderInterface header(*this, block.offset, (block.offset - fixedStartOffset_) / fixedBlockFinalSize_);
            if (config_.debugHeaders && !header.ValidateFree())
            {
                throw MemoryException(MemoryException::ErrorCode::InvalidHeader, "Attempted allocation to used block!");
            }
            freeStart_ = *reinterpret_cast<FreeChain_t*>(addr);
            if (config_.debugPatterns)
            {
                // Check patterns.
                char* working = reinterpret_cast<char*>(addr);
                for (const char* iter = working - padSize_; iter < working; ++iter)
                {
                    if (*iter != PATTERN::PADDING)
                    {
                        throw MemoryException(MemoryException::ErrorCode::InvalidPattern, "Padding start corrupted on allocation!");
                    }
                }
                char* const workingEnd = working + block.size;
                // Set pattern with memset.
                std::memset(working, PATTERN::ALLOCATED, sizeof(FreeChain_t));
                for (char* iter = working + sizeof(FreeChain_t); iter < workingEnd; ++iter)
                {
                    if (*iter != PATTERN::UNALLOCATED && *iter != PATTERN::DEALLOCATED)
                    {
                        throw MemoryException(MemoryException::ErrorCode::InvalidPattern, "Block does not have valid pattern on allocation!");
                    }
                    // Update patterns while checking.
                    *iter = PATTERN::ALLOCATED;
                }
                const char* const workingPadEnd = workingEnd + padSize_;
                for (const char* iter = workingEnd; iter < workingPadEnd; ++iter)
                {
                    if (*iter != PATTERN::PADDING)
                    {
                        throw MemoryException(MemoryException::ErrorCode::InvalidPattern, "Padding end corrupted on allocation!");
                    }
                }
                //// Set pattern with memset.
                //std::memset(working, PATTERN::ALLOCATED, blockSize);
            }
            header.Allocate();
            --freeBlockCount_;
            freeSpace_ -= block.size;
        }
            break;
        case BlockType::Variable:
        {
            const bool appendingBlock = block.offset >= variableUnallocStart_;
            const bool prependFreeBlock = block.alignPad >= variableMinAlloc_;
            const bool appendFreeBlock = block.size - block.alignPad - size >= variableMinAlloc_;
            // If the padding needed for alignment can fit a block, fill it with a free block.
            if (prependFreeBlock)
            {
                size_t preSize = block.alignPad - variableMinAlloc_;
                // Add a new free block.
                MemoryBlock preBlock(block.offset, preSize, 0, block.headerOffset);
                if (appendingBlock)
                {
                    CreateVariableBlock(true, preBlock);
                }
                HeaderInterface header(*this, block.headerOffset);
                if (config_.debugHeaders && !header.ValidateFree())
                {
                    assert(false);
                    throw MemoryException(MemoryException::ErrorCode::InvalidHeader, "Attempted allocation to used block!");
                }
                if (!appendingBlock)
                {
                    header.SetBlockSize(preSize);
                }
                block.offset += block.alignPad;
                block.alignPad = 0;
                if (!appendingBlock)
                {
                    CreateVariableBlock(true, block);
                }
            }
            switch (headerOrg_)
            {
            case BlockHeaderOrganization::Interleaved:
            {
                if (appendingBlock)
                {
                    CreateVariableBlock(false, block);
                    // Update unallocated start offset.
                    variableUnallocStart_ = block.offset + block.alignPad + size + padSize_;
                    // Pad unallocated start offset to account for next header size and alignment.
                    auto headerPad = headerAlignment_ - 1;
                    size_t align = AlignOp(variableUnallocStart_, headerAlignment_, headerPad);
                    variableUnallocStart_ += align + headerSize_;
                }
                else
                {
                    if (appendFreeBlock)
                    {
                        auto postHeaderPad = headerAlignment_ - 1;
                        size_t postAlign = AlignOp(block.alignPad + size, headerAlignment_, postHeaderPad);
                        size_t postUsed = block.alignPad + size + postAlign;
                        if (block.size - postUsed >= variableMinAlloc_)
                        {
                            size_t postOffset = block.offset + postUsed;
                            // Split this free block into a free block and a used block.
                            size_t postSize = block.size - (postUsed + variableMinAlloc_);
                            MemoryBlock postBlock(postOffset, postSize, 0, 0);
                            CreateVariableBlock(true, postBlock);
                        }
                        else
                        {
                            // Mark this free block as a used block.
                            SetVariableBlockUsage(false, block);
                        }
                    }
                    else
                    {
                        // Mark this free block as a used block.
                        SetVariableBlockUsage(false, block);
                    }
                }
            }
                break;
            case BlockHeaderOrganization::Planar:
            {
                if (appendingBlock)
                {
                    CreateVariableBlock(false, block);
                    // Update unallocated start offset.
                    variableUnallocStart_ = block.offset + block.alignPad + size + padSize_;
                }
                else
                {
                    if (appendFreeBlock)
                    {
                        size_t postUsed = block.alignPad + size;
                        size_t postOffset = block.offset + postUsed;
                        // Split this free block into a free block and a used block.
                        size_t postSize = block.size - (postUsed + variableMinAlloc_);
                        MemoryBlock postBlock(postOffset, postSize, 0, 0);
                        CreateVariableBlock(true, postBlock);
                    }
                    else
                    {
                        // Mark this free block as a used block.
                        SetVariableBlockUsage(false, block);
                    }
                }
            }
                break;
            default:
                assert(false);
                throw MemoryException(MemoryException::ErrorCode::Generic, "Invalid header organization for variable block type!");
            }
            if (appendingBlock)
            {
                variableUnallocStart_ = MathMin(variableUnallocStart_, GetVariableUnallocatedSpaceEnd());
            }
            addr = reinterpret_cast<char*>(page_) + block.offset;
            HeaderInterface header(*this, block.headerOffset);
            if (!header.ValidateFree())
            {
                assert(false);
                throw MemoryException(MemoryException::ErrorCode::Generic, "Attempted allocation to used block!");
            }
            header.Allocate();
            if (config_.debugPatterns)
            {
                char* working = reinterpret_cast<char*>(addr);
                // Set padding pattern.
                std::memset(working - padSize_, PATTERN::PADDING, padSize_);
                // Check patterns.
                char* const workingEnd = working + block.size;
                for (char* iter = working; iter < workingEnd; ++iter)
                {
                    if (*iter != PATTERN::UNALLOCATED && *iter != PATTERN::DEALLOCATED)
                    {
                        assert(false);
                        throw MemoryException(MemoryException::ErrorCode::Generic, "Block does not have valid pattern on allocation!");
                    }
                    // Update patterns while checking.
                    *iter = PATTERN::ALLOCATED;
                }
                //// Set allocated pattern.
                //std::memset(working, PATTERN::ALLOCATED, blockSize);
                // Set padding pattern.
                std::memset(workingEnd, PATTERN::PADDING, padSize_);
            }
        }
            break;
        default:
            assert(false);
            throw MemoryException(MemoryException::ErrorCode::Generic, "Invalid block type!");
        }
        return addr;
    }

    bool MemoryPage::Deallocate(void* data)
    {
        size_t offset = static_cast<size_t>(reinterpret_cast<char*>(data) - reinterpret_cast<char*>(page_));
        if (offset >= size_)
        {
            return false;
        }
        /*MemoryBlock* iterStart = GetFreeIteratorBegin();
        MemoryBlock* iterFreeEnd = iterStart - freeBlockCount_;
        MemoryBlock* iterEnd = iterStart - blockCount_;
        // TODO: BST
        for (MemoryBlock* iter = iterFreeEnd; iter > iterEnd; --iter)
        {
            MemoryBlock& block = *iter;
            if (block.offset == offset)
            {
                freeSpace_ += block.size;
                // Merge this block with any neighboring free blocks.
                size_t head = block.offset;
                size_t tail = head + block.size;
                size_t freeSpaceGain = 0;
                MemoryBlock* backBlock = iter;
                for (MemoryBlock* iterFree = iterStart; iterFree > iterFreeEnd; --iterFree)
                {
                    if (iterFree->offset + iterFree->size == head)
                    {
                        // Merge head.
                        block.offset = iterFree->offset;
                        block.size += iterFree->size;
                        freeSpaceGain += sizeof(MemoryBlock);
                        // Decrement the free block count and swap the iterFree block with the end of the free list (which is now in the "used" list).
                        ++iterFreeEnd;
                        --freeBlockCount_;
                        std::swap(*iterFree, *iterFreeEnd);
                        // Replace the expired block at the end of the "free" list with the block at the end of the "used" list. Decrement total blocks.
                        ++iterEnd;
                        --blockCount_;
                        *iterFreeEnd = *iterEnd;
                        // The value at this iterator has changed, so increment to redo the loop.
                        ++iterFree;
                        continue;
                    }
                    else if (iterFree->offset == tail)
                    {
                        // Merge tail.
                        block.size += iterFree->size;
                        freeSpaceGain += sizeof(MemoryBlock);
                        // Decrement the free block count and swap the current block with the end of the free list (which is now in the "used" list).
                        ++iterFreeEnd;
                        --freeBlockCount_;
                        std::swap(*iterFree, *iterFreeEnd);
                        // Replace the expired block at the end of the "free" list with the block at the end of the "used" list. Decrement total blocks.
                        ++iterEnd;
                        --blockCount_;
                        *iterFreeEnd = *iterEnd;
                        // The value at this iterator has changed, so increment to redo the loop.
                        ++iterFree;
                        continue;
                    }
                    if (iterFree->offset > backBlock->offset)
                    {
                        backBlock = iterFree;
                    }
                }
                backBlock->size += freeSpaceGain;
                freeSpace_ += freeSpaceGain;
                std::swap(*iter, *iterFreeEnd);
                ++freeBlockCount_;
                SortBlocks();
                return true;
            }
            else if (block.offset > offset)
            {
                break;
            }
        }*/
        throw MemoryException(MemoryException::ErrorCode::InvalidHeader, "Failed to find deallocation block!");
    }

    size_t MemoryPage::GetLargestFreeBlockSize(void) const
    {
        switch (blockType_)
        {
        case BlockType::Fixed:
            return freeBlockCount_ > 0 ? fixedSize_ : 0;
        case BlockType::Variable:
        {
            BlockSize_t sizeMax = GetVariableUnallocatedSpaceEnd() - variableUnallocStart_;
            for (FreeListIterator iter(*this); iter != FreeListIterator::end; ++iter)
            {
                BlockSize_t size = iter.Header().GetBlockSize();
                sizeMax = MathMax(sizeMax, size);
            }
            return sizeMax;
        }
        default:
            assert(false);
            throw MemoryException(MemoryException::ErrorCode::Generic, "Invalid block type!");
        }
    }

    size_t MemoryPage::GetSmallestFreeBlockSize(void) const
    {
        switch (blockType_)
        {
        case BlockType::Fixed:
            return freeBlockCount_ > 0 ? fixedSize_ : 0;
        case BlockType::Variable:
        {
            BlockSize_t sizeMin = GetVariableUnallocatedSpaceEnd() - variableUnallocStart_;
            for (FreeListIterator iter(*this); iter != FreeListIterator::end; ++iter)
            {
                BlockSize_t size = iter.Header().GetBlockSize();
                sizeMin = MathMin(sizeMin, size);
            }
            return sizeMin;
        }
            break;
        default:
            assert(false);
            throw MemoryException(MemoryException::ErrorCode::Generic, "Invalid block type!");
        }
    }

    size_t MemoryPage::GetSize(void) const
    {
        return size_;
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

    void MemoryPage::Initialize(void)
    {
        switch (blockType_)
        {
        case BlockType::Fixed:
        {
            const uint32_t 
                  fixedAlignment = static_cast<uint32_t>(fixedAlignment_)
                , fixedAlignmentPadOffset = fixedAlignment - 1
                ;

            const size_t addr = reinterpret_cast<size_t>(page_);

            switch (headerOrg_)
            {
            case BlockHeaderOrganization::Interleaved:
            {
                // Interleaved headers (stored preceding their respective block):
                // START: Header0 - DataAlign - Pad - Data0 - Pad - HeaderAlign - Header - DataAlign - Pad - Data1 ...

                // Calculate the starting offset of the first fixed block.
                {
                    uint32_t startRequired = headerSize_ + padSize_; // Minimum number of bytes that are required to exist before a block.
                    fixedAlignSize_ = AlignOp(addr + startRequired, fixedAlignment, fixedAlignmentPadOffset);
                    fixedStartOffset_ = static_cast<uint32_t>(startRequired + fixedAlignSize_);
                    headerInterleavedBlockOffset_ = static_cast<HeaderSize_t>(fixedStartOffset_);
                }
                // Calculate the alignment for the next header.
                uint32_t blockEnd = fixedSize_ + padSize_;
                {
                    uint32_t padOffset = static_cast<uint32_t>(headerAlignment_ - 1);
                    fixedHeaderAlignSize_ = AlignOp(addr + blockEnd, headerAlignment_, padOffset);
                }
                fixedBlockFinalSize_ = blockEnd + fixedHeaderAlignSize_ + fixedStartOffset_;
                // Calculate the number of blocks that can fit within the page.
                blockCount_ = static_cast<decltype(blockCount_)>((size_ - fixedStartOffset_ - padSize_) / fixedBlockFinalSize_);
            }
                break;
            case BlockHeaderOrganization::Planar:
            {
                // Planar headers (stored elsewhere):
                // START: AlignStart - Pad - Data0 - Pad - AlignInter - Pad - Data1 ...
                
                // Calculate the starting offset of the first fixed block.
                {
                    uint32_t startRequired = padSize_; // Minimum number of bytes that are required to exist before the first block.
                    fixedStartAlignment_ = AlignOp(addr + startRequired, fixedAlignment, fixedAlignmentPadOffset);
                    fixedStartOffset_ = static_cast<uint32_t>(startRequired + fixedStartAlignment_);
                }
                // Calculate the number of inter-block alignment bytes required.
                uint32_t paddedBlock = fixedSize_ + 2 * padSize_;
                {
                    fixedAlignSize_ = AlignOp(addr + paddedBlock, fixedAlignment, fixedAlignmentPadOffset);
                }
                fixedBlockFinalSize_ = paddedBlock + fixedAlignSize_;
                // Calculate the number of blocks that can fit within the page.
                size_t available = size_ - fixedStartOffset_ - padSize_;
                // If the header is only the use flag, then the header section can be implemented as a bitmap.
                if (headerFlags_ == HeaderFlags::UseFlag)
                {
                    // In order to avoid potential integer overflows due to multiplying size by 8, the calculation is split into groups of 8, followed by getting the remainder.
                    size_t finalSizeBits = 1ull + 8ull * fixedBlockFinalSize_;
                    // Number of groups of 8 blocks that can fit.
                    size_t groups = available / finalSizeBits;
                    // Update the amount of size available. Each group is 8 * blockFinalSize + 1 bytes.
                    available -= groups * (8ull * fixedBlockFinalSize_) + groups;
                    // Now that available is a lower number, calculate the remaining blocks that can fit.
                    size_t remainder = (8ull * available) / finalSizeBits;
                    blockCount_ = static_cast<decltype(blockCount_)>(8ull * groups + remainder);
                    // Calculate the offset of the planar header section.
                    {
                        headerAlignment_ = static_cast<HeaderSize_t>(BitMapSize);
                        size_t pad = AlignOp(blockCount_, BitMapSizeBits, BitMapPackPadOffset);
                        size_t headerPlaneSize = (blockCount_ + pad) / BitMapSize;
                        fixedPlanarHeaderOffset_ = size_ - headerPlaneSize;
                    }
                }
                else
                {
                    blockCount_ = static_cast<decltype(blockCount_)>(available / (static_cast<size_t>(headerSize_) + fixedBlockFinalSize_));
                    // Calculate the offset of the planar header section.
                    {
                        fixedPlanarHeaderOffset_ = size_ - static_cast<size_t>(headerSize_) * blockCount_;
                    }
                }
                if (static_cast<size_t>(blockCount_) * fixedBlockFinalSize_ + fixedStartOffset_ - padSize_ > fixedPlanarHeaderOffset_)
                {
                    assert(false);
                    throw MemoryException(MemoryException::ErrorCode::PageInitFailure, "Invalid header plane size for planar fixed block type!");
                }
                if (fixedPlanarHeaderOffset_ % headerAlignment_ != 0)
                {
                    assert(false);
                    throw MemoryException(MemoryException::ErrorCode::PageInitFailure, "Invalid header plane alignment for planar fixed block type!");
                }
            }
                break;
            default:
                assert(false);
                throw MemoryException(MemoryException::ErrorCode::PageInitFailure, "Invalid header organization for fixed block type!");
            }
            // Ensure configuration satisfies constraints.
            if (fixedStartOffset_ % fixedAlignment_ != 0)
            {
                assert(false);
                throw MemoryException(MemoryException::ErrorCode::PageInitFailure, "Start offset must match block alignment requirements!");
            }
            if ((fixedStartOffset_ + fixedBlockFinalSize_) % fixedAlignment_ != 0)
            {
                assert(false);
                throw MemoryException(MemoryException::ErrorCode::PageInitFailure, "Start offset added with final size must match block alignment requirements!");
            }
            if (fixedSize_ < sizeof(FreeChain_t) || fixedAlignment_ % alignof(FreeChain_t) != 0)
            {
                assert(false);
                throw MemoryException(MemoryException::ErrorCode::PageInitFailure, "Size and alignment of fixed block must be able to support entry for free list!");
            }
            // Update freeBlockCount_ and freeSpace_ to accurately represent availability after accounting for the final block size.
            freeBlockCount_ = blockCount_;
            freeSpace_ = static_cast<size_t>(freeBlockCount_) * fixedSize_;
            // Initialize the blocks and headers.
            fixedBlockLast_ = 0;
            if (config_.debugPatterns && headerOrg_ == BlockHeaderOrganization::Planar)
            {
                std::memset(page_, PATTERN::ALIGNMENT, fixedStartAlignment_);
            }
            if (fixedDelayedInitialization_ == 0)
            {
                InitializeFixedBlocks(blockCount_);
            }
        }
            break;
        case BlockType::Variable:
        {
            if (config_.debugPatterns)
            {
                assert(freeSpace_ == size_ && "FreeSpace should be initialized to size!");
                std::memset(page_, PATTERN::UNALLOCATED, freeSpace_);
            }
            switch (headerOrg_)
            {
            case BlockHeaderOrganization::Interleaved:
            {
                variableUnallocStart_ = headerSize_;
                variableMinAlloc_ = headerInterleavedBlockOffset_ + padSize_;
            }
            break;
            case BlockHeaderOrganization::Planar:
            {
                variableUnallocStart_ = 0;
                variableMinAlloc_ = padSize_ * 2;
            }
            break;
            default:
                assert(false);
                throw MemoryException(MemoryException::ErrorCode::PageInitFailure, "Invalid header organization for fixed block type!");
            }
            // Any allocation will require a header, so its size is subtracted so it is never misrepresented as potential free space.
            freeSpace_ -= headerSize_;
        }
            break;
        default:
            assert(false);
            throw MemoryException(MemoryException::ErrorCode::PageInitFailure, "Invalid block type!");
        }
    }

    void MemoryPage::InitializeHeader(void* header)
    {
#define InitHeaderFlag(enumVal, type, initVal)\
        if (static_cast<bool>(headerFlags_ & HeaderFlags::enumVal))\
        {\
            type* val = reinterpret_cast<type*>(reinterpret_cast<char*>(header) + blockInfo_.header.offset##enumVal);\
            *val = (initVal);\
        }\

        InitHeaderFlag(Label, HeaderLabel_t, nullptr);
        InitHeaderFlag(DeallocFuncPtr, HeaderDeallocFuncPtr_t, nullptr);
        InitHeaderFlag(AllocNum, HeaderAllocNum_t, 0);
        InitHeaderFlag(AllocCount, HeaderAllocCount_t, 0);
        InitHeaderFlag(IntendedSize, HeaderIntendedSize_t, 0);
        InitHeaderFlag(UseFlag, HeaderUseFlag_t, HeaderUseFlagInit);
#undef InitHeaderFlag
    }

    void MemoryPage::InitializeVariableHeader(void* header, size_t offset, size_t size)
    {
        BlockSize_t* data = reinterpret_cast<BlockSize_t*>(header);
        *data = size;
        ++data;
        if (headerOrg_ == BlockHeaderOrganization::Planar)
        {
            *data = offset;
            ++data;
        }
        InitializeHeader(data);
    }

    void MemoryPage::InitializeFixedBlocks(uint32_t num)
    {
        assert(blockType_ == BlockType::Fixed && "Only call this when using fixed blocks!");
        const auto blockEnd = MathMin(blockCount_, fixedBlockLast_ + num);
        num = blockEnd - blockCount_;
        // If the free list is empty and we're going to initialize blocks, set the free list to point to the first block that is about to be initialized.
        if (freeStart_ == InvalidFree && fixedBlockLast_ < blockEnd)
        {
            freeStart_ = static_cast<FreeChain_t>(fixedStartOffset_) + static_cast<FreeChain_t>(fixedBlockLast_) * fixedBlockFinalSize_;
        }
        // Because all of the headers are initialized to the same value, create one base and then copy it.
        array_t<unsigned char, 6 * sizeof(void*)> headerBase;
        assert(sizeof(headerBase) >= headerSize_ && "HeaderBase buffer must be able to contain the entire header!");
        InitializeHeader(&headerBase);
        // If using planar headers, initialize them first.
        if (headerOrg_ == BlockHeaderOrganization::Planar)
        {
            if (headerFlags_ == HeaderFlags::UseFlag)
            {
                BitMap_t* const headerStart = reinterpret_cast<BitMap_t*>(reinterpret_cast<char*>(page_) + fixedPlanarHeaderOffset_);
                BitMap_t* const beginC = headerStart + (fixedBlockLast_ + BitMapPackPadOffset) / BitMapSize;
                BitMap_t* const endC = headerStart + (blockEnd + BitMapPackPadOffset) / BitMapSize;
                for (BitMap_t* iterC = beginC; iterC < endC; ++iterC)
                {
                    static constexpr BitMap_t BitMapInit = ~(static_cast<BitMap_t>(HeaderUseFlagInit) - 1);
                    static_assert(BitMapInit == 0 || ~BitMapInit == 0, "BitMapInit must have the same value in all bits!");
                    *iterC = BitMapInit;
                }
            }
            else
            {
                char* const beginC = reinterpret_cast<char*>(page_) + fixedPlanarHeaderOffset_ + static_cast<size_t>(fixedBlockLast_) * headerSize_;
                char* const endC = beginC + num * headerSize_;
                for (char* iterC = beginC; iterC < endC; iterC += headerSize_)
                {
                    MemoryManager::Copy(iterC, &headerBase, headerSize_);
                }
            }
        }
        char* const dataC = reinterpret_cast<char*>(page_) + fixedStartOffset_;
        void(*initFunc)(const MemoryPage&, char*, void*);
        switch (headerOrg_)
        {
        case BlockHeaderOrganization::Interleaved:
        {
            static void(*initHeaderFunc)(const MemoryPage&, char*, void*) = [](const MemoryPage& page, char* data, void* headerBase)
            {
                char* header = data - page.fixedStartOffset_;
                MemoryManager::Copy(header, headerBase, page.headerSize_);
            };
            if (config_.debugPatterns)
            {
                initFunc = [](const MemoryPage& page, char* data, void* headerBase)
                {
                    initHeaderFunc(page, data, headerBase);
                    char* working = data - page.padSize_;
                    std::memset(working - page.fixedAlignSize_, PATTERN::ALIGNMENT, page.fixedAlignSize_);
                    std::memset(working, PATTERN::PADDING, page.padSize_);
                    std::memset(working += page.padSize_, PATTERN::UNALLOCATED, page.fixedSize_);
                    std::memset(working += page.fixedSize_, PATTERN::PADDING, page.padSize_);
                    std::memset(working + page.padSize_, PATTERN::ALIGNMENT, page.fixedHeaderAlignSize_);
                };
            }
            else
            {
                initFunc = initHeaderFunc;
            }
        }
        break;
        case BlockHeaderOrganization::Planar:
        {
            if (config_.debugPatterns)
            {
                initFunc = [](const MemoryPage& page, char* data, void*)
                {
                    char* working = data;
                    std::memset(working - page.padSize_, PATTERN::PADDING, page.padSize_);
                    std::memset(working, PATTERN::UNALLOCATED, page.fixedSize_);
                    std::memset(working += page.fixedSize_, PATTERN::PADDING, page.padSize_);
                    std::memset(working + page.padSize_, PATTERN::ALIGNMENT, page.fixedAlignSize_);
                };
            }
            else
            {
                initFunc = [](const MemoryPage&, char*, void*)
                {
                };
            }
        }
        break;
        default:
            assert(false);
            throw MemoryException(MemoryException::ErrorCode::Generic, "Invalid header organization for fixed block type!");
        }
        for (size_t offset = static_cast<size_t>(fixedBlockLast_) * fixedBlockFinalSize_; fixedBlockLast_ < blockEnd; ++fixedBlockLast_, offset += fixedBlockFinalSize_)
        {
            // Set this block to point to the next block for the free list.
            FreeChain_t* data = reinterpret_cast<FreeChain_t*>(dataC + offset);
            *data = static_cast<FreeChain_t>(offset) + fixedBlockFinalSize_;
            initFunc(*this, reinterpret_cast<char*>(data), &headerBase);
        }
        {
            // After the last block has been initialized, ensure that its free list pointer is set as invalid. There should not be any free blocks after the last initialized block.
            assert(fixedBlockLast_ <= blockCount_ && "Last block index should never exceed the block count!");
            FreeChain_t* data = reinterpret_cast<FreeChain_t*>(dataC + fixedBlockFinalSize_ * (fixedBlockLast_ - 1));
            *data = InvalidFree;
        }
    }

    void MemoryPage::SortBlocks(void)
    {
        MemoryBlock* iterFreeStart = GetFreeIteratorBegin();
        MemoryBlock* iterFreeEnd = iterFreeStart - freeBlockCount_;
        MemoryBlock* iterUsedStart = iterFreeEnd;
        MemoryBlock* iterUsedEnd = iterFreeStart - blockCount_;
        // Free blocks.
        std::sort(iterFreeEnd, iterFreeStart, [](const auto& a, const auto& b)
        {
            return std::tie(a.size, b.offset) < std::tie(b.size, a.offset);
        });
        // Used blocks.
        std::sort(iterUsedEnd, iterUsedStart, [](const auto& a, const auto& b)
        {
            return b.offset < a.offset;
        });
    }

    size_t MemoryPage::GetVariableUnallocatedSpaceEnd(void) const
    {
        assert(blockType_ == BlockType::Variable && "Only call this when using variable blocks!");
        switch (headerOrg_)
        {
        case BlockHeaderOrganization::Interleaved:
            return size_;
        case BlockHeaderOrganization::Planar:
            return size_ - static_cast<BlockSize_t>(blockCount_) * headerSize_;
        default:
            assert(false);
            throw MemoryException(MemoryException::ErrorCode::Generic, "Invalid header organization for variable block type!");
        }
    }

    bool MemoryPage::CreateVariableBlock(bool isFree, [[maybe_unused]] MemoryBlock& descriptor)
    {
        assert(blockType_ == BlockType::Variable && "Only call this when using variable blocks!");
        switch (headerOrg_)
        {
        case BlockHeaderOrganization::Interleaved:
        {

        }
            break;
        case BlockHeaderOrganization::Planar:
        {

        }
            break;
        default:
            assert(false);
            throw MemoryException(MemoryException::ErrorCode::Generic, "Invalid header organization for variable block type!");
        }
        ++blockCount_;
        if (isFree)
        {
            ++freeBlockCount_;
        }
        return true;
    }

    bool MemoryPage::DestroyVariableBlock(bool isFree, [[maybe_unused]] const MemoryBlock& descriptor)
    {
        assert(blockType_ == BlockType::Variable && "Only call this when using variable blocks!");
        switch (headerOrg_)
        {
        case BlockHeaderOrganization::Interleaved:
        {

        }
            break;
        case BlockHeaderOrganization::Planar:
        {

        }
            break;
        default:
            assert(false);
            throw MemoryException(MemoryException::ErrorCode::Generic, "Invalid header organization for variable block type!");
        }
        --blockCount_;
        if (isFree)
        {
            --freeBlockCount_;
        }
        return true;
    }

    bool MemoryPage::SetVariableBlockUsage(bool isFree, [[maybe_unused]] const MemoryBlock& descriptor)
    {
        assert(blockType_ == BlockType::Variable && "Only call this when using variable blocks!");
        switch (headerOrg_)
        {
        case BlockHeaderOrganization::Interleaved:
        {

        }
            break;
        case BlockHeaderOrganization::Planar:
        {

        }
            break;
        default:
            assert(false);
            throw MemoryException(MemoryException::ErrorCode::Generic, "Invalid header organization for variable block type!");
        }
        if (isFree)
        {
            ++freeBlockCount_;
        }
        else
        {
            --freeBlockCount_;
        }
        return true;
    }

    MemoryBlock* MemoryPage::GetFreeIteratorBegin(void) const
    {
        return reinterpret_cast<MemoryBlock*>(reinterpret_cast<char*>(page_) + size_) - 1;
    }

    MemoryBlock* MemoryPage::GetFreeIteratorEnd(void) const
    {
        return reinterpret_cast<MemoryBlock*>(reinterpret_cast<char*>(page_) + size_) - freeBlockCount_;
    }

    MemoryBlock* MemoryPage::GetUsedIteratorBegin(void) const
    {
        return reinterpret_cast<MemoryBlock*>(reinterpret_cast<char*>(page_) + size_) - freeBlockCount_;
    }

    MemoryBlock* MemoryPage::GetUsedIteratorEnd(void) const
    {
        return reinterpret_cast<MemoryBlock*>(reinterpret_cast<char*>(page_) + size_) - blockCount_;
    }

#pragma endregion Private Functions

    //--------------------------------------------------------------------------
    // Iterator:
    //--------------------------------------------------------------------------

#pragma region Iterator

    const MemoryPage::FreeListIterator MemoryPage::FreeListIterator::end{};

    MemoryPage::FreeListIterator::FreeListIterator(void)
        : page(nullptr)
        , current(InvalidFree)
    {
    }

    MemoryPage::FreeListIterator::FreeListIterator(const MemoryPage& page_)
        : page(&page_)
        , current(page_.freeStart_)
    {
    }

    MemoryPage::FreeListIterator& MemoryPage::FreeListIterator::operator++(void)
    {
        if (operator!=(end))
        {
            FreeChain_t* next = reinterpret_cast<FreeChain_t*>(reinterpret_cast<char*>(page->page_) + current);
            current = *next;
        }
        return *this;
    }

    bool MemoryPage::FreeListIterator::operator==(const FreeListIterator& other) const
    {
        return current == other.current;
    }

    bool MemoryPage::FreeListIterator::operator!=(const FreeListIterator& other) const
    {
        return current != other.current;
    }

    MemoryPage::HeaderInterface MemoryPage::FreeListIterator::Header(void) const
    {
        switch (page->headerOrg_)
        {
        case BlockHeaderOrganization::Interleaved:
            return HeaderInterface(*page, operator!=(end) ? current - page->headerSize_ : 0);
        case BlockHeaderOrganization::Planar:
            return HeaderInterface(*page, operator!=(end) ? current : 0);
        default:
            assert(false);
            throw MemoryException(MemoryException::ErrorCode::Generic, "Invalid header organization for variable block type!");
        }
    }

    size_t MemoryPage::FreeListIterator::HeaderOffset(void) const
    {
        // TODO: Header offset.
        return current;
    }

    char* MemoryPage::FreeListIterator::Data(void) const
    {
        switch (page->headerOrg_)
        {
        case BlockHeaderOrganization::Interleaved:
            return operator!=(end) ? reinterpret_cast<char*>(page->page_) + current : nullptr;
        case BlockHeaderOrganization::Planar:
            return operator!=(end) ? reinterpret_cast<char*>(page->page_) + current : nullptr;
        default:
            assert(false);
            throw MemoryException(MemoryException::ErrorCode::Generic, "Invalid header organization for variable block type!");
        }
    }

    size_t MemoryPage::FreeListIterator::DataOffset(void) const
    {
        return current;
    }

#pragma endregion Iterator

    //--------------------------------------------------------------------------
    // Header Interface:
    //--------------------------------------------------------------------------

#pragma region Header Interface

    MemoryPage::HeaderInterface::HeaderInterface(const MemoryPage& page_, size_t offset_)
        : page(&page_)
        , offset(offset_)
        , fixedIndex(offset_)
        , start(reinterpret_cast<char*>(page->page_) + offset_)
    {
    }

    MemoryPage::HeaderInterface::HeaderInterface(const MemoryPage& page_, size_t offset_, size_t index)
        : page(&page_)
        , offset(offset_)
        , fixedIndex(index)
        , start(reinterpret_cast<char*>(page->page_) + offset_)
    {
    }

    bool MemoryPage::HeaderInterface::ValidateFree(void) const
    {
        return false;
    }

    void MemoryPage::HeaderInterface::Allocate(void) const
    {
    }

    bool MemoryPage::HeaderInterface::ValidateUsed(void) const
    {
        return false;
    }

    void MemoryPage::HeaderInterface::Deallocate(void) const
    {
    }

    BlockSize_t MemoryPage::HeaderInterface::GetBlockSize(void) const
    {
        switch (page->blockType_)
        {
        case BlockType::Fixed:
            return page->fixedSize_;
        case BlockType::Variable:
            return *reinterpret_cast<BlockSize_t*>(start);
        default:
            assert(false);
            throw MemoryException(MemoryException::ErrorCode::Generic, "Invalid block type!");
        }
    }

    void MemoryPage::HeaderInterface::SetBlockSize(BlockSize_t size)
    {
        switch (page->blockType_)
        {
        case BlockType::Variable:
            *reinterpret_cast<BlockSize_t*>(start) = size;
            break;
        case BlockType::Fixed:
        default:
            assert(false);
            throw MemoryException(MemoryException::ErrorCode::Generic, "Invalid block type!");
        }
    }

    BlockSize_t MemoryPage::HeaderInterface::GetBlockOffset(void) const
    {
        switch (page->blockType_)
        {
        case BlockType::Fixed:
            return static_cast<BlockSize_t>(page->fixedStartOffset_ + fixedIndex * page->fixedSize_);
        case BlockType::Variable:
            switch (page->headerOrg_)
            {
            case BlockHeaderOrganization::Interleaved:
                return static_cast<BlockSize_t>((start + page->headerSize_ + page->padSize_) - reinterpret_cast<char*>(page->page_));
            case BlockHeaderOrganization::Planar:
                return *reinterpret_cast<BlockSize_t*>(start + 2 * sizeof(BlockSize_t));
            default:
                assert(false);
                throw MemoryException(MemoryException::ErrorCode::Generic, "Invalid header organization for variable block type!");
            }
            break;
        default:
            assert(false);
            throw MemoryException(MemoryException::ErrorCode::Generic, "Invalid block type!");
        }
    }

    void MemoryPage::HeaderInterface::SetBlockOffset(BlockSize_t offset_)
    {
        switch (page->blockType_)
        {
        case BlockType::Variable:
            switch (page->headerOrg_)
            {
            case BlockHeaderOrganization::Planar:
                *reinterpret_cast<BlockSize_t*>(start + 2 * sizeof(BlockSize_t)) = offset_;
                break;
            case BlockHeaderOrganization::Interleaved:
            default:
                assert(false);
                throw MemoryException(MemoryException::ErrorCode::Generic, "Invalid header organization for setting offset of variable block type!");
            }
            break;
        case BlockType::Fixed:
        default:
            assert(false);
            throw MemoryException(MemoryException::ErrorCode::Generic, "Invalid block type!");
        }
    }

    BlockSize_t MemoryPage::HeaderInterface::GetBlockAlignmentPadding(void) const
    {
        switch (page->blockType_)
        {
        case BlockType::Fixed:
            return 0;
        case BlockType::Variable:
            return *reinterpret_cast<BlockSize_t*>(start + sizeof(BlockSize_t));
        default:
            assert(false);
            throw MemoryException(MemoryException::ErrorCode::Generic, "Invalid block type!");
        }
    }

    void MemoryPage::HeaderInterface::SetBlockAlignmentPadding(BlockSize_t padding)
    {
        switch (page->blockType_)
        {
        case BlockType::Variable:
            *reinterpret_cast<BlockSize_t*>(start + sizeof(BlockSize_t)) = padding;
            break;
        case BlockType::Fixed:
        default:
            assert(false);
            throw MemoryException(MemoryException::ErrorCode::Generic, "Invalid block type!");
        }
    }

#pragma endregion Header Interface

}	// namespace
