//------------------------------------------------------------------------------
//
// File Name:	Camera.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Component for handling a camera.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Component.h"
#include "Camera.h"
#include "Transform.h"
#include "Entity.h"
#include "Scene.h"

#include "GraphicsSystem.h"
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

	Camera::Camera(void)
		: Component(TypeEnum::Camera)
		, CameraSource()
	{
	}

	Camera::Camera(const Camera& other)
		: Component(other)
		, CameraSource(other)
	{
	}

	//--------------------------------------------------------------------------

	Camera::~Camera(void)
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

	Camera* Camera::Clone(void) const
	{
		return new Camera(*this);
	}

	void Camera::Read(StreamIn& stream)
	{
		stream.TryReadValue(ReadKeyRespectWindowSize, respectWindowSize_);
		stream.TryReadValue(ReadKeyOrthoSize, orthoSize_);
		if (stream.TryReadValue(ReadKeyPerspectiveFOV, perspFOV_))
		{
			perspFOV_ = glm::radians(perspFOV_);
		}
		stream.TryReadEnum(ReadKeyMode, ENUM_READ(Mode), mode_);
		projectionDirty_ = true;
	}

	void Camera::Write(StreamOut& stream)
	{
		stream.WriteValue(ReadKeyRespectWindowSize, respectWindowSize_);
		stream.WriteValue(ReadKeyOrthoSize, orthoSize_);
		stream.WriteValue(ReadKeyPerspectiveFOV, glm::degrees(perspFOV_));
		stream.WriteEnum(ReadKeyMode, ENUM_WRITE(Mode), mode_);
	}

	void Camera::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		GetParent()->GetScene()->SetCamera(this);

		Transform* tfm = EntityHas(Transform);
		assert(tfm != nullptr);

		if (tfm->GetUpdated())
		{
			SetTransformMatrix(tfm->GetMatrix());
		}

		GetProjectionMatrix();
	}

	void Camera::Render() const
	{
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
