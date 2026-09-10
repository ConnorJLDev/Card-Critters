//------------------------------------------------------------------------------
//
// File Name:	stdafx.h
// Author(s):	Matthew Picioccio (mattpic), Doug Schilling (dschilling), Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		This file includes frequently used standard system includes.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

// Disable warnings for single-line comments, since Microsoft's stdlib, etc. use those extensively.
#pragma warning(push)
#pragma warning(disable : 4001)

#include "targetver.h" /* required by Windows */

#include <stdlib.h> /* NULL, malloc/free */
#include <stdio.h> /* printf, fgets */
#include <stdbool.h> /* bool */
#include <string.h> /* strcmp, strcpy_s, strtok_s */
#include <winerror.h>

// Used to clean /W4 unused parameters for functions that must match a function-pointer type 
// NOTE: copied from winnt.h, but we don't want to include that here, otherwise.
#define UNREFERENCED_PARAMETER(P) (P)

// Pop the warning-disable
#pragma warning(pop)

// Standard libraries
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
// STL containers
#include <array>
#include <vector>
#include <map>
#include <stack>
#include <set>
#include <unordered_set>
#include <queue>
// 
#include <ranges>
#include <functional>
#include <algorithm>
#include <string>
#include <string_view>
#include <format>
#include <source_location>
#include <stdexcept>
#include <chrono>
#include <regex>
#include <numeric>
#include <numbers>
#include <complex>
#include <variant>
#include <thread>
#include <atomic>
#include <mutex>
#include <optional>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cinttypes>

// Commonly used framework headers
#include "MacroManipulation.h"
#include "ConstexprAssert.h"
#include "MathDefs.h"
#include "MathTypeChecks.h"
using namespace Nightjar::Utility;
#include "MathFuncs.h"
#include "UtilityStringView.h"
#include "ConstexprString.h"
#include "MemoryManager.h"
#include "STLTypes.h"
#include "FixedPoint.h"
#include "UtilityTypeChecks.h"
#include "FixedString.h"
#include "UtilityString.h"
#include "StringToType.h"
#include "SafeDelete.h"
#include "CallbackWrapper.h"
#include "EnumDefs.h"
#include "StaticObjects.h"
#include "Stopwatch.h"
#include "Trace.h"
#include "Container.h"
#include "Library.h"
#include "LibraryStatic.h"
#include "Factory.h"
#include "Object.h"
#include "NamedObject.h"
#include "IOSystem.h"
#include "StreamIn.h"
#include "StreamOut.h"

#include "ContainerDearray.h"