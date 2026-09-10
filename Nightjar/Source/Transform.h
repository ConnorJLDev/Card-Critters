//------------------------------------------------------------------------------
//
// File Name:	Transform.h
// Author(s):	Andrew Lamb (andrew.lamb)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Transform component header.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Component.h"
#include "TransformBase.h"

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
	class Transform : public TransformBase<Component, Transform>
	{
		// Public Constants, Enums, and Structs:
	public:
		// Common component data.
		COMPONENT_COMMON(Transform);

		// Constructors/Destructors:
	public:
		Transform();
	private:
		Transform(const Transform& other);
	public:
		~Transform();

		// Public Static Functions:
	public:

		// Public Functions:
	public:
		/// <summary>
		/// Clone the Transform.
		/// </summary>
		/// <returns>A clone of this Transform.</returns>
		Transform* Clone() const override;

		/// <summary>
		/// Read Transform data from a stream.
		/// </summary>
		/// <param name="stream">The stream to read from.</param>
		void Read(StreamIn& stream) override;

		/// <summary>
		/// Writes the Transform data into the stream.
		/// </summary>
		/// <param name="stream">The stream to write into.</param>
		void Write(StreamOut& stream) override;

		/// <summary>
		/// Update this Transform.
		/// </summary>
		/// <param name="dt">The time elapsed since the previous tick.</param>
		void Update(float dt) override;

		/// <summary>
		/// Gets the world transform matrix.
		/// </summary>
		/// <returns>The matrix representing the world transform.</returns>
		const Mat4& GetMatrix();

		/// <summary>
		/// Gets the 2D translation of this transform.
		/// </summary>
		/// <returns>The 2D translation.</returns>
		const Vec2& GetTranslation() const;

		/// <summary>
		/// Gets the world 2D translation of this transform.
		/// </summary>
		/// <returns>The world 2D translation.</returns>
		Vec2 GetTranslationWorld() const;

		/// <summary>
		/// Sets the 2D translation of this transform.
		/// </summary>
		/// <param name="translation">The new 2D translation for this transform.</param>
		void SetTranslation(const Vec2& translation);

		/// <summary>
		/// Gets the 2D rotation of this transform.
		/// </summary>
		/// <returns>The 2D rotation.</returns>
		float GetRotation() const;

		/// <summary>
		/// Gets the world 2D rotation of this transform.
		/// </summary>
		/// <returns>The world 2D rotation.</returns>
		float GetRotationWorld() const;

		/// <summary>
		/// Gets the 2D rotation of this transform.
		/// </summary>
		/// <param name="rotation">The new 2D rotation for this transform.</param>
		void SetRotation(float rotation);

		/// <summary>
		/// Gets the 2D scale of this transform.
		/// </summary>
		/// <returns>The 2D scale.</returns>
		const Vec2& GetScale() const;

		/// <summary>
		/// Gets the world 2D scale of this transform.
		/// </summary>
		/// <returns>The world 2D scale.</returns>
		Vec2 GetScaleWorld() const;

		/// <summary>
		/// Gets the 2D scale of this transform.
		/// </summary>
		/// <param name="scale">The new 2D scale for this transform.</param>
		void SetScale(const Vec2& scale);

		/// <summary>
		/// Checks if another transform is a child of this transform.
		/// </summary>
		/// <param name="other">The transform to search for.</param>
		/// <returns>True if other is a child of this transform, else false.</returns>
		bool IsChild(const Transform* other) const;

		/// <summary>
		/// Finds a child of this transform by entity name.
		/// </summary>
		/// <param name="name">The name of the entity containing the child.</param>
		/// <returns>The child if it was found, else nullptr.</returns>
		Transform* FindChild(string_view_arg name) const;

		/// <summary>
		/// Checks if the transform was updated recently.
		/// </summary>
		/// <returns>True if the transform was updated recently, false otherwise.</returns>
		bool GetUpdated() const;

		// Public Event Handlers
	public:

		// Private Constants, Enums, and Structs:
	private:

		// Private Static Functions:
	private:

		// Private Functions:
	private:

		// Private Static Variables:
	private:

		// Private Variables:
	private:
		bool mode3D_;
		bool updated_;
		bool updatedLast_;
	};

}	// namespace
