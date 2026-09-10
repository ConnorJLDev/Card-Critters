//------------------------------------------------------------------------------
//
// File Name:	Particle.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class describing particle behavior.
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

#include "TransformBase.h"
#include "PhysicsBase.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	// Forward Declarations:
	class Mesh;
	class Material;
	class Shader;
	class AnimationSequence;
	class ParticleEmitter;

	// Typedefs:

	// Class Definition:
	class Particle : public Object, public TransformInheritance, public PhysicsBase
	{
		friend class ParticleEmitter;

		// Public Constants, Enums, and Structs:
	public:

		// Constructors/Destructors:
	public:
		Particle(void);

		Particle(const Particle& other);

		Particle(Particle&& other) noexcept;

		Particle& operator=(const Particle& other);

		~Particle(void);

		// Public Static Functions:
	public:

		// Public Functions:
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="dt"></param>
		/// <param name="anim"></param>
		/// <param name="animBuffer"></param>
		void Update(float dt, const AnimationSequence* anim, vector_t<float>& animBuffer);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="mesh"></param>
		void Render(const Mesh* mesh) const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		Material* GetMaterial(void) const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		bool Expired(void) const;

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        float GetLifeTime(void) const;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="seed"></param>
        void SetLifeTime(float lifeTime);

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        unsigned int GetRandomSeed(void) const;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="seed"></param>
        void SetRandomSeed(unsigned int seed);

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
        //
		ParticleEmitter* emitter_;
        //
		Material* material_;
        //
		float lifetimeRemaining_;
        //
        unsigned int randSeed_;
	};

}	// namespace
