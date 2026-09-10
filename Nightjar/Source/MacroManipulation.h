//------------------------------------------------------------------------------
//
// File Name:	MacroManipulation.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Macros for macro argument manipulation and iteration.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Definitions:
//------------------------------------------------------------------------------

// Reference: https://www.scs.stanford.edu/~dm/blog/va-opt.html

// Empty pair of parenthesis. Used for calling macros during macro expansion.
#define PARENS ()

// Removes containing parenthesis from a macro argument.
#define UNPAREN(...) __VA_ARGS__

// Gets the first argument in a variadic macro argument list.
#define FIRST_ARG(FIRST, ...) FIRST

// Stringifies the given argument.
#define STR(ARG) #ARG
// Stringifies the given argument after expanding it once.
#define STR1(ARG) STR(ARG)
// Stringifies the given argument after expanding it twice.
#define STR2(ARG) STR1(ARG)

// Expands the given arguments to their final state (assuming 256 expansions is enough).
#define EXPAND(...) INTERNAL_EXPAND4(INTERNAL_EXPAND4(INTERNAL_EXPAND4(INTERNAL_EXPAND4(__VA_ARGS__))))
#define INTERNAL_EXPAND4(...) INTERNAL_EXPAND3(INTERNAL_EXPAND3(INTERNAL_EXPAND3(INTERNAL_EXPAND3(__VA_ARGS__))))
#define INTERNAL_EXPAND3(...) INTERNAL_EXPAND2(INTERNAL_EXPAND2(INTERNAL_EXPAND2(INTERNAL_EXPAND2(__VA_ARGS__))))
#define INTERNAL_EXPAND2(...) INTERNAL_EXPAND1(INTERNAL_EXPAND1(INTERNAL_EXPAND1(INTERNAL_EXPAND1(__VA_ARGS__))))
#define INTERNAL_EXPAND1(...) __VA_ARGS__

#define INTERNAL_FOR_EACH_0_1_1(macro, delim, ...)\
  __VA_OPT__(EXPAND(INTERNAL_FOR_EACH_HELPER_0_1_1(macro, delim, __VA_ARGS__)))
#define INTERNAL_FOR_EACH_HELPER_0_1_1(macro, delim, arg0, ...)\
  macro(arg0)\
  __VA_OPT__(UNPAREN delim INTERNAL_FOR_EACH_AGAIN_0_1_1 PARENS (macro, delim, __VA_ARGS__))
#define INTERNAL_FOR_EACH_AGAIN_0_1_1() INTERNAL_FOR_EACH_HELPER_0_1_1

#define INTERNAL_FOR_EACH_1_1_1(macro, delim, front0, ...)\
  __VA_OPT__(EXPAND(INTERNAL_FOR_EACH_HELPER_1_1_1(macro, delim, front0, __VA_ARGS__)))
#define INTERNAL_FOR_EACH_HELPER_1_1_1(macro, delim, front0, arg0, ...)\
  macro(front0, arg0)\
  __VA_OPT__(UNPAREN delim INTERNAL_FOR_EACH_AGAIN_1_1_1 PARENS (macro, delim, front0, __VA_ARGS__))
#define INTERNAL_FOR_EACH_AGAIN_1_1_1() INTERNAL_FOR_EACH_HELPER_1_1_1

// Runs a macro through variadic arguments: 0 arguments from front, 1 argument view, 1 argument step.
#define FOR_EACH_0_1_1(macro, ...) INTERNAL_FOR_EACH_0_1_1(macro, (), __VA_ARGS__)
// Runs a macro through variadic arguments: 0 arguments from front, 1 argument view, 1 argument step, comma delimited.
#define FOR_EACH_0_1_1_C(macro, ...) INTERNAL_FOR_EACH_0_1_1(macro, (, ), __VA_ARGS__)

// Runs a macro through variadic arguments: 1 argument from front, 1 argument view, 1 argument step.
#define FOR_EACH_1_1_1(macro, front0, ...) INTERNAL_FOR_EACH_1_1_1(macro, (), front0, __VA_ARGS__)
// Runs a macro through variadic arguments: 1 argument from front, 1 argument view, 1 argument step, comma delimited.
#define FOR_EACH_1_1_1_C(macro, front0, ...) INTERNAL_FOR_EACH_1_1_1(macro, (, ), front0, __VA_ARGS__)

#define INTERNAL_FOR_EACH_0_2_1(macro, delim, ...)\
  __VA_OPT__(EXPAND(INTERNAL_FOR_EACH_HELPER_0_2_1(macro, delim, __VA_ARGS__)))
