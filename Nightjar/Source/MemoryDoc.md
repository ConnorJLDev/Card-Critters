//------------------------------------------------------------------------------
//
// File Name:	MemoryDoc.md
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Rough documentation of how the custom memory management 
//              operates.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

### Memory managment overview
- Memory Manager
    - Has several Memory Page Allocators.
    - When allocating memory, selects an Allocator to categorize and group things.
    - When deallocating memory, first attempts to deallocate from the matching Allocator per categorization. If this fails, then each Allocator is checked.

- Memory Page Allocator
    - Manages multiple Memory Pages.
    - When allocating memory, attempts to find a Page with a Block that either satisfies First-Fit or Best-Fit, configurable.
    - When deallocating memory, scans through each Page to check if the address is within those bounds. If a match is found, deallocation is passed to the matching Page.

- Memory Page
    - Represents a single, contiguous page of memory.
    - Contains a variable number of blocks.
    - Storage pattern for packed block headers is as follows:
        - From the lowest point, memory allocations point here.
        - From the highest point, Memory Blocks are stored as a stack with the following characteristics:
            - First (closest to end), all of the free Blocks are stored as an interleaved pair of entries, one with descending size, and the other with increasing offset.
            - Then (furthest from end), all of the occupied Blocks are stored in increasing offset.
    - When allocating memory, attempts to find a Block that either satisfies First-Fit or Best-Fit, configurable.
        - On success, the Block is either marked or split (depending on size) for usage, and the Block stack is sorted.
    - When deallocating memory, if the address is within the bounds, then a matching Block is searched for. If a match is found, the matching Block is marked as free and a free block merge check is performed. If a match is not found, something has gone wrong.
        - On success, the Block is either marked as free, checked to see if it can merge into neighboring Blocks (and performed if possible), and the Block stack is sorted.

- Memory Block
    - Represents EITHER:
        - a single allocation of memory to the client, or 
        - a segment of unallocated memory.
