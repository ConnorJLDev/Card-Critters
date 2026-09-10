//------------------------------------------------------------------------------
//
// File Name:	GraphicsBuffer.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class that handles data transfers between local/host memory
//              and external interfaces.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "GraphicsBuffer.h"

#include GRAPHICS_INCLUDE

#define ALWAYS_BIND

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

    GraphicsBuffer::GraphicsBuffer(const BufferDescriptor& descriptor, size_t arraySize)
        : descriptor_(descriptor)
        , id_(ResourceUnbind)
        , blockCount_(arraySize)
        , blockCapacity_(arraySize)
        , local_(nullptr)
    {
    }

    GraphicsBuffer::GraphicsBuffer(const GraphicsBuffer& other)
        : descriptor_(other.descriptor_)
        , id_(ResourceUnbind)
        , blockCount_(other.blockCount_)
        , blockCapacity_(other.blockCapacity_)
        , local_(nullptr)
    {
        if (other.GetInitialized())
        {
            Initialize();
        }
        SetUsingLocalCopy(other.GetUsingLocalCopy());
        CopyBuffer(other);
    }

    GraphicsBuffer::GraphicsBuffer(GraphicsBuffer&& other) noexcept
        : descriptor_(other.descriptor_)
        , id_(std::exchange(other.id_, ResourceUnbind))
        , blockCount_(std::move(other.blockCount_))
        , blockCapacity_(std::move(other.blockCapacity_))
        , local_(std::exchange(other.local_, nullptr))
    {
    }

    GraphicsBuffer::GraphicsBuffer(GraphicsBuffer&& other, const BufferDescriptor& descriptor) noexcept
        : descriptor_(descriptor)
        , id_(std::exchange(other.id_, ResourceUnbind))
        , blockCount_(std::move(other.blockCount_))
        , blockCapacity_(std::move(other.blockCapacity_))
        , local_(std::exchange(other.local_, nullptr))
    {
    }

    GraphicsBuffer& GraphicsBuffer::operator=(const GraphicsBuffer& other) noexcept
    {
        assert(&descriptor_ == &other.descriptor_ && "Assignment operator must be between buffers with the same descriptor!");

        CopyBuffer(other);

        return *this;
    }

    //--------------------------------------------------------------------------

    GraphicsBuffer::~GraphicsBuffer(void)
    {
        DestroyBuffer();
        SetUsingLocalCopy(false);
    }

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

    void GraphicsBuffer::CopyActivate(GraphicsBuffer& destination, const GraphicsBuffer& source)
    {
        glBindBuffer(GL_COPY_READ_BUFFER, source.id_);
        OpenGLCheckError();
        glBindBuffer(GL_COPY_WRITE_BUFFER, destination.id_);
        OpenGLCheckError();
    }

    void GraphicsBuffer::CopyDeactivate(GraphicsBuffer& destination, const GraphicsBuffer& source)
    {
        UNREFERENCED_PARAMETER(destination);
        UNREFERENCED_PARAMETER(source);
        glBindBuffer(GL_COPY_READ_BUFFER, ResourceUnbind);
        OpenGLCheckError();
        glBindBuffer(GL_COPY_WRITE_BUFFER, ResourceUnbind);
        OpenGLCheckError();
    }

    void GraphicsBuffer::Copy(GraphicsBuffer& destination, size_t destinationOffset, const GraphicsBuffer& source, size_t sourceOffset, size_t size)
    {
#ifdef ALWAYS_BIND
        CopyActivate(destination, source);
#else
        UNREFERENCED_PARAMETER(destination);
        UNREFERENCED_PARAMETER(source);
#endif
        glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, static_cast<GLintptr>(sourceOffset), static_cast<GLintptr>(destinationOffset), static_cast<GLsizeiptr>(size));
        OpenGLCheckError();
    }

#pragma endregion Public Static Functions

    //--------------------------------------------------------------------------
    // Public Functions:
    //--------------------------------------------------------------------------

