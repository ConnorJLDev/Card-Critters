//------------------------------------------------------------------------------
//
// File Name:	Texture.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Texture encapsulator.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

static constexpr size_t memId_stbi = MEM_ID("STBI");

#ifndef STB_IMAGE_IMPLEMENTATION
// Using stb image for image file loading
#define STB_IMAGE_IMPLEMENTATION
#define STBI_MALLOC(sz)           Nightjar::MemoryManager::AllocateNoexcept(sz, memId_stbi)
#define STBI_REALLOC(p,newsz)     Nightjar::MemoryManager::ReallocateNoexcept(p, newsz, memId_stbi)
#define STBI_FREE(p)              Nightjar::MemoryManager::Deallocate(p, memId_stbi)
#include "stb_image.h"
#endif

#include "Texture.h"

#include GRAPHICS_INCLUDE

#include "Library.inl"
#include "LibraryStatic.inl"

#undef TRACE_CATEGORY
#define TRACE_CATEGORY TEXTURE

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

    static constexpr array_t<string_view_t, 3> TextureExtensions
	{
		".png",
		".bmp",
		".jpg",
	};

	static constexpr array_t<ResourceEnum, (size_t)Texture::Format::NUM_ENTRIES> Formats
	{
		GL_RED,
		GL_RG,
		GL_RGB,
		GL_BGR,
		GL_RGBA,
		GL_BGRA,
		GL_RED_INTEGER,
		GL_RG_INTEGER,
		GL_RGB_INTEGER,
		GL_BGR_INTEGER,
		GL_RGBA_INTEGER,
		GL_BGRA_INTEGER,
        GL_DEPTH_COMPONENT,
		GL_STENCIL_INDEX,
		GL_DEPTH_STENCIL,
	};

    static constexpr array_t<uint8_t, (size_t)Texture::Format::NUM_ENTRIES> ComponentCounts =
    {
        1,
        2,
        3,
        3,
        4,
        4,
        1,
        2,
        3,
        3,
        4,
        4,
        1,
        1,
        1,
    };

    static constexpr array_t<ResourceEnum, (size_t)Texture::ChannelType::NUM_ENTRIES> Types
	{
		GL_UNSIGNED_BYTE,
		GL_BYTE,
		GL_UNSIGNED_SHORT,
		GL_SHORT,
		GL_UNSIGNED_INT,
		GL_INT,
		GL_HALF_FLOAT,
		GL_FLOAT,
		GL_UNSIGNED_BYTE_3_3_2,
		GL_UNSIGNED_BYTE_2_3_3_REV,
		GL_UNSIGNED_SHORT_5_6_5,
		GL_UNSIGNED_SHORT_5_6_5_REV,
		GL_UNSIGNED_SHORT_4_4_4_4,
		GL_UNSIGNED_SHORT_4_4_4_4_REV,
		GL_UNSIGNED_SHORT_5_5_5_1,
		GL_UNSIGNED_SHORT_1_5_5_5_REV,
		GL_UNSIGNED_INT_8_8_8_8,
		GL_UNSIGNED_INT_8_8_8_8_REV,
		GL_UNSIGNED_INT_10_10_10_2,
		GL_UNSIGNED_INT_2_10_10_10_REV,
	};

	//--------------------------------------------------------------------------
	// Private Static Variables:
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// Private Variables:
	//--------------------------------------------------------------------------

#define dimensions3_ baseMap_.dimensions3
#define dimensions2_ baseMap_.dimensions2
#define dimensions1_ baseMap_.dimensions1
#define data_ baseMap_.data

	//--------------------------------------------------------------------------
	// Constructors/Destructors:
	//--------------------------------------------------------------------------

