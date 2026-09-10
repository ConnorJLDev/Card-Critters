//------------------------------------------------------------------------------
//
// File Name:	SpriteText.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		SpriteText component.
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
	class Material;
	class TextMesh;
	class Font;

	// Typedefs:

	// Class Definition:
	class SpriteText : public Component
	{
		friend class ImGuiSystem;

		// Public Constants, Enums, and Structs:
	public:
		// Common component data.
		COMPONENT_COMMON(SpriteText);

		// Constructors/Destructors:
	public:
		SpriteText(void);
	private:
		SpriteText(const SpriteText& other);
	public:
		~SpriteText(void) override;

		// Public Static Functions:
	public:

		// Public Functions:
	public:
		/// <summary>
		/// Clone the SpriteText.
		/// </summary>
		/// <returns>A clone of this SpriteText.</returns>
		SpriteText* Clone(void) const override;

		/// <summary>
		/// Read SpriteText data from a stream.
		/// </summary>
		/// <param name="stream">The stream to read from.</param>
		void Read(StreamIn& stream) override;

		/// <summary>
		/// Writes the SpriteText data into the stream.
		/// </summary>
		/// <param name="stream">The stream to write into.</param>
		void Write(StreamOut& stream) override;

		/// <summary>
		/// Update this SpriteText.
		/// </summary>
		/// <param name="dt">The time elapsed since the previous tick.</param>
		void Update(float dt) override;

		/// <summary>
		/// Render this SpriteText.
		/// </summary>
		void Render(void) const override;

		/// <summary>
		/// Gets the opacity of the text.
		/// </summary>
		/// <returns>The current text opacity.</returns>
		float GetAlpha(void) const;

		/// <summary>
		/// Sets the opacity of the text.
		/// </summary>
		/// <param name="alpha">The new text opacity.</param>
		void SetAlpha(float alpha);

		/// <summary>
		/// Gets the tint and opacity of the text.
		/// </summary>
		/// <returns>The current text tint and opacity.</returns>
		const Vec4& GetTint(void) const;

		/// <summary>
		/// Sets the tint and opacity of the text.
		/// </summary>
		/// <param name="color">The new text tint and opacity.</param>
		void SetTint(const Vec4& color);

		/// <summary>
		/// Sets the tint of the text.
		/// </summary>
		/// <param name="color">The new text tint.</param>
		void SetTint(const Vec3& color);

		/// <summary>
		/// Gets the font of the text.
		/// </summary>
		/// <returns>A pointer to the current font being used for the text, or nullptr if there is no font.</returns>
		const Font* GetFont(void);

		/// <summary>
		/// Sets the font of the text by name.
		/// </summary>
		/// <param name="fontName">The name of the new font to use.</param>
		void SetFont(const string_t& fontName);

		/// <summary>
		/// Sets the font of the text.
		/// </summary>
		/// <param name="font">The new font to use.</param>
		void SetFont(const Font* font);

		/// <summary>
		/// Gets the text currently displayed.
		/// </summary>
		/// <returns></returns>
		const string_t& GetText(void) const;

		/// <summary>
		/// Sets the text to display.
		/// </summary>
		/// <param name="newText">The new text to display.</param>
		void SetText(string_view_arg newText);

		/// <summary>
		/// Gets the strength of the text border.
		/// </summary>
		/// <returns>The strength of the text border.</returns>
		float GetTextBorderStrength(void) const;

		/// <summary>
		/// Sets the strength of the text border.
		/// </summary>
		/// <param name="strength">The new text border strength.</param>
		void SetTextBorderStrength(float strength);

		/// <summary>
		/// Gets the color of the text border.
		/// </summary>
		/// <returns>The color of the text border.</returns>
		const Vec4& GetTextBorderColor(void) const;

		/// <summary>
		/// Sets the color of the text border.
		/// </summary>
		/// <param name="color">The new text border color.</param>
		void SetTextBorderColor(const Vec4& color);

		/// <summary>
		/// Gets the Z layer this SpriteText is rendered on.
		/// </summary>
		/// <returns>The Z layer being rendered on.</returns>
		int GetZlayer(void) const;

		/// <summary>
		/// Sets the Z layer to render this SpriteText on.
		/// </summary>
		/// <param name="layer">The new layer to render on.</param>
		void SetZLayer(int layer);

		// Public Event Handlers
	public:

		// Private Constants, Enums, and Structs:
	private:
        // 
		static const StreamKey ReadKeyAlpha, ReadKeyTint, ReadKeyBorderColor, ReadKeyBorderStrength, ReadKeyZLayer, ReadKeyText, ReadKeyFont, ReadKeyTextAlignment;

        // 
		static const IVec2 ZBounds;
        // 
		static const Vec2 ZBoundsFactor;

		// Private Static Functions:
	private:

		// Private Functions:
	private:
        /// <summary>
        /// Updates the internal mesh used for displaying the text.
        /// </summary>
        void UpdateTextMeta(void);

		/// <summary>
		/// Calculates and updates the maximum width of the text displayed.
		/// </summary>
		void UpdateTextWidth(void);

		// Private Static Variables:
	private:

		// Private Variables:
	private:
        // The font that is being rendered with.
		const Font* font_;
        // Material containing the render settings for this text.
		Material* material_;
        // Mesh used for rendering the text.
		TextMesh* textMesh_;
        // The text that is displayed.
		string_t text_;
        // The color and opacity of the text.
		Vec4 tint_;
        // The color of the text border.
		Vec4 textBorderColor_;
        // The strength of the text border.
		float textBorderStrength_;
        // The width of the longest line of text.
		float textWidth_;
        // The Z layer this text is displayed on.
		int zLayer_;
        // The left/center/right alignment of the text.
		float textAlignment_;
        // Dirty flag for if render settings have changed.
        mutable bool dirty_;
	};

}	// namespace