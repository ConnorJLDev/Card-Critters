//------------------------------------------------------------------------------
//
// File Name:	 Scene.h
// Author(s):	 Doug Schilling (dschilling)
// Course:       GAM200F25
// Project:      Card Critters
// Purpose:      Class representing a Scene.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	// Forward Declarations:
	class Entity;
	class EntityContainer;
	class Camera;

	// Typedefs:

	// Class Definition:
	class Scene : public NamedObject
	{
		friend class SceneSystem_;

		// Public Constants, Enums, and Structs:
	public:
		DECLARE_READ_KEY(Scene);
		DECLARE_READ_DIR(DIR_DATA"Scenes");

		// Constructors/Destructors:
	public:
		Scene(void);

		~Scene(void);

		// Public Static Functions:
	public:

		// Public Functions:
	public:
		/// <summary>
		/// 
		/// </summary>
		void Load() const;

		/// <summary>
		/// 
		/// </summary>
		void Init() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="dt"></param>
		void Update(float& dt);

		/// <summary>
		/// 
		/// </summary>
		void Render() const;

		/// <summary>
		/// 
		/// </summary>
		void Exit() const;

		/// <summary>
		/// 
		/// </summary>
		void Unload() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		Entity* FindEntity(string_view_arg name) const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		EntityContainer* FindContainer() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="filepath"></param>
		void Read(const string_t& filepath);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="filepath"></param>
		void Write(const string_t& filepath) const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		Camera* GetCamera() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="camera"></param>
		void SetCamera(Camera* camera);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const Vec2& GetPointingPosition();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const Vec3& GetPointingRayPosition();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const Vec3& GetPointingRayDirection();

		void SetDtMultiplier(float multiplier);

		float GetDtMultiplier();

		// Public Event Handlers
	public:

		// Private Constants, Enums, and Structs:
	private:
		static const StreamKey ReadKeyEntities;

		// Private Static Functions:
	private:

		// Private Functions:
	private:
		
		// Private Static Variables:
	private:

		// Private Variables:
	private:
		EntityContainer* entities_;
		Camera* camera_;
		Vec2 pointingPos_;
		Vec3 pointingRayPos_;
		Vec3 pointingRayDir_;
		bool pointingPosDirty_;
		bool pointingRayPosDirty_;
		bool pointingRayDirDirty_;
		float dtMultiplier_;
	};

}	// namespace
