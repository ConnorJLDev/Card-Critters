//------------------------------------------------------------------------------
//
// File Name:	ModelData.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Model encapsulator.
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
	class Armature;
	class Mesh;
	class Material;
	class ModelData;

	// Typedefs:
	typedef LibraryStatic<ModelData> ModelLibrary;

	// Class Definition:
	class ModelData : public NamedObject
	{
		// Public Constants, Enums, and Structs:
	public:
		//
		DECLARE_READ_KEY(Model);

		//
		DECLARE_READ_DIR(DIR_DATA"Models");

		// Constructors/Destructors:
	public:
		ModelData(void);
		
		ModelData(const ModelData& other);

		~ModelData(void);

		// Public Static Functions:
	public:

		// Public Functions:
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="stream"></param>
		void Read(StreamIn& stream);

		//
		void Render(const Mat4& transform);

		//
		Armature* GetArmature() const;

		//
		Material* GetMaterial(const string_t& name) const;

		//
		const Mesh* GetMesh(const string_t& name) const;

		//
		void UpdateMatrices();

		//
		void DisplayIMGUI();

		// Public Event Handlers
	public:

		// Private Constants, Enums, and Structs:
	private:
		//
		struct Submesh
		{
			const Mesh* mesh;
			Material* material;
		};

		// Private Static Functions:
	private:

		// Private Functions:
	private:

		// Private Static Variables:
	private:

		// Private Variables:
	private:
		Armature* armature_;
		vector_t<Submesh> submeshes_;
		vector_t<string_t> boneNames_;
		map_t<string_t, Material*> materials_;
		map_t<string_t, const Mesh*> meshes_;
		vector_t<Mat4> boneMtxs_;
		vector_t<Mat4> boneNormalMtxs_;
		bool dirty_;
	};

}	// namespace
