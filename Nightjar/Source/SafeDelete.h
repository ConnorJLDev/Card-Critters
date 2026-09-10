//------------------------------------------------------------------------------
//
// File Name:	SafeDelete.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Convenience macros for preventing dangling pointers.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "MathDefs.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar::Utility
{

    // Convenience macro for deleting pointers.
#define SafeDelete(ptr) { delete ptr; ptr = nullptr; }

    // Convenience macro for deleting pointers to arrays.
#define SafeDeleteArr(ptr) { delete[] ptr; ptr = nullptr; }

    // Convenience macro for checking and deleting pointers.
#define SafeDeleteCheck(ptr) if (ptr != nullptr) SafeDelete(ptr)

    // Convenience macro for checking and deleting pointers to arrays.
#define SafeDeleteArrCheck(ptr) if (ptr != nullptr) SafeDeleteArr(ptr)

}	// namespace
