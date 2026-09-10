//------------------------------------------------------------------------------
//
// File Name:	Armature.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Armature encapsulator.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ArmatureBone.h"

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
	typedef LibraryStatic<Armature> ArmatureLibrary;

	// Class Definition:
	class Armature : public NamedObject
	{
		friend class ModelData;

		// Public Constants, Enums, and Structs:
	public:
		//
		DECLARE_READ_KEY(Armature);

		//
		DECLARE_READ_DIR(DIR_DATA"Armatures");

		// Constructors/Destructors:
	public:
		Armature(void);

		Armature(const Armature& other);

		~Armature(void);

		// Public Static Functions:
	public:

		// Public Functions:
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="stream"></param>
		void Read(StreamIn& stream);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		ArmatureBone* GetBone(const string_t& name) const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		ArmatureBone* GetRootBone() const;

		/// <summary>
		/// 
		/// </summary>
		void DisplayIMGUI();

		// Public Event Handlers
	public:

		// Private Constants, Enums, and Structs:
	private:
		//
		static const StreamKey ReadKeyBones;

		// Private Static Functions:
	private:

		// Private Functions:
	private:

		// Private Static Variables:
	private:

		// Private Variables:
	private:
		map_t<string_t, ArmatureBone*> bones_;
	};

}	// namespace