#pragma region Public Functions

    const BufferDescriptor& GraphicsBuffer::GetDescriptor(void) const
    {
        return descriptor_;
    }

    size_t GraphicsBuffer::Size(void) const
    {
        return BlockSize() * BlockCount();
    }

    size_t GraphicsBuffer::Capacity(void) const
    {
        return BlockSize() * BlockCapacity();
    }

    size_t GraphicsBuffer::BlockSize(void) const
    {
        return descriptor_.size_;
    }

    size_t GraphicsBuffer::BlockCount(void) const
    {
        return blockCount_;
    }

    size_t GraphicsBuffer::BlockCapacity(void) const
    {
        return blockCapacity_;
    }

    void GraphicsBuffer::Activate(void)
    {
        assert(GetInitialized() && "Cannot bind uninitialized buffer!");
        glBindBuffer(descriptor_.bind_, id_);
        OpenGLCheckError();
    }

    void GraphicsBuffer::Deactivate(void)
    {
        glBindBuffer(descriptor_.bind_, ResourceUnbind);
        OpenGLCheckError();
    }

    void GraphicsBuffer::Resize(size_t count, bool preserveData)
    {
        Reserve(count, preserveData);
        blockCount_ = count;
    }

    void GraphicsBuffer::Reserve(size_t count, bool preserveData)
    {
        if (count > blockCapacity_)
        {
            ResizeForce(count, preserveData);
        }
    }

    bool GraphicsBuffer::GetInitialized(void) const
    {
        return id_ != ResourceUnbind;
    }

    void GraphicsBuffer::Initialize(void)
    {
        CreateBuffer();
    }

    bool GraphicsBuffer::GetUsingLocalCopy(void) const
    {
        return local_ != nullptr;
    }

    void GraphicsBuffer::SetUsingLocalCopy(bool useLocalCopy)
    {
        if (useLocalCopy)
        {
            if (!GetUsingLocalCopy())
            {
                local_ = MemoryManager::Allocate(Size());
            }
        }
        else
        {
            if (GetUsingLocalCopy())
            {
                MemoryManager::Deallocate(local_);
                local_ = nullptr;
            }
        }
    }

    void GraphicsBuffer::CreateBuffer()
    {
        if (DestroyBuffer())
        {
            TraceWarning("Overwriting existing buffer {} \"{}\"", id_, descriptor_.GetName());
        }
        glGenBuffers(1, &id_);
        OpenGLCheckError();

        glBindBuffer(descriptor_.bind_, id_);
        OpenGLCheckError();

        if (blockCount_ == blockCapacity_ || !GetUsingLocalCopy())
        {
            glBufferData(descriptor_.bind_, static_cast<GLsizeiptr>(Capacity()), local_, BufferDescriptor::BehaviorArray[(unsigned)descriptor_.behavior_]);
            OpenGLCheckError();
        }
        else
        {
            glBufferData(descriptor_.bind_, static_cast<GLsizeiptr>(Capacity()), nullptr, BufferDescriptor::BehaviorArray[(unsigned)descriptor_.behavior_]);
            OpenGLCheckError();
            glBufferSubData(descriptor_.bind_, static_cast<GLintptr>(0), static_cast<GLsizeiptr>(Size()), local_);
            OpenGLCheckError();
        }
    }

    bool GraphicsBuffer::DestroyBuffer()
    {
        bool exists = GetInitialized();
        if (exists)
        {
            glDeleteBuffers(1, &id_);
            OpenGLCheckError();
            id_ = ResourceUnbind;
        }
        return exists;
    }

    void GraphicsBuffer::ResizeForce(size_t count, bool preserveData)
    {
        size_t
            sizeOld = Capacity()
            , sizeNew = BlockSize() * count
            , sizeMin = MathMin(sizeOld, sizeNew)
            ;

        if (GetInitialized())
        {
            ResourceID idNew;
            glGenBuffers(1, &idNew);
            OpenGLCheckError();

            glBindBuffer(descriptor_.bind_, idNew);
            OpenGLCheckError();

            glBufferData(descriptor_.bind_, static_cast<GLsizeiptr>(sizeNew), nullptr, BufferDescriptor::BehaviorArray[(unsigned)descriptor_.behavior_]);
            OpenGLCheckError();

            if (preserveData && id_ != ResourceUnbind)
            {
                glBindBuffer(GL_COPY_READ_BUFFER, id_);
                OpenGLCheckError();
                glBindBuffer(GL_COPY_WRITE_BUFFER, idNew);
                OpenGLCheckError();
                glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, static_cast<GLsizeiptr>(sizeMin));
                OpenGLCheckError();
            }

            DestroyBuffer();
            id_ = idNew;
        }
        if (GetUsingLocalCopy())
        {
            local_ = MemoryManager::Reallocate(local_, sizeNew);
        }
        blockCapacity_ = count;
        blockCount_ = MathMin(blockCount_, count);
    }

