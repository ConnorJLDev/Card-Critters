//------------------------------------------------------------------------------
//
// File Name:	ImGuiWindowBase.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Base class for ImGui windows.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiWindowBase.h"

#include "ImGuiInclude.h"

//------------------------------------------------------------------------------
// Preprocessor Directives:
//------------------------------------------------------------------------------

#pragma warning(push)
#pragma warning(disable : 4514)

#define READKEY_HASH fnv1a_hash
#define READKEY_CHECK(hashedKey) case READKEY_HASH(hashedKey): if (key != hashedKey) { return ReadError::InvalidKey; }
#define READKEY_VALUE(out) parseSuccess = TryParseValue(value, out);
#define READKEY_BITFIELD(out) { decltype(out) temp; READKEY_VALUE(temp); if (parseSuccess) { out = temp; } }
#define READKEY_COLOR(color) READKEY_VALUE(color); if (parseSuccess) { color /= 255.0f; }
#define READKEY_ENUM(enumType, enumValue) { auto iter = ENUM_READ(enumType).find(value); parseSuccess = iter != ENUM_READ(enumType).end(); if (parseSuccess) { enumValue = iter->second; } }
#define READKEY_RETURN (parseSuccess ? ReadError::None : ReadError::InvalidValue)

#define WRITE_VALUE(name, var)\
{\
    buffer += name;\
    buffer += '=';\
    FORMAT_TO(buffer, "{}", var);\
    buffer += '\n';\
}

#define WRITE_BOOL(name, var) WRITE_VALUE(name, (int)var)

#define WRITE_COLOR(name, var)\
{\
    buffer += name;\
    buffer += '=';\
    const IVec4 col(var * 255.0f);\
    FORMAT_TO(buffer, "{},{},{},{}", col.r, col.g, col.b, col.a);\
    buffer += '\n';\
}

#define WRITE_STRING(name, var)\
{\
    buffer += name;\
    buffer += '=';\
    buffer += var;\
    buffer += '\n';\
}

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
