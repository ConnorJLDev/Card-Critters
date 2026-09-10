//------------------------------------------------------------------------------
//
// File Name:	Variant.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Struct that can contain any basic data type.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Variant.h"

//------------------------------------------------------------------------------
// Preprocessor Directives:
//------------------------------------------------------------------------------

#ifndef USE_STD_VARIANT
#pragma warning(push)
#pragma warning(disable : 4583)
#endif

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

    static constexpr string_view_t
          ReadKeyType = "Type"
        , ReadKeyValue = "Value"
        ;

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

    Variant::Variant(void)
        : type(DataType::Invalid)
        , typeLock(false)
    {
    }

    Variant::Variant(const Variant& other)
        : type(other.type)
        , typeLock(other.typeLock)
    {
#ifndef USE_STD_VARIANT
        switch (type)
        {
#define VARIANT_PER(type, name) case DataType::name: ::new (&value##name) type(other.value##name); break;
            FOR_EACH_0_2_2(VARIANT_PER, VARIANT_TYPES)
#undef VARIANT_PER
        default:
            break;
        }
#endif
    }

    Variant::Variant(Variant&& other) noexcept
        : type(std::exchange(other.type, DataType::Invalid))
        , typeLock(std::move(other.typeLock))
    {
#ifndef USE_STD_VARIANT
        switch (type)
        {
#define VARIANT_PER(type, name) case DataType::name: ::new (&value##name) type(std::move(other.value##name)); break;
            FOR_EACH_0_2_2(VARIANT_PER, VARIANT_TYPES)
#undef VARIANT_PER
        default:
            break;
        }
#endif
    }

    Variant& Variant::operator=(const Variant& other)
    {
#ifndef USE_STD_VARIANT
        if (type == other.type)
        {
            switch (type)
            {
#define VARIANT_PER(type, name) case DataType::name: value##name = other.value##name; break;
            FOR_EACH_0_2_2(VARIANT_PER, VARIANT_TYPES)
#undef VARIANT_PER
            default:
                break;
            }
        }
        else if (!typeLock)
        {
            DestructValue();
            type = other.type;
            switch (type)
            {
#define VARIANT_PER(type, name) case DataType::name: ::new (&value##name) type(other.value##name); break;
                FOR_EACH_0_2_2(VARIANT_PER, VARIANT_TYPES)
#undef VARIANT_PER
            default:
                break;
            }
        }
#endif

        return *this;
    }

    Variant& Variant::operator=(Variant&& other) noexcept
    {
        if (&other != this)
        {
            DestructValue();
            type = std::exchange(other.type, DataType::Invalid);
            typeLock = std::move(other.typeLock);
#ifndef USE_STD_VARIANT
            switch (type)
            {
#define VARIANT_PER(type, name) case DataType::name: ::new (&value##name) type(std::move(other.value##name)); break;
                FOR_EACH_0_2_2(VARIANT_PER, VARIANT_TYPES)
#undef VARIANT_PER
            default:
                break;
            }
#endif
        }
        return *this;
    }

    //--------------------------------------------------------------------------

    Variant::~Variant(void)
    {
        DestructValue();
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

    bool Variant::Read(StreamIn& stream)
    {
        DataType streamType = stream.GetElementType(true);
        if (streamType == DataType::Object)
        {
            if (!(stream.StreamHas(ReadKeyType) && stream.StreamHas(ReadKeyValue)))
            {
                return false;
            }
            DataType readType;
            if (!stream.TryReadEnum(ReadKeyType, ENUM_READ(DataType), readType))
            {
                return false;
            }
            SetType(readType);
            switch (type)
            {
#define VARIANT_PER(ctype, name)\
            case DataType::name:\
            if (!stream.TryReadValue(ReadKeyValue, value##name))\
            { TraceWarning("Failed to read explicit value type \"{}\" from \"{}\"", ENUM_WRITE_VALUE(DataType, type), stream.GetHierarchy()); }\
            break;
                FOR_EACH_0_2_2(VARIANT_PER, VARIANT_TYPES)
#undef VARIANT_PER
            default:
                return false;
            }
            return true;
        }
        SetType(streamType);
        switch (type)
        {
#define VARIANT_PER(ctype, name)\
            case DataType::name:\
            if (!stream.TryReadValue(value##name))\
            { TraceWarning("Failed to read implicit value type \"{}\" from \"{}\"", ENUM_WRITE_VALUE(DataType, type), stream.GetHierarchy()); }\
            break;
            FOR_EACH_0_2_2(VARIANT_PER, VARIANT_TYPES)
#undef VARIANT_PER
        default:
            return false;
        }
        return true;
    }

    bool Variant::Write(StreamOut& stream, bool includeMeta)
    {
        if (includeMeta)
        {
            stream.WriteObjectStart();
            stream.WriteValue(ReadKeyType, ENUM_WRITE_VALUE(DataType, type));
            stream.SetNextKey(ReadKeyValue);
        }
        switch (type)
        {
#define VARIANT_PER(type, name) case DataType::name: stream.WriteValue(value##name); break;
            FOR_EACH_0_2_2(VARIANT_PER, VARIANT_TYPES)
#undef VARIANT_PER
        default:
            return false;
        }
        if (includeMeta)
        {
            stream.WriteObjectEnd();
        }
        return true;
    }

    DataType Variant::GetType(void) const
    {
        return type;
    }

    void Variant::SetType(DataType type_)
    {
        if (type == type_)
        {
            return;
        }
        DestructValue();
        type = type_;
#ifndef USE_STD_VARIANT
        switch (type)
        {
#define VARIANT_PER(type, name) case DataType::name: ::new (&value##name) type(); break;
            FOR_EACH_0_2_2(VARIANT_PER, VARIANT_TYPES)
#undef VARIANT_PER
        default:
            break;
        }
#endif
    }

    bool Variant::GetLocked(void) const
    {
        return typeLock;
    }

    void Variant::SetLocked(bool locked)
    {
        typeLock = locked;
    }

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

    void Variant::DestructValue(void)
    {
#ifndef USE_STD_VARIANT
        switch (type)
        {
        case DataType::String:
            valueString.~string_t();
            break;
        case DataType::WString:
            valueWString.~wstring_t();
            break;
        default:
            break;
        }
#endif
    }

    /*template <> bool StreamIn::TryReadValueInternal<Variant>(const StreamValue& streamValue, Variant& value)
    {
        if (streamValue.IsString())
        {
            value = { streamValue.GetString(), streamValue.GetStringLength() };
            return true;
        }
        return false;
    }

    template <> StreamValue StreamOut::WriteValueRaw<Variant>(const Variant& value)
    {
        return WriteValueRaw(value.GetName());
    }

    template <> StreamValue StreamOut::WriteValueRaw<const Variant*>(const Variant* const& value)
    {
        return value == nullptr ? WriteValueRaw(string_view_t()) : WriteValueRaw(value->GetName());
    }*/

#pragma endregion Private Functions

}	// namespace

#ifndef USE_STD_VARIANT
#pragma warning(pop)
#endif
