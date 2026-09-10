//------------------------------------------------------------------------------
//
// File Name:	DataTypes.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Enum and maps for reading and writing common types.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "DataTypes.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    ENUM_MAPS_SEQUENTIAL(DataType,
        Bool,
        Char,
        WChar,
        Byte,
        UByte,
        Short,
        UShort,
        Int,
        UInt,
        Long,
        ULong,
        Single,
        Double,
        Vec2,
        Vec3,
        Vec4,
        DVec2,
        DVec3,
        DVec4,
        ByteVec2,
        ByteVec3,
        ByteVec4,
        UByteVec2,
        UByteVec3,
        UByteVec4,
        ShortVec2,
        ShortVec3,
        ShortVec4,
        UShortVec2,
        UShortVec3,
        UShortVec4,
        IntVec2,
        IntVec3,
        IntVec4,
        UIntVec2,
        UIntVec3,
        UIntVec4,
        LongVec2,
        LongVec3,
        LongVec4,
        ULongVec2,
        ULongVec3,
        ULongVec4,
        BVec2,
        BVec3,
        BVec4,
        Mat2,
        Mat2x3,
        Mat2x4,
        Mat3x2,
        Mat3,
        Mat3x4,
        Mat4x2,
        Mat4x3,
        Mat4,
        DMat2,
        DMat2x3,
        DMat2x4,
        DMat3x2,
        DMat3,
        DMat3x4,
        DMat4x2,
        DMat4x3,
        DMat4,
        Texture1D,
        Texture2D,
        Texture3D,
        Sampler,
        String,
        WString,
        Array,
        Object
    );

