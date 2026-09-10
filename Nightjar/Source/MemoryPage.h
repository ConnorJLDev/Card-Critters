//------------------------------------------------------------------------------
//
// File Name:	MemoryPage.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class that represents a contiguous region of memory, split into
//              MemoryBlocks.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "MemoryBlock.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    // Forward Declarations:

    // Typedefs:

    // Class Definition:
    class MemoryPage
    {
        friend class ImGuiWindowTestMemory;

        // Public Constants, Enums, and Structs:
    public:
        // Modes for how each block represents its memory.
        enum class BlockType : uint_fast8_t
        {
            Fixed,      // Each block is a fixed size.
            Variable,   // Each block has a variable size.
            NUM_ENTRIES,
        };

        // Modes for how block headers are organized.
        enum class BlockHeaderOrganization : uint_fast8_t
        {
            Interleaved,// Block headers are adjacent to their allocation.
            Planar,     // Block headers are packed at the end of the page.
            NUM_ENTRIES,
        };

        // Flags for what is stored in block headers.
        enum class HeaderFlags : uint8_t
        {
            // Block has no extra data.
            Empty = 0,
            // Flag to indicate that headers store a label, as a pointer to a null-terminated string.
            Label = 1 << 0,
            // Flag to indicate that headers store a function pointer to properly destruct their memory block.
            DeallocFuncPtr = 1 << 1,
            // Flag to indicate that headers store the current allocation counter of the page when being allocated.
            AllocNum = 1 << 2,
            // Flag to indicate that headers track the number of times their block has been allocated.
            AllocCount = 1 << 3,
            // Flag to indicate that headers store the intended allocation size, which is always equal to or smaller than the block size.
            IntendedSize = 1 << 4,
            // Flag to indicate that headers store a flag to indicate if their block is in use (allocated).
            UseFlag = 1 << 5,
        };

        // Represents information on each block and how they are handled.
        struct Config
        {
            // The type of block allocations in this page.
            BlockType type;
            // The organization of block headers.
            BlockHeaderOrganization headerOrganization;
            // Flag for performing debug checks on memory patterns.
            bool debugPatterns : 1;
            // Flag for performing debug checks on memory block headers.
            bool debugHeaders : 1;
            // Flags for what is stored in headers.
            HeaderFlags headerFlags;
            // Size of padding on each side of each block, in bytes.
            uint32_t padSize;
            union
            {
                // Configuration settings specific to fixed blocks.
                struct
                {
                    // Size of each fixed block.
                    uint32_t size;
                    // Alignment of each fixed block.
                    uint32_t alignment;
                    // Number of fixed blocks initialized when the last block is reached. 0 means all blocks are initialized on page initialization.
                    uint32_t delayedInitialization;
                } fixed;
                // Configuration settings specific to variable blocks.
                struct
                {
                    // Maximum size that neighboring free blocks can be to absorb when allocating a block.
                    uint32_t absorbNeighboringEmpty;
                    // Flag to indicate that planar headers should be grouped by used vs free.
                    bool groupPlanarHeaders;
                    // Flag to indicate that planar headers should be sorted by size and/or offset.
                    bool sortPlanarHeaders;
                } variable;
            };
        };

        // Constructors/Destructors:
    public:
        MemoryPage(void) = delete;
        MemoryPage(const Config& config, size_t size);
        MemoryPage(const Config& config, void* page, size_t size);
        MemoryPage(const MemoryPage& other) = delete;
        MemoryPage(MemoryPage&& other) noexcept;
        MemoryPage& operator=(const MemoryPage& other) = delete;

        ~MemoryPage(void);

        // Public Static Functions:
    public:
        ENUM_DECLARE_OPS(HeaderFlags);

        ENUM_MAPS_CLASS_SEQUENTIAL_DECLARE(BlockType);
        ENUM_MAPS_CLASS_SEQUENTIAL_DECLARE(BlockHeaderOrganization);
        ENUM_MAPS_CLASS_DECLARE(HeaderFlags);

        // Public Functions:
    public:
        /// <summary>
        /// Finds the first block of memory that can accommodate the requested size and alignment for data allocation.
        /// </summary>
        /// <param name="size">The size of the data to be stored, in bytes.</param>
        /// <param name="alignment">The alignment of the data to be stored.</param>
        /// <returns>The first memory block that fits the requested parameters if found, or nullptr if no block was found.</returns>
        MemoryBlock FirstFit(size_t size, size_t alignment = 1);
        
        /// <summary>
        /// Finds the block of memory that best matches the requested size and alignment for data allocation.
        /// </summary>
        /// <param name="size">The size of the data to be stored, in bytes.</param>
        /// <param name="alignment">The alignment of the data to be stored.</param>
        /// <returns>The best memory block for the requested parameters, or nullptr if no block was found.</returns>
        MemoryBlock BestFit(size_t size, size_t alignment = 1);

        /// <summary>
        /// Allocates memory using the requested memory block.
        /// </summary>
        /// <param name="block">The block being allocated to.</param>
        /// <param name="size">The size of the memory allocation.</param>
        /// <param name="alignment">The alignment of the start of the memory allocation.</param>
        /// <returns>A pointer to the requested memory.</returns>
        void* Allocate(MemoryBlock block, size_t size, size_t alignment = 1);

        /// <summary>
        /// Deallocates memory.
        /// </summary>
        /// <param name="data">The address of the memory being deallocated.</param>
        /// <returns>True if the address is within this page, false otherwise.</returns>
        bool Deallocate(void* data);

        /// <summary>
        /// Gets the size of the largest block of free memory, in bytes.
        /// </summary>
        /// <returns>The size of the largest block of free memory.</returns>
        size_t GetLargestFreeBlockSize(void) const;

        /// <summary>
        /// Gets the size of the smallest block of free memory, in bytes.
        /// </summary>
        /// <returns>The size of the smallest block of free memory.</returns>
        size_t GetSmallestFreeBlockSize(void) const;

        /// <summary>
        /// Gets the size of this page in bytes.
        /// </summary>
        /// <returns>The size of this page.</returns>
        size_t GetSize(void) const;

        // Public Event Handlers
    public:

        // Private Constants, Enums, and Structs:
    private:
        // Type used for header sizes, alignments, and offsets.
        using HeaderSize_t = uint8_t;

        // Type used for links in the free list chain.
        using FreeChain_t = size_t;

        // Interface for getting header information about blocks.
        struct HeaderInterface
        {
        public:
            HeaderInterface(void) = delete;
            HeaderInterface(const MemoryPage& page, size_t offset);
            HeaderInterface(const MemoryPage& page, size_t offset, size_t index);
            HeaderInterface(const HeaderInterface& other) = delete;
            HeaderInterface& operator=(const HeaderInterface& other) = delete;

            // Validates that the header is free and can be allocated to.
            bool ValidateFree(void) const;

            // Updates the header after its block has been allocated.
            void Allocate(void) const;

            // Validates that the header is in use and can be deallocated.
            bool ValidateUsed(void) const;

            // Updates the header after its block has been deallocated.
            void Deallocate(void) const;

            // Gets the size of the block this header represents.
            BlockSize_t GetBlockSize(void) const;

            // Sets the size of the block this header represents.
            void SetBlockSize(BlockSize_t size);

            // Gets the offset into the page of the block this header represents.
            BlockSize_t GetBlockOffset(void) const;

            // Sets the offset into the page of the block this header represents.
            void SetBlockOffset(BlockSize_t offset);

            // Gets the amount of padding bytes used for the block this header represents.
            BlockSize_t GetBlockAlignmentPadding(void) const;

            // Sets the amount of padding bytes used for the block this header represents.
            void SetBlockAlignmentPadding(BlockSize_t padding);

        private:
            // The page containing the header being inspected.
            const MemoryPage* const page;
            // Offset into the page of the block being inspected.
            const size_t offset;
            // Block index of the fixed block being inspected.
            const size_t fixedIndex;
            // The address of the start of the header.
            char* const start;
        };

        // Iterator for conveniently traversing the free list.
        struct FreeListIterator
        {
        public:
            FreeListIterator(const MemoryPage& page);
            FreeListIterator(const FreeListIterator& other) = delete;
            FreeListIterator& operator=(const FreeListIterator& other) = delete;

            FreeListIterator& operator++(void);
            bool operator==(const FreeListIterator& other) const;
            bool operator!=(const FreeListIterator& other) const;

            // Gets an interface for the header of the current block data.
            HeaderInterface Header(void) const;

            // Gets the offset of the header of the current block data into the page.
            size_t HeaderOffset(void) const;

            // Gets a pointer to the current block data.
            char* Data(void) const;

            // Gets the offset of the current block data into the page.
            size_t DataOffset(void) const;

        public:
            // Iterator indicating the end of the free list.
            static const FreeListIterator end;

        private:
            // The page this iterator is traversing.
            const MemoryPage* const page;
            // The current block being represented.
            size_t current;
            // Default constructor for internal use.
            FreeListIterator(void);
        };

        // Represents information on each block and how they are handled.
        struct BlockInfo
        {
            BlockInfo(void);
            BlockInfo(const Config& config, size_t size);

            // Metadata specific to block headers.
            struct HeaderMeta
            {
                HeaderMeta(void);
                HeaderMeta(const Config& config);

                // Size of each block header.
                HeaderSize_t size;
                // Alignment of each block header.
                HeaderSize_t alignment;
                // How many bytes before a block the header appears when using interleaved headers.
                HeaderSize_t interleavedBlockOffset;
                // Offset of Label header field.
                HeaderSize_t offsetLabel;
                // Offset of DeallocFuncPtr header field.
                HeaderSize_t offsetDeallocFuncPtr;
                // Offset of AllocNum header field.
                HeaderSize_t offsetAllocNum;
                // Offset of AllocCount header field.
                HeaderSize_t offsetAllocCount;
                // Offset of IntendedSize header field.
                HeaderSize_t offsetIntendedSize;
                // Offset of UseFlag header field.
                HeaderSize_t offsetUseFlag;
            } header;
            union
            {
                // Metadata specific to fixed blocks.
                struct
                {
                    // Number of alignment bytes used for fixed inline block headers.
                    uint32_t headerAlignSize;
                    // Number of alignment bytes used for the first fixed block (with planar headers).
                    uint32_t startAlignment;
                    // Final size of fixed blocks after accounting for padding and alignment.
                    uint32_t blockFinalSize;
                    // Number of alignment bytes used for fixed blocks.
                    uint32_t alignSize;
                    // Offset into the page to the data of the first fixed block.
                    uint32_t startOffset;
                    // Index of the last initialized fixed block.
                    uint32_t blockLast;
                    // Offset into the page where the headers start when using fixed planar blocks.
                    size_t planarHeaderOffset;
                } fixed;
                // Metadata specific to variable blocks.
                struct
                {
                    // Offset to where memory has never been allocated in this page. The memory between here and the end of the page (or the start of the planar header section) is always free.
                    size_t unallocStart;
                    // Minimum allocatable amount of space.
                    size_t minAlloc;
                } variable;
            };
        };

        static const HeaderSize_t InvalidHeaderOffset;

        // Private Static Functions:
    private:

        // Private Functions:
    private:
        /// <summary>
        /// Initializes the page.
        /// </summary>
        void Initialize(void);
        
        /// <summary>
        /// Initializes a block header.
        /// </summary>
        /// <param name="header">The address of the block header to initialize.</param>
        void InitializeHeader(void* header);
        
        /// <summary>
        /// Initializes a variable block header.
        /// </summary>
        /// <param name="header">The address of the block header to initialize.</param>
        /// <param name="offset">The offset of the block.</param>
        /// <param name="size">The size of the block.</param>
        void InitializeVariableHeader(void* header, size_t offset, size_t size);
        
        /// <summary>
        /// Initializes a set number of fixed blocks.
        /// </summary>
        /// <param name="num">The number of blocks to initialize.</param>
        void InitializeFixedBlocks(uint32_t num);
        
        /// <summary>
        /// Sorts block headers.
        /// </summary>
        void SortBlocks(void);

        /// <summary>
        /// Gets the offset of the end of unallocated space for variable blocks.
        /// </summary>
        /// <returns>The offset of the end of unallocated space.</returns>
        size_t GetVariableUnallocatedSpaceEnd(void) const;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="isFree"></param>
        /// <param name="descriptor"></param>
        /// <returns></returns>
        bool CreateVariableBlock(bool isFree, MemoryBlock& descriptor);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="isFree"></param>
        /// <param name="descriptor"></param>
        /// <returns></returns>
        bool DestroyVariableBlock(bool isFree, const MemoryBlock& descriptor);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="isFree"></param>
        /// <param name="descriptor"></param>
        /// <returns></returns>
        bool SetVariableBlockUsage(bool isFree, const MemoryBlock& descriptor);

        /// <summary>
        /// Gets an iterator to the first free block.
        /// </summary>
        /// <returns>An iterator to the first free block.</returns>
        MemoryBlock* GetFreeIteratorBegin(void) const;

        /// <summary>
        /// Gets an iterator to the end of the free blocks.
        /// </summary>
        /// <returns>An iterator to the end of the free blocks.</returns>
        MemoryBlock* GetFreeIteratorEnd(void) const;

        /// <summary>
        /// Gets an iterator to the first used block.
        /// </summary>
        /// <returns>An iterator to the first used block.</returns>
        MemoryBlock* GetUsedIteratorBegin(void) const;

        /// <summary>
        /// Gets an iterator to the end of the used blocks.
        /// </summary>
        /// <returns>An iterator to the end of the used blocks.</returns>
        MemoryBlock* GetUsedIteratorEnd(void) const;

        // Private Static Variables:
    private:

        // Private Variables:
    private:
        // Pointer to allocated memory.
        void* page_;
        // Size of the page in bytes.
        const size_t size_;
        // Amount of free space in bytes.
        size_t freeSpace_;
        // Number of free blocks available.
        uint32_t freeBlockCount_;
        // Total number of blocks.
        uint32_t blockCount_;
        // Flag for if this page owns the memory it points to.
        bool owning_;
        // Configuration of blocks and block headers handling.
        Config config_;
        // Information on each block and how they are handled.
        BlockInfo blockInfo_;
        // Start of the free blocks.
        FreeChain_t freeStart_;
    };

}	// namespace