#pragma region Constructors

	Texture::Texture(void)
		: NamedObject()
		, id_(ResourceUnbind)
		, format_(Format::RGBA)
		, type_(ChannelType::UByte)
        , baseMap_()
		, mipmaps_()
        , mipmapLimits_(0, 1000)
	{
        BindMipMaps();
	}

	Texture::Texture(const Texture& other)
		: NamedObject(other)
		, id_(ResourceUnbind)
		, format_(other.format_)
		, type_(other.type_)
        , baseMap_(other.baseMap_)
        , mipmaps_(other.mipmaps_)
        , mipmapLimits_(other.mipmapLimits_)
	{
        BindMipMaps();
	}

    Texture::Texture(Texture&& other) noexcept
        : NamedObject(std::move(other))
        , id_(std::exchange(other.id_, ResourceUnbind))
        , format_(std::move(other.format_))
        , type_(std::move(other.type_))
        , baseMap_(std::move(other.baseMap_))
        , mipmaps_(std::move(other.mipmaps_))
        , mipmapLimits_(std::move(other.mipmapLimits_))
    {
        BindMipMaps();
    }

    Texture& Texture::operator=(const Texture& other)
    {
        NamedObject::operator=(other);
        DestroyBuffers();
		format_ = other.format_;
		type_ = other.type_;
        baseMap_ = other.baseMap_;
        mipmaps_ = other.mipmaps_;
        BindMipMaps();
        mipmapLimits_ = other.mipmapLimits_;
        return *this;
    }

    Texture::Texture(const string_t& name, const UVec1& dimensions, Format format, ChannelType type)
        : Texture(name, UVec3(dimensions, 0, 0), format, type)
    {
    }

	Texture::Texture(const string_t& name, const UVec2& dimensions, Format format, ChannelType type)
		: Texture(name, UVec3(dimensions, 0), format, type)
	{
	}

    Texture::Texture(const string_t& name, const UVec3& dimensions, Format format, ChannelType type)
        : NamedObject(name)
        , id_(ResourceUnbind)
        , format_(format)
        , type_(type)
        , baseMap_()
        , mipmaps_()
    {
        BindMipMaps();
        dimensions3_ = dimensions;
        data_ = new unsigned char[GetSize()](0);
    }

    Texture::MipMap::MipMap()
        : texture(nullptr)
        , data(nullptr)
        , dimensions3(0, 0, 0)
        , level(0)
    {
    }

    Texture::MipMap::MipMap(const MipMap& other)
        : texture(nullptr)
        , data(other.IsValid() ? new unsigned char[other.GetSize()](0) : nullptr)
        , dimensions3(other.dimensions3)
        , level(other.level)
    {
        if (IsValid() && other.IsValid())
        {
            memcpy_s(data, GetSize(), other.data, other.GetSize());
        }
    }

    Texture::MipMap::MipMap(MipMap&& other) noexcept
        : texture(nullptr)
        , data(std::exchange(other.data, nullptr))
        , dimensions3(std::move(other.dimensions3))
        , level(std::move(other.level))
    {
    }

    Texture::MipMap& Texture::MipMap::operator=(const MipMap& other)
    {
        dimensions3 = other.dimensions3;
        SafeDeleteArrCheck(data);
        if (other.IsValid())
        {
            data = new unsigned char[GetSize()](0);
            memcpy_s(data, GetSize(), other.data, other.GetSize());
        }
        level = other.level;
        return *this;
    }

	//--------------------------------------------------------------------------

    Texture::MipMap::~MipMap()
    {
        SafeDeleteArrCheck(data);
    }

	Texture::~Texture(void)
	{
        DestroyBuffers();
	}

#pragma endregion Constructors

	//--------------------------------------------------------------------------
	// Public Static Functions:
	//--------------------------------------------------------------------------

#pragma region Public Static Functions

	Texture* Texture::Read(string_view_arg name)
	{
		string_t nameConfirm = string_t(ReadDir) + "/" + string_t(name);
		if (!IOSystem::FindWithoutExtension(nameConfirm, TextureExtensions))
		{
			TraceError("Failed to find texture \"{}\"", nameConfirm);
			return nullptr;
		}

		TraceInfo("Reading texture \"{}\" ({})", name, nameConfirm);

		Texture* tex = new Texture();
		tex->SetName(name);
		tex->ReadFile(nameConfirm);
		tex->Register();
        tex->GenerateMipMaps();

		return tex;
	}

