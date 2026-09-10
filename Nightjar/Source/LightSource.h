//------------------------------------------------------------------------------
//
// File Name:	LightSource.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class representing a light source.
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
#include "Shader.h"

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
	class LightSource : public Object
	{
		friend class GraphicsSystem_;
		friend class Light;

		// Public Constants, Enums, and Structs:
	public:

		// Constructors/Destructors:
	public:
		LightSource(void);

		LightSource(const LightSource& other);

		~LightSource(void);

		// Public Static Functions:
	public:
		//
		static void SetAmbientLight(const Vec4& color);

		// Public Functions:
	public:
		//
		void UpdateGraphics();

		//
		bool DisplayIMGUIIndex();

		//
		bool DisplayIMGUIColor();

		//
		bool DisplayIMGUITransform();

		//
		bool DisplayIMGUIAttenuation();

		// Public Event Handlers
	public:

		// Private Constants, Enums, and Structs:
	private:

		// Private Static Functions:
	private:
		//
		static void Init();
		
		//
		static void Exit();

		//
		static void BindGlobalBuffer(const Shader::UniformBlockInfoGlobal* ubo);

		//
		static const Shader::UniformBlockInfoGlobal* GetUBO();

		// Private Functions:
	private:

		// Private Static Variables:
	private:
		//
		static ResourceID lightingBuffer_;

		//
		//static vector_t<LightSource*> globalLights_;

		// Private Variables:
	private:
		Vec4 color_;
		Vec3 pos_;
		Vec3 normal_;
		Vec3 attenuationDistance_;
		Vec3 attenuationAngle_;
		int index_;
	};

}	// namespace
