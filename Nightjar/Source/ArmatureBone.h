//------------------------------------------------------------------------------
//
// File Name:	ArmatureBone.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Armature bone encapsulator.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

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
	class Armature;

	// Typedefs:

	// Class Definition:
	class ArmatureBone : public TransformBase<NamedObject, ArmatureBone>
	{
		// Public Constants, Enums, and Structs:
	public:

		// Constructors/Destructors:
	public:
		ArmatureBone(void);

		ArmatureBone(const ArmatureBone& other);

		ArmatureBone& operator=(const ArmatureBone& other) = delete;

		~ArmatureBone(void);

		// Public Static Functions:
	public:
		//
		static ArmatureBone* Read(StreamIn& stream, const Armature* armature);

		// Public Functions:
	public:
		//
		void SetArmature(Armature* armature);
		
		//
		const Vec3& GetTranslation() const;

		//
		void SetTranslation(const Vec3& translation);

		//
		const Vec3& GetRotation() const;

		//
		void SetRotation(const Vec3& rotation);

		//
		const Vec3& GetScale() const;

		//
		void SetScale(const Vec3& scale);

		//
		const Mat4& GetMatrixFinal();

		//
		const Mat3& GetMatrixObjectNormal();

		//
		const Mat4& GetInverseBindMatrix();

		/// <summary>
		/// 
		/// </summary>
		void DisplayIMGUI();

		// Public Event Handlers
	public:

		// Private Constants, Enums, and Structs:
	private:

		// Private Static Functions:
	private:

		// Private Functions:
	private:
		/// <summary>
		/// Sets the inverse bind matrix from the current transform.
		/// </summary>
		/// <returns>The inverse bind matrix.</returns>
		const Mat4& SetInverseBindMatrix();

		// Private Static Variables:
	private:

		// Private Variables:
	private:
		Mat4 mtxBindInverse_;
		Mat3 mtxObjNrm_;
	};

}	// namespace
