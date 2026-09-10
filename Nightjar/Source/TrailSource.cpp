//------------------------------------------------------------------------------
//
// File Name:	TrailSource.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class for managing a visual trail.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "TrailSource.h"
#include "Mesh.h"

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

	TrailSource::TrailSource(void)
		: Object()
        , segments_()
        , shell_(nullptr)
        , lines_(nullptr)
        , material_(nullptr)
        , subdivisionCount_(0)
        , slerp_(false)
	{
	}

	TrailSource::TrailSource(const TrailSource& other)
		: Object(other)
        , segments_(other.segments_)
        , shell_(other.shell_)
        , lines_(GraphicsSystem::BuildMesh(other.lines_))
        , material_(GraphicsSystem::BuildMaterial(other.material_))
        , subdivisionCount_(other.subdivisionCount_)
        , slerp_(other.slerp_)
	{
	}

	TrailSource::TrailSource(TrailSource&& other) noexcept
		: Object(std::move(other))
        , segments_(std::move(other.segments_))
        , shell_(std::move(other.shell_))
        , lines_(std::move(other.lines_))
        , material_(std::move(other.material_))
        , subdivisionCount_(std::move(other.subdivisionCount_))
        , slerp_(std::move(other.slerp_))
	{
	}

	TrailSource& TrailSource::operator=(const TrailSource& other)
	{
		Object::operator=(other);
        segments_ = other.segments_;
        shell_ = other.shell_;
        lines_ = other.lines_;
        material_ = other.material_;
        subdivisionCount_ = other.subdivisionCount_;
        slerp_ = other.slerp_;
		return *this;
	}

	//--------------------------------------------------------------------------

	TrailSource::~TrailSource(void)
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

    void TrailSource::AddSegment(void)
    {
        // Subdivision should be handled here.
        segments_.emplace_back();
    }

    const Mesh* TrailSource::GetShell(void) const
    {
        return shell_;
    }

    void TrailSource::SetShell(string_view_arg shellName)
    {
        shell_ = GraphicsSystem::GetMesh(shellName);
    }

    void TrailSource::SetShell(const Mesh* shell)
    {
        shell_ = shell;
    }

    const Material* TrailSource::GetMaterial(void) const
    {
        return material_;
    }

    void TrailSource::SetMaterial(string_view_arg materialName)
    {
        material_ = GraphicsSystem::BuildMaterial(materialName);
    }

    void TrailSource::SetMaterial(const Material* material)
    {
        material_ = GraphicsSystem::BuildMaterial(material);
    }

    bool TrailSource::GetSlerp(void) const
    {
        return slerp_;
    }

    void TrailSource::SetSlerp(bool slerp)
    {
        slerp_ = slerp;
    }

    void TrailSource::Render(const Mat4& transform) const
    {
        GraphicsSystem::DrawCall(transform, *lines_, *material_);
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

}	// namespace
