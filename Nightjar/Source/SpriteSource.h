//------------------------------------------------------------------------------
//
// File Name:	 SpriteSource.h
// Author(s):	 Jason Nguyen (jason.nguyen), Jonathan Sandquist (jonathan.s)
// Course:       GAM200F25
// Project:      Card Critters
// Purpose:      Class that holds data for a sprite sheet.
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
	class Texture;
	class Sampler;
	class Mesh;
	class SpriteSource;

	// Typedefs:
	typedef LibraryStatic<SpriteSource> SpriteSourceLibrary;

	// Class Definition:
	class SpriteSource : public NamedObject
	{
		friend class Library<SpriteSource>;
		friend class ImGuiSystem;

		// Public Constants, Enums, and Structs:
	public:
		DECLARE_READ_KEY(SpriteSource);
		DECLARE_READ_DIR(DIR_DATA"SpriteSources");

		// Constructors/Destructors:
	public:
		SpriteSource(void);
		
		SpriteSource(string_view_arg name, const Texture* texture);

		~SpriteSource(void);

		// Public Static Functions:
	public:

		// Public Functions:
	public:
		/// <summary>
		/// Read from a stream
		/// </summary>
		/// <param name="stream"></param>
		void Read(StreamIn& stream);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		unsigned int GetFrameCount() const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		float GetFrameDuration() const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const Mesh* GetMesh() const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const Texture* GetTexture() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="name"></param>
		void SetTexture(string_view_arg name);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const Sampler* GetSampler() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="name"></param>
		void SetSampler(string_view_arg name);

		/// <summary>
		/// Get UV coordinates for a frame index.
		/// </summary>
		/// <param name="frameIndex"></param>
		/// <param name="uv"></param>
		void GetFrameUV(unsigned int frameIndex, Vec2& uv) const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const Vec2& GetFrameUVSize() const;

		/// <summary>
		/// 
		/// </summary>
		void DisplayIMGUI();

		// Public Event Handlers
	public:

		// Private Constants, Enums, and Structs:
	private:

		// Private Static Functions:
	private:

		// Private Functions:
	private:

		// Private Static Variables:
	private:
		static const StreamKey SpriteName, ReadKeyTexture, ReadKeySampler, ReadKeyRows, ReadKeyColumns;

		// Private Variables:
	private:
		const Texture* texture_;
		const Sampler* sampler_;
		const Mesh* mesh_;
		UVec2 dimensions_;
		Vec2 uvSize_;
		float frameDuration_;
	};

}	// namespace