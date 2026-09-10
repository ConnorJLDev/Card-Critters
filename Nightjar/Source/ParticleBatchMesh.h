//------------------------------------------------------------------------------
//
// File Name:	ParticleBatchMesh.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class that manages a mesh representing particles.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Mesh.h"
#include "Particle.h"
#include "Material.h"

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
	class ParticleBatchMesh : public Mesh
	{
        friend class ParticleBatch;

		// Public Constants, Enums, and Structs:
	public:

		// Constructors/Destructors:
	public:
        ParticleBatchMesh(void);

		~ParticleBatchMesh(void);

		// Public Static Functions:
	public:
        //
        static void ApplyMeshToMaterial(const Mesh& mesh, Material& material);

		// Public Functions:
	public:
        //
        void SetParticle(const Particle& particle, unsigned index);
        
        //
        void SetParticles(const size_t* particleIndices, unsigned count, const Particle* particles, unsigned index);

        //
        void GetParticle(Particle& particle, unsigned index);

        //
        void Resize(size_t size);

        /// <summary>
        /// Processes any changes that may have happened during the previous frame.
        /// </summary>
        void PreUpdate(void);

        //
        void Update(Material& batchUpdater);

        //
        void Render(const Mat4& transform, Material& batchRenderer);

        // The number of active particles. Used for determining how many primitives to draw.
        unsigned int GetCountParticlesActive(void);

        //
        void SetCountParticlesActive(unsigned int count);

		// Public Event Handlers
	public:

		// Private Constants, Enums, and Structs:
	private:

		// Private Static Functions:
	private:

		// Private Functions:
	private:
        //
        void UpdatePre(void);

        //
        void UpdatePost(void);

		// Private Static Variables:
	private:

		// Private Variables:
	private:
        // Query object used for getting the number of primitives returned when updating.
        ResourceID mainQuery_, swapQuery_;
        // Secondary Vertex Array Object used for swapped Vertex Buffer Objects.
        ResourceID swapVertArray_;
        // The most recent VBO used with the swapped VAO.
        ResourceID swapVertDataLast_;
        // The most recent VBO used with the original VAO.
        ResourceID mainVertDataLast_;
	};

}	// namespace
