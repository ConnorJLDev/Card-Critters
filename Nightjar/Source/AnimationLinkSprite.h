//------------------------------------------------------------------------------
//
// File Name:	AnimationLinkSprite.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Interface between an animation component and sprite components.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "AnimationLink.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	class AnimationLinkSprite : public AnimationLink
	{
		// Public Constants, Enums, and Structs:
	public:
		enum class Options : unsigned int
		{
			None = 0,
			FrameIndexExists =	1 << 0,
			RedExists =			1 << 1,
			GreenExists =		1 << 2,
			BlueExists =		1 << 3,
			AlphaExists =		1 << 4,
			FrameIndexConst =	1 << 5,
			RedConst =			1 << 6,
			GreenConst =		1 << 7,
			BlueConst =			1 << 8,
			AlphaConst =		1 << 9,
		};

		// Constructors/Destructors:
	public:
		AnimationLinkSprite();

		~AnimationLinkSprite();

		// Public Static Functions:
	public:
		ENUM_DECLARE_OPS(Options)

		// Public Functions:
	public:
		//
		void Start(Entity* entity) override;

		//
		void Update(Entity* entity, const vector_t<float>& evaluated) override;

		//
		void Update(Particle* particle, const vector_t<float>& evaluated) override;

		// Public Event Handlers
	public:

		// Private Constants, Enums, and Structs:
	private:
		enum class Indices
		{
			FrameIndex,
			Red,
			Green,
			Blue,
			Alpha,

			NUM_ENTRIES,
		};

		// Private Static Functions:
	private:

		ENUM_MAPS_CLASS_DECLARE(Options);

		// Private Functions:
	private:
		//
		unsigned OnRead(StreamIn& stream, const vector_t<float>& curveIndices) override;

		// Private Static Variables:
	private:

		// Private Variables:
	private:
		Options options_;
		array_t<ElementData, (size_t)Indices::NUM_ENTRIES> elementData_;
	};

}	// namespace