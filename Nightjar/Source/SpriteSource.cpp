//------------------------------------------------------------------------------
//
// File Name:    SpriteSource.cpp
// Author(s):    Jason Nguyen (jason.nguyen), Jonathan Sandquist (jonathan.s)
// Course:       GAM200F25
// Project:      Card Critters
// Purpose:      Class that holds data for a sprite sheet.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Material.h"
#include "SpriteSource.h"
#include "GraphicsSystem.h"
#include "Texture.h"
#include "Sampler.h"
#include "Mesh.h"
#include "Shader.h"
#include "Trace.h"

#include "Library.inl"
#include "LibraryStatic.inl"

#undef TRACE_CATEGORY
#define TRACE_CATEGORY SPRITESOURCE

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

    constexpr StreamKey
          SpriteSource::SpriteName = "Sprite"
        , SpriteSource::ReadKeyTexture = "Texture"
        , SpriteSource::ReadKeySampler = "Sampler"
        , SpriteSource::ReadKeyRows = "Rows"
        , SpriteSource::ReadKeyColumns = "Columns"
        ;

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

    SpriteSource::SpriteSource()
        : NamedObject()
        , texture_(nullptr)
        , sampler_(GraphicsSystem::GetSampler(SpriteName))
        , mesh_(GraphicsSystem::GetMesh(SpriteName))
        , dimensions_(1, 1)
        , uvSize_(1, 1)
        , frameDuration_(0.1f)
    {
    }

    SpriteSource::SpriteSource(string_view_arg name, const Texture* texture)
        : NamedObject(name)
        , texture_(texture)
        , sampler_(GraphicsSystem::GetSampler(SpriteName))
        , mesh_(GraphicsSystem::GetMesh(SpriteName))
        , dimensions_(1, 1)
        , uvSize_(1, 1)
        , frameDuration_(0.1f)
    {
    }

    //--------------------------------------------------------------------------

    SpriteSource::~SpriteSource()
    {
    }

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

#pragma endregion Public Static Functions

    //--------------------------------------------------------------------------
    // Public Functions:
    //--------------------------------------------------------------------------

#pragma region Public Functions

    void SpriteSource::Read(StreamIn& stream)
    {
        ReadName(stream);

        stream.TryReadValue(ReadKeyTexture, texture_);
        stream.TryReadValue(ReadKeySampler, sampler_);
        stream.TryReadValue(ReadKeyColumns, dimensions_.x);
        stream.TryReadValue(ReadKeyRows, dimensions_.y);

        uvSize_ = 1.0f / Vec2(dimensions_);
    }

    unsigned int SpriteSource::GetFrameCount() const
    {
        return dimensions_.x * dimensions_.y;
    }

    float SpriteSource::GetFrameDuration() const
    {
        return frameDuration_;
    }

    const Mesh* SpriteSource::GetMesh() const
    {
        return mesh_;
    }

    const Texture* SpriteSource::GetTexture() const
    {
        return texture_;
    }

    void SpriteSource::SetTexture(string_view_arg name)
    {
        texture_ = GraphicsSystem::GetTexture(name);
    }

    const Sampler* SpriteSource::GetSampler() const
    {
        return sampler_;
    }

    void SpriteSource::SetSampler(string_view_arg name)
    {
        sampler_ = GraphicsSystem::GetSampler(name);
    }

    void SpriteSource::GetFrameUV(unsigned int frameIndex, Vec2& uv) const
    {
        if (dimensions_.x == 0 || dimensions_.y == 0)
        {
            uv.x = 0;
            uv.y = 0;
        }
        else
        {
            frameIndex %= GetFrameCount();
            Vec2 coords{ frameIndex % dimensions_.x, frameIndex / dimensions_.x };
            uv = coords / Vec2(dimensions_);
        }
    }

    const Vec2& SpriteSource::GetFrameUVSize() const
    {
        return uvSize_;
    }

#pragma endregion Public Functions

    //--------------------------------------------------------------------------
    // Private Static Functions:
    //--------------------------------------------------------------------------

#pragma region Private Static Functions

#pragma endregion Private Static Functions

    //--------------------------------------------------------------------------
    // Private Functions:
    //--------------------------------------------------------------------------

#pragma region Private Functions

#pragma endregion Private Functions

    //--------------------------------------------------------------------------
    // Library Implementation:
    //--------------------------------------------------------------------------

#pragma region Library Implementation

    template SpriteSourceLibrary;

    INSTANTIATE_LIBRARY_IMGUI_SELECTOR(SpriteSource);

    IMPLEMENT_READ(SpriteSource);

    IMPLEMENT_WRITE(SpriteSource);

#pragma endregion Library Implementation

}	// namespace
