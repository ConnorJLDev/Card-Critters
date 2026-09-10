//------------------------------------------------------------------------------
//
// File Name:	Texture.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Texture encapsulator.
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

namespace Nightjar
{
    class Texture;
}

namespace ImGui
{
    void Image(const Nightjar::Texture* texture, const Nightjar::Vec2& size);
}

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	// Forward Declarations:
	class Texture;

	// Typedefs:
	typedef LibraryStatic<Texture> TextureLibrary;

	// Class Definition:
	class Texture : public NamedObject
	{
        friend void ImGui::Image(const Texture* texture, const Vec2& size);
		friend class FrameBuffer;
		friend class Font;
		friend class GraphicsSystem_;

		// Public Constants, Enums, and Structs:
	public:
		DECLARE_READ_KEY(Texture);
		DECLARE_READ_DIR(DIR_ASSETS"Textures");

        // Enum describing what channels are in a texture.
		enum class Format : unsigned char
		{
			Invalid = (unsigned char)-1,
			R = 0,
			RG,
			RGB,
			BGR,
			RGBA,
			BGRA,
			IntR,
			IntRG,
			IntRGB,
			IntBGR,
			IntRGBA,
			IntBGRA,
            Depth,
			Stencil,
			DepthStencil,
            NUM_ENTRIES,
		};

        // Enum describing the channel structure of each texel.
		enum class ChannelType : unsigned char
		{
			Invalid = (unsigned char)-1,
			UByte = 0,
			Byte,
			UShort,
			Short,
			UInt,
			Int,
			HalfFloat,
			Float,
			UByte332,
			UByte233Rev,
			UShort565,
			UShort565Rev,
			UShort4444,
			UShort4444Rev,
			UShort5551,
			UShort1555Rev,
			UInt8888,
			UInt8888Rev,
			UIntAAA2,
			UInt2AAARev,
            NUM_ENTRIES,
		};

		// Constructors/Destructors:
	public:
		Texture(void);

		Texture(const Texture& other);

		Texture(Texture&& other) noexcept;

        Texture& operator=(const Texture& other);

		Texture(const string_t& name, const UVec1& dimensions, Format format, ChannelType type);

		Texture(const string_t& name, const UVec2& dimensions, Format format, ChannelType type);

		Texture(const string_t& name, const UVec3& dimensions, Format format, ChannelType type);

		~Texture(void);

		// Public Static Functions:
	public:
		/// <summary>
		/// Reads a given file in as a texture.
		/// </summary>
		/// <param name="name">The name of the texture to read.</param>
		/// <returns>A pointer to the new texture, or nullptr on failure.</returns>
		static Texture* Read(string_view_arg name);

		// Public Functions:
	public:
		/// <summary>
		/// Reads a texture in from a stream.
		/// </summary>
		/// <param name="stream">The stream to read from.</param>
		void Read(StreamIn& stream);

        /// <summary>
        /// Checks if this texture is valid (non-zero dimensions and non-null data).
        /// </summary>
        /// <returns>True if the texture is valid, false otherwise.</returns>
        bool IsValid(void) const;

		/// <summary>
		/// Gets the dimensions (width, height) of the texture, in texels.
		/// </summary>
		/// <returns>The texture dimensions.</returns>
		const UVec2& GetDimensions() const;

		/// <summary>
		/// Sets the dimensions (width, height) of the texture, in texels.
		/// </summary>
		/// <param name="dimensions">The dimensions to resize to.</param>
		void SetDimensions(const UVec2& dimensions);

        /// <summary>
        /// Gets the total size of this texture.
        /// </summary>
        /// <returns>The size of this texture in bytes.</returns>
        size_t GetSize(void) const;

        /// <summary>
        /// Gets the pixel format of this texture.
        /// </summary>
        /// <returns>The pixel format in use for this texture.</returns>
        Format GetFormat(void) const;

        /// <summary>
        /// Gets the type that each color channel is stored as in this texture.
        /// </summary>
        /// <returns>The color channel type in use for this texture.</returns>
        ChannelType GetChannelType(void) const;

		/// <summary>
		/// Gets a pointer to the pixel data this texture holds.
		/// </summary>
		/// <returns>A pointer to the pixel data as unsigned char.</returns>
		unsigned char* Data(void) const;

        /// <summary>
        /// Automatically generates mipmaps from the base map.
        /// </summary>
        void GenerateMipMaps(void);

        /// <summary>
        /// Sets this texture as active for rendering.
        /// </summary>
        void Activate(void) const;

        /// <summary>
        /// Displays Texture information in ImGui.
        /// </summary>
        void DisplayIMGUI(void);

		// Public Event Handlers
	public:

		// Private Constants, Enums, and Structs:
	private:
        // Struct defining data of a mipmap for a given texture.
        struct MipMap
        {
            Texture* texture;
            unsigned char* data;
            union
            {
                UVec1 dimensions1;
                UVec2 dimensions2;
                UVec3 dimensions3;
            };
            int level;

            MipMap();
            MipMap(const MipMap& other);
            MipMap(MipMap&& other) noexcept;
            MipMap& operator=(const MipMap& other);
            ~MipMap();

            /// <summary>
            /// Checks if this texture is valid (non-zero dimensions and non-null data).
            /// </summary>
            /// <returns>True if the texture is valid, false otherwise.</returns>
            bool IsValid(void) const;

            /// <summary>
            /// Gets the total size of this texture.
            /// </summary>
            /// <returns>The size of this texture in bytes.</returns>
            size_t GetSize(void) const;
        };

		// Private Static Functions:
	private:
        /// <summary>
        /// Unbinds all textures from the current context.
        /// </summary>
        static void UnbindBuffers(void);

		// Private Functions:
	private:
		/// <summary>
		/// Registers this texture with the graphics API.
		/// </summary>
		void Register(void);

		/// <summary>
		/// Reads in pixel data from a file.
		/// </summary>
		/// <param name="filename">The file to read from.</param>
		void ReadFile(string_view_arg filename);

        /// <summary>
        /// Creates graphics data with this texture.
        /// </summary>
        void CreateBuffers(void);

        /// <summary>
        /// Destroys this texture's graphics data.
        /// </summary>
        /// <returns>True if the graphics data existed, false otherwise.</returns>
        bool DestroyBuffers(void);

        /// <summary>
        /// Binds this texture to the current context.
        /// </summary>
        void BindBuffers(void) const;

        /// <summary>
        /// Binds the mipmaps of this texture to itself.
        /// </summary>
        void BindMipMaps(void);

		// Private Static Variables:
	private:

		// Private Variables:
	private:
		ResourceID id_;
		Format format_;
		ChannelType type_;
        MipMap baseMap_; // The base mipmap is included directly to help with cache coherency, since it's likely most commonly accessed.
        vector_t<MipMap> mipmaps_;
        union
        {
            IVec2 mipmapLimits_;
            struct
            {
                int mipmapMin_;
                int mipmapMax_;
            };
        };
	};

}	// namespace
