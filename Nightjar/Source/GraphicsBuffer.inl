//------------------------------------------------------------------------------
//
// File Name:	GraphicsBuffer.inl
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Templated functions for GraphicsBuffer.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "GraphicsBuffer.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    //--------------------------------------------------------------------------
    // Public Functions:
    //--------------------------------------------------------------------------

#pragma region Public Functions

#pragma region SetDirect

    template<BufferValue T>
    void GraphicsBuffer::SetDirectValue(string_view_arg name, const T& value)
    {
        SetDirect(name, 0, reinterpret_cast<const void*>(&value), static_cast<size_t>(sizeof(T)));
    }

    template<BufferValue T>
    void GraphicsBuffer::SetDirectValue(string_view_arg name, size_t index, const T& value)
    {
        SetDirect(name, static_cast<size_t>(index * sizeof(T)), reinterpret_cast<const void*>(&value), static_cast<size_t>(sizeof(T)));
    }

    template<BufferValue T>
    void GraphicsBuffer::SetDirectValues(string_view_arg name, size_t startIndex, const T* values, size_t count)
    {
        SetDirect(name, static_cast<size_t>(startIndex * sizeof(T)), reinterpret_cast<const void*>(values), static_cast<size_t>(count * sizeof(T)));
    }

    template<BufferValue T>
    void GraphicsBuffer::SetDirectBlockValue(size_t blockIndex, string_view_arg name, const T& value)
    {
        SetDirectBlock(blockIndex, name, 0, reinterpret_cast<const void*>(&value), static_cast<size_t>(sizeof(T)));
    }

    template<BufferValue T>
    void GraphicsBuffer::SetDirectBlockValue(size_t blockIndex, string_view_arg name, size_t index, const T& value)
    {
        SetDirectBlock(blockIndex, name, static_cast<size_t>(index * sizeof(T)), reinterpret_cast<const void*>(&value), static_cast<size_t>(sizeof(T)));
    }

    template<BufferValue T>
    void GraphicsBuffer::SetDirectBlockValues(size_t blockIndex, string_view_arg name, size_t index, const T* values, size_t count)
    {
        SetDirectBlock(blockIndex, name, static_cast<size_t>(index * sizeof(T)), reinterpret_cast<const void*>(values), static_cast<size_t>(count * sizeof(T)));
    }

    template<BufferValue T>
    void GraphicsBuffer::SetDirectValue(size_t offset, const T& value)
    {
        SetDirect(offset, reinterpret_cast<const void*>(&value), static_cast<size_t>(sizeof(T)));
    }

    template<BufferValue T>
    void GraphicsBuffer::SetDirectValues(size_t offset, const T* values, size_t count)
    {
        SetDirect(offset, reinterpret_cast<const void*>(values), static_cast<size_t>(count * sizeof(T)));
    }

    template<BufferValue T>
    void GraphicsBuffer::SetDirectBlockValue(size_t blockIndex, size_t offset, const T& value)
    {
        SetDirectBlock(blockIndex, offset, reinterpret_cast<const void*>(&value), static_cast<size_t>(sizeof(T)));
    }

    template<BufferValue T>
    void GraphicsBuffer::SetDirectBlockValues(size_t blockIndex, size_t offset, const T* values, size_t count)
    {
        SetDirectBlock(blockIndex, offset, reinterpret_cast<const void*>(values), static_cast<size_t>(count * sizeof(T)));
    }

#pragma endregion SetDirect

