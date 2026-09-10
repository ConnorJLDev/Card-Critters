//------------------------------------------------------------------------------
//
// File Name:	Font.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class describing a font.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//#define _GLYPH_REUSE

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "TextSystem.h"
#include "Font.h"

#include TEXT_INCLUDE
#include FT_GLYPH_H

#include "Texture.h"
#include "Material.h"

#include "Library.inl"
#include "LibraryStatic.inl"

#undef TRACE_CATEGORY
#define TRACE_CATEGORY FONT

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	//--------------------------------------------------------------------------
	// Public Constants:
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// Public Static Variables:
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// Public Variables:
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// Private Constants:
	//--------------------------------------------------------------------------

    //
    struct Glyph
    {
        unsigned int    index;  // glyph index
        IVec2           pos;    // glyph origin on the baseline
        FT_Glyph        image;  // glyph image
    };

	//--------------------------------------------------------------------------
	// Private Static Variables:
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// Private Variables:
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// Constructors/Destructors:
	//--------------------------------------------------------------------------

#pragma region Constructors

	Font::Font()
		: NamedObject()
		, face_(nullptr)
		, tss_(new TextSpriteSheet())
	{
	}

	Font::Font(string_view_arg name, FT_Face face)
		: NamedObject(name)
		, face_(face)
		, tss_(new TextSpriteSheet())
	{
	}

	//--------------------------------------------------------------------------

	Font::~Font(void)
	{
		auto error = FT_Done_Face(face_);
		if (error != FT_Err_Ok)
		{
			TraceError("Error deleting font face: 0x{:X}", error);
		}
        SafeDeleteCheck(tss_);
	}

#pragma endregion Constructors

	//--------------------------------------------------------------------------
	// Public Static Functions:
	//--------------------------------------------------------------------------

#pragma region Public Static Functions

	Font* Font::Read(string_view_arg name)
	{
		FT_Face face;
		string_t dirName = "Assets/Fonts/" + string_t(name);
		auto error = FT_New_Face(TextSystem::GetLibrary(), dirName.c_str(), 0, &face);
		if (error != FT_Err_Ok)
		{
			TraceError("Error creating font face {} from {}: 0x{:X}", name, dirName, error);
			return nullptr;
		}
		Font* font = new Font(name, face);

		Texture* texture = font->RenderText();

		texture->Register();
        texture->GenerateMipMaps();

		TextureLibrary::GetInstance()->Add(texture);

		return font;
	}

#pragma endregion Public Static Functions

	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

	void Font::Read(const StreamIn& stream)
	{
		UNREFERENCED_PARAMETER(stream);
		assert(false);
	}

	void Font::SetSize(int ptSize, unsigned int dpi) const
	{
		auto error = FT_Set_Char_Size(face_, ptSize << 6, 0, dpi, 0);
		if (error != FT_Err_Ok)
		{
			TraceError("Error setting size of font face {}: {:#X}", GetName().c_str(), error);
		}
	}

	unsigned char* Font::Draw(const string_t& text, const IVec2& size, const Mat2& mtx, const Vec2& pos, bool useKerning) const
	{
		unsigned char* imageRender = new unsigned char[static_cast<unsigned>(size.x) * static_cast<unsigned>(size.y)](0);
		if (imageRender != nullptr)
		{
			Draw_(text, imageRender, size, mtx, pos, useKerning);
		}
		return imageRender;
	}

	Texture* Font::RenderText() const
	{
		constexpr string_view_t defaultText = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~ ";

		SetSize(20, 200);

		IVec2 size{ 512, 512 };
		tss_->start = ' ';
		tss_->size = static_cast<unsigned>(defaultText.size());
		tss_->symbols = new TextSpriteSheet::Rect[defaultText.size()]();
		tss_->texture = new Texture("Text_" + GetName(), (UVec2)size, Texture::Format::RG, Texture::ChannelType::UByte);
		tss_->material = GraphicsSystem::BuildMaterial("Text");
		if (tss_->texture != nullptr && tss_->symbols != nullptr && tss_->material != nullptr)
		{
			BuildTSS(defaultText, tss_);
			// TODO: Move convolution to separate function and parametrize Window.
			unsigned char* data = tss_->texture->Data();
			static constexpr int Window = 4;
			for (int y = 0; y < size.y; ++y)
			{
				for (int x = 0; x < size.x; ++x)
				{
					unsigned int sum = 0;
					unsigned int count = 0;
					for (int yDiff = -Window; yDiff <= Window; ++yDiff)
					{
						int yCurr = y + yDiff;
						if ((unsigned int)yCurr >= (unsigned int)size.y)
						{
							continue;
						}
						const int yDiffSqr = yDiff * yDiff;
						for (int xDiff = -Window; xDiff <= Window; ++xDiff)
						{
							if (xDiff * xDiff + yDiffSqr >= 20)
							{
								continue;
							}
							int xCurr = x + xDiff;
							if ((unsigned int)xCurr >= (unsigned int)size.x)
							{
								continue;
							}
							sum += data[(yCurr * size.x + xCurr) * 2 + 0];
							++count;
						}
					}
					unsigned char avg = (unsigned char)std::min(255u, sum / count);
					data[(y * size.x + x) * 2 + 1] = avg;
				}
			}
		}
		else
		{
			TraceWarning("Failed to create TSS!");
		}

		return tss_->texture;
	}

