//------------------------------------------------------------------------------
//
// File Name:	ParticleBatch.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class describing batches of particles.
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
#include "Particle.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    // Forward Declarations:
    class Texture;
    class Mesh;
    class ParticleBatchMesh;
    class Material;
    class Shader;

    // Typedefs:

    // Class Definition:
    class ParticleBatch : public Object
    {
        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    public:
        ParticleBatch(void);

        ParticleBatch(const ParticleBatch& other);

        ~ParticleBatch(void);

        // Public Static Functions:
    public:

        // Public Functions:
    public:
        //
        Particle& operator[](size_t index);
        
        //
        const Particle& operator[](size_t index) const;

        /// <summary>
        /// Processes any changes that may have happened during the previous frame.
        /// </summary>
        void PreUpdate(void);

        /// <summary>
        /// Update all contained particles.
        /// </summary>
        /// <param name="dt">The time elapsed since the previous tick.</param>
        void Update(float dt, const AnimationSequence* anim, vector_t<float>& animBuffer);

        /// <summary>
        /// Render all contained particles.
        /// </summary>
        void Render(const Mesh* mesh) const;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="num"></param>
        void Emit(unsigned int num, const Particle& archetype);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="mesh"></param>
        /// <param name="material"></param>
        void EnableBatching(const Mesh& mesh, Material& material);

        /// <summary>
        /// 
        /// </summary>
        /// <param name=""></param>
        /// <returns></returns>
        unsigned int GetMaxParticleCount(void) const;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="num"></param>
        void SetMaxParticleCount(size_t num);

        // Public Event Handlers
    public:

        // Private Constants, Enums, and Structs:
    protected:

        // Private Static Functions:
    protected:

        // Private Functions:
    protected:
        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        bool IsBatch() const;

        // Private Static Variables:
    protected:

        // Private Variables:
    protected:
        // Mesh containing buffer of vertex points to instance particles from.
        ParticleBatchMesh* batchMesh_;
        // Shader used for batch updating particles.
        Material* batchMaterial_;
        // Shader used for batch rendering particles.
        Material* batchMaterialRender_;
        // Buffer of active particle pointers. Utilized for quickly swapping expired particles to the end.
        vector_t<size_t> particles_;
        // Buffer of the actual particles.
        vector_t<Particle> particlesRaw_;
        // Number of active particles.
        size_t particlesCount_;
        // Maximum number of particles that can exist at once in this batch.
        size_t particlesMax_;
    };

}	// namespace
