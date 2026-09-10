//------------------------------------------------------------------------------
//
// File Name:	ConstexprAssert.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Macro to assert compile-time usage of constexpr functions. This
//              mainly exists because consteval functions get unhappy when
//              parameters are passed to nested consteval functions, but having
//              nested constexpr functions is fine.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Definitions:
//------------------------------------------------------------------------------

#define ASSERT_CONSTEVAL \
if (!std::is_constant_evaluated())\
{\
    throw "This function is only intended for compile-time usage!";\
}\