#pragma endregion Public Functions

	//--------------------------------------------------------------------------
	// Private Static Functions:
	//--------------------------------------------------------------------------

#pragma region Private Static Functions

	static void DrawBitmap(const FT_Bitmap* bitmap, const IVec2& pos, const IVec2& size, unsigned char* image, unsigned char step, unsigned char offset)
	{
		// Much of this code is adapted from https://freetype.org/freetype2/docs/tutorial/example1.c
		int x_max = pos.x + static_cast<int>(bitmap->width), y_max = pos.y + static_cast<int>(bitmap->rows);

		/* for simplicity, we assume that `bitmap->pixel_mode' */
		/* is `FT_PIXEL_MODE_GRAY' (i.e., not a bitmap font)   */
		for (int i = pos.x, p = 0; i < x_max; i++, p++)
		{
			for (int j = pos.y, q = 0; j < y_max; j++, q++)
			{
				if (i < 0 || j < 0 || i >= size.x || j >= size.y)
				{
					continue;
				}
				image[(j * size.x + i) * step + offset] |= bitmap->buffer[q * bitmap->width + p];
			}
		}
	}

	static void CalculateBBox(const Glyph glyphs[], int numGlyphs, FT_BBox& bbox)
	{
		bbox.xMin = bbox.yMin = 32000;
		bbox.xMax = bbox.yMax = -32000;

		for (int n = 0; n < numGlyphs; n++)
		{
			FT_BBox glyph_bbox;

			FT_Glyph_Get_CBox(glyphs[n].image, FT_GLYPH_BBOX_SUBPIXELS,
				&glyph_bbox);
#ifndef _GLYPH_REUSE
			glyph_bbox.xMin += glyphs[n].pos.x;
			glyph_bbox.xMax += glyphs[n].pos.x;
			glyph_bbox.yMin += glyphs[n].pos.y;
			glyph_bbox.yMax += glyphs[n].pos.y;
#endif
			if (glyph_bbox.xMin < bbox.xMin)
				bbox.xMin = glyph_bbox.xMin;

			if (glyph_bbox.yMin < bbox.yMin)
				bbox.yMin = glyph_bbox.yMin;

			if (glyph_bbox.xMax > bbox.xMax)
				bbox.xMax = glyph_bbox.xMax;

			if (glyph_bbox.yMax > bbox.yMax)
				bbox.yMax = glyph_bbox.yMax;
		}

		if (bbox.xMin > bbox.xMax)
		{
			bbox.xMin = 0;
			bbox.yMin = 0;
			bbox.xMax = 0;
			bbox.yMax = 0;
		}
	}