#pragma endregion Public Static Functions

	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

	void Texture::Read(StreamIn& stream)
	{
		ReadName(stream);

		TraceInfo("Reading texture: {}", GetName());

		assert(false && "Reading a texture from a stream is not implemented!");
	}

    bool Texture::IsValid(void) const
    {
        return data_ != nullptr && dimensions2_.x > 0 && dimensions2_.y > 0;
    }

	const UVec2& Texture::GetDimensions(void) const
	{
        return dimensions2_;
	}

    void Texture::SetDimensions(const UVec2& dimensions)
    {
        // TODO: Move data, main/video memory.

        assert(false && "Not implemented!");

        dimensions2_ = dimensions;
    }

    size_t Texture::GetSize(void) const
    {
        return static_cast<size_t>(
            dimensions3_.x *
            std::max(dimensions3_.y, 1u) *
            std::max(dimensions3_.z, 1u) *
            ComponentCounts[(unsigned int)format_]);
    }

    Texture::Format Texture::GetFormat(void) const
    {
        return format_;
    }

    Texture::ChannelType Texture::GetChannelType(void) const
    {
        return type_;
    }

    unsigned char* Texture::Data(void) const
	{
		return data_;
	}

    void Texture::GenerateMipMaps(void)
    {
        assert(IsValid() && "Texture must be valid to generate mipmaps!");

        BindBuffers();
        glGenerateMipmap(GL_TEXTURE_2D);
        OpenGLCheckError();
        glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, &mipmapMin_);
        OpenGLCheckError();
        glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, &mipmapMax_);
        OpenGLCheckError();
        GLint numLevels;
        glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_VIEW_NUM_LEVELS, &numLevels);
        OpenGLCheckError();
        mipmapMax_ = std::bit_width(MathMax(MathMax(dimensions3_.x, dimensions3_.y), dimensions3_.z)) - 1;
        numLevels = mipmapMax_ + 1;
        baseMap_.level = mipmapMin_;
        if (numLevels <= 0)
        {
            // No mipmaps.
            return;
        }
        mipmaps_.resize((size_t)numLevels - 1);
        BindMipMaps();
        for (int level = mipmapMin_ + 1; level <= mipmapMax_; ++level)
        {
            MipMap& mipmap = mipmaps_[(size_t)level - mipmapMin_ - 1];
            mipmap.level = level;
            glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_WIDTH, reinterpret_cast<GLint*>(&mipmap.dimensions3.x));
            OpenGLCheckError();
            glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_HEIGHT, reinterpret_cast<GLint*>(&mipmap.dimensions3.y));
            OpenGLCheckError();
            glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_DEPTH, reinterpret_cast<GLint*>(&mipmap.dimensions3.z));
            OpenGLCheckError();
#ifdef _DEBUG
            ResourceEnum format;
            glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_INTERNAL_FORMAT, reinterpret_cast<GLint*>(&format));
            OpenGLCheckError();
            assert(format == Formats[(unsigned int)format_]);
#endif
            SafeDeleteArrCheck(mipmap.data);
            // glGetTexImage returns data as an RGBA formatted buffer, so the spacing has to be accounted for.
            mipmap.data = new unsigned char[(mipmap.GetSize() / ComponentCounts[(unsigned int)format_]) * 4]();
            glGetTexImage(GL_TEXTURE_2D, level, Formats[(unsigned int)format_], Types[(unsigned int)type_], mipmap.data);
        }
    }

    void Texture::Activate(void) const
    {
        BindBuffers();
        // Mipmap limits
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, (GLint)mipmapMin_);
        OpenGLCheckError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, (GLint)mipmapMax_);
        OpenGLCheckError();
    }

    bool Texture::MipMap::IsValid(void) const
    {
        return data != nullptr && dimensions2.x > 0 && dimensions2.y > 0;
    }

    size_t Texture::MipMap::GetSize(void) const
    {
        return static_cast<size_t>(
            dimensions3.x *
            MathMax(dimensions3.y, 1u) *
            MathMax(dimensions3.z, 1u) *
            ComponentCounts[(unsigned int)texture->format_]);
    }

#pragma endregion Public Functions

	//--------------------------------------------------------------------------
	// Private Static Functions:
	//--------------------------------------------------------------------------

#pragma region Private Static Functions

    void Texture::UnbindBuffers(void)
    {
        glBindTexture(GL_TEXTURE_2D, ResourceUnbind);
        OpenGLCheckError();
    }

