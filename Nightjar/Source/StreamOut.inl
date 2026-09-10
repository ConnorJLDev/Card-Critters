//------------------------------------------------------------------------------
//
// File Name:	StreamOut.inl
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class for writing to a file.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "StreamOut.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

#pragma endregion Public Static Functions

    //--------------------------------------------------------------------------
    // Public Functions:
    //--------------------------------------------------------------------------

#pragma region Public Functions

    template<typename T>
    void StreamOut::WriteValue(const T& value)
    {
        if (nextKey_.empty())
        {
            auto& current = GetCurrent();
            assert(current.IsArray());
            current.PushBack(WriteValueRaw(value), allocator_);
        }
        else
        {
            WriteValue(nextKey_, value);
        }
    }

    template<typename T>
    void StreamOut::WriteValue(string_view_arg key, const T& value)
    {
        auto& current = GetCurrent();
        assert(current.IsObject());
        current.AddMember(rapidjson::GenericStringRef<char>(key.data(), static_cast<rapidjson::SizeType>(key.size())), WriteValueRaw(value), allocator_);
        ClearNextKey();
    }

    template<typename T, typename TContainer>
    void StreamOut::WriteEnum(const TContainer& map, T& value)
    {
        if constexpr (is_map_like_v<TContainer>)
        {
            WriteValue(map.at(value));
        }
        else
        {
            WriteValue(map[(unsigned)value]);
        }
    }

    template<typename T, typename TContainer>
    void StreamOut::WriteEnum(string_view_arg key, const TContainer& map, T& value)
    {
        if constexpr (is_map_like_v<TContainer>)
        {
            WriteValue(key, map.at(value));
        }
        else
        {
            WriteValue(key, map[(unsigned)value]);
        }
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