#pragma region Local Send/Fetch

    void GraphicsBuffer::LocalSend(void)
    {
        LocalSend(0, Size());
    }

    void GraphicsBuffer::LocalSend(size_t offset, size_t size)
    {
        assert(GetUsingLocalCopy() && "Buffer must have local copy to use local functions!");
        SetDirect(offset, GetLocalBuffer(offset), size);
    }

    void GraphicsBuffer::LocalSendBlocks(size_t index, size_t count)
    {
        LocalSend(index * BlockSize(), count * BlockSize());
    }

    void GraphicsBuffer::LocalFetch(void)
    {
        LocalFetch(0, Size());
    }

    void GraphicsBuffer::LocalFetch(size_t offset, size_t size)
    {
        assert(GetUsingLocalCopy() && "Buffer must have local copy to use local functions!");
        GetDirect(offset, GetLocalBuffer(offset), size);
    }

    void GraphicsBuffer::LocalFetchBlocks(size_t index, size_t count)
    {
        LocalFetch(index * BlockSize(), count * BlockSize());
    }

#pragma endregion Local Send/Fetch

#pragma region GetLocalBuffer

    const void* GraphicsBuffer::GetLocalBuffer(size_t offset) const
    {
        assert((offset == 0 || GetUsingLocalCopy()) && "Can only get local buffer if it is being used!");
        return reinterpret_cast<const void*>(reinterpret_cast<const char*>(local_) + offset);
    }

    void* GraphicsBuffer::GetLocalBuffer(size_t offset)
    {
        assert((offset == 0 || GetUsingLocalCopy()) && "Can only get local buffer if it is being used!");
        return reinterpret_cast<void*>(reinterpret_cast<char*>(local_) + offset);
    }

    const void* GraphicsBuffer::GetLocalBufferBlock(size_t index, size_t offset) const
    {
        return GetLocalBuffer(offset + index * BlockSize());
    }

    void* GraphicsBuffer::GetLocalBufferBlock(size_t index, size_t offset)
    {
        return GetLocalBuffer(offset + index * BlockSize());
    }

#pragma endregion GetLocalBuffer

#pragma region GetOffset

    size_t GraphicsBuffer::GetOffset(string_view_arg name)
    {
        size_t offset(0);
        descriptor_.GetOffset(name, offset);
        return offset;
    }

    size_t GraphicsBuffer::GetOffset(string_view_arg name, size_t index)
    {
        size_t offset(0);
        descriptor_.GetOffset(name, index, offset);
        return offset;
    }

    size_t GraphicsBuffer::GetOffsetBlock(size_t blockIndex, string_view_arg name)
    {
        size_t offset(blockIndex * BlockSize());
        descriptor_.GetOffset(name, offset);
        return offset;
    }

    size_t GraphicsBuffer::GetOffsetBlock(size_t blockIndex, string_view_arg name, size_t index)
    {
        size_t offset(blockIndex * BlockSize());
        descriptor_.GetOffset(name, index, offset);
        return offset;
    }

#pragma endregion GetOffset

#pragma region SetDirect

    void GraphicsBuffer::SetDirect(string_view_arg name, size_t offset, const void* data, size_t size)
    {
        if (descriptor_.GetOffset(name, offset))
        {
            SetDirect(offset, data, size);
        }
    }

    void GraphicsBuffer::SetDirectBlock(size_t blockIndex, string_view_arg name, size_t offset, const void* data, size_t size)
    {
        if (descriptor_.GetOffset(name, offset))
        {
            SetDirectBlock(blockIndex, offset, data, size);
        }
    }

    void GraphicsBuffer::SetDirect(size_t offset, const void* data, size_t size)
    {
        assert(GetInitialized() && "Buffer must be initialized!");
#ifdef ALWAYS_BIND
        Activate();
#endif
        assert(offset + size < Size() && "Offset plus size is beyond the size of the buffer!");
        glBufferSubData(descriptor_.bind_, static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(size), data);
        OpenGLCheckError();
    }

    void GraphicsBuffer::SetDirectBlock(size_t blockIndex, size_t offset, const void* data, size_t size)
    {
        assert(offset + size < BlockSize() && "Offset plus size is beyond the size of the buffer descriptor!");
        offset += blockIndex * BlockSize();
        SetDirect(offset, data, size);
    }