#pragma endregion Private Static Functions

	//--------------------------------------------------------------------------
	// Private Functions:
	//--------------------------------------------------------------------------

#pragma region Private Functions

	void Texture::Register(void)
	{
        CreateBuffers();
        BindBuffers();

        assert(data_ != nullptr && "Attempted to register a texture with null data!");

        // Set the filter mode to nearest to prevent warnings about mipmaps.
        glActiveTexture(GL_TEXTURE0);
        OpenGLCheckError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        OpenGLCheckError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        OpenGLCheckError();

        assert(baseMap_.texture == this && "Base map should be bound to parent texture!");

        // Store the texture data
        if (dimensions3_.x <= 0) // 0D
        {
            assert(false && "Invalid texture dimensions while registering!");
        }
        else if (dimensions3_.y <= 0) // 1D
        {
            assert(false && "1D texture registration not implemented!");
        }
        else if (dimensions3_.z <= 0) // 2D
        {
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                static_cast<GLint>(Formats[(unsigned int)format_]),
                static_cast<GLsizei>(dimensions2_.x),
                static_cast<GLsizei>(dimensions2_.y),
                0,
                Formats[(unsigned int)format_],
                Types[(unsigned int)type_],
                data_
            );
        }
        else // 3D
        {
            assert(false && "3D texture registration not implemented!");
        }
        OpenGLCheckError();

        // Unbinding the buffers causes an OpenGL warning on some platforms, and isn't required for anything other than resetting bindings.
        // The warnings usually show up when the framebuffer is cleared.
        //UnbindBuffers();
	}

	void Texture::ReadFile(string_view_arg filename)
	{
		// Load the image data using stbi image
		int componentCount;
        if (data_ != nullptr)
        {
            TraceWarning("Loading file \"{}\" into existing texture \"{}\"!", filename, GetName());
            assert(false);
        }
		data_ = stbi_load(
            filename.data(),
            reinterpret_cast<int*>(&dimensions2_.x),
            reinterpret_cast<int*>(&dimensions2_.y),
            &componentCount,
            0
        );
        if (data_ == nullptr)
        {
            string_view_t reason{ stbi_failure_reason() };
            TraceWarning("Failed to load texture \"{}\" with reason: {}", filename, reason);
            assert(false);
            format_ = Format::Invalid;
            return;
        }
        static constexpr array_t<Format, 4> loadedFormats
        {
            Format::R,
            Format::RG,
            Format::RGB,
            Format::RGBA,
        };
        --componentCount;
        assert((size_t)componentCount < loadedFormats.size());
        format_ = loadedFormats[(unsigned int)componentCount];
        dimensions3_.z = 0;
	}

    void Texture::CreateBuffers(void)
    {
        assert(id_ == ResourceUnbind);
        glGenTextures(1, &id_);
        OpenGLCheckError();
    }

    bool Texture::DestroyBuffers(void)
    {
        bool existed = id_ != ResourceUnbind;
        if (existed)
        {
            glDeleteTextures(1, &id_);
            OpenGLCheckError();
            id_ = ResourceUnbind;
        }
        return existed;
    }

    void Texture::BindBuffers(void) const
    {
        glBindTexture(GL_TEXTURE_2D, id_);
        OpenGLCheckError();
    }

    void Texture::BindMipMaps(void)
    {
        baseMap_.texture = this;
        assert(baseMap_.level == 0);
        for (auto& mipmap : mipmaps_)
        {
            mipmap.texture = this;
            //assert(mipmap.level != 0);
        }
    }

#pragma endregion Private Functions

	//--------------------------------------------------------------------------
	// Library Implementation:
	//--------------------------------------------------------------------------

#pragma region Library Implementation

	template TextureLibrary;

	INSTANTIATE_LIBRARY_IMGUI_SELECTOR(Texture);

	template <> Texture* Library<Texture>::Build(string_view_arg name)
	{
		Texture* texture = name.ends_with(".json") ? BuildDefault(name) : Texture::Read(name);
		return texture;
	}

	IMPLEMENT_READ(Texture);

	IMPLEMENT_WRITE(Texture);

#pragma endregion Library Implementation

}	// namespace
