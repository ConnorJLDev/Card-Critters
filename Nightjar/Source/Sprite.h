//------------------------------------------------------------------------------
//
// File Name:	Sprite.h
// Author(s):	Jason Nguyen (jason.nguyen), Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Sprite component.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//#define TEXT_DEBUG

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Component.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	// Forward Declarations:
	class SpriteSource;
	class Material;

	// Typedefs:

	// Class Definition:
	class Sprite : public Component
	{
		// Public Constants, Enums, and Structs:
	public:
		// Common component data.
		COMPONENT_COMMON(Sprite);

		// Constructors/Destructors:
	public:
		Sprite(void);
	private:
		Sprite(const Sprite& other);
	public:
		~Sprite(void) override;

		// Public Static Functions:
	public:

		// Public Functions:
	public:
		/// <summary>
		/// Clone the Sprite.
		/// </summary>
		/// <returns>A clone of this Sprite.</returns>
		Sprite* Clone() const override;

		/// <summary>
		/// Read Sprite data from a stream.
		/// </summary>
		/// <param name="stream">The stream to read from.</param>
		void Read(StreamIn& stream) override;

		/// <summary>
		/// Writes the Sprite data into the stream.
		/// </summary>
		/// <param name="stream">The stream to write into.</param>
		void Write(StreamOut& stream) override;

		/// <summary>
		/// Render this Sprite.
		/// </summary>
		void Render() const override;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		float GetAlpha() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="alpha"></param>
		void SetAlpha(float alpha);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const Vec4& GetTint() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="color"></param>
		void SetTint(const Vec4& color);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="color"></param>
		void SetTint(const Vec3& color);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		unsigned int GetFrameIndex() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="frameIndex"></param>
		void SetFrameIndex(unsigned int frameIndex);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const SpriteSource* GetSpriteSource() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="spriteSourceName"></param>
		void SetSpriteSource(const string_t& spriteSourceName);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="spriteSource"></param>
		void SetSpriteSource(const SpriteSource* spriteSource);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		int GetZlayer() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="layer"></param>
		void SetZLayer(int layer);

		// Public Event Handlers
	public:

		// Private Constants, Enums, and Structs:
	private:
		static const StreamKey ReadKeyAlpha, ReadKeyTint, ReadKeyFrameIndex, ReadKeyZLayer, ReadKeyText, ReadKeySpriteSource, ReadKeyFont, ReadKeyTextAlignment;

		static const IVec2 ZBounds;
		static const Vec2 ZBoundsFactor;

		// Private Static Functions:
	private:

		// Private Functions:
	private:

		// Private Static Variables:
	private:
        static const SpriteSource* DebugSpriteSource;

		// Private Variables:
	private:
		const SpriteSource* spriteSource_;
		Material* material_;
		unsigned int frameIndex_;
		Vec4 tint_;
		int zLayer_;
        mutable bool dirty_;
	};

}	// namespace