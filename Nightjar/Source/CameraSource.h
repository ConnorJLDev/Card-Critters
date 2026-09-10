//------------------------------------------------------------------------------
//
// File Name:	CameraSource.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class representing a camera.
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
#include "PlatformSystem.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	// Forward Declarations:

	// Typedefs:

	// Class Definition:
	class CameraSource : public Object
	{
        friend class GraphicsSystem_;

		// Public Constants, Enums, and Structs:
	public:

		enum class Mode : unsigned char
		{
			Orthographic,
			Perspective,
			NUM_ENTRIES,
		};

		// Constructors/Destructors:
	public:
		CameraSource(void);

		CameraSource(const CameraSource& other);

		CameraSource(CameraSource&& other) noexcept;

		CameraSource& operator=(const CameraSource& other);

		~CameraSource(void);

		// Public Static Functions:
	public:

		// Public Functions:
	public:

		const Mode& GetMode() const;

		void SetMode(const Mode& mode);

		const bool& GetRespectWindowSize() const;

		void SetRespectWindowSize(const bool& respect);

		const Vec2& GetOrthoSize() const;

		void SetOrthoSize(const Vec2& orthoSize);

		const Vec2& GetClipDistances() const;

		void SetClipDistances(const Vec2& clipDistances);

		const float& GetClipDistanceNear() const;

		void SetClipDistanceNear(const float& clipDistance);

		const float& GetClipDistanceFar() const;

		void SetClipDistanceFar(const float& clipDistance);

		const float& GetPerspectiveFOV() const;

		void SetPerspectiveFOV(const float& fov);

		Vec2 GetViewSize() const;

		const Mat4& GetTransformMatrix() const;

		void SetTransformMatrix(const Mat4& transform);

		const Mat4& GetViewMatrix() const;

		const Mat4& GetProjectionMatrix();

		void DisplayIMGUI();

		// Public Event Handlers
	public:

		// Private Constants, Enums, and Structs:
	protected:

		static const StreamKey ReadKeyMode, ReadKeyPerspectiveFOV, ReadKeyRespectWindowSize, ReadKeyOrthoSize;

		ENUM_MAPS_CLASS_SEQUENTIAL_DECLARE(Mode);

		// Private Static Functions:
	protected:

		// Private Functions:
	protected:

		// Private Static Variables:
	protected:

		// Private Variables:
	protected:
		Mode mode_;
		bool respectWindowSize_;
		bool projectionDirty_;
		CallbackID resizeCallback_;
		Vec2 orthoSize_;
		union
		{
			Vec2 clipDistances_;
			struct
			{
				float clipDistanceNear_;
				float clipDistanceFar_;
			};
		};
		float perspFOV_;
		Mat4 view_, projection_, transform_;
	};

}	// namespace