#pragma region SetLocal

    template<BufferValue T>
    void GraphicsBuffer::SetLocalValue(string_view_arg name, const T& value)
    {
        size_t offset = 0;
        if (descriptor_.GetOffset(name, offset))
        {
            SetLocal(offset, value);
        }
    }

    template<BufferValue T>
    void GraphicsBuffer::SetLocalValue(string_view_arg name, size_t index, const T& value)
    {
        assert(GetUsingLocalCopy() && "Buffer must have local copy to use local functions!");
        size_t offset = 0;
        if (descriptor_.GetOffset(name, index, offset))
        {
            SetLocal(offset, value);
        }
    }

    template<BufferValue T>
    void GraphicsBuffer::SetLocalValues(string_view_arg name, size_t index, const T* values, size_t count)
    {
        SetLocal(name, static_cast<size_t>(index * sizeof(T)), reinterpret_cast<const void*>(values), static_cast<size_t>(count * sizeof(T)));
    }

    template<BufferValue T>
    void GraphicsBuffer::SetLocalBlockValue(size_t blockIndex, string_view_arg name, const T& value)
    {
        size_t offset = 0;
        if (descriptor_.GetOffset(name, offset))
        {
            SetLocalBlock(blockIndex, offset, value);
        }
    }

    template<BufferValue T>
    void GraphicsBuffer::SetLocalBlockValue(size_t blockIndex, string_view_arg name, size_t index, const T& value)
    {
        assert(GetUsingLocalCopy() && "Buffer must have local copy to use local functions!");
        size_t offset = 0;
        if (descriptor_.GetOffset(name, index, offset))
        {
            SetLocalBlock(blockIndex, offset, value);
        }
    }

    template<BufferValue T>
    void GraphicsBuffer::SetLocalBlockValues(size_t blockIndex, string_view_arg name, size_t index, const T* values, size_t count)
    {
        SetLocalBlock(blockIndex, name, static_cast<size_t>(index * sizeof(T)), reinterpret_cast<const void*>(values), static_cast<size_t>(count * sizeof(T)));
    }

    template<BufferValue T>
    void GraphicsBuffer::SetLocalValue(size_t offset, const T& value)
    {
        assert(GetUsingLocalCopy() && "Buffer must have local copy to use local functions!");
        MemoryManager::Copy(GetLocalBuffer(offset), &value, sizeof(T));
        //*reinterpret_cast<T*>(GetLocalBuffer(offset)) = value;
    }

    template<BufferValue T>
    void GraphicsBuffer::SetLocalValues(size_t offset, const T* values, size_t count)
    {
        SetLocal(offset, reinterpret_cast<const void*>(values), static_cast<size_t>(count * sizeof(T)));
    }

    template<BufferValue T>
    void GraphicsBuffer::SetLocalBlockValue(size_t blockIndex, size_t offset, const T& value)
    {
        SetLocal(offset + (blockIndex * BlockSize()), value);
    }

    template<BufferValue T>
    void GraphicsBuffer::SetLocalBlockValues(size_t blockIndex, size_t offset, const T* values, size_t count)
    {
        SetLocalBlock(blockIndex, offset, reinterpret_cast<const void*>(values), static_cast<size_t>(count * sizeof(T)));
    }

#pragma endregion SetLocal

#pragma region GetDirect

    template<BufferValue T>
    void GraphicsBuffer::GetDirectValue(string_view_arg name, T& value)
    {
        GetDirect(name, 0, reinterpret_cast<void*>(&value), static_cast<size_t>(sizeof(T)));
    }

    template<BufferValue T>
    void GraphicsBuffer::GetDirectValue(string_view_arg name, size_t index, T& value)
    {
        GetDirect(name, static_cast<size_t>(index * sizeof(T)), reinterpret_cast<void*>(&value), static_cast<size_t>(sizeof(T)));
    }

    template<BufferValue T>
    void GraphicsBuffer::GetDirectValues(string_view_arg name, size_t index, T* values, size_t count)
    {
        GetDirect(name, static_cast<size_t>(index * sizeof(T)), reinterpret_cast<void*>(values), static_cast<size_t>(count * sizeof(T)));
    }

    template<BufferValue T>
    void GraphicsBuffer::GetDirectBlockValue(size_t blockIndex, string_view_arg name, T& value)
    {
        GetDirectBlock(blockIndex, name, 0, reinterpret_cast<void*>(&value), static_cast<size_t>(sizeof(T)));
    }

    template<BufferValue T>
    void GraphicsBuffer::GetDirectBlockValue(size_t blockIndex, string_view_arg name, size_t index, T& value)
    {
        GetDirectBlock(blockIndex, name, static_cast<size_t>(index * sizeof(T)), reinterpret_cast<void*>(value), static_cast<size_t>(sizeof(T)));
    }

    template<BufferValue T>
    void GraphicsBuffer::GetDirectBlockValues(size_t blockIndex, string_view_arg name, size_t index, T* values, size_t count)
    {
        GetDirectBlock(blockIndex, name, static_cast<size_t>(index * sizeof(T)), reinterpret_cast<void*>(values), static_cast<size_t>(count * sizeof(T)));
    }

    template<BufferValue T>
    void GraphicsBuffer::GetDirectValue(size_t offset, T& value)
    {
        GetDirect(offset, reinterpret_cast<void*>(&value), static_cast<size_t>(sizeof(T)));
    }

    template<BufferValue T>
    void GraphicsBuffer::GetDirectValues(size_t offset, T* values, size_t count)
    {
        GetDirect(offset, reinterpret_cast<void*>(values), static_cast<size_t>(count * sizeof(T)));
    }

    template<BufferValue T>
    void GraphicsBuffer::GetDirectBlockValue(size_t blockIndex, size_t offset, T& value)
    {
        GetDirectBlock(blockIndex, offset, reinterpret_cast<void*>(&value), static_cast<size_t>(sizeof(T)));
    }

    template<BufferValue T>
    void GraphicsBuffer::GetDirectBlockValues(size_t blockIndex, size_t offset, T* values, size_t count)
    {
        GetDirectBlock(blockIndex, offset, reinterpret_cast<void*>(values), static_cast<size_t>(count * sizeof(T)));
    }

