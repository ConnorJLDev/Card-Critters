//------------------------------------------------------------------------------
//
// File Name:	FixedString.inl
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Structure for handling small strings with fixed-length buffers.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "FixedString.h"
#include "MemoryManager.h"

//------------------------------------------------------------------------------
// Preprocessor Directives:
//------------------------------------------------------------------------------

#pragma warning(push)
#pragma warning(disable : 4514)

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

    template<typename TChar, uint8_t N, bool NullTerminated>
    constexpr uint8_t basic_string_fixed_t<TChar, N, NullTerminated>::capacity = N - (NullTerminated ? 1 : 0);

    template<typename TChar, uint8_t N, bool NullTerminated>
    constexpr TChar basic_string_fixed_t<TChar, N, NullTerminated>::terminator = TChar('\0');

    //--------------------------------------------------------------------------
    // Private Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Constructors/Destructors:
    //--------------------------------------------------------------------------

#pragma region Constructors

    template<typename TChar, uint8_t N, bool NullTerminated>
    basic_string_fixed_t<TChar, N, NullTerminated>::basic_string_fixed_t(void)
        : size(0)
        , data{}
    {
        if constexpr (NullTerminated)
        {
            data[size] = terminator;
        }
    }

    template<typename TChar, uint8_t N, bool NullTerminated>
    basic_string_fixed_t<TChar, N, NullTerminated>::basic_string_fixed_t(basic_string_view_t<TChar> str)
        : size(static_cast<uint8_t>(MathMin(static_cast<size_t>(capacity), str.size())))
        , data{}
    {
        MemoryManager::Copy(data.data(), str.data(), size);
        if constexpr (NullTerminated)
        {
            data[size] = terminator;
        }
    }

    template<typename TChar, uint8_t N, bool NullTerminated>
    basic_string_fixed_t<TChar, N, NullTerminated>& basic_string_fixed_t<TChar, N, NullTerminated>::operator=(basic_string_view_t<TChar> str)
    {
        size = static_cast<uint8_t>(MathMin(static_cast<size_t>(capacity), str.size()));
        MemoryManager::Copy(data.data(), str.data(), size);
        if constexpr (NullTerminated)
        {
            data[size] = terminator;
        }
        return *this;
    }

    //--------------------------------------------------------------------------

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

    template <typename TChar, uint8_t N, bool NullTerminated>
    template <typename... Args>
    uint8_t basic_string_fixed_t<TChar, N, NullTerminated>::Format(const std::format_string<Args...> formatString, Args&&... args)
    {
        std::counted_iterator<TChar*> iter(data.data(), capacity);
        auto result = std::vformat_to(iter, formatString.get(), std::make_format_args(args...));
        uint8_t outSize = static_cast<uint8_t>(iter.count());
        size = outSize;
        if constexpr (NullTerminated)
        {
            data[size] = terminator;
        }
        return outSize;
    }

    template <typename TChar, uint8_t N, bool NullTerminated>
    template <typename... Args>
    uint8_t basic_string_fixed_t<TChar, N, NullTerminated>::FormatTo(const std::format_string<Args...> formatString, Args&&... args)
    {
        std::counted_iterator<TChar*> iter(data.data() + size, static_cast<int64_t>(capacity) - size);
        auto result = std::vformat_to(iter, formatString.get(), std::make_format_args(args...));
        uint8_t outSize = static_cast<uint8_t>(iter.count());
        size += outSize;
        if constexpr (NullTerminated)
        {
            data[size] = terminator;
        }
        return outSize;
    }

    template <typename TChar, uint8_t N, bool NullTerminated>
    basic_string_fixed_t<TChar, N, NullTerminated>::operator basic_string_view_t<TChar>(void) const
    {
        return basic_string_view_t<TChar>(data.data(), size);
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

#pragma warning(pop)
