//------------------------------------------------------------------------------
//
// File Name:	Mesh.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Mesh encapsulator.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Mesh.h"

#include GRAPHICS_INCLUDE

#include "Mesh.inl"

#include "Library.inl"
#include "LibraryStatic.inl"

#undef TRACE_CATEGORY
#define TRACE_CATEGORY MESH

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
	// Private Static Constants:
	//--------------------------------------------------------------------------

    constexpr StreamKey
          Mesh::ReadKeyVertCount = "VertCount"
        , Mesh::ReadKeyDrawType = "DrawType"
        , Mesh::ReadKeyImmediateDrawCount = "ImmediateDrawCount"
        , Mesh::ReadKeyStoreMain = "StoreMain"
        , Mesh::ReadKeyAttributes = "Attributes"
        , Mesh::ReadKeyPrimitives = "Primitives"
        ;

	//--------------------------------------------------------------------------
	// Private Constants:
	//--------------------------------------------------------------------------

    ENUM_MAPS_CLASS(Mesh, Attributes,
        Position,
        Normal,
        TexCoord0,
        TexCoord1,
        TexCoord2,
        TexCoord3,
        Color0,
        Color1,
        DeformIndex,
        DeformWeight
        );

    ENUM_MAPS_CLASS_SEQUENTIAL(Mesh, DrawType, 
        Points,
        Lines,
        LineStrip,
        Triangles,
        TriangleStrip,
        TriangleFan
        );

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

    Mesh::Mesh()
        : NamedObject()
        , idVertexBuffer_(ResourceUnbind)
        , idVertexDescriptor_(ResourceUnbind)
        , vertexCount_(0)
        , immediateDrawCount_(0)
        , vertexSize_(0)
        , immediateDrawType_(DrawType::Invalid)
        , attributeFlags_(Attributes::None)
        , vertexBuffer_(nullptr)
        , primitives_()
        , attributes_()
	{
	}

    Mesh::Mesh(string_view_arg name)
        : NamedObject(name)
        , idVertexBuffer_(ResourceUnbind)
        , idVertexDescriptor_(ResourceUnbind)
        , vertexCount_(0)
        , immediateDrawCount_(0)
        , vertexSize_(0)
        , immediateDrawType_(DrawType::Invalid)
        , attributeFlags_(Attributes::None)
        , vertexBuffer_(nullptr)
        , primitives_()
        , attributes_()
    {
    }

    Mesh::Mesh(const Mesh& other)
        : NamedObject(other)
        , idVertexBuffer_(ResourceUnbind)
        , idVertexDescriptor_(ResourceUnbind)
        , vertexCount_(0)
        , immediateDrawCount_(0)
        , vertexSize_(0)
        , immediateDrawType_(other.immediateDrawType_)
        , attributeFlags_(other.attributeFlags_)
        , vertexBuffer_(nullptr)
        , primitives_()
        , attributes_()
    {
        assert(false);
    }

    Mesh::Mesh(Mesh&& other) noexcept
        : NamedObject(std::move(other))
        , idVertexBuffer_(std::exchange(other.idVertexBuffer_, ResourceUnbind))
        , idVertexDescriptor_(std::exchange(other.idVertexDescriptor_, ResourceUnbind))
        , vertexCount_(std::move(other.vertexCount_))
        , immediateDrawCount_(std::move(other.immediateDrawCount_))
        , vertexSize_(std::move(other.vertexSize_))
        , immediateDrawType_(std::move(other.immediateDrawType_))
        , attributeFlags_(std::move(other.attributeFlags_))
        , vertexBuffer_(std::exchange(other.vertexBuffer_, nullptr))
        , primitives_(std::move(other.primitives_))
        , attributes_(std::move(other.attributes_))
    {
    }

    Mesh& Mesh::operator=(const Mesh& other)
    {
        assert(false);
        NamedObject::operator=(other);
        return *this;
    }

	//--------------------------------------------------------------------------

	Mesh::~Mesh(void)
	{
        DestroyVertexBuffer();
        DestroyVertexDescriptor();
        SafeDeleteCheck(vertexBuffer_);
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

    void Mesh::Read(StreamIn& stream)
    {
        ReadName(stream);

        TraceInfo("Reading mesh: {}", GetName());

        if (stream.TryReadValue(ReadKeyVertCount, vertexCount_))
        {
            immediateDrawCount_ = vertexCount_;
        }

        stream.TryReadValue(ReadKeyImmediateDrawCount, immediateDrawCount_);

        stream.TryReadEnum(ReadKeyDrawType, ENUM_READ(DrawType), immediateDrawType_);

        if (stream.StreamHas(ReadKeyAttributes))
        {
            attributes_.clear();
            attributes_.reserve(static_cast<unsigned>(stream.GetArraySize(ReadKeyAttributes)));
            stream.ReadArrayObjects(ReadKeyAttributes, [this](StreamIn& stream, int)
            {
                auto& attr = attributes_.emplace_back();
                attr.Read(stream, attributeFlags_);
                if (attr.dataCount <= 0)
                {
                    attributes_.pop_back();
                }
            });
        }

        if (stream.StreamHas(ReadKeyPrimitives))
        {
            if (immediateDrawType_ != DrawType::Invalid)
            {
                TraceWarning("Mesh \"{}\" specifies draw type but has primitives!", GetName());
            }
            immediateDrawType_ = DrawType::Invalid;
            primitives_.clear();
            primitives_.reserve(static_cast<unsigned>(stream.GetArraySize(ReadKeyPrimitives)));
            stream.ReadArrayObjects(ReadKeyPrimitives, [this](StreamIn& stream, int)
            {
                auto& primitive = primitives_.emplace_back();
                primitive.Read(stream);
                if (primitive.vertexCount <= 0)
                {
                    primitives_.pop_back();
                }
                else
                {
                    for (unsigned i = 0; i < primitives_.size() - 1; ++i)
                    {
                        if (primitives_[i].Merge(primitive))
                        {
                            primitives_.pop_back();
                            break;
                        }
                    }
                }
            });
            for (auto& primitive : primitives_)
            {
                primitive.GenerateBuffer();
            }
        }
        // Initialize the mesh.
        {
            // Set up local data (main memory).
            CalculateAttributes();
            Resize(vertexCount_); // Resizes the local buffer
            SetVertexData();
            // Create buffers.
            CreateVertexDescriptor();
            CreateVertexBuffer();
            // Set up video data.
            InitializeVertexBuffer();
            RefreshVertexDescriptor();
            // Unbind to clean up.
            UnbindVertexDescriptor();
            UnbindVertexBuffer();
            bool keep;
            if (!(stream.TryReadValue(ReadKeyStoreMain, keep) && keep))
            {
                SafeDeleteCheck(vertexBuffer_);
            }
        }
    }

#pragma endregion Public Functions

    //--------------------------------------------------------------------------
    // Private Static Functions:
    //--------------------------------------------------------------------------

#pragma region Private Static Functions

    ENUM_IMPLEMENT_OPS(Mesh::Attributes);

    void Mesh::UnbindVertexDescriptor()
    {
        glBindVertexArray(ResourceUnbind);
        OpenGLCheckError();
    }

    void Mesh::UnbindVertexBuffer()
    {
        glBindBuffer(GL_ARRAY_BUFFER, ResourceUnbind);
        OpenGLCheckError();
    }

#pragma endregion Private Static Functions

	//--------------------------------------------------------------------------
	// Private Functions:
	//--------------------------------------------------------------------------

#pragma region Private Functions

    void Mesh::CreateVertexDescriptor()
    {
        if (DestroyVertexDescriptor())
        {
            TraceWarning("Creating mesh vertex descriptor over existing: \"{}\"", GetName());
        }
        glGenVertexArrays(1, &idVertexDescriptor_);
        OpenGLCheckError();
    }

    void Mesh::CreateVertexBuffer()
    {
        if (DestroyVertexBuffer())
        {
            TraceWarning("Creating mesh vertex buffer over existing: \"{}\"", GetName());
        }
        glGenBuffers(1, &idVertexBuffer_);
        OpenGLCheckError();
    }

    void Mesh::RefreshVertexDescriptor()
    {
        BindVertexBuffer();
        BindVertexDescriptor();
        for (const Attribute& attr : attributes_)
        {
            assert((size_t)attr.dataType < Attribute::DataTypesInternal.size());
            ResourceEnum attrType = Attribute::DataTypesInternal[(unsigned)attr.dataType];
            assert(attrType != 0);
            if (attr.dataType == DataType::Single || attr.dataType == DataType::Double)
            {
                glVertexAttribPointer(attr.index, attr.dataCount, attrType, (attr.normalized ? GL_TRUE : GL_FALSE), vertexSize_, (void*)attr.dataOffset);
            }
            else
            {
                glVertexAttribIPointer(attr.index, attr.dataCount, attrType, vertexSize_, (void*)attr.dataOffset);
            }
            OpenGLCheckError();
        }

        for (const Attribute& attr : attributes_)
        {
            glEnableVertexAttribArray(attr.index);
            OpenGLCheckError();
        }
    }

    void Mesh::ApplyVertexBuffer(void)
    {
        BindVertexBuffer();
        glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<GLsizeiptr>(vertexSize_) * vertexCount_, vertexBuffer_);
        OpenGLCheckError();
    }

    void Mesh::RetrieveVertexBuffer(void)
    {
        BindVertexBuffer();
        size_t meshSize = (size_t)vertexCount_ * vertexSize_;
#ifdef _DEBUG
        GLint bufsize;
        glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufsize);
        OpenGLCheckError();
        assert((size_t)bufsize == meshSize && "Buffer size should match this mesh size");
