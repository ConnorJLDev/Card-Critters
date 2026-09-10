//------------------------------------------------------------------------------
//
// File Name:	 Scene.cpp
// Author(s):	 Doug Schilling (dschilling)
// Course:       GAM200F25
// Project:      Card Critters
// Purpose:      Class representing a Scene.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "IOSystem.h"
#include "IOSystem.inl"
#include "Entity.h"
#include "EntityContainer.h"
#include "Scene.h"
#include "Camera.h"
#include "GraphicsSystem.h"
#include "InputSystem.h"

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
		Scene::ReadKeyEntities = "Entities";

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

	Scene::Scene(void)
		: NamedObject()
		, entities_(new EntityContainer())
		, camera_(nullptr)
		, pointingPos_()
		, pointingRayPos_()
		, pointingRayDir_()
		, pointingPosDirty_(true)
		, pointingRayPosDirty_(true)
		, pointingRayDirDirty_(true)
		, dtMultiplier_(1.0f)
	{
		assert(entities_ != nullptr);
		entities_->scene_ = this;
	}

	//--------------------------------------------------------------------------

	Scene::~Scene(void)
	{
		SafeDeleteCheck(entities_);
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

	Entity* Scene::FindEntity(string_view_arg name) const
	{
		return entities_->Find(name);
	}

	EntityContainer* Scene::FindContainer() const
	{
		return entities_;
	}

	void Scene::Read(const string_t& filepath)
	{
		bool success;
		StreamIn stream = IOSystem::OpenStreamIn<Scene>(filepath, success);
		assert(success);

		SetName(filepath);

		assert(stream.StreamHas(ReadKeyEntities));

		stream.ReadArrayObjects(ReadKeyEntities, [this](StreamIn& stream, int index)
			{
				UNREFERENCED_PARAMETER(index);

				Entity* entity = Entity::TryRead(stream);
				if (entity != nullptr)
				{
					entities_->Add(entity);
				}
			});

		stream.TryReadValue("dtMultiplier", dtMultiplier_);
	}

	void Scene::Write(const string_t& filepath) const
	{
		string_t filePath = string_t(ReadDir) + "/" + filepath + ".json";

		StreamOut stream(filePath);

		assert(stream.IsValid());

		stream.WriteObjectStart(ReadKey);
		// Write entities.
		stream.WriteArrayStart(ReadKeyEntities);
		EntityContainer* entitiesContainer = FindContainer(); //grabs container
		for (unsigned int i = 0; i < entitiesContainer->Count(); ++i)  //loops through all entities
		{
			Entity* entity = entitiesContainer->GetFromIndex(i);
			entity->Write(stream);
		}
		stream.WriteArrayEnd();

		stream.WriteObjectEnd();

		stream.Write();
	}

	Camera* Scene::GetCamera() const
	{
		assert(camera_ != nullptr && "Entity with Camera component should be the first entity in a given scene!");
		return camera_;
	}

	void Scene::SetCamera(Camera* camera)
	{
		if (camera_ != nullptr)
		{
			TraceWarning("Multiple cameras active!");
			assert(false);
		}
		camera_ = camera;
		pointingPosDirty_ = true;
		pointingRayPosDirty_ = true;
		pointingRayDirDirty_ = true;
	}

	const Vec2& Scene::GetPointingPosition()
	{
		if (pointingPosDirty_)
		{
			GetCamera();

			Vec4 mousePos4{ InputSystem::GetMousePos() * camera_->GetViewSize() * 0.5f, 0, 1 };

			const Mat4& mtx = camera_->GetTransformMatrix();

			mousePos4 = mtx * mousePos4;

			pointingPos_ = (Vec2)mousePos4;
			pointingPosDirty_ = false;
		}
		return pointingPos_;
	}

	const Vec3& Scene::GetPointingRayPosition()
	{
		if (pointingRayPosDirty_)
		{
			GetCamera();

			const Mat4& mtx = camera_->GetTransformMatrix();

			pointingRayPos_ = Vec3(mtx[3][0], mtx[3][1], mtx[3][2]);
			pointingRayPosDirty_ = false;
		}
		return pointingRayPos_;
	}

	const Vec3& Scene::GetPointingRayDirection()
	{
		if (pointingRayDirDirty_)
		{
			GetCamera();

			Vec4 camNear{ InputSystem::GetMousePos() * camera_->GetViewSize() * 0.5f, 0, 1 };
			Vec4 camFar{ camNear.x, camNear.y, -1, 1 };

			Mat4 mtx = camera_->GetViewMatrix() * camera_->GetProjectionMatrix();

			mtx = glm::inverse(mtx);

			camNear = mtx * camNear;
			camFar = mtx * camFar;

			Vec4 camDir4 = camFar - camNear;

			pointingRayDir_ = glm::normalize((Vec3)camDir4);
			pointingRayDirDirty_ = false;
		}
		return pointingRayDir_;
	}

	void Scene::SetDtMultiplier(float multiplier)
	{
		dtMultiplier_ = multiplier;
	}

	float Scene::GetDtMultiplier()
	{
		return dtMultiplier_;
	}

	void Scene::Load() const
	{
		TraceInfo("{}: Scene Load", GetName());
	}

	void Scene::Init() const
	{
		TraceInfo("{}: Scene Init", GetName());
	}

	void Scene::Update(float& dt)
	{
		TraceVerbose("{}: Scene Update", GetName());

		camera_ = nullptr;

		// Execute the Update function.
		entities_->UpdateAll(dt);

		dt *= dtMultiplier_;

		// Check collisions
		entities_->CheckCollisions();
	}

	void Scene::Render() const
	{
		if (camera_ == nullptr)
		{
			TraceError("{}: No active camera to render scene!", GetName());
			return;
		}

		TraceVerbose("{}: Scene Render", GetName());

		GraphicsSystem::SetActiveCamera(camera_);

		entities_->RenderAll();
	}

	void Scene::Exit() const
	{
		TraceInfo("{}: Scene Exit", GetName());

		entities_->FreeAll();
	}

	void Scene::Unload() const
	{
		TraceInfo("{}: Unload", GetName());

		entities_->FreeAll();
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
