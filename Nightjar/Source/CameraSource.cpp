//------------------------------------------------------------------------------
//
// File Name:	CameraSource.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class representing a camera.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "CameraSource.h"
#include "PlatformSystem.h"

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
		  CameraSource::ReadKeyMode = "Mode"
		, CameraSource::ReadKeyPerspectiveFOV = "PerspectiveFOV"
		, CameraSource::ReadKeyRespectWindowSize = "RespectWindowSize"
		, CameraSource::ReadKeyOrthoSize = "OrthoSize"
		;

	ENUM_MAPS_CLASS_SEQUENTIAL(CameraSource, Mode, Orthographic, Perspective);

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

	CameraSource::CameraSource(void)
		: Object()
		, mode_(Mode::Orthographic)
		, respectWindowSize_(false)
		, projectionDirty_(true)
		, resizeCallback_()
		, orthoSize_(1920, 1080)
		, clipDistances_(0.1f, 10000.0f)
		, perspFOV_(45.0f)
		, view_()
		, projection_()
		, transform_()
	{
		resizeCallback_ = PlatformSystem::ViewportResizeCallbackAdd([this](const IVec2&)
		{
			projectionDirty_ = true;
		});
	}

	CameraSource::CameraSource(const CameraSource& other)
		: Object(other)
		, mode_(other.mode_)
		, respectWindowSize_(other.respectWindowSize_)
		, projectionDirty_(true)
		, resizeCallback_()
		, orthoSize_(other.orthoSize_)
		, clipDistances_(other.clipDistances_)
		, perspFOV_(other.perspFOV_)
		, view_(other.view_)
		, projection_()
		, transform_(other.transform_)
	{
		PlatformSystem::ViewportResizeCallbackAdd([this](const IVec2&)
		{
			projectionDirty_ = true;
		});
	}

	CameraSource::CameraSource(CameraSource&& other) noexcept
		: Object(std::move(other))
		, mode_(std::move(other.mode_))
		, respectWindowSize_(std::move(other.respectWindowSize_))
		, projectionDirty_(true)
		, resizeCallback_(std::exchange(other.resizeCallback_, ResourceID()))
		, orthoSize_(std::move(other.orthoSize_))
		, clipDistances_(std::move(other.clipDistances_))
		, perspFOV_(std::move(other.perspFOV_))
		, view_(std::move(other.view_))
		, projection_()
		, transform_(std::move(other.transform_))
	{
	}

	CameraSource& CameraSource::operator=(const CameraSource& other)
	{
		mode_ = other.mode_;
		respectWindowSize_ = other.respectWindowSize_;
		projectionDirty_ = true;
		orthoSize_ = other.orthoSize_;
		clipDistances_ = other.clipDistances_;
		perspFOV_ = other.perspFOV_;
		view_ = other.view_;
		transform_ = other.transform_;
		return *this;
	}

	//--------------------------------------------------------------------------

	CameraSource::~CameraSource(void)
	{
		PlatformSystem::ViewportResizeCallbackRemove(resizeCallback_);
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

	const CameraSource::Mode& CameraSource::GetMode() const
	{
		return mode_;
	}

	void CameraSource::SetMode(const Mode& mode)
	{
		mode_ = mode;
		projectionDirty_ = true;
	}

	const bool& CameraSource::GetRespectWindowSize() const
	{
		return respectWindowSize_;
	}

	void CameraSource::SetRespectWindowSize(const bool& respect)
	{
		respectWindowSize_ = respect;
		projectionDirty_ = true;
	}

	const Vec2& CameraSource::GetOrthoSize() const
	{
		return orthoSize_;
	}

	void CameraSource::SetOrthoSize(const Vec2& orthoSize)
	{
		orthoSize_ = orthoSize;
		projectionDirty_ = true;
	}

	const Vec2& CameraSource::GetClipDistances() const
	{
		return clipDistances_;
	}

	void CameraSource::SetClipDistances(const Vec2& clipDistances)
	{
		clipDistances_ = clipDistances;
		projectionDirty_ = true;
	}

	const float& CameraSource::GetClipDistanceNear() const
	{
		return clipDistanceNear_;
	}

	void CameraSource::SetClipDistanceNear(const float& clipDistance)
	{
		clipDistanceNear_ = clipDistance;
		projectionDirty_ = true;
	}

	const float& CameraSource::GetClipDistanceFar() const
	{
		return clipDistanceFar_;
	}

	void CameraSource::SetClipDistanceFar(const float& clipDistance)
	{
		clipDistanceFar_ = clipDistance;
		projectionDirty_ = true;
	}

	const float& CameraSource::GetPerspectiveFOV() const
	{
		return perspFOV_;
	}

	void CameraSource::SetPerspectiveFOV(const float& fov)
	{
		perspFOV_ = fov;
		projectionDirty_ = true;
	}

	Vec2 CameraSource::GetViewSize() const
	{
		return respectWindowSize_ ? orthoSize_ * (Vec2)PlatformSystem::GetWindowSize() : orthoSize_;
	}

	const Mat4& CameraSource::GetViewMatrix() const
	{
		return view_;
	}

	const Mat4& CameraSource::GetTransformMatrix() const
	{
		return transform_;
	}

	void CameraSource::SetTransformMatrix(const Mat4& transform)
	{
		transform_ = transform;
		view_ = glm::inverse(transform);
	}

	const Mat4& CameraSource::GetProjectionMatrix()
	{
		if (projectionDirty_)
		{
			switch (mode_)
			{
			case Mode::Orthographic:
			{
				// Create an orthogonal projection matrix
				Vec2 orthoSizeHalf = GetViewSize() * 0.5f;
				projection_ = glm::ortho(-orthoSizeHalf.x, orthoSizeHalf.x, -orthoSizeHalf.y, orthoSizeHalf.y);
			}
			break;
			case Mode::Perspective:
			{
				// Create a perspective projection matrix
				const Vec2 window(GetViewSize());
				projection_ = glm::perspective(perspFOV_, window.x / window.y, clipDistanceNear_, clipDistanceFar_);
			}
			break;
			default:
				assert(false);
				break;
			}
			projectionDirty_ = false;
		}
		return projection_;
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
