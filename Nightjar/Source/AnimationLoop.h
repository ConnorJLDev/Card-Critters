//------------------------------------------------------------------------------
//
// File Name:	AnimationLoop.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Enum for describing how animations should loop.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	// Forward Declarations:

	// Typedefs:

	// Enum Definition:
	enum class AnimationLoop : unsigned char
	{
		Stop,
		Loop,
		Mirror,
		MirrorOnce,
		NUM_ENTRIES,
	};

	ENUM_MAPS_SEQUENTIAL_DECLARE(AnimationLoop);

}	// namespace
