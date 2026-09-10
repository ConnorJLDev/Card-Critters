//------------------------------------------------------------------------------
//
// File Name:	MeshPrimitive.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Mesh primitive encapsulator.
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

    //--------------------------------------------------------------------------
    // Private Constants:
    //--------------------------------------------------------------------------

    constexpr StreamKey
        Mesh::Primitive::ReadKeyType = "Type",
        Mesh::Primitive::ReadKeyIndices = "Indices";

    constexpr array_t<ResourceEnum, Mesh::DrawType::NUM_ENTRIES> Mesh::Primitive::PrimitiveTypes
    {
        /*Points*/        GL_POINTS,
        /*Lines*/         GL_LINES,
        /*LineStrip*/     GL_LINE_STRIP,
        /*Triangles*/     GL_TRIANGLES,
        /*TriangleStrip*/ GL_TRIANGLE_STRIP,
        /*TriangleFan*/   GL_TRIANGLE_FAN,
    };

    constexpr Mesh::VertexIndex_t Mesh::Primitive::ResetIndex { (VertexIndex_t)-1 };

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

    Mesh::Primitive::Primitive()
        : type(DrawType::Invalid)
        , typeRaw()
        , indexType(GL_UNSIGNED_INT)
        , vertexCount(0)
        , indexID(ResourceUnbind)
        , indices()
    {
    }

    Mesh::Primitive::Primitive(DrawType drawType_, const vector_t<VertexIndex_t>& indices_)
        : type(drawType_)
        , typeRaw(PrimitiveTypes[(unsigned int)type])
        , indexType(GL_UNSIGNED_INT)
        , vertexCount((VertexIndex_t)indices_.size())
        , indexID(ResourceUnbind)
        , indices(indices_)
    {
    }

    Mesh::Primitive::Primitive(DrawType drawType_)
        : type(drawType_)
        , typeRaw(PrimitiveTypes[(unsigned int)type])
        , indexType(GL_UNSIGNED_INT)
        , vertexCount(0)
        , indexID(ResourceUnbind)
        , indices()
    {
    }

    Mesh::Primitive::Primitive(Primitive&& other) noexcept
        : type(std::move(other.type))
        , typeRaw(std::move(other.typeRaw))
        , indexType(std::move(other.indexType))
        , vertexCount(std::move(other.vertexCount))
        , indexID(std::exchange(other.indexID, ResourceUnbind))
        , indices(std::move(other.indices))
    {
    }

    //--------------------------------------------------------------------------

    Mesh::Primitive::~Primitive()
    {
        DeleteBuffer();
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

    void Mesh::Primitive::Read(StreamIn& stream)
    {
        if (stream.TryReadEnum(ReadKeyType, ENUM_READ(DrawType), type))
        {
            typeRaw = Primitive::PrimitiveTypes[(unsigned int)type];
        }

        if (stream.StreamHas(ReadKeyIndices))
        {
            stream.ReadVector(ReadKeyIndices, indices);
            vertexCount = static_cast<VertexIndex_t>(indices.size());
        }
    }

    void Mesh::Primitive::GenerateBuffer()
    {
        if (DeleteBuffer())
        {
            TraceWarning("Mesh overwriting primitive buffer!");
        }
        glGenBuffers(1, &indexID);
        OpenGLCheckError();
        vertexCount = static_cast<VertexIndex_t>(indices.size());
        BindBuffer();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(VertexIndex_t)), indices.data(), GL_STATIC_DRAW);
        OpenGLCheckError();
    }

    void Mesh::Primitive::BindBuffer() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexID);
        OpenGLCheckError();
    }

    bool Mesh::Primitive::DeleteBuffer()
    {
        bool success = indexID != ResourceUnbind;
        if (success)
        {
            glDeleteBuffers(1, &indexID);
            OpenGLCheckError();
            indexID = ResourceUnbind;
        }
        return success;
    }

    void Mesh::Primitive::SetDrawType(DrawType drawType)
    {
        assert(drawType != DrawType::Invalid);
        type = drawType;
        typeRaw = Primitive::PrimitiveTypes[(unsigned int)type];
    }

    void Mesh::Primitive::SetIndices(const vector_t<VertexIndex_t>& indices_)
    {
        indices = indices_;
        vertexCount = static_cast<VertexIndex_t>(indices.size());
        if (DeleteBuffer())
        {
            GenerateBuffer();
        }
    }

    bool Mesh::Primitive::Merge(Primitive& other)
    {
        if (type != other.type || typeRaw != other.typeRaw || &other == this)
        {
            return false;
        }

        indices.reserve(indices.size() + 1 + other.indices.size());
        indices.push_back(ResetIndex);
        indices.insert(indices.cend(), other.indices.begin(), other.indices.end());
        other.indices.clear();

        other.DeleteBuffer();
        if (DeleteBuffer())
        {
            GenerateBuffer();
        }

        return true;
    }

    void Mesh::Primitive::Draw() const
    {
        BindBuffer();
        glDrawElements(typeRaw, static_cast<GLsizei>(vertexCount), indexType, nullptr);
        OpenGLCheckError();
    }

    void Mesh::Primitive::Draw(InstanceIndex_t instances) const
    {
        BindBuffer();
        glDrawElementsInstanced(typeRaw, static_cast<GLsizei>(vertexCount), indexType, nullptr, static_cast<GLsizei>(instances));
        OpenGLCheckError();
    }

#pragma endregion Public Functions

    //--------------------------------------------------------------------------
    // Private Functions:
    //--------------------------------------------------------------------------

#pragma region Private Functions

#pragma endregion Private Functions

}	// namespace
