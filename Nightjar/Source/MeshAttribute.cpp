//------------------------------------------------------------------------------
//
// File Name:	MeshAttribute.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Mesh attribute encapsulator.
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
          Mesh::Attribute::ReadKeyType = "Type"
        , Mesh::Attribute::ReadKeyCount = "Count"
        , Mesh::Attribute::ReadKeyData = "Data"
        , Mesh::Attribute::ReadKeyNormalized = "Normalized"
        ;

    constexpr array_t<ResourceEnum, DataType::NUM_ENTRIES_ATOMIC> Mesh::Attribute::DataTypesInternal
    {
        0,
        0,
        0,
        GL_BYTE,
        GL_UNSIGNED_BYTE,
        GL_SHORT,
        GL_UNSIGNED_SHORT,
        GL_INT,
        GL_UNSIGNED_INT,
        0,
        0,
        GL_FLOAT,
        GL_DOUBLE,
    };

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

    Mesh::Attribute::Attribute()
        : dataType(DataType::Invalid)
        , dataCount(0)
        , dataOffset(0)
        , index(0)
        , normalized(false)
        , attribute(Attributes::None)
        , data(nullptr)
    {
    }

    Mesh::Attribute::Attribute(Attribute&& other) noexcept
        : dataType(std::move(other.dataType))
        , dataCount(std::move(other.dataCount))
        , dataOffset(std::move(other.dataOffset))
        , index(std::move(other.index))
        , normalized(std::move(other.normalized))
        , attribute(std::move(other.attribute))
        , data(std::exchange(other.data, nullptr))
    {
    }

    Mesh::Attribute::Attribute(DataType dataType_, unsigned dataCount_)
        : dataType(DataTypeInfos[(unsigned int)dataType_].typeBase)
        , dataCount(dataType == dataType_ ? static_cast<unsigned char>(dataCount_) : DataTypeInfos[(unsigned int)dataType_].elementCount)
        , dataOffset(0)
        , index(0)
        , normalized(false)
        , attribute(Attributes::None)
        , data(nullptr)
    {
        assert(dataType_ == dataType || dataCount_ == 1);
    }

    //--------------------------------------------------------------------------

    Mesh::Attribute::~Attribute()
    {
        SafeDeleteCheck(data);
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

    void Mesh::Attribute::Read(StreamIn& stream, Attributes& meshAttributes)
    {
        if (stream.StreamHas(NamedObject::ReadKeyName))
        {
            string_t attrName = stream.ReadString(NamedObject::ReadKeyName);
            attribute = stream.ReadEnum<Attributes>(NamedObject::ReadKeyName, ENUM_READ(Attributes));
            meshAttributes |= attribute;
        }

        stream.TryReadEnum(ReadKeyType, ENUM_READ(DataType), dataType);

        stream.TryReadValue(ReadKeyCount, dataCount);

        if (stream.StreamHas(ReadKeyData))
        {
            unsigned int typeSize = DataTypeInfos[(unsigned int)dataType].size;
            switch (dataType)
            {
#define READ_ATTR(type)\
                vector_t<type> values;\
                stream.ReadVector(ReadKeyData, values);\
                data = new unsigned char[values.size() * typeSize];\
                MemoryManager::Move(data, values.data(), values.size() * typeSize);\

            case DataType::Byte:
            case DataType::UByte:
            {
                READ_ATTR(char);
                break;
            }
            case DataType::Short:
            case DataType::UShort:
            {
                READ_ATTR(short);
                break;
            }
            case DataType::Int:
            case DataType::UInt:
            {
                READ_ATTR(int);
                break;
            }
            case DataType::Single:
            {
                READ_ATTR(float);
                break;
            }
            case DataType::Double:
            {
                READ_ATTR(double);
                break;
            }
            case DataType::Invalid:
            default:
                assert(false && "Unhandled attribute data type!");
                break;
            }
        }
        else
        {
            assert(false && "Attributes must always have data!");
        }

        stream.TryReadValue(ReadKeyNormalized, normalized);
    }

    void Mesh::Attribute::SetData(Mesh& mesh)
    {
        unsigned typeSize = (unsigned)DataTypeInfos[(unsigned)dataType].size * dataCount;
        unsigned char *offsetWrite = reinterpret_cast<unsigned char*>(mesh.vertexBuffer_) + dataOffset, *offsetRead = reinterpret_cast<unsigned char*>(data);
        // Having the switch lets the compiler optimize memcpy into specific instructions,
        // as opposed to a runtime copy size.
        switch (typeSize)
        {
            // Memcpy avoids potential unaligned writes (which is undefined behavior by C++ standards)
#define SET_DATA(size, type)\
        case sizeof(type):\
            static_assert(sizeof(type) == size);\
            for (unsigned i = 0; i < mesh.vertexCount_; ++i, offsetWrite += mesh.vertexSize_, offsetRead += size)\
            {\
                MemoryManager::Copy(offsetWrite, offsetRead, size);\
            }\
            break;\

        SET_DATA(1, char)
        SET_DATA(2, short)
        SET_DATA(4, int)
        SET_DATA(8, long long)
        SET_DATA(12, IVec3)
        SET_DATA(16, IVec4)
        SET_DATA(24, DVec3)
        SET_DATA(32, DVec4)
        SET_DATA(36, Mat3)
        SET_DATA(48, Mat3x4)
        SET_DATA(64, Mat4)
        SET_DATA(72, DMat3)
        SET_DATA(96, DMat3x4)
        SET_DATA(128, DMat4)
        default:
            assert(false);
            break;
        }
    }

    unsigned char Mesh::Attribute::Size() const
    {
        return static_cast<unsigned char>(DataTypeInfos[(unsigned int)dataType].size * dataCount);
    }

#pragma endregion Public Functions

    //--------------------------------------------------------------------------
    // Private Functions:
    //--------------------------------------------------------------------------

#pragma region Private Functions

#pragma endregion Private Functions

}	// namespace
