//------------------------------------------------------------------------------
//
// File Name:	Variant.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Struct that can contain any basic data type.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "DataTypes.h"

//------------------------------------------------------------------------------
// Preprocessor Directives:
//------------------------------------------------------------------------------

// Compiler switch to utilize std::variant.
//#define USE_STD_VARIANT

#ifndef USE_STD_VARIANT
#pragma warning(push)
#pragma warning(disable : 4582)
#endif

#define VARIANT_TYPES_NO_STR \
    bool, Bool,\
    char, Char,\
    wchar_t, WChar,\
    int8_t, Byte,\
    uint8_t, UByte,\
    int16_t, Short,\
    uint16_t, UShort,\
    int32_t, Int,\
    uint32_t, UInt,\
    int64_t, Long,\
    uint64_t, ULong,\
    float, Single,\
    double, Double,\
    Vec2, Vec2,\
    Vec3, Vec3,\
    Vec4, Vec4,\
    DVec2, DVec2,\
    DVec3, DVec3,\
    DVec4, DVec4,\
    I8Vec2, I8Vec2,\
    I8Vec3, I8Vec3,\
    I8Vec4, I8Vec4,\
    U8Vec2, U8Vec2,\
    U8Vec3, U8Vec3,\
    U8Vec4, U8Vec4,\
    I16Vec2, I16Vec2,\
    I16Vec3, I16Vec3,\
    I16Vec4, I16Vec4,\
    U16Vec2, U16Vec2,\
    U16Vec3, U16Vec3,\
    U16Vec4, U16Vec4,\
    I32Vec2, I32Vec2,\
    I32Vec3, I32Vec3,\
    I32Vec4, I32Vec4,\
    U32Vec2, U32Vec2,\
    U32Vec3, U32Vec3,\
    U32Vec4, U32Vec4,\
    I64Vec2, I64Vec2,\
    I64Vec3, I64Vec3,\
    I64Vec4, I64Vec4,\
    U64Vec2, U64Vec2,\
    U64Vec3, U64Vec3,\
    U64Vec4, U64Vec4,\
    BVec2, BVec2,\
    BVec3, BVec3,\
    BVec4, BVec4\

#define VARIANT_TYPES \
    VARIANT_TYPES_NO_STR,\
    string_t, String,\
    wstring_t, WString\

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
    template <typename T>
    concept variants_t = requires(T)
    {
        std::disjunction_v<
#define VARIANT_PER(type, name) std::is_same<type, T>
            FOR_EACH_0_2_2_C(VARIANT_PER, VARIANT_TYPES)
#undef VARIANT_PER
        >;
    };

    // Class Definition:
    struct Variant
    {
        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    public:
        Variant(void);
        Variant(const Variant& other);
        Variant(Variant&& other) noexcept;
        Variant& operator=(const Variant& other);
        Variant& operator=(Variant&& other) noexcept;

        ~Variant(void);

        // Public Static Functions:
    public:

        // Public Functions:
    public:
        /// <summary>
        /// Tries to assign a value to this struct.
        /// </summary>
        /// <typeparam name="T">The type of the value being assigned.</typeparam>
        /// <param name="other">The value that is being assigned.</param>
        /// <returns>A reference to this struct.</returns>
        template <variants_t T>
        Variant& operator=(const T& other);

        /// <summary>
        /// Reads this variant from a stream.
        /// </summary>
        /// <param name="stream">The stream being read from.</param>
        /// <returns>True if the value was read successfully, false otherwise.</returns>
        bool Read(StreamIn& stream);

        /// <summary>
        /// Writes this variant to a stream.
        /// </summary>
        /// <param name="stream">The stream being written to.</param>
        /// <param name="includeMeta">True to write as an object specifying the exact data type, false to only write the value.</param>
        /// <returns>True if the value was written successfully, false otherwise.</returns>
        bool Write(StreamOut& stream, bool includeMeta);

        /// <summary>
        /// Gets the data type of the current value.
        /// </summary>
        /// <returns>The data type of the current value.</returns>
        DataType GetType(void) const;

        /// <summary>
        /// Sets the data type that should be contained.
        /// </summary>
        /// <param name="type">The new data type to contain.</param>
        void SetType(DataType type);
        
        /// <summary>
        /// Checks if the data type is locked or not.
        /// </summary>
        /// <returns>True if the data type is locked, false otrherwise.</returns>
        bool GetLocked(void) const;

        /// <summary>
        /// Enables or disables the lock on the data type.
        /// </summary>
        /// <param name="locked">True to lock the data type, false to unlock.</param>
        void SetLocked(bool locked);

        /// <summary>
        /// Tries to get the value of this entry as the given type.
        /// </summary>
        /// <typeparam name="T">The type to get the value as.</typeparam>
        /// <param name="value">A reference that is set to the value if the type is valid, unchanged otherwise.</param>
        /// <returns>True if the type is valid, false otherwise.</returns>
        template<variants_t T>
        bool GetValue(T& value);

        /// <summary>
        /// Tries to set the value of this entry.
        /// </summary>
        /// <typeparam name="T">The type to get the value as.</typeparam>
        /// <param name="valuePtr">A pointer reference that is set to the value if the type is valid, or nullptr otherwise.</param>
        /// <returns>True if the type is correct, false otherwise.</returns>
        template<variants_t T>
        bool SetValue(const T& value);

        // Public Variables:
    public:

        // Private Constants, Enums, and Structs:
    private:

        // Private Static Functions:
    private:

        // Private Functions:
    private:
        /// <summary>
        /// Calls the destructor on the currently contained value, if applicable.
        /// </summary>
        void DestructValue(void);

        // Private Static Variables:
    private:

        // Private Variables:
    private:
        // The data type of the current value.
        DataType type;
        // Flag to allow/disallow the value from changing type.
        bool typeLock;
        // Property containing the value represented by this struct.
#ifdef USE_STD_VARIANT
        std::variant<
#define VARIANT_PER(type, name) type
            FOR_EACH_0_2_2_C(VARIANT_PER, VARIANT_TYPES)
        > value;
#else
        union
        {
#define VARIANT_PER(type, name) type value##name;
            FOR_EACH_0_2_2(VARIANT_PER, VARIANT_TYPES)
        };
#endif
#undef VARIANT_PER
    };

}	// namespace

#ifndef USE_STD_VARIANT
#pragma warning(pop)
#endif

#include "Variant.inl"
