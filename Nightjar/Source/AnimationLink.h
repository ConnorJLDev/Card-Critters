//------------------------------------------------------------------------------
//
// File Name:	AnimationLink.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Interface between an animation component and other components.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	// Forward Declarations:
	class Entity;
	class Particle;

	// Typedefs:

	// Class Definition:
	class AnimationLink : public Object
	{
		// Public Constants, Enums, and Structs:
	public:

		// Constructors/Destructors:
	public:
		AnimationLink();

		virtual ~AnimationLink() = 0;

		// Public Static Functions:
	public:
		static AnimationLink* Read(StreamIn& stream);

		// Public Functions:
	public:
		//
		void Read_(StreamIn& stream);

		//
		virtual void Start(Entity* entity) = 0;

		//
		virtual void Update(Entity* entity, const vector_t<float>& evaluated) = 0;

		//
		virtual void Update(Particle* particle, const vector_t<float>& evaluated) = 0;

		//
		virtual void DisplayIMGUI();

		// Public Event Handlers
	public:

		// Protected Constants, Enums, and Structs:
	protected:
		//
		union ElementData
		{
			// TODO: curveIndex offset and scale.
			unsigned curveIndex;
			float constValue;
			void Invalidate();
		};

		// Keys for reading from stream.
		static const StreamKey ReadKeyType, ReadKeyFlags, ReadKeyCurves;

		//
		static const map_t<string_t, std::function<AnimationLink*()>>& KeysType(void);

		//
		static const float InvalidValue;

		// Protected Static Functions:
	protected:

		// Protected Functions:
	protected:
		//
		virtual unsigned OnRead(StreamIn& stream, const vector_t<float>& curveIndices) = 0;

		// Private Static Variables:
	private:

		// Private Variables:
	private:
	};

}	// namespace
