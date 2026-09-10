//------------------------------------------------------------------------------
//
// File Name:	Sampler.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Sampler used for textures in shaders.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "GraphicsSystem.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	// Forward Declarations:
    class Sampler;

    // Typedefs:
    typedef LibraryStatic<Sampler> SamplerLibrary;

	// Class Definition:
	class Sampler : public NamedObject
	{
		friend class ImGuiSystem;
		friend class Material;
		friend class Texture;

		// Public Constants, Enums, and Structs:
	public:
		//
		DECLARE_READ_KEY(Sampler);

		//
		DECLARE_READ_DIR(DIR_DATA"Samplers");

        // Methods of wrapping to use when texture coordinates go outside the texture.
        enum class WrapMode : unsigned char
        {
            Clamp,  // The coordinates are clamped to the edge of the texture.
            Border, // If coordinates are outside of the texture, the border color is used.
            Repeat, // The coordinates loop back around.
            Mirror, // The coordinates reflect off each edge.
            NUM_ENTRIES,
        };

        // Methods of filtering the texture when magnifying.
        enum class FilterMagnification : unsigned char
        {
            Nearest,    // The color is selected from the nearest texel.
            Linear,     // The color is linearly blended from the nearest four texels (also called bilinear).
            NUM_ENTRIES,
        };

        // Methods of filtering the texture when minifying.
        enum class FilterMinification : unsigned char
        {
            Nearest,                // The color is selected from the nearest texel.
            Linear,                 // The color is linearly blended from the nearest four texels (also called bilinear).
            NearestMipmapNearest,   // The color is selected from the nearest texel on the nearest mipmap.
            LinearMipmapNearest,    // The color is linearly blended from the nearest four texels on the nearest mipmap.
            NearestMipmapLinear,    // The color is linearly blended between the nearest texel on each of the two closest mipmaps.
            LinearMipmapLinear,     // The color is linearly blended between the linear blend of the nearest four texels on each of the two closest mipmaps (also called trilinear).
            NUM_ENTRIES,
        };

        // Available channels to select when swizzling.
        enum class Swizzle : unsigned char
        {
            Red,    // Use the texture's red channel.
            Green,  // Use the texture's green channel.
            Blue,   // Use the texture's blue channel.
            Alpha,  // Use the texture's alpha channel.
            Zero,   // Use a value of 0.
            One,    // Use a value of 1.
            NUM_ENTRIES,
        };

        ENUM_MAPS_CLASS_SEQUENTIAL_DECLARE(WrapMode);
        ENUM_MAPS_CLASS_SEQUENTIAL_DECLARE(FilterMagnification);
        ENUM_MAPS_CLASS_SEQUENTIAL_DECLARE(FilterMinification);
        ENUM_MAPS_CLASS_SEQUENTIAL_DECLARE(Swizzle);

		// Constructors/Destructors:
	public:
		Sampler(void);

        Sampler(const Sampler& other);

        Sampler(Sampler&& other) noexcept;

        Sampler& operator=(const Sampler& other);

		~Sampler(void);

		// Public Static Functions:
	public:

		// Public Functions:
	public:
        /// <summary>
        /// Reads Sampler data from a stream.
        /// </summary>
        /// <param name="stream">The stream to read from.</param>
        void Read(StreamIn& stream);

        /// <summary>
        /// Gets the wrapping mode for depth.
        /// </summary>
        /// <returns>The depth wrap mode used.</returns>
        WrapMode GetWrapR(void) const;

        /// <summary>
        /// Sets the wrapping mode for depth.
        /// </summary>
        /// <param name="wrap">The new depth wrap mode to use.</param>
        void SetWrapR(WrapMode wrap);

        /// <summary>
        /// Gets the wrapping mode for height.
        /// </summary>
        /// <returns>The height wrap mode used.</returns>
        WrapMode GetWrapS(void) const;

        /// <summary>
        /// Sets the wrapping mode for height.
        /// </summary>
        /// <param name="wrap">The new height wrap mode to use.</param>
        void SetWrapS(WrapMode wrap);

        /// <summary>
        /// Gets the wrapping mode for width.
        /// </summary>
        /// <returns>The width wrap mode used.</returns>
        WrapMode GetWrapT(void) const;

        /// <summary>
        /// Sets the wrapping mode for width.
        /// </summary>
        /// <param name="wrap">The new width wrap mode to use.</param>
        void SetWrapT(WrapMode wrap);

        /// <summary>
        /// Gets the filter used for minification.
        /// </summary>
        /// <returns>The minification filter used.</returns>
        FilterMinification GetFilterMinification(void) const;

        /// <summary>
        /// Sets the filter used for minification.
        /// </summary>
        /// <param name="filter">The new minification filter to use.</param>
        void SetFilterMinification(FilterMinification filter);

        /// <summary>
        /// Gets the filter used for magnification.
        /// </summary>
        /// <returns>The magnification filter used.</returns>
        FilterMagnification GetFilterMagnification(void) const;

        /// <summary>
        /// Sets the filter used for magnification.
        /// </summary>
        /// <param name="filter">The new magnification filter to use.</param>
        void SetFilterMagnification(FilterMagnification filter);

        /// <summary>
        /// Gets the channels used for swizzling.
        /// </summary>
        /// <returns>The swizzle channels used.</returns>
        array_t<Swizzle, 4> GetSwizzle(void) const;

        /// <summary>
        /// Sets the channels used for swizzling.
        /// </summary>
        /// <param name="swizzle">The new swizzle channels to use.</param>
        void SetSwizzle(array_t<Swizzle, 4> swizzle);

        /// <summary>
        /// Gets the minimum LOD used when sampling.
        /// </summary>
        /// <returns>The minimum LOD used.</returns>
        float GetLODMin(void) const;

        /// <summary>
        /// Sets the minimum LOD used when sampling.
        /// </summary>
        /// <param name="lod">The new minimum LOD to use.</param>
        void SetLODMin(float lod);

        /// <summary>
        /// Gets the maximum LOD used when sampling.
        /// </summary>
        /// <returns>The maximum LOD used.</returns>
        float GetLODMax(void) const;

        /// <summary>
        /// Sets the maximum LOD used when sampling.
        /// </summary>
        /// <param name="lod">The new maximum LOD to use.</param>
        void SetLODMax(float lod);

        /// <summary>
        /// Gets the LOD bias used when sampling.
        /// </summary>
        /// <returns>The LOD bias used.</returns>
        float GetLODBias(void) const;

        /// <summary>
        /// Sets the LOD bias used when sampling.
        /// </summary>
        /// <param name="bias">The new LOD bias to use.</param>
        void SetLODBias(float bias);

        /// <summary>
        /// Gets the border color used when a wrap mode uses "Border".
        /// </summary>
        /// <returns>The border color used.</returns>
        const Vec4& GetBorderColor(void) const;

        /// <summary>
        /// Sets the border color used when a wrap mode uses "Border".
        /// </summary>
        /// <param name="color">The new border color to use.</param>
        void SetBorderColor(const Vec4& color);

        /// <summary>
        /// Activates this sampler's properties for the graphics API.
        /// </summary>
        void Activate() const;

        /// <summary>
        /// Displays Sampler properties in IMGUI.
        /// </summary>
        void DisplayIMGUI();

		// Public Event Handlers
	public:

		// Private Constants, Enums, and Structs:
	private:
        static const StreamKey ReadKeyWrapR, ReadKeyWrapS, ReadKeyWrapT, ReadKeyFilterMin, ReadKeyFilterMag, ReadKeySwizzleR, ReadKeySwizzleG, ReadKeySwizzleB, ReadKeySwizzleA, ReadKeyLODMin, ReadKeyLODMax, ReadKeyLODBias, ReadKeyBorderColor;

        static const array_t<ResourceEnum, WrapMode::NUM_ENTRIES> WrapModeValues;

        static const array_t<ResourceEnum, FilterMinification::NUM_ENTRIES> FilterMinificationValues;

        static const array_t<ResourceEnum, FilterMagnification::NUM_ENTRIES> FilterMagnificationValues;

        static const array_t<ResourceEnum, Swizzle::NUM_ENTRIES> SwizzleValues;

		// Private Static Functions:
	private:

		// Private Functions:
	private:

		// Private Static Variables:
	private:

		// Private Variables:
	private:
        WrapMode wrapR_, wrapS_, wrapT_;
        FilterMinification filterMin_;
        FilterMagnification filterMag_;
        Swizzle swizzleR_, swizzleG_, swizzleB_, swizzleA_;
        union
        {
            Vec2 lodLimits_;
            struct
            {
                float lodMin_;
                float lodMax_;
            };
        };
        float lodBias_;
        Vec4 borderColor_;
	};

}	// namespace