#define INTERNAL_FOR_EACH_HELPER_0_2_1(macro, delim, arg0, arg1, ...)\
  macro(arg0, arg1)\
  __VA_OPT__(UNPAREN delim INTERNAL_FOR_EACH_AGAIN_0_2_1 PARENS (macro, delim, arg1, __VA_ARGS__))
#define INTERNAL_FOR_EACH_AGAIN_0_2_1() INTERNAL_FOR_EACH_HELPER_0_2_1

#define INTERNAL_FOR_EACH_1_2_1(macro, delim, front0, ...)\
  __VA_OPT__(EXPAND(INTERNAL_FOR_EACH_HELPER_1_2_1(macro, delim, front0, __VA_ARGS__)))
#define INTERNAL_FOR_EACH_HELPER_1_2_1(macro, delim, front0, arg0, arg1, ...)\
  macro(front0, arg0, arg1)\
  __VA_OPT__(UNPAREN delim INTERNAL_FOR_EACH_AGAIN_1_2_1 PARENS (macro, delim, front0, arg1, __VA_ARGS__))
#define INTERNAL_FOR_EACH_AGAIN_1_2_1() INTERNAL_FOR_EACH_HELPER_1_2_1

// Runs a macro through variadic arguments: 0 arguments from front, 2 argument view, 1 argument step.
#define FOR_EACH_0_2_1(macro, ...) INTERNAL_FOR_EACH_0_2_1(macro, (), __VA_ARGS__)
// Runs a macro through variadic arguments: 0 arguments from front, 2 argument view, 1 argument step, comma delimited.
#define FOR_EACH_0_2_1_C(macro, ...) INTERNAL_FOR_EACH_0_2_1(macro, (, ), __VA_ARGS__)

// Runs a macro through variadic arguments: 1 argument from front, 2 argument view, 1 argument step.
#define FOR_EACH_1_2_1(macro, front0, ...) INTERNAL_FOR_EACH_1_2_1(macro, (), front0, __VA_ARGS__)
// Runs a macro through variadic arguments: 1 argument from front, 2 argument view, 1 argument step, comma delimited.
#define FOR_EACH_1_2_1_C(macro, front0, ...) INTERNAL_FOR_EACH_1_2_1(macro, (, ), front0, __VA_ARGS__)

#define INTERNAL_FOR_EACH_0_2_2(macro, delim, ...)\
  __VA_OPT__(EXPAND(INTERNAL_FOR_EACH_HELPER_0_2_2(macro, delim, __VA_ARGS__)))
#define INTERNAL_FOR_EACH_HELPER_0_2_2(macro, delim, arg0, arg1, ...)\
  macro(arg0, arg1)\
  __VA_OPT__(UNPAREN delim INTERNAL_FOR_EACH_AGAIN_0_2_2 PARENS (macro, delim, __VA_ARGS__))
#define INTERNAL_FOR_EACH_AGAIN_0_2_2() INTERNAL_FOR_EACH_HELPER_0_2_2

#define INTERNAL_FOR_EACH_1_2_2(macro, delim, front0, ...)\
  __VA_OPT__(EXPAND(INTERNAL_FOR_EACH_HELPER_1_2_2(macro, delim, front0, __VA_ARGS__)))
#define INTERNAL_FOR_EACH_HELPER_1_2_2(macro, delim, front0, arg0, arg1, ...)\
  macro(front0, arg0, arg1)\
  __VA_OPT__(UNPAREN delim INTERNAL_FOR_EACH_AGAIN_1_2_2 PARENS (macro, delim, front0, __VA_ARGS__))
#define INTERNAL_FOR_EACH_AGAIN_1_2_2() INTERNAL_FOR_EACH_HELPER_1_2_2

// Runs a macro through variadic arguments: 0 arguments from front, 2 argument view, 2 argument step.
#define FOR_EACH_0_2_2(macro, ...) INTERNAL_FOR_EACH_0_2_2(macro, (), __VA_ARGS__)
// Runs a macro through variadic arguments: 0 arguments from front, 2 argument view, 2 argument step, comma delimited.
#define FOR_EACH_0_2_2_C(macro, ...) INTERNAL_FOR_EACH_0_2_2(macro, (, ), __VA_ARGS__)

// Runs a macro through variadic arguments: 1 argument from front, 2 argument view, 2 argument step.
#define FOR_EACH_1_2_2(macro, front0, ...) INTERNAL_FOR_EACH_1_2_2(macro, (), front0, __VA_ARGS__)
// Runs a macro through variadic arguments: 1 argument from front, 2 argument view, 2 argument step, comma delimited.
#define FOR_EACH_1_2_2_C(macro, front0, ...) INTERNAL_FOR_EACH_1_2_2(macro, (, ), front0, __VA_ARGS__)