#define DT_INFO(type, typeBase, elementCount, size) { DataType::type, DataType::typeBase, static_cast<uint8_t>(elementCount), static_cast<uint8_t>(size), }
#define DT_INFO_VEC(type, typeBase, cppType)\
    DT_INFO(type##2, typeBase, 2, sizeof(cppType##2)),\
    DT_INFO(type##3, typeBase, 3, sizeof(cppType##3)),\
    DT_INFO(type##4, typeBase, 4, sizeof(cppType##4))
#define DT_INFO_MAT_PART(type, typeBase, colCount, cppType)\
    DT_INFO(type##colCount##x2, typeBase, colCount * 2, sizeof(cppType##colCount##x2)),\
    DT_INFO(type##colCount##x3, typeBase, colCount * 3, sizeof(cppType##colCount##x3)),\
    DT_INFO(type##colCount##x4, typeBase, colCount * 4, sizeof(cppType##colCount##x4))
#define DT_INFO_MAT(type, typeBase, cppType)\
    DT_INFO_MAT_PART(type, typeBase, 2, cppType),\
    DT_INFO_MAT_PART(type, typeBase, 3, cppType),\
    DT_INFO_MAT_PART(type, typeBase, 4, cppType)
#define DT_INFO_ATOMIC(type, cppType) DT_INFO(type, type, 1, sizeof(cppType))
#define DT_INFO_COMPOSITE(type) DT_INFO(type, type, 1, 0)
#define DT_INFO_DYNAMIC(type, typeBase) DT_INFO(type, typeBase, 0, 0)

    constexpr array_t<DataTypeInfo, DataType::NUM_ENTRIES> DataTypeInfos =
    {{
        DT_INFO_ATOMIC(Bool, bool),
        DT_INFO_ATOMIC(Char, char),
        DT_INFO_ATOMIC(WChar, wchar_t),
        DT_INFO_ATOMIC(Byte, int8_t),
        DT_INFO_ATOMIC(UByte, uint8_t),
        DT_INFO_ATOMIC(Short, int16_t),
        DT_INFO_ATOMIC(UShort, uint16_t),
        DT_INFO_ATOMIC(Int, int32_t),
        DT_INFO_ATOMIC(UInt, uint32_t),
        DT_INFO_ATOMIC(Long, int64_t),
        DT_INFO_ATOMIC(ULong, uint64_t),
        DT_INFO_ATOMIC(Single, float),
        DT_INFO_ATOMIC(Double, double),
        DT_INFO_VEC(Vec, Single, Vec),
        DT_INFO_VEC(DVec, Double, DVec),
        DT_INFO_VEC(ByteVec, Byte, I8Vec),
        DT_INFO_VEC(UByteVec, UByte, U8Vec),
        DT_INFO_VEC(ShortVec, Short, I16Vec),
        DT_INFO_VEC(UShortVec, UShort, U16Vec),
        DT_INFO_VEC(IntVec, Int, I32Vec),
        DT_INFO_VEC(UIntVec, UInt, U32Vec),
        DT_INFO_VEC(LongVec, Long, I64Vec),
        DT_INFO_VEC(ULongVec, ULong, U64Vec),
        DT_INFO_VEC(BVec, Bool, BVec),
        DT_INFO_MAT(Mat, Single, Mat),
        DT_INFO_MAT(DMat, Double, DMat),
        DT_INFO_COMPOSITE(Texture1D),
        DT_INFO_COMPOSITE(Texture2D),
        DT_INFO_COMPOSITE(Texture3D),
        DT_INFO_COMPOSITE(Sampler),
        DT_INFO_DYNAMIC(String, Char),
        DT_INFO_DYNAMIC(WString, WChar),
        DT_INFO_COMPOSITE(Array),
        DT_INFO_COMPOSITE(Object),
    }};

#define TYPE_ASSIGNMENT \
    Bool, bool,\
    Char, char,\
    WChar, wchar_t,\
    Byte, int8_t,\
    UByte, uint8_t,\
    Short, int16_t,\
    UShort, uint16_t,\
    Int, int32_t,\
    UInt, uint32_t,\
    Long, int64_t,\
    ULong, uint64_t,\
    Single, float,\
    Double, double,\
    Vec2, Vec2,\
    Vec3, Vec3,\
    Vec4, Vec4,\
    DVec2, DVec2,\
    DVec3, DVec3,\
    DVec4, DVec4,\
    ByteVec2, I8Vec2,\
    ByteVec3, I8Vec3,\
    ByteVec4, I8Vec4,\
    UByteVec2, U8Vec2,\
    UByteVec3, U8Vec3,\
    UByteVec4, U8Vec4,\
    ShortVec2, I16Vec2,\
    ShortVec3, I16Vec3,\
    ShortVec4, I16Vec4,\
    UShortVec2, U16Vec2,\
    UShortVec3, U16Vec3,\
    UShortVec4, U16Vec4,\
    IntVec2, I32Vec2,\
    IntVec3, I32Vec3,\
    IntVec4, I32Vec4,\
    UIntVec2, U32Vec2,\
    UIntVec3, U32Vec3,\
    UIntVec4, U32Vec4,\
    LongVec2, I64Vec2,\
    LongVec3, I64Vec3,\
    LongVec4, I64Vec4,\
    ULongVec2, U64Vec2,\
    ULongVec3, U64Vec3,\
    ULongVec4, U64Vec4,\
    BVec2, BVec2,\
    BVec3, BVec3,\
    BVec4, BVec4,\
    Mat2, Mat2,\
    Mat2x3, Mat2x3,\
    Mat2x4, Mat2x4,\
    Mat3x2, Mat3x2,\
    Mat3, Mat3,\
    Mat3x4, Mat3x4,\
    Mat4x2, Mat4x2,\
    Mat4x3, Mat4x3,\
    Mat4, Mat4,\
    DMat2, DMat2,\
    DMat2x3, DMat2x3,\
    DMat2x4, DMat2x4,\
    DMat3x2, DMat3x2,\
    DMat3, DMat3,\
    DMat3x4, DMat3x4,\
    DMat4x2, DMat4x2,\
    DMat4x3, DMat4x3,\
    DMat4, DMat4,\

#define GENERATE_CONV_LL_FLAG(from, index, to) | ((is_lossless_convertible_v<from, to> ? 1ull : 0ull) << static_cast<uint32_t>(DataType::index))

#define GENERATE_CONV_LL_FLAGS(from) (0 FOR_EACH_1_2_2(GENERATE_CONV_LL_FLAG, from, TYPE_ASSIGNMENT))

    static constexpr array_t<uint64_t, DataType::NUM_ENTRIES> DataTypeLosslessConverts =
    {{
        //FOR_EACH_0_2_2_C(GENERATE_FLAGS, TYPE_ASSIGNMENT)
        GENERATE_CONV_LL_FLAGS(bool),
        GENERATE_CONV_LL_FLAGS(char),
        GENERATE_CONV_LL_FLAGS(wchar_t),
        GENERATE_CONV_LL_FLAGS(int8_t),
        GENERATE_CONV_LL_FLAGS(uint8_t),
        GENERATE_CONV_LL_FLAGS(int16_t),
        GENERATE_CONV_LL_FLAGS(uint16_t),
        GENERATE_CONV_LL_FLAGS(int32_t),
        GENERATE_CONV_LL_FLAGS(uint32_t),
        GENERATE_CONV_LL_FLAGS(int64_t),
        GENERATE_CONV_LL_FLAGS(uint64_t),
        GENERATE_CONV_LL_FLAGS(float),
        GENERATE_CONV_LL_FLAGS(double),
        GENERATE_CONV_LL_FLAGS(Vec2),
        GENERATE_CONV_LL_FLAGS(Vec3),
        GENERATE_CONV_LL_FLAGS(Vec4),
        GENERATE_CONV_LL_FLAGS(DVec2),
        GENERATE_CONV_LL_FLAGS(DVec3),
        GENERATE_CONV_LL_FLAGS(DVec4),
        GENERATE_CONV_LL_FLAGS(I8Vec2),
        GENERATE_CONV_LL_FLAGS(I8Vec3),
        GENERATE_CONV_LL_FLAGS(I8Vec4),
        GENERATE_CONV_LL_FLAGS(U8Vec2),
        GENERATE_CONV_LL_FLAGS(U8Vec3),
        GENERATE_CONV_LL_FLAGS(U8Vec4),
        GENERATE_CONV_LL_FLAGS(I16Vec2),
        GENERATE_CONV_LL_FLAGS(I16Vec3),
        GENERATE_CONV_LL_FLAGS(I16Vec4),
        GENERATE_CONV_LL_FLAGS(U16Vec2),
        GENERATE_CONV_LL_FLAGS(U16Vec3),
        GENERATE_CONV_LL_FLAGS(U16Vec4),
        GENERATE_CONV_LL_FLAGS(I32Vec2),
        GENERATE_CONV_LL_FLAGS(I32Vec3),
        GENERATE_CONV_LL_FLAGS(I32Vec4),
        GENERATE_CONV_LL_FLAGS(U32Vec2),
        GENERATE_CONV_LL_FLAGS(U32Vec3),
        GENERATE_CONV_LL_FLAGS(U32Vec4),
        GENERATE_CONV_LL_FLAGS(I64Vec2),
        GENERATE_CONV_LL_FLAGS(I64Vec3),
        GENERATE_CONV_LL_FLAGS(I64Vec4),
        GENERATE_CONV_LL_FLAGS(U64Vec2),
        GENERATE_CONV_LL_FLAGS(U64Vec3),
        GENERATE_CONV_LL_FLAGS(U64Vec4),
        GENERATE_CONV_LL_FLAGS(BVec2),
        GENERATE_CONV_LL_FLAGS(BVec3),
        GENERATE_CONV_LL_FLAGS(BVec4),
        GENERATE_CONV_LL_FLAGS(Mat2),
        GENERATE_CONV_LL_FLAGS(Mat2x3),
        GENERATE_CONV_LL_FLAGS(Mat2x4),
        GENERATE_CONV_LL_FLAGS(Mat3x2),
        GENERATE_CONV_LL_FLAGS(Mat3),
        GENERATE_CONV_LL_FLAGS(Mat3x4),
        GENERATE_CONV_LL_FLAGS(Mat4x2),
        GENERATE_CONV_LL_FLAGS(Mat4x3),
        GENERATE_CONV_LL_FLAGS(Mat4),
        GENERATE_CONV_LL_FLAGS(DMat2),
        GENERATE_CONV_LL_FLAGS(DMat2x3),
        GENERATE_CONV_LL_FLAGS(DMat2x4),
        GENERATE_CONV_LL_FLAGS(DMat3x2),
        GENERATE_CONV_LL_FLAGS(DMat3),
        GENERATE_CONV_LL_FLAGS(DMat3x4),
        GENERATE_CONV_LL_FLAGS(DMat4x2),
        GENERATE_CONV_LL_FLAGS(DMat4x3),
        GENERATE_CONV_LL_FLAGS(DMat4),
        0ull, // Texture1D
        0ull, // Texture2D
        0ull, // Texture3D
        0ull, // Sampler
        0ull, // String
        0ull, // WString
        0ull, // Array
        0ull, // Object
    }};

    bool DataTypeIsLosslesslyConvertible(DataType from, DataType to)
    {
        return ((1ull << static_cast<uint_fast8_t>(from)) & DataTypeLosslessConverts[static_cast<uint_fast8_t>(to)]) != 0;
    }

#define GENERATE_CONV_FLAG(from, index, to) | ((is_convertible_v<from, to> ? 1ull : 0ull) << static_cast<uint32_t>(DataType::index))

#define GENERATE_CONV_FLAGS(from) (0 FOR_EACH_1_2_2(GENERATE_CONV_FLAG, from, TYPE_ASSIGNMENT))

    static constexpr array_t<uint64_t, DataType::NUM_ENTRIES> DataTypeConverts =
    {{
        GENERATE_CONV_FLAGS(bool),
        GENERATE_CONV_FLAGS(char),
        GENERATE_CONV_FLAGS(wchar_t),
        GENERATE_CONV_FLAGS(int8_t),
        GENERATE_CONV_FLAGS(uint8_t),
        GENERATE_CONV_FLAGS(int16_t),
        GENERATE_CONV_FLAGS(uint16_t),
        GENERATE_CONV_FLAGS(int32_t),
        GENERATE_CONV_FLAGS(uint32_t),
        GENERATE_CONV_FLAGS(int64_t),
        GENERATE_CONV_FLAGS(uint64_t),
        GENERATE_CONV_FLAGS(float),
        GENERATE_CONV_FLAGS(double),
        GENERATE_CONV_FLAGS(Vec2),
        GENERATE_CONV_FLAGS(Vec3),
        GENERATE_CONV_FLAGS(Vec4),
        GENERATE_CONV_FLAGS(DVec2),
        GENERATE_CONV_FLAGS(DVec3),
        GENERATE_CONV_FLAGS(DVec4),
        GENERATE_CONV_FLAGS(I8Vec2),
        GENERATE_CONV_FLAGS(I8Vec3),
        GENERATE_CONV_FLAGS(I8Vec4),
        GENERATE_CONV_FLAGS(U8Vec2),
        GENERATE_CONV_FLAGS(U8Vec3),
        GENERATE_CONV_FLAGS(U8Vec4),
        GENERATE_CONV_FLAGS(I16Vec2),
        GENERATE_CONV_FLAGS(I16Vec3),
        GENERATE_CONV_FLAGS(I16Vec4),
        GENERATE_CONV_FLAGS(U16Vec2),
        GENERATE_CONV_FLAGS(U16Vec3),
        GENERATE_CONV_FLAGS(U16Vec4),
        GENERATE_CONV_FLAGS(I32Vec2),
        GENERATE_CONV_FLAGS(I32Vec3),
        GENERATE_CONV_FLAGS(I32Vec4),
        GENERATE_CONV_FLAGS(U32Vec2),
        GENERATE_CONV_FLAGS(U32Vec3),
        GENERATE_CONV_FLAGS(U32Vec4),
        GENERATE_CONV_FLAGS(I64Vec2),
        GENERATE_CONV_FLAGS(I64Vec3),
        GENERATE_CONV_FLAGS(I64Vec4),
        GENERATE_CONV_FLAGS(U64Vec2),
        GENERATE_CONV_FLAGS(U64Vec3),
        GENERATE_CONV_FLAGS(U64Vec4),
        GENERATE_CONV_FLAGS(BVec2),
        GENERATE_CONV_FLAGS(BVec3),
        GENERATE_CONV_FLAGS(BVec4),
        GENERATE_CONV_FLAGS(Mat2),
        GENERATE_CONV_FLAGS(Mat2x3),
        GENERATE_CONV_FLAGS(Mat2x4),
        GENERATE_CONV_FLAGS(Mat3x2),
        GENERATE_CONV_FLAGS(Mat3),
        GENERATE_CONV_FLAGS(Mat3x4),
        GENERATE_CONV_FLAGS(Mat4x2),
        GENERATE_CONV_FLAGS(Mat4x3),
        GENERATE_CONV_FLAGS(Mat4),
        GENERATE_CONV_FLAGS(DMat2),
        GENERATE_CONV_FLAGS(DMat2x3),
        GENERATE_CONV_FLAGS(DMat2x4),
        GENERATE_CONV_FLAGS(DMat3x2),
        GENERATE_CONV_FLAGS(DMat3),
        GENERATE_CONV_FLAGS(DMat3x4),
        GENERATE_CONV_FLAGS(DMat4x2),
        GENERATE_CONV_FLAGS(DMat4x3),
        GENERATE_CONV_FLAGS(DMat4),
        0ull, // Texture1D
        0ull, // Texture2D
        0ull, // Texture3D
        0ull, // Sampler
        0ull, // String
        0ull, // WString
        0ull, // Array
        0ull, // Object
    }};

    bool DataTypeIsConvertible(DataType from, DataType to)
    {
        return ((1ull << static_cast<uint_fast8_t>(from)) & DataTypeConverts[static_cast<uint_fast8_t>(to)]) != 0;
    }

}	// namespace
