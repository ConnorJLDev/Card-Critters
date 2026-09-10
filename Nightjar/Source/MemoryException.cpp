//------------------------------------------------------------------------------
//
// File Name:	MemoryException.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class that represents exceptions relating to memory management.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "MemoryException.h"

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

    static constexpr MemoryException::ErrorCode ErrorCodeMax = MemoryException::ErrorCode::InvalidHeader;

    static constexpr array_t<string_view_t, static_cast<unsigned>(ErrorCodeMax) + 1> ErrorCodeNames =
    {
        "Generic",
        "PageInitFailure",
        "NoMemory",
        "InvalidPattern",
        "InvalidHeader",
    };

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

    MemoryException::MemoryException(ErrorCode code, string_view_arg message) noexcept
        : code_(code)
        , message_(message)
    {
    }

    //--------------------------------------------------------------------------

    MemoryException::~MemoryException(void) noexcept
    {
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

    const char* MemoryException::what(void) const noexcept
    {
        return message_.data.data();
    }

    MemoryException::ErrorCode MemoryException::code(void) const noexcept
    {
        return code_;
    }

    const char* MemoryException::code_str(void) const noexcept
    {
        return code_ <= ErrorCodeMax ? ErrorCodeNames[static_cast<unsigned>(code_)].data() : "";
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
