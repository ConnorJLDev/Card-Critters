//------------------------------------------------------------------------------
//
// File Name:	AnimationLinkTransform.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Interface between an animation component and transform components.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "AnimationLink.h"

#include "TransformBase.h"

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

	// Class Definition:
	class AnimationLinkTransform : public AnimationLink
	{
		friend class AnimationLinkModel;

		// Public Constants, Enums, and Structs:
	public:
		enum class Options : unsigned int
		{
			None = 0,
			TranslationExists   = 1 <<  0,
			RotationExists      = 1 <<  1,
			ScaleExists         = 1 <<  2,
			TranslationIdentity = 1 <<  3,
			RotationIdentity    = 1 <<  4,
			ScaleIdentity       = 1 <<  5,
			TranslationUniform  = 1 <<  6,
			RotationUniform     = 1 <<  7,
			ScaleUniform        = 1 <<  8,
			TranslationConst    = 1 <<  9,
			RotationConst       = 1 << 10,
			ScaleConst          = 1 << 11,
			TranslationXExists  = 1 << 12,
			TranslationYExists  = 1 << 13,
			TranslationZExists  = 1 << 14,
			RotationXExists     = 1 << 15,
			RotationYExists     = 1 << 16,
			RotationZExists     = 1 << 17,
			ScaleXExists        = 1 << 18,
			ScaleYExists        = 1 << 19,
			ScaleZExists        = 1 << 20,
			TranslationXConst   = 1 << 21,
			TranslationYConst   = 1 << 22,
			TranslationZConst   = 1 << 23,
			RotationXConst      = 1 << 24,
			RotationYConst      = 1 << 25,
			RotationZConst      = 1 << 26,
			ScaleXConst         = 1 << 27,
			ScaleYConst         = 1 << 28,
			ScaleZConst         = 1 << 29,
		};

		// Constructors/Destructors:
	public:
		AnimationLinkTransform();

		~AnimationLinkTransform();

		// Public Static Functions:
	public:
		ENUM_DECLARE_OPS(Options);

		// Public Functions:
	public:
		//
		void Start(Entity* entity) override;

		//
		void Update(Entity* entity, const vector_t<float>& evaluated) override;

		//
		void Update(Particle* particle, const vector_t<float>& evaluated) override;

        //
        template <typename T>
		void Apply(T& transform, const vector_t<float>& evaluated) const;

		// Public Event Handlers
	public:

		// Private Constants, Enums, and Structs:
	private:
		enum class Indices
		{
			TranslationX,
			TranslationY,
			TranslationZ,
			RotationX,
			RotationY,
			RotationZ,
			ScaleX,
			ScaleY,
			ScaleZ,

			NUM_ENTRIES,
		};

		struct TransformData
		{
			Options options;
			array_t<ElementData, (size_t)Indices::NUM_ENTRIES> elementData;

			void Read(StreamIn& stream, const vector_t<float>& curveIndices, unsigned& curveCount);

            template <typename T>
			void Start(T& transform) const;

            template <typename T>
			void Apply(T& transform, const vector_t<float>& evaluated) const;
		};

		ENUM_MAPS_CLASS_DECLARE(Options);

		// Private Static Functions:
	private:

		// Private Functions:
	private:
		//
		unsigned OnRead(StreamIn& stream, const vector_t<float>& curveIndices) override;

		// Private Static Variables:
	private:

		// Private Variables:
	private:
		TransformData data_;
	};

}	// namespace