#pragma endregion GetDirect

#pragma region GetLocal

    template<BufferValue T>
    void GraphicsBuffer::GetLocalValue(string_view_arg name, T& value)
    {
        size_t offset = 0;
        if (descriptor_.GetOffset(name, offset))
        {
            GetLocal(offset, value);
        }
    }

    template<BufferValue T>
    void GraphicsBuffer::GetLocalValue(string_view_arg name, size_t index, T& value)
    {
        assert(GetUsingLocalCopy() && "Buffer must have local copy to use local functions!");
        size_t offset = 0;
        if (descriptor_.GetOffset(name, index, offset))
        {
            GetLocal(offset, value);
        }
    }

    template<BufferValue T>
    void GraphicsBuffer::GetLocalValues(string_view_arg name, size_t index, T* values, size_t count)
    {
        GetLocal(name, static_cast<size_t>(index * sizeof(T)), reinterpret_cast<void*>(values), static_cast<size_t>(count * sizeof(T)));
    }

    template<BufferValue T>
    void GraphicsBuffer::GetLocalBlockValue(size_t blockIndex, string_view_arg name, T& value)
    {
        size_t offset = 0;
        if (descriptor_.GetOffset(name, offset))
        {
            GetLocalBlock(blockIndex, offset, value);
        }
    }

    template<BufferValue T>
    void GraphicsBuffer::GetLocalBlockValue(size_t blockIndex, string_view_arg name, size_t index, T& value)
    {
        assert(GetUsingLocalCopy() && "Buffer must have local copy to use local functions!");
        size_t offset = 0;
        if (descriptor_.GetOffset(name, index, offset))
        {
            GetLocalBlock(blockIndex, offset, value);
        }
    }

    template<BufferValue T>
    void GraphicsBuffer::GetLocalBlockValues(size_t blockIndex, string_view_arg name, size_t index, T* values, size_t count)
    {
        GetLocalBlock(blockIndex, name, static_cast<size_t>(index * sizeof(T)), reinterpret_cast<void*>(values), static_cast<size_t>(count * sizeof(T)));
    }

    template<BufferValue T>
    void GraphicsBuffer::GetLocalValue(size_t offset, T& value)
    {
        assert(GetUsingLocalCopy() && "Buffer must have local copy to use local functions!");
        MemoryManager::Copy(&value, GetLocalBuffer(offset), sizeof(T));
        //value = *reinterpret_cast<T*>(GetLocalBuffer(offset));
    }

    template<BufferValue T>
    void GraphicsBuffer::GetLocalValues(size_t offset, T* values, size_t count)
    {
        GetLocal(offset, reinterpret_cast<void*>(values), static_cast<size_t>(count * sizeof(T)));
    }

    template<BufferValue T>
    void GraphicsBuffer::GetLocalBlockValue(size_t blockIndex, size_t offset, T& value)
    {
        GetLocal(offset + (blockIndex * BlockSize()), value);
    }

    template<BufferValue T>
    void GraphicsBuffer::GetLocalBlockValues(size_t blockIndex, size_t offset, T* values, size_t count)
    {
        GetLocalBlock(blockIndex, offset, reinterpret_cast<void*>(values), static_cast<size_t>(count * sizeof(T)));
    }

#pragma endregion SetLocal

#pragma endregion Public Functions

}	// namespace
