//------------------------------------------------------------------------------
//
// File Name:	StreamIn.inl
// Author(s):	Connor Lariviere (connor.lariviere), Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class for reading from a file.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "StreamIn.h"

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

    template<modifiable_t T, bool clear>
    void StreamIn::ReadVector(string_view_arg key, T& vec) requires (is_vector_v<T>)
    {
        const auto& arrayValue = (*current_)[&key.front()];
        assert(arrayValue.IsArray());

        if constexpr (clear)
        {
            vec.clear();
            vec.reserve(arrayValue.Size());
        }
        else
        {
            vec.reserve(vec.size() + arrayValue.Size());
        }

        auto array = arrayValue.GetArray();

        stack_.emplace_back("<ARRAY_INTERNAL>", current_);
        for (current_ = array.Begin(); current_ < array.End(); )
        {
#ifdef _DEBUG
            // Used to make sure that the array index always progresses forward
            const StreamValue* currentStart = current_;
#endif
            vec.push_back(std::move(ReadArrayElement<typename T::value_type>()));
#ifdef _DEBUG
            assert(current_ > currentStart);
#endif
        }
        StepBack();
    }

    template<typename T, typename TMap>
    T StreamIn::ReadEnum(string_view_arg key, const TMap& map)
    {
        T enumValue;
        [[maybe_unused]]
        bool success = TryReadEnum(key, map, enumValue);
        assert(success);
        return enumValue;
    }

    template<modifiable_t T, typename TMap>
    bool StreamIn::TryReadEnum(string_view_arg key, const TMap& map, T& value)
    {
        string_view_t valueName;
        bool success = TryReadValue(key, valueName);
        if (success)
        {
            auto iter = map.find(valueName);
            if (iter != map.end())
            {
                value = iter->second;
            }
            else
            {
                TraceWarning("Unknown enum name \"{}\" in \"{}\"", valueName, GetHierarchy());
            }
        }
        return success;
    }

    template<modifiable_t T, typename TMap>
    bool StreamIn::TryReadEnumFlags(string_view_arg key, const TMap& map, T& value)
    {
        bool hasKey = StreamHas(key);
        if (hasKey)
        {
            const auto& arrayValue = (*current_)[&key.front()];
            assert(arrayValue.IsArray());
            auto array = arrayValue.GetArray();
            stack_.emplace_back("<ARRAY_INTERNAL>", current_);
            for (current_ = array.Begin(); current_ < array.End(); )
            {
#ifdef _DEBUG
                // Used to make sure that the array index always progresses forward
                const StreamValue* currentStart = current_;
#endif
                string_view_t flagName = ReadArrayElement<string_view_t>();
#ifdef _DEBUG
                assert(current_ > currentStart);
#endif
                auto flagData = map.find(flagName);
                if (flagData != map.end())
                {
                    value |= flagData->second;
                }
                else
                {
                    TraceWarning("Unknown enum flag name \"{}\" in \"{}\"", flagName, GetHierarchy());
                }
            }
            StepBack();
        }
        return hasKey;
    }

    template<modifiable_t T>
    void StreamIn::ReadValue(string_view_arg key, T& value)
    {
        [[maybe_unused]]
        bool success = TryReadValue(key, value);
        assert(success);
    }

    template<typename T>
    T StreamIn::ReadValue_(string_view_arg key)
    {
        T value{};
        ReadValue(key, value);
        return value;
    }

    template<modifiable_t T>
    bool StreamIn::TryReadValue(T& value)
    {
        lastError_ = ReadValueError::None;
        bool success = TryReadValueInternal(*current_, value);
        if (!success && lastError_ == ReadValueError::None)
        {
            lastError_ = ReadValueError::KeyType;
        }
        return lastError_ == ReadValueError::None;
    }

    template<modifiable_t T>
    bool StreamIn::TryReadValue(string_view_arg key, T& value)
    {
        const StreamValue* streamValue;
        if (FindMember(key, streamValue))
        {
            lastError_ = ReadValueError::None;
            bool success = TryReadValueInternal(*streamValue, value);
            if (!success && lastError_ == ReadValueError::None)
            {
                lastError_ = ReadValueError::KeyType;
            }
        }
        else
        {
            lastError_ = ReadValueError::KeyMissing;
        }
        return lastError_ == ReadValueError::None;
    }

    template<modifiable_t T>
    bool StreamIn::TryReadValue(string_view_arg key, string_view_arg keyOld, T& value)
    {
        if (TryReadValue(key, value))
        {
            return true;
        }
        return TryReadValue(keyOld, value);
    }

    template<modifiable_t T>
    bool StreamIn::TryReadValue(size_t index, T& value)
    {
        const StreamValue* streamValue;
        if (FindMember(index, streamValue))
        {
            lastError_ = ReadValueError::None;
            bool success = TryReadValueInternal(*streamValue, value);
            if (!success && lastError_ == ReadValueError::None)
            {
                lastError_ = ReadValueError::KeyType;
            }
        }
        else
        {
            lastError_ = ReadValueError::KeyMissing;
        }
        return lastError_ == ReadValueError::None;
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

#pragma endregion Private Functions

}	// namespace
