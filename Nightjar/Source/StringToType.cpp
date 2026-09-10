//------------------------------------------------------------------------------
//
// File Name:	StringToType.cpp
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
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar::Utility
{

    template<modifiable_t T>
    bool TryParseValue([[maybe_unused]] string_view_arg valueStr, [[maybe_unused]] T& value)
    {
        static_assert(sizeof(T) == 0, "Only specializations allowed!");
        return false;
    }

    template <> bool TryParseValue<bool>(string_view_arg valueStr, bool& value)
    {
        int temp;
        [[maybe_unused]] auto [ptr, ec] = std::from_chars(valueStr.data(), valueStr.data() + valueStr.size(), temp);
        value = temp != 0;
        return ec == std::errc{};
    }

#define REGULAR_PARSE(type)\
    template <> bool TryParseValue<type>(string_view_arg valueStr, type& value)\
    {\
        const unsigned char plusOffset = valueStr.starts_with('+');\
        [[maybe_unused]] auto [ptr, ec] = std::from_chars(valueStr.data() + plusOffset, valueStr.data() + valueStr.size(), value);\
        return ec == std::errc{};\
    }\

#define INT_PARSE(type)\
    REGULAR_PARSE(type)\
    template <> bool TryParseValue<type, true>(string_view_arg valueStr, type& value, int base)\
    {\
        const unsigned char plusOffset = valueStr.starts_with('+');\
        unsigned char numOffset = plusOffset;\
        bool negated = valueStr.starts_with('-');\
        const unsigned char baseOffset = static_cast<unsigned char>(numOffset + negated);\
        if (valueStr.size() > baseOffset + 2u && valueStr[baseOffset] == '0' && valueStr[baseOffset + 1u] > '9')\
        {\
            numOffset = static_cast<unsigned char>(baseOffset + 2u);\
            unsigned char baseChar = static_cast<unsigned char>(valueStr[baseOffset + 1u]);\
            if (baseChar >= 'a' && baseChar <= 'z')\
            {\
                baseChar += static_cast<unsigned char>('A' - 'a');\
            }\
            if (baseChar == 'X')\
            {\
                base = 16;\
            }\
            else if (baseChar == 'O')\
            {\
                base = 8;\
            }\
            else if (baseChar == 'B')\
            {\
                base = 2;\
            }\
            else\
            {\
                numOffset = plusOffset;\
                negated = false;\
            }\
        }\
        type temp;\
        [[maybe_unused]] auto [ptr, ec] = std::from_chars(valueStr.data() + numOffset, valueStr.data() + valueStr.size(), temp, base);\
        bool success = ec == std::errc{};\
        if (success)\
        {\
            value = negated ? static_cast<type>(-static_cast<std::make_signed_t<type>>(temp)) : temp;\
        }\
        return success;\
    }\
    template <> bool TryParseValue<type, false>(string_view_arg valueStr, type& value, int base)\
    {\
        const unsigned char plusOffset = valueStr.starts_with('+');\
        [[maybe_unused]] auto [ptr, ec] = std::from_chars(valueStr.data() + plusOffset, valueStr.data() + valueStr.size(), value, base);\
        return ec == std::errc{};\
    }\

    FOR_EACH_0_1_1(INT_PARSE,
        uint8_t,
        int8_t,
        uint16_t,
        int16_t,
        uint32_t,
        int32_t,
        uint64_t,
        int64_t,
        char
        )

    FOR_EACH_0_1_1(REGULAR_PARSE,
        float,
        double
        )

}	// namespace
