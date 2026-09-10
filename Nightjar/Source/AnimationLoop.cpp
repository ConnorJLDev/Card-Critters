//------------------------------------------------------------------------------
//
// File Name:	AnimationLoop.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Enum for describing how animations should loop.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "AnimationLoop.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{

	ENUM_MAPS_SEQUENTIAL(AnimationLoop, Stop, Loop, Mirror, MirrorOnce);

}	// namespace
