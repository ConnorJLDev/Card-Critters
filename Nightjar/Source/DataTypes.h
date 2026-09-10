//------------------------------------------------------------------------------
//
// File Name:	DataTypes.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Enum and maps for reading and writing common types.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    enum class DataType : unsigned char
    {
        Invalid = static_cast<std::underlying_type_t<DataType>>(-1),
        Bool = 0,   // 1 bit boolean value.
        Char,       // 8 bit character that is neither signed nor unsigned.
        WChar,      // 16 bit character that is neither signed nor unsigned.
        I8,         // 8 bit signed integer.
        U8,         // 8 bit unsigned integer.
        I16,        // 16 bit signed integer.
        U16,        // 16 bit unsigned integer.
        I32,        // 32 bit signed integer.
        U32,        // 32 bit unsigned integer.
        I64,        // 64 bit signed integer.
        U64,        // 64 bit unsigned integer.
        Byte = I8,          // Alias for I8.
        UByte = U8,         // Alias for U8.
        Short = I16,        // Alias for I16.
        UShort = U16,       // Alias for U16.
        Int = I32,          // Alias for I32.
        UInt = U32,         // Alias for U32.
        Long = I64,         // Alias for I64.
        ULong = U64,        // Alias for U64.
        Single,     // Floating single-precision value.
        Float = Single,     // Alias for Single.
        Double,     // Floating double-precision value.
        Vec2,       // 2 dimensional vector of floating single-precision values (Singles/Floats).
        Vec3,       // 3 dimensional vector of floating single-precision values (Singles/Floats).
        Vec4,       // 4 dimensional vector of floating single-precision values (Singles/Floats).
        DVec2,      // 2 dimensional vector of floating double-precision values (Doubles).
        DVec3,      // 3 dimensional vector of floating double-precision values (Doubles).
        DVec4,      // 4 dimensional vector of floating double-precision values (Doubles).
        I8Vec2,     // 2 dimensional vector of 8 bit signed integers (Bytes).
        I8Vec3,     // 3 dimensional vector of 8 bit signed integers (Bytes).
        I8Vec4,     // 4 dimensional vector of 8 bit signed integers (Bytes).
        U8Vec2,     // 2 dimensional vector of 8 bit unsigned integers (UBytes).
        U8Vec3,     // 3 dimensional vector of 8 bit unsigned integers (UBytes).
        U8Vec4,     // 4 dimensional vector of 8 bit unsigned integers (UBytes).
        I16Vec2,    // 2 dimensional vector of 16 bit signed integers (Shorts).
        I16Vec3,    // 3 dimensional vector of 16 bit signed integers (Shorts).
        I16Vec4,    // 4 dimensional vector of 16 bit signed integers (Shorts).
        U16Vec2,    // 2 dimensional vector of 16 bit unsigned integers (UShorts).
        U16Vec3,    // 3 dimensional vector of 16 bit unsigned integers (UShorts).
        U16Vec4,    // 4 dimensional vector of 16 bit unsigned integers (UShorts).
        I32Vec2,    // 2 dimensional vector of 32 bit signed integers (Ints).
        I32Vec3,    // 3 dimensional vector of 32 bit signed integers (Ints).
        I32Vec4,    // 4 dimensional vector of 32 bit signed integers (Ints).
        U32Vec2,    // 2 dimensional vector of 32 bit unsigned integers (UInts).
        U32Vec3,    // 3 dimensional vector of 32 bit unsigned integers (UInts).
        U32Vec4,    // 4 dimensional vector of 32 bit unsigned integers (UInts).
        I64Vec2,    // 2 dimensional vector of 64 bit signed integers (Longs).
        I64Vec3,    // 3 dimensional vector of 64 bit signed integers (Longs).
        I64Vec4,    // 4 dimensional vector of 64 bit signed integers (Longs).
        U64Vec2,    // 2 dimensional vector of 64 bit unsigned integers (ULongs).
        U64Vec3,    // 3 dimensional vector of 64 bit unsigned integers (ULongs).
        U64Vec4,    // 4 dimensional vector of 64 bit unsigned integers (ULongs).
        IVec2 = I32Vec2,        // Alias for I32Vec2.
        IVec3 = I32Vec3,        // Alias for I32Vec3.
        IVec4 = I32Vec4,        // Alias for I32Vec4.
        UVec2 = U32Vec2,        // Alias for U32Vec2.
        UVec3 = U32Vec3,        // Alias for U32Vec3.
        UVec4 = U32Vec4,        // Alias for U32Vec4.
        ByteVec2 = I8Vec2,      // Alias for I8Vec2.
        ByteVec3 = I8Vec3,      // Alias for I8Vec3.
        ByteVec4 = I8Vec4,      // Alias for I8Vec4.
        UByteVec2 = U8Vec2,     // Alias for U8Vec2.
        UByteVec3 = U8Vec3,     // Alias for U8Vec3.
        UByteVec4 = U8Vec4,     // Alias for U8Vec4.
        ShortVec2 = I16Vec2,    // Alias for I16Vec2.
        ShortVec3 = I16Vec3,    // Alias for I16Vec3.
        ShortVec4 = I16Vec4,    // Alias for I16Vec4.
        UShortVec2 = U16Vec2,   // Alias for U16Vec2.
        UShortVec3 = U16Vec3,   // Alias for U16Vec3.
        UShortVec4 = U16Vec4,   // Alias for U16Vec4.
        IntVec2 = I32Vec2,      // Alias for I32Vec2.
        IntVec3 = I32Vec3,      // Alias for I32Vec3.
        IntVec4 = I32Vec4,      // Alias for I32Vec4.
        UIntVec2 = U32Vec2,     // Alias for U32Vec2.
        UIntVec3 = U32Vec3,     // Alias for U32Vec3.
        UIntVec4 = U32Vec4,     // Alias for U32Vec4.
        LongVec2 = I64Vec2,     // Alias for I64Vec2.
        LongVec3 = I64Vec3,     // Alias for I64Vec3.
        LongVec4 = I64Vec4,     // Alias for I64Vec4.
        ULongVec2 = U64Vec2,    // Alias for U64Vec2.
        ULongVec3 = U64Vec3,    // Alias for U64Vec3.
        ULongVec4 = U64Vec4,    // Alias for U64Vec4.
        BVec2,      // 2 dimensional vector of Bools.
        BVec3,      // 3 dimensional vector of Bools.
        BVec4,      // 4 dimensional vector of Bools.
        BoolVec2 = BVec2,   // Alias for BVec2. Prioritize BVec2 over this for consistency.
        BoolVec3 = BVec3,   // Alias for BVec3. Prioritize BVec3 over this for consistency.
        BoolVec4 = BVec4,   // Alias for BVec4. Prioritize BVec4 over this for consistency.
        Mat2,       // Matrix of Singles with 2 columns and 2 rows (column major).
        Mat2x2 = Mat2,      // Alias for Mat2. Prioritize Mat2 over this for consistency.
        Mat2x3,     // Matrix of Singles with 2 columns and 3 rows (column major).
        Mat2x4,     // Matrix of Singles with 2 columns and 4 rows (column major).
        Mat3x2,     // Matrix of Singles with 3 columns and 2 rows (column major).
        Mat3,       // Matrix of Singles with 3 columns and 3 rows (column major).
        Mat3x3 = Mat3,      // Alias for Mat3. Prioritize Mat3 over this for consistency.
        Mat3x4,     // Matrix of Singles with 3 columns and 4 rows (column major).
        Mat4x2,     // Matrix of Singles with 4 columns and 2 rows (column major).
        Mat4x3,     // Matrix of Singles with 4 columns and 3 rows (column major).
        Mat4,       // Matrix of Singles with 4 columns and 4 rows (column major).
        Mat4x4 = Mat4,      // Alias for Mat4. Prioritize Mat4 over this for consistency.
        DMat2,      // Matrix of Doubles with 2 columns and 2 rows (column major).
        DMat2x2 = DMat2,    // Alias for DMat2. Prioritize DMat2 over this for consistency.
        DMat2x3,    // Matrix of Doubles with 2 columns and 3 rows (column major).
        DMat2x4,    // Matrix of Doubles with 2 columns and 4 rows (column major).
        DMat3x2,    // Matrix of Doubles with 3 columns and 2 rows (column major).
        DMat3,      // Matrix of Doubles with 3 columns and 3 rows (column major).
        DMat3x3 = DMat3,    // Alias for DMat3. Prioritize DMat3 over this for consistency.
        DMat3x4,    // Matrix of Doubles with 3 columns and 4 rows (column major).
        DMat4x2,    // Matrix of Doubles with 4 columns and 2 rows (column major).
        DMat4x3,    // Matrix of Doubles with 4 columns and 3 rows (column major).
        DMat4,      // Matrix of Doubles with 4 columns and 4 rows (column major).
        DMat4x4 = DMat4,    // Alias for DMat4. Prioritize DMat4 over this for consistency.
        Texture1D,  // 1 dimensional texture.
        Texture2D,  // 2 dimensional texture.
        Texture3D,  // 3 dimensional texture.
        Sampler,    // Sampler that is used on a texture.
        String,     // String of characters.
        WString,    // String of wide characters.
        Array,      // Array of values.
        Object,     // Object of key-value pairs.
        NUM_ENTRIES,    // Total number of types.
        NUM_ENTRIES_ATOMIC = Double + 1,    // Total number of atomic types (not composite).
        NUM_ENTRIES_ARITHMETIC = DMat4 + 1, // Total number of arithmetic types.
    };

    ENUM_MAPS_SEQUENTIAL_DECLARE(DataType);

    struct alignas(4) DataTypeInfo
    {
        DataType type;              // The type this struct describes.
        DataType typeBase;          // The contained type for vectors or matrices.
        uint8_t elementCount;       // How many elements are in this vector or matrix.
        uint8_t size;               // The size of this type in bytes.
    };

    // Array containing each DataType's DataTypeInfo struct.
    extern const array_t<DataTypeInfo, DataType::NUM_ENTRIES> DataTypeInfos;

    /// <summary>
    /// Checks if a data type can be converted to another data type.
    /// </summary>
    /// <param name="from">The data type being converted from.</param>
    /// <param name="to">The data type attempting to be converted to.</param>
    /// <returns>True if the conversion is possible, false otherwise.</returns>
    bool DataTypeIsConvertible(DataType from, DataType to);

    /// <summary>
    /// Checks if a data type can be losslessly converted to another data type. In other words, checks if the "to" data type can represent any and every value of the "from" data type with perfect accuracy.
    /// </summary>
    /// <param name="from">The data type being converted from.</param>
    /// <param name="to">The data type attempting to be converted to.</param>
    /// <returns>True if the conversion is possible and lossless, false otherwise.</returns>
    bool DataTypeIsLosslesslyConvertible(DataType from, DataType to);

}	// namespace
