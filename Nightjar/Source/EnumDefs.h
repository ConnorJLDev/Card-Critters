//------------------------------------------------------------------------------
//
// File Name:	EnumDefs.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Provides definitions for commonly used enum operations.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include <format>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include "ConstexprString.h"
#include "MemoryManager.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{

#define ENUM_DECLARE_OPS(eName)\
/* Logical OR operator for enum eName. */\
friend eName operator|(eName lhs, eName rhs);\
/* Logical OR assignment operator for enum eName. */\
friend eName& operator|=(eName& lhs, eName rhs);\
/* Logical AND operator for enum eName. */\
friend eName operator&(eName lhs, eName rhs);\
/* Logical AND assignment operator for enum eName. */\
friend eName& operator&=(eName& lhs, eName rhs);\
/* Logical XOR operator for enum eName. */\
friend eName operator^(eName lhs, eName rhs);\
/* Logical XOR assignment operator for enum eName. */\
friend eName& operator^=(eName& lhs, eName rhs);\
/* Logical NOT operator for enum eName. */\
friend eName operator~(eName val);\

#define ENUM_IMPLEMENT_OPS(eName)\
eName operator|(eName lhs, eName rhs)\
{ return static_cast<eName>(static_cast<std::underlying_type_t<eName>>(lhs) | static_cast<std::underlying_type_t<eName>>(rhs)); }\
eName& operator|=(eName& lhs, eName rhs)\
{ return lhs = lhs | rhs; }\
eName operator&(eName lhs, eName rhs)\
{ return static_cast<eName>(static_cast<std::underlying_type_t<eName>>(lhs) & static_cast<std::underlying_type_t<eName>>(rhs)); }\
eName& operator&=(eName& lhs, eName rhs)\
{ return lhs = lhs & rhs; }\
eName operator^(eName lhs, eName rhs)\
{ return static_cast<eName>(static_cast<std::underlying_type_t<eName>>(lhs) ^ static_cast<std::underlying_type_t<eName>>(rhs)); }\
eName& operator^=(eName& lhs, eName rhs)\
{ return lhs = lhs ^ rhs; }\
eName operator~(eName val)\
{ return static_cast<eName>(~static_cast<std::underlying_type_t<eName>>(val)); }\

    // Source - https://stackoverflow.com/questions/79359256/text-formatting-using-stdformat-for-enums
    // Posted by Barry, modified by community. See post 'Timeline' for change history
    // Retrieved 2025-11-08, License - CC BY-SA 4.0

    template <class E> requires std::is_enum_v<E>
    auto format_as(E e)
    {
        return static_cast<std::underlying_type_t<E>>(e);
    }

}	// namespace

template <typename T>
using format_as_type = decltype(format_as(std::declval<T>()));

template <class T>
    requires requires { typename format_as_type<T>; }
struct std::formatter<T> : formatter<format_as_type<T>> {
    auto format(T e, auto& ctx) const {
        return formatter<format_as_type<T>>::format(format_as(e), ctx);
    }
};

#pragma region Enum Maps

// Formatting: ENUM_MAPS + _CLASS + _SEQUENTIAL + _UPPER + _DECLARE
// _CLASS means this is a static class member.
// _SEQUENTIAL means all the registered values are incremental and can be optimized into an array (when writing).
// _UPPER means this the strings will be uppercased.
// _DECLARE means this is a declaration (for use in headers).

// Base macros for common enum map/array generator creation.
#pragma region Bases

#define ENUM_BASE_STR string_view_t

#define ENUM_BASE_MAP_T unordered_map_t

#define ENUM_BASE_NUM_ENTRIES NUM_ENTRIES

