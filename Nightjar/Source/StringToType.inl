//------------------------------------------------------------------------------
//
// File Name:	StringToType.inl
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Utility functions for converting strings to other value types.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "StringToType.h"

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

namespace Nightjar::Utility
{

    template <modifiable_t T>
    bool TryParseValue<T>(string_view_arg valueStr, T& value) requires (is_vec_v<T>)
    {
        bool success = true;
        size_t indexStart = 0;
        for (glm::length_t i = 0; success && i < T::length(); ++i)
        {
            size_t indexEnd = MathMin(valueStr.find_first_of(',', indexStart), valueStr.size());
            [[maybe_unused]] auto [ptr, ec] = std::from_chars(valueStr.data() + indexStart, valueStr.data() + indexEnd, value[i]);
            indexStart = indexEnd + 1; // static_cast<size_t>(ptr - valueStr.data());
            success = ec == std::errc{};
        }
        return success;
    }

}	// namespace

#pragma warning(pop)