#pragma endregion Private Static Functions

	//--------------------------------------------------------------------------
	// Private Functions:
	//--------------------------------------------------------------------------

#pragma region Private Functions

	void Font::Draw_(const string_t& text, unsigned char* imageRender, const IVec2& size, const Mat2& mtx, const Vec2& pos, bool useKerning) const
	{
		Mat2 mtxTemp = mtx * (float)0x10000;
		Vec2 posTemp = pos * (float)0x40;
		FT_Error error = FT_Err_Ok;
		// Much of this code is adapted from https://freetype.org/freetype2/docs/tutorial/example1.c
		// and from https://freetype.org/freetype2/docs/tutorial/step2.html
		FT_Matrix matrix{ (int)mtxTemp[0][0], (int)mtxTemp[0][1], (int)mtxTemp[1][0], (int)mtxTemp[1][1] };
		FT_Vector penAim{ (int)posTemp.x, (int)posTemp.y };
		FT_Vector pen{ 0, 0 };
		useKerning &= FT_HAS_KERNING(face_);
		unsigned previous = 0;
		const size_t numChars(text.size());

		auto slot = face_->glyph;

#ifndef _GLYPH_REUSE
		FT_Set_Transform(face_, &matrix, &pen);
#endif

		///////////////////////////////////////////////////////////////////////////////////////////////////
		// Centering
		///////////////////////////////////////////////////////////////////////////////////////////////////

		Glyph* glyphs = new Glyph[numChars];
		int num_glyphs = 0;
		for (size_t n = 0; n < numChars; n++)
		{
			const unsigned c = static_cast<unsigned>(text[n]);
			Glyph& glyph = glyphs[num_glyphs];
			glyph.index = FT_Get_Char_Index(face_, c);
			/* retrieve kerning distance and move pen position */
			if (useKerning && previous && glyph.index)
			{
				FT_Vector delta;

				FT_Get_Kerning(face_, previous, glyph.index, FT_KERNING_DEFAULT, &delta);

				pen.x += delta.x;
				pen.y += delta.y;
			}

            glyph.pos = Vec2{pen.x, pen.y};

			error = FT_Load_Glyph(face_, glyph.index, FT_LOAD_DEFAULT);
			if (error != FT_Err_Ok)
			{
				TraceError("Failed load glyph from font face {}: 0x{:X}", GetName(), error);
				continue;
			}

			error = FT_Get_Glyph(face_->glyph, &glyphs[num_glyphs].image);
			if (error != FT_Err_Ok)
			{
				TraceError("Failed get glyph from font face {}: 0x{:X}", GetName(), error);
				continue;
			}
#ifdef _GLYPH_REUSE
			error = FT_Glyph_Transform(glyph.image, nullptr, &glyph.pos);
			if (error != FT_Err_Ok)
			{
				TraceError("Failed glyph transform from font face {}: 0x{:X}", GetName(), error);
				continue;
			}
#endif
			pen.x += slot->advance.x;
			pen.y += slot->advance.y;

			previous = glyph.index;

			num_glyphs++;
		}

		FT_BBox string_bbox;
		CalculateBBox(glyphs, num_glyphs, string_bbox);

		FT_Vector stringSize{ string_bbox.xMax - string_bbox.xMin, string_bbox.yMax - string_bbox.yMin };
		// TODO: Adjust BBox for GLYPH_REUSE when matrix is not identity.
		FT_Vector start{ penAim.x - stringSize.x / 2, penAim.y - stringSize.y / 2 };

		///////////////////////////////////////////////////////////////////////////////////////////////////
		// Rendering
		///////////////////////////////////////////////////////////////////////////////////////////////////

		for (int n = 0; n < num_glyphs; n++)
		{
			FT_Glyph& image = glyphs[n].image;
#ifdef _GLYPH_REUSE
			error = FT_Glyph_Copy(glyphs[n].image, &image);
			if (error != FT_Err_Ok)
			{
				TraceError("Failed glyph to bitmap from font face {}: 0x{:X}", GetName(), error);
				continue;
			}

			FT_Glyph_Transform(image, &matrix, &start);

			FT_BBox bbox;
			FT_Glyph_Get_CBox(image, FT_GLYPH_BBOX_PIXELS, &bbox);
			if (bbox.xMax <= 0 || bbox.xMin >= size.x || bbox.yMax <= 0 || bbox.yMin >= size.y)
			{
				continue;
			}

			error = FT_Glyph_To_Bitmap(&image, FT_RENDER_MODE_NORMAL, nullptr, true);
#else
			pen.x = start.x + glyphs[n].pos.x;
			pen.y = start.y + glyphs[n].pos.y;

            FT_BitmapGlyph bitmap = reinterpret_cast<FT_BitmapGlyph>(image);
            error = FT_Glyph_To_Bitmap(reinterpret_cast<FT_Glyph*>(&bitmap), FT_RENDER_MODE_NORMAL, &pen, false);
#endif
			if (error != FT_Err_Ok)
			{
				TraceError("Failed glyph to bitmap from font face {}: 0x{:X}", GetName(), error);
                bitmap = nullptr;
			}
			else
			{
				IVec2 posBitmap{ bitmap->left, size.y - bitmap->top };
				DrawBitmap(&bitmap->bitmap, posBitmap, size, imageRender, 1, 0);

			}
            FT_Done_Glyph(reinterpret_cast<FT_Glyph>(bitmap));
            FT_Done_Glyph(image);
		}

		delete[] glyphs;
	}

	void Font::BuildTSS(string_view_arg text, TextSpriteSheet* tss) const
	{
		FT_Error error = FT_Err_Ok;

		auto slot = face_->glyph;

		const IVec2& size = tss->texture->GetDimensions();
		const Vec2 sizeFactor = size * 0x40;

		Mat2 mtx{ 1.0f, 0.0f, 0.0f, 1.0f };
		constexpr IVec2 padBorder{ 2,8 }, padBetween{ 2,16 };
		IVec2 pos{ padBorder };
		mtx *= 0x10000;
		pos *= 0x40;
		FT_Matrix matrix{ (int)mtx[0][0], (int)mtx[0][1], (int)mtx[1][0], (int)mtx[1][1] };
		FT_Vector penStart{ (int)pos.x, (int)pos.y }, pen{ penStart };
		FT_Set_Transform(face_, &matrix, &pen);

		Material*& mat = tss->material;

		mat->SetFaceCull(Material::FaceCull::Back);
		mat->SetUniform("symbolStart", tss_->start);
		mat->SetUniform("symbolCount", tss_->size);
		mat->SetTexture("inTexture", tss_->texture);

		int penMaxY = 0;

		for (unsigned n = 0; n < tss->size; n++)
		{
			const unsigned c = static_cast<unsigned>(text[n]);
			Glyph glyph{};
			glyph.index = FT_Get_Char_Index(face_, c);
            glyph.pos.x = pen.x;
            glyph.pos.y = pen.y;

			error = FT_Load_Glyph(face_, glyph.index, FT_LOAD_DEFAULT);
			if (error != FT_Err_Ok)
			{
				TraceError("Failed load glyph from font face {}: 0x{:X}", GetName(), error);
				continue;
			}

			error = FT_Get_Glyph(slot, &glyph.image);
			if (error != FT_Err_Ok)
			{
				TraceError("Failed get glyph from font face {}: 0x{:X}", GetName(), error);
				continue;
			}

			FT_BBox glyph_bbox;

			FT_Glyph_Get_CBox(glyph.image, FT_GLYPH_BBOX_SUBPIXELS, &glyph_bbox);

			if ((glyph_bbox.xMax + glyph.pos.x) / 0x40 >= size.x - padBorder.x)
			{
				pen.x = penStart.x;
				pen.y = penMaxY + padBetween.y * 0x40;
				--n;
                FT_Done_Glyph(glyph.image);
				continue;
			}

			penMaxY = std::max(penMaxY, static_cast<int>(glyph_bbox.yMax + glyph.pos.y));

			auto& symbol = tss->symbols[n];

			// TODO: Parametrize convolutionPad.
			static constexpr int convolutionPad = (int)(4.0f * 0x40);
			glyph_bbox.xMin -= convolutionPad;
			glyph_bbox.yMin -= convolutionPad;
			glyph_bbox.xMax += convolutionPad;
			glyph_bbox.yMax += convolutionPad;

			symbol.uvPos = Vec2(glyph_bbox.xMin + glyph.pos.x, glyph_bbox.yMin + glyph.pos.y);// -Vec2(1 * 0x40, 24 * 0x40);
			symbol.uvPos /= static_cast<float>(0x40);
			symbol.uvPos.y = static_cast<float>(size.y) - symbol.uvPos.y;
			symbol.uvPos /= size;
			//symbol.uvPos /= sizeFactor;
			//symbol.uvPos.y *= 0.5f;
			symbol.uvSize = Vec2(glyph_bbox.xMax - glyph_bbox.xMin, glyph_bbox.yMax - glyph_bbox.yMin);// +Vec2(2 * 0x40, 2 * 0x40);
			symbol.uvSize /= static_cast<float>(0x40);
			symbol.uvSize /= size;
			symbol.uvSize.y *= -1;
			//symbol.uvSize /= sizeFactor;
			symbol.bearingH = Vec2(slot->metrics.horiBearingX, slot->metrics.horiBearingY);
			symbol.bearingV = Vec2(slot->metrics.vertBearingX, slot->metrics.vertBearingY);
			symbol.glyphSize = Vec2(slot->metrics.width, slot->metrics.height);
			symbol.glyphSize /= 0x40;
			symbol.advanceH = static_cast<float>(slot->metrics.horiAdvance);
			symbol.advanceV = static_cast<float>(slot->metrics.vertAdvance);

			FORMAT_MAKE(prefix, "symbols[{}].", n);
			mat->SetUniform(prefix + "pos", symbol.uvPos);
			mat->SetUniform(prefix + "size", symbol.uvSize);
			mat->SetUniform(prefix + "worldSize", symbol.glyphSize * 0.25f);

            FT_BitmapGlyph bitmap = reinterpret_cast<FT_BitmapGlyph>(glyph.image);
			error = FT_Glyph_To_Bitmap(reinterpret_cast<FT_Glyph*>(&bitmap), FT_RENDER_MODE_NORMAL, &pen, false);

			if (error != FT_Err_Ok)
			{
				TraceError("Failed glyph to bitmap from font face {}: 0x{:X}", GetName(), error);
                bitmap = nullptr;
			}
			else
			{
				IVec2 posBitmap{ bitmap->left, size.y - bitmap->top };
				DrawBitmap(&bitmap->bitmap, posBitmap, size, tss->texture->Data(), 2, 0);

                pen.x += slot->advance.x + padBorder.x * 0x40 + convolutionPad * 2;
                pen.y += slot->advance.y;
			}

            FT_Done_Glyph(reinterpret_cast<FT_Glyph>(bitmap));
            FT_Done_Glyph(glyph.image);
		}
	}

#pragma endregion Private Functions

	//--------------------------------------------------------------------------
	// Library Implementation:
	//--------------------------------------------------------------------------

#pragma region Library Implementation

	template FontLibrary;

	INSTANTIATE_LIBRARY_IMGUI_SELECTOR(Font);

	template<> Font* Library<Font>::Build(string_view_arg name)
	{
		Font* font = name.ends_with(".json") ? BuildDefault(name) : Font::Read(name);
		return font;
	}

	IMPLEMENT_READ(Font);

	IMPLEMENT_WRITE(Font);

#pragma endregion Library Implementation

    Font::TextSpriteSheet::~TextSpriteSheet()
    {
        if (symbols != nullptr)
        {
            delete[] symbols;
            symbols = nullptr;
        }
        GraphicsSystem::DestroyMaterial(material);
        GraphicsSystem::DestroyTexture(texture);
    }

}	// namespace

#undef _RENDER_GLYPH
