//------------------------------------------------------------------------------
//
// File Name:	Font.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class describing a font.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "TextSystem.h"

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
	class Material;
	class Font;

	// Typedefs:
	typedef LibraryStatic<Font> FontLibrary;

	// Class Definition:
	class Font : public NamedObject
	{
		friend class Sprite;
		friend class SpriteText;

		// Public Constants, Enums, and Structs:
	public:
		DECLARE_READ_KEY(Font);
		DECLARE_READ_DIR(DIR_ASSETS"Fonts");

		// Constructors/Destructors:
	public:
		Font();
	private:
		Font(string_view_arg name, FT_FaceRec_* face);
	public:
		~Font(void);

		// Public Static Functions:
	public:
		//
		static Font* Read(string_view_arg name);

		// Public Functions:
	public:
		//
		void Read(const StreamIn& stream);

		//
		void SetSize(int ptSize, unsigned int dpi) const;

		//
		unsigned char* Draw(const string_t& text, const IVec2& size, const Mat2& mtx, const Vec2& pos, bool useKerning = true) const;

		//
		Texture* RenderText() const;

		//
		void DisplayIMGUI();

		// Public Event Handlers
	public:

		// Private Constants, Enums, and Structs:
	private:
		//
		struct TextSpriteSheet
		{
			struct Rect
			{
				Vec2 uvPos, uvSize, glyphSize, bearingH, bearingV;
				float advanceH, advanceV;
			};
			Texture* texture;
			Material* material;
			Rect* symbols;
			unsigned start;
			unsigned size;
            ~TextSpriteSheet();
		};

		// Private Static Functions:
	private:

		// Private Functions:
	private:
		//
		void Draw_(const string_t& text, unsigned char* texture, const IVec2& size, const Mat2& mtx, const Vec2& pos, bool useKerning = true) const;
		
		//
		void BuildTSS(string_view_arg text, TextSpriteSheet* tss) const;

		// Private Static Variables:
	private:

		// Private Variables:
	private:
        FT_FaceRec_* face_;
		TextSpriteSheet* tss_;
	};

}	// namespace