// Base format for a map used for enum reading/writing.
#define ENUM_BASE_MAP(suffix, key, value, iter, op, className, name, ...)\
const ENUM_BASE_MAP_T<key, value>& className##name##Map##suffix(void)\
{\
    static const ENUM_BASE_MAP_T<key, value>& name##Map##suffix##_\
    (\
        StaticManagement::UnorderedMapWrapper<key, value>::CreateConstant(\
            ENUM_BASE_MAP_T<key, value>\
            {\
                iter(op, className##name, __VA_ARGS__)\
            }\
        )\
    );\
    return name##Map##suffix##_;\
}\

// Base format for a map used for enum reading.
#define ENUM_BASE_MAP_READ(iter, op, className, name, ...) ENUM_BASE_MAP(Read, ENUM_BASE_STR, className##name, iter, op, className, name, __VA_ARGS__)

// Base format for a map used for enum writing/printing.
#define ENUM_BASE_MAP_WRITE(iter, op, className, name, ...) ENUM_BASE_MAP(Write, className##name, ENUM_BASE_STR, iter, op, className, name, __VA_ARGS__)

#define ENUM_BASE_ARR_CAST std::size_t

// Static assert check that all members of an array are sequential and start at zero.
#define ENUM_BASE_ARR_WRITE_ASSERT(basename, value0, value1) static_assert((ENUM_BASE_ARR_CAST)(basename::value1) == 1 + (ENUM_BASE_ARR_CAST)(basename::value0), #basename": "#value1" must be one greater than "#value0" for array!");

// Base format for an array used for enum writing/printing.
#define ENUM_BASE_ARR_WRITE(op, className, name, ...)\
const array_t<ENUM_BASE_STR, className##name::ENUM_BASE_NUM_ENTRIES>& className##name##MapWrite(void)\
{\
    static_assert(static_cast<ENUM_BASE_ARR_CAST>(className##name::FIRST_ARG(__VA_ARGS__)) == 0, #className #name": " STR2(FIRST_ARG(__VA_ARGS__)) " must be zero for start of array!");\
    FOR_EACH_1_2_1(ENUM_BASE_ARR_WRITE_ASSERT, className##name, __VA_ARGS__, ENUM_BASE_NUM_ENTRIES)\
    static constexpr array_t<ENUM_BASE_STR, name::ENUM_BASE_NUM_ENTRIES> name##MapWrite_\
    {\
        FOR_EACH_1_1_1_C(op, className##name, __VA_ARGS__)\
    };\
    return name##MapWrite_;\
}\

#pragma endregion Bases

#pragma region Individual Generators

#define ENUM_MAP_READ_ENTRY(basename, value) { #value, basename::value }
#define ENUM_MAP_READ(className, name, ...) ENUM_BASE_MAP_READ(FOR_EACH_1_1_1_C, ENUM_MAP_READ_ENTRY, className, name, __VA_ARGS__)

#define ENUM_MAP_READ_UPPER_ENTRY(basename, value) { BuildStr(StringToUpperConst, #value), basename::value }
#define ENUM_MAP_READ_UPPER(className, name, ...) ENUM_BASE_MAP_READ(FOR_EACH_1_1_1_C, ENUM_MAP_READ_UPPER_ENTRY, className, name, __VA_ARGS__)

#define ENUM_MAP_READ_BACKUP_ENTRY(basename, value, backup) { #value, basename::value }, { backup, basename::value }
#define ENUM_MAP_READ_BACKUP(className, name, ...) ENUM_BASE_MAP_READ(FOR_EACH_1_2_2_C, ENUM_MAP_READ_BACKUP_ENTRY, className, name, __VA_ARGS__)

#define ENUM_MAP_WRITE_ENTRY(basename, value) { basename::value, #value }
#define ENUM_MAP_WRITE(className, name, ...) ENUM_BASE_MAP_WRITE(FOR_EACH_1_1_1_C, ENUM_MAP_WRITE_ENTRY, className, name, __VA_ARGS__)

#define ENUM_MAP_WRITE_UPPER_ENTRY(basename, value) { basename::value, BuildStr(StringToUpperConst, #value) }
#define ENUM_MAP_WRITE_UPPER(className, name, ...) ENUM_BASE_MAP_WRITE(FOR_EACH_1_1_1_C, ENUM_MAP_WRITE_UPPER_ENTRY, className, name, __VA_ARGS__)

#define ENUM_MAP_WRITE_ARR_ENTRY(basename, value) #value
#define ENUM_MAP_WRITE_ARR(className, name, ...) ENUM_BASE_ARR_WRITE(ENUM_MAP_WRITE_ARR_ENTRY, className, name, __VA_ARGS__)

#define ENUM_MAP_WRITE_ARR_UPPER_ENTRY(basename, value) BuildStr(StringToUpperConst, #value)
#define ENUM_MAP_WRITE_ARR_UPPER(className, name, ...) ENUM_BASE_ARR_WRITE(ENUM_MAP_WRITE_ARR_UPPER_ENTRY, className, name, __VA_ARGS__)

#pragma endregion Individual Generators

#define ENUM_NUM_T uint_fast8_t
#define ENUM_NUM(enumName) static_cast<ENUM_NUM_T>(enumName)
#define ENUM_MAX_NUM(enumName, name) static_cast<ENUM_NUM_T>(enumName::name)
#define ENUM_VAL(enumName) static_cast<std::make_unsigned_t<std::underlying_type_t<std::decay_t<decltype(enumName)>>>>(enumName)

#define ENUM_MAPS_DECLARE(name)\
extern const ENUM_BASE_MAP_T<ENUM_BASE_STR, name>& name##MapRead(void);\
extern const ENUM_BASE_MAP_T<name, ENUM_BASE_STR>& name##MapWrite(void);\

#define ENUM_MAPS_CLASS_DECLARE(name)\
static const ENUM_BASE_MAP_T<ENUM_BASE_STR, name>& name##MapRead(void);\
static const ENUM_BASE_MAP_T<name, ENUM_BASE_STR>& name##MapWrite(void);\

#define ENUM_MAPS(name, ...)\
ENUM_MAP_READ(name, __VA_ARGS__);\
ENUM_MAP_WRITE(name, __VA_ARGS__);\

#define ENUM_MAPS_UPPER(name, ...)\
ENUM_MAP_READ_UPPER(name, __VA_ARGS__);\
ENUM_MAP_WRITE_UPPER(name, __VA_ARGS__);\

#define ENUM_MAPS_CLASS(className, name, ...)\
ENUM_MAP_READ(className::, name, __VA_ARGS__);\
ENUM_MAP_WRITE(className::, name, __VA_ARGS__);\

#define ENUM_MAPS_CLASS_UPPER(className, name, ...)\
ENUM_MAP_READ_UPPER(className::, name, __VA_ARGS__);\
ENUM_MAP_WRITE_UPPER(className::, name, __VA_ARGS__);\

#define ENUM_NUM_NAME_INT2(name,suffix) name##_##suffix
#define ENUM_NUM_NAME_INT(name,suffix) ENUM_NUM_NAME_INT2(name, suffix)
#define ENUM_NUM_NAME(name) ENUM_NUM_NAME_INT(name, ENUM_BASE_NUM_ENTRIES)

#define ENUM_MAPS_SEQUENTIAL_DECLARE(name)\
extern const ENUM_BASE_MAP_T<ENUM_BASE_STR, name>& name##MapRead(void);\
extern const array_t<ENUM_BASE_STR, (ENUM_BASE_ARR_CAST)name::ENUM_BASE_NUM_ENTRIES>& name##MapWrite(void);\

#define ENUM_MAPS_CLASS_SEQUENTIAL_DECLARE(name)\
static const ENUM_BASE_MAP_T<ENUM_BASE_STR, name>& name##MapRead(void);\
static const array_t<ENUM_BASE_STR, (ENUM_BASE_ARR_CAST)name::ENUM_BASE_NUM_ENTRIES>& name##MapWrite(void);\

#define ENUM_MAPS_SEQUENTIAL(name, ...)\
ENUM_MAP_READ(, name, __VA_ARGS__);\
ENUM_MAP_WRITE_ARR(, name, __VA_ARGS__);\

#define ENUM_MAPS_SEQUENTIAL_UPPER(name, ...)\
ENUM_MAP_READ_UPPER(, name, __VA_ARGS__);\
ENUM_MAP_WRITE_ARR_UPPER(, name, __VA_ARGS__);\

#define ENUM_MAPS_CLASS_SEQUENTIAL(className, name, ...)\
ENUM_MAP_READ(className::, name, __VA_ARGS__);\
ENUM_MAP_WRITE_ARR(className::, name, __VA_ARGS__);\

#define ENUM_MAPS_CLASS_SEQUENTIAL_UPPER(className, name, ...)\
ENUM_MAP_READ_UPPER(className::, name, __VA_ARGS__);\
ENUM_MAP_WRITE_ARR_UPPER(className::, name, __VA_ARGS__);\

#pragma region Accessors

#define ENUM_READ(name) name##MapRead()
#define ENUM_WRITE(name) name##MapWrite()
#define ENUM_WRITE_VALUE(name, varName) name##MapWrite()[ENUM_NUM(varName)]

#pragma endregion Accessors

#pragma endregion Enum Maps
