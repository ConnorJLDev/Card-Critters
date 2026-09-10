//------------------------------------------------------------------------------
//
// File Name:	ParticleEmitter.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Particle Emitter component.
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
#include "Particle.h"
#include "ParticleBatch.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    // Forward Declarations:
    class Transform;
    class Texture;
    class Mesh;
    class Material;
    class Shader;
    class Particle;
    class AnimationSequence;

    // Typedefs:

    // Class Definition:
    class ParticleEmitter : public Component, public ParticleBatch
    {
        friend class Particle;

        // Public Constants, Enums, and Structs:
    public:
        // Common component data.
        COMPONENT_COMMON(ParticleEmitter);

        // Constructors/Destructors:
    public:
        ParticleEmitter(void);
    private:
        ParticleEmitter(const ParticleEmitter& other);
    public:
        ~ParticleEmitter(void);

        // Public Static Functions:
    public:

        // Public Functions:
    public:
        /// <summary>
        /// Clone the ParticleEmitter.
        /// </summary>
        /// <returns>A clone of this ParticleEmitter.</returns>
        ParticleEmitter* Clone() const override;
        
        /// <summary>
        /// Read ParticleEmitter data from a stream.
        /// </summary>
        /// <param name="stream">The stream to read from.</param>
        void Read(StreamIn& stream) override;

        /// <summary>
        /// Writes the ParticleEmitter data into the stream.
        /// </summary>
        /// <param name="stream">The stream to write into.</param>
        void Write(StreamOut& stream) override;

        /// <summary>
        /// Update this ParticleEmitter.
        /// </summary>
        /// <param name="dt">The time elapsed since the previous tick.</param>
        void Update(float dt) override;
        
        /// <summary>
        /// Render this ParticleEmitter.
        /// </summary>
        void Render() const override;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="num"></param>
        void Emit(unsigned int num);

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        const Mesh* GetMesh(void) const;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="mesh"></param>
        void SetMesh(const Mesh* mesh);

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        Material* GetMaterial(void) const;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="material"></param>
        void SetMaterial(Material* material);

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        bool GetIsWorldSpace(void) const;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="worldSpace"></param>
        void SetIsWorldSpace(bool worldSpace);

        // Public Event Handlers
    public:

        // Private Constants, Enums, and Structs:
    private:

        // Private Static Functions:
    private:

        // Private Functions:
    private:
        /// <summary>
        /// 
        /// </summary>
        /// <param name="num"></param>
        /// <param name="base"></param>
        /// <param name="interval"></param>
        void EmitInterval(unsigned int num, float base = 0.0f, float interval = 0.0f);

        // Private Static Variables:
    private:

        // Private Variables:
    private:
        // Mesh that each particle will be instanced to.
        const Mesh* mesh_;
        // Particle to copy
        Particle particleArchetype_;
        // Particle to copy
        Particle particleArchetypeRandom_;
        //
        const AnimationSequence* anim_;
        //
        vector_t<float> animBuffer_;
        //
        Transform* transform_;
        //
        float speed_;
        //
        float emitInterval_;
        //
        float emitIntervalCurrent_;
        //
        bool emitIntervalActive_;
        //
        bool isWorldSpace_;
    };

}	// namespace