#endif
        vertexBuffer_ = reinterpret_cast<unsigned char*>(MemoryManager::Reallocate(vertexBuffer_, meshSize));
        glGetBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<GLsizeiptr>(meshSize), vertexBuffer_);
        OpenGLCheckError();
    }

    void Mesh::InitializeVertexBuffer()
    {
        BindVertexBuffer();
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertexSize_) * vertexCount_, vertexBuffer_, GL_DYNAMIC_DRAW);
        OpenGLCheckError();
    }

    bool Mesh::DestroyVertexDescriptor()
    {
        bool exists = idVertexDescriptor_ != ResourceUnbind;
        if (exists)
        {
            glDeleteVertexArrays(1, &idVertexDescriptor_);
            OpenGLCheckError();
            idVertexDescriptor_ = ResourceUnbind;
        }
        return exists;
    }

    bool Mesh::DestroyVertexBuffer()
    {
        bool exists = idVertexBuffer_ != ResourceUnbind;
        if (exists)
        {
            glDeleteBuffers(1, &idVertexBuffer_);
            OpenGLCheckError();
            idVertexBuffer_ = ResourceUnbind;
        }
        return exists;
    }

    void Mesh::BindVertexDescriptor() const
    {
        glBindVertexArray(idVertexDescriptor_);
        OpenGLCheckError();
    }

    void Mesh::BindVertexBuffer() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, idVertexBuffer_);
        OpenGLCheckError();
    }

    void Mesh::CalculateAttributes()
    {
        vertexSize_ = 0;
        unsigned char attrIndex = 0;
        for (auto& attr : attributes_)
        {
            attr.dataOffset = vertexSize_;
            attr.index = attrIndex++;
            vertexSize_ += attr.Size();
        }
    }

    void Mesh::SetVertexData()
    {
        for (auto& attr : attributes_)
        {
            attr.SetData(*this);
        }
    }

    void Mesh::Resize(size_t size, bool resizeLocal)
    {
        GLsizeiptr
            sizeOld = static_cast<GLsizeiptr>(vertexCount_) * vertexSize_,
            sizeNew = static_cast<GLsizeiptr>(size) * vertexSize_,
            sizeMin = MathMin(sizeOld, sizeNew);
        if (idVertexBuffer_ != ResourceUnbind)
        {
            ResourceID bufferNew;
            // Make the new buffer.
            {
                // Generate
                glGenBuffers(1, &bufferNew);
                OpenGLCheckError();
                // Bind
                glBindBuffer(GL_ARRAY_BUFFER, bufferNew);
                OpenGLCheckError();
                // Allocate
                glBufferData(GL_ARRAY_BUFFER, sizeNew, nullptr, GL_DYNAMIC_DRAW);
                OpenGLCheckError();
                // Unbind
                glBindBuffer(GL_ARRAY_BUFFER, ResourceUnbind);
                OpenGLCheckError();
            }
            // Copy the data.
            if (sizeMin > 0)
            {
                // Bind
                glBindBuffer(GL_COPY_READ_BUFFER, idVertexBuffer_);
                OpenGLCheckError();
                glBindBuffer(GL_COPY_WRITE_BUFFER, bufferNew);
                OpenGLCheckError();
                // Copy
                glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, sizeMin);
                OpenGLCheckError();
                // Unbind
                glBindBuffer(GL_COPY_READ_BUFFER, ResourceUnbind);
                OpenGLCheckError();
                glBindBuffer(GL_COPY_WRITE_BUFFER, ResourceUnbind);
                OpenGLCheckError();
            }
            // Delete our old buffer id and replace it with the new one.
            {
                DestroyVertexBuffer();
                idVertexBuffer_ = bufferNew;
            }
            // Associate the new buffer with the descriptor.
            {
                RefreshVertexDescriptor();
                UnbindVertexDescriptor();
                UnbindVertexBuffer();
            }
        }
        if (resizeLocal)
        {
            vertexBuffer_ = reinterpret_cast<unsigned char*>(MemoryManager::Reallocate(vertexBuffer_, static_cast<size_t>(sizeNew)));
        }
        vertexCount_ = static_cast<decltype(vertexCount_)>(size);
    }

    void Mesh::SetDataInternal(const Attribute& attr, const void* data, size_t size, unsigned vertIndex)
    {
        assert(vertexBuffer_ != nullptr);
        unsigned offset = vertIndex * vertexSize_ + attr.dataOffset;
        MemoryManager::Copy(reinterpret_cast<unsigned char*>(vertexBuffer_) + offset, (size_t)vertexCount_ * vertexSize_ - offset, data, size);
    }

    void Mesh::SetDataDirect(const Attribute& attr, const void* data, size_t size, unsigned vertIndex)
    {
        unsigned offset = vertIndex * vertexSize_ + attr.dataOffset;
        glBufferSubData(GL_ARRAY_BUFFER, offset, static_cast<GLsizeiptr>(size), data);
        OpenGLCheckError();
    }

    void Mesh::Draw() const
    {
        BindVertexDescriptor();

        if (immediateDrawType_ == DrawType::Invalid)
        {
            for (const Primitive& primitive : primitives_)
            {
                primitive.Draw();
            }
        }
        else
        {
            BindVertexBuffer();
            const auto mode(Primitive::PrimitiveTypes[(unsigned)immediateDrawType_]);
            glDrawArrays(mode, 0, static_cast<GLsizei>(immediateDrawCount_));
            OpenGLCheckError();
        }
    }

    void Mesh::Draw(InstanceIndex_t instances) const
    {
        BindVertexDescriptor();

        if (immediateDrawType_ == DrawType::Invalid)
        {
            for (const Primitive& primitive : primitives_)
            {
                primitive.Draw();
            }
        }
        else
        {
            BindVertexBuffer();
            const auto mode(Primitive::PrimitiveTypes[(unsigned)immediateDrawType_]);
            glDrawArraysInstanced(mode, 0, static_cast<GLsizei>(immediateDrawCount_), (GLsizei)instances);
            OpenGLCheckError();
        }
    }

#pragma endregion Private Functions

    //--------------------------------------------------------------------------
    // Library Implementation:
    //--------------------------------------------------------------------------

#pragma region Library Implementation

    template MeshLibrary;

    INSTANTIATE_LIBRARY_IMGUI_SELECTOR(Mesh);

    IMPLEMENT_READ(Mesh);

    IMPLEMENT_WRITE(Mesh);

#pragma endregion Library Implementation

}	// namespace