#pragma endregion SetDirect

#pragma region SetLocal

    void GraphicsBuffer::SetLocal(string_view_arg name, size_t offset, const void* data, size_t size)
    {
        if (descriptor_.GetOffset(name, offset))
        {
            SetLocal(offset, data, size);
        }
    }

    void GraphicsBuffer::SetLocalBlock(size_t blockIndex, string_view_arg name, size_t offset, const void* data, size_t size)
    {
        if (descriptor_.GetOffset(name, offset))
        {
            SetLocalBlock(blockIndex, offset, data, size);
        }
    }

    void GraphicsBuffer::SetLocal(size_t offset, const void* data, size_t size)
    {
        MemoryManager::Copy(GetLocalBuffer(offset), data, size);
    }

    void GraphicsBuffer::SetLocalBlock(size_t blockIndex, size_t offset, const void* data, size_t size)
    {
        assert(offset + size < BlockSize() && "Offset plus size is beyond the size of the buffer descriptor!");
        offset += blockIndex * BlockSize();
        SetLocal(offset, data, size);
    }

#pragma endregion SetLocal

#pragma region GetDirect

    void GraphicsBuffer::GetDirect(string_view_arg name, size_t offset, void* data, size_t size)
    {
        if (descriptor_.GetOffset(name, offset))
        {
            GetDirect(offset, data, size);
        }
    }

    void GraphicsBuffer::GetDirectBlock(size_t blockIndex, string_view_arg name, size_t offset, void* data, size_t size)
    {
        if (descriptor_.GetOffset(name, offset))
        {
            GetDirectBlock(blockIndex, offset, data, size);
        }
    }

    void GraphicsBuffer::GetDirect(size_t offset, void* data, size_t size)
    {
        assert(GetInitialized() && "Buffer must be initialized!");
#ifdef ALWAYS_BIND
        Activate();
#endif
        assert(offset + size < Size() && "Offset plus size is beyond the size of the buffer!");
        glGetBufferSubData(descriptor_.bind_, static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(size), data);
        OpenGLCheckError();
    }

    void GraphicsBuffer::GetDirectBlock(size_t blockIndex, size_t offset, void* data, size_t size)
    {
        assert(offset + size < BlockSize() && "Offset plus size is beyond the size of the buffer descriptor!");
        offset += blockIndex * BlockSize();
        GetDirect(offset, data, size);
    }

#pragma endregion GetDirect

#pragma region GetLocal

    void GraphicsBuffer::GetLocal(string_view_arg name, size_t offset, void* data, size_t size)
    {
        if (descriptor_.GetOffset(name, offset))
        {
            GetLocal(offset, data, size);
        }
    }

    void GraphicsBuffer::GetLocalBlock(size_t blockIndex, string_view_arg name, size_t offset, void* data, size_t size)
    {
        if (descriptor_.GetOffset(name, offset))
        {
            GetLocalBlock(blockIndex, offset, data, size);
        }
    }

    void GraphicsBuffer::GetLocal(size_t offset, void* data, size_t size)
    {
        MemoryManager::Copy(data, GetLocalBuffer(offset), size);
    }

    void GraphicsBuffer::GetLocalBlock(size_t blockIndex, size_t offset, void* data, size_t size)
    {
        assert(offset + size < BlockSize() && "Offset plus size is beyond the size of the buffer descriptor!");
        offset += blockIndex * BlockSize();
        GetLocal(offset, data, size);
    }

#pragma endregion GetLocal

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

    void GraphicsBuffer::CopyBuffer(const GraphicsBuffer& other)
    {
        Resize(other.blockCount_);
        size_t sizeMin = MathMin(Size(), other.Size());
        if (GetInitialized() && other.GetInitialized())
        {
            CopyActivate(*this, other);
            Copy(*this, 0, other, 0, sizeMin);
        }
        if (GetUsingLocalCopy() && other.GetUsingLocalCopy())
        {
            MemoryManager::Copy(local_, other.local_, sizeMin);
        }
    }

#pragma endregion Private Functions

}	// namespace
