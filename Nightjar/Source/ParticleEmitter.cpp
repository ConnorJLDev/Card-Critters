//------------------------------------------------------------------------------
//
// File Name:	ParticleEmitter.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Particle Emitter component.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Component.h"
#include "ParticleEmitter.h"
#include "Particle.h"
#include "Transform.h"
#include "Entity.h"
#include "Shader.h"
#include "Material.h"
#include "Mesh.h"
#include "ParticleBatchMesh.h"
#include "Texture.h"
#include "AnimationSequence.h"
#include "GraphicsSystem.h"
#include "Random.h"

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

    static constexpr StreamKey
          ReadKeyMaterial = "Material"
        , ReadKeyMesh = "Mesh"
        //, ReadKeyBatchMesh = "BatchMesh"
        , ReadKeyBatchMaterial = "BatchMaterial"
        , ReadKeyBatchMaterialRender = "BatchMaterialRender"
        , ReadKeyMaxParticleCount = "MaxParticleCount"
        , ReadKeyAnimation = "Animation"
        , ReadKeySpeed = "Speed"
        , ReadKeyIsWorldSpace = "IsWorldSpace"
        , ReadKeyEmitIntervalActive = "EmitIntervalActive"
        , ReadKeyEmitInterval = "EmitInterval"
        , ReadKeyStartingLifetime = "StartingLifetime"
        , ReadKeyStartingTranslation = "StartingTranslation"
        , ReadKeyStartingRotation = "StartingRotation"
        , ReadKeyStartingScale = "StartingScale"
        , ReadKeyStartingVelocity = "StartingVelocity"
        , ReadKeyStartingAcceleration = "StartingAcceleration"
        , ReadKeyStartingRotationalVelocity = "StartingRotationalVelocity"
        , ReadKeyStartingDrag = "StartingDrag"
        , ReadKeyStartingRotationalDrag = "StartingRotationalDrag"
        , ReadKeyRandomLifetime = "RandomLifetime"
        , ReadKeyRandomTranslation = "RandomTranslation"
        , ReadKeyRandomRotation = "RandomRotation"
        , ReadKeyRandomScale = "RandomScale"
        , ReadKeyRandomVelocity = "RandomVelocity"
        , ReadKeyRandomAcceleration = "RandomAcceleration"
        , ReadKeyRandomRotationalVelocity = "RandomRotationalVelocity"
        , ReadKeyRandomDrag = "RandomDrag"
        , ReadKeyRandomRotationalDrag = "RandomRotationalDrag"
        ;

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

    ParticleEmitter::ParticleEmitter(void)
        : Component(TypeEnum::ParticleEmitter)
        , ParticleBatch()
        , mesh_(nullptr)
        , particleArchetype_()
        , particleArchetypeRandom_()
        , anim_(nullptr)
        , animBuffer_()
        , transform_(nullptr)
        , speed_(1.0f)
        , emitInterval_(1.0f)
        , emitIntervalCurrent_(emitInterval_)
        , emitIntervalActive_(true)
        , isWorldSpace_(false)
    {
        particleArchetype_.emitter_ = this;
    }

    ParticleEmitter::ParticleEmitter(const ParticleEmitter& other)
        : Component(other)
        , ParticleBatch(other)
        , mesh_(other.mesh_)
        , particleArchetype_(other.particleArchetype_)
        , particleArchetypeRandom_(other.particleArchetypeRandom_)
        , anim_(other.anim_)
        , animBuffer_()
        , transform_(nullptr)
        , speed_(other.speed_)
        , emitInterval_(other.emitInterval_)
        , emitIntervalCurrent_(emitInterval_)
        , emitIntervalActive_(other.emitIntervalActive_)
        , isWorldSpace_(other.isWorldSpace_)
    {
        particleArchetype_.emitter_ = this;
        particleArchetype_.material_ = GraphicsSystem::BuildMaterial(other.particleArchetype_.material_);
        if (mesh_ != nullptr && batchMaterialRender_ != nullptr)
        {
            EnableBatching(*mesh_, *batchMaterialRender_);
        }
    }

    //--------------------------------------------------------------------------

    ParticleEmitter::~ParticleEmitter(void)
    {
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

    ParticleEmitter* ParticleEmitter::Clone(void) const
    {
        return new ParticleEmitter(*this);
    }

    void ParticleEmitter::Read(StreamIn& stream)
    {
        stream.TryReadValue(ReadKeyMesh, mesh_);
        stream.TryReadValue(ReadKeyMaterial, particleArchetype_.material_);
        stream.TryReadValue(ReadKeyBatchMaterialRender, batchMaterialRender_);
        if (stream.TryReadValue(ReadKeyMaxParticleCount, particlesMax_))
        {
            SetMaxParticleCount(particlesMax_);
        }
        //stream.TryReadValue(ReadKeyBatchMesh, batchMesh_);
        if (stream.TryReadValue(ReadKeyBatchMaterial, batchMaterial_) && mesh_ != nullptr && batchMaterialRender_ != nullptr)
        {
            //std::swap(batchMaterialRender_, particleArchetype_.material_);
            EnableBatching(*mesh_, *batchMaterialRender_);
        }
        stream.TryReadValue(ReadKeyAnimation, anim_);
        stream.TryReadValue(ReadKeySpeed, speed_);
        stream.TryReadValue(ReadKeyIsWorldSpace, isWorldSpace_);
        stream.TryReadValue(ReadKeyEmitIntervalActive, emitIntervalActive_);
        if (stream.TryReadValue(ReadKeyEmitInterval, emitInterval_))
        {
            emitIntervalCurrent_ = emitInterval_;
        }
        stream.TryReadValue(ReadKeyStartingLifetime, particleArchetype_.lifetimeRemaining_);
        stream.TryReadValue(ReadKeyStartingTranslation, particleArchetype_.translation3_);
        stream.TryReadValue(ReadKeyStartingRotation, particleArchetype_.rotation3_);
        stream.TryReadValue(ReadKeyStartingScale, particleArchetype_.scale3_);
        stream.TryReadValue(ReadKeyStartingVelocity, particleArchetype_.velocity3_);
        stream.TryReadValue(ReadKeyStartingAcceleration, particleArchetype_.acceleration3_);
        stream.TryReadValue(ReadKeyStartingRotationalVelocity, particleArchetype_.rotationalVelocity3_);
        stream.TryReadValue(ReadKeyStartingDrag, particleArchetype_.drag_);
        stream.TryReadValue(ReadKeyStartingRotationalDrag, particleArchetype_.rotationalDrag_);
        stream.TryReadValue(ReadKeyRandomLifetime, particleArchetypeRandom_.lifetimeRemaining_);
        stream.TryReadValue(ReadKeyRandomTranslation, particleArchetypeRandom_.translation3_);
        stream.TryReadValue(ReadKeyRandomRotation, particleArchetypeRandom_.rotation3_);
        stream.TryReadValue(ReadKeyRandomScale, particleArchetypeRandom_.scale3_);
        stream.TryReadValue(ReadKeyRandomVelocity, particleArchetypeRandom_.velocity3_);
        stream.TryReadValue(ReadKeyRandomAcceleration, particleArchetypeRandom_.acceleration3_);
        stream.TryReadValue(ReadKeyRandomRotationalVelocity, particleArchetypeRandom_.rotationalVelocity3_);
        stream.TryReadValue(ReadKeyRandomDrag, particleArchetypeRandom_.drag_);
        stream.TryReadValue(ReadKeyRandomRotationalDrag, particleArchetypeRandom_.rotationalDrag_);
    }

    void ParticleEmitter::Write(StreamOut& stream)
    {
        stream.WriteValue(ReadKeyMesh, mesh_);
        stream.WriteValue(ReadKeyMaterial, particleArchetype_.material_);
        stream.WriteValue(ReadKeyBatchMaterialRender, batchMaterialRender_);
        stream.WriteValue(ReadKeyMaxParticleCount, particlesMax_);
        //stream.WriteValue(ReadKeyBatchMesh, batchMesh_);
        stream.WriteValue(ReadKeyBatchMaterial, batchMaterial_);
        stream.WriteValue(ReadKeyAnimation, anim_);
        stream.WriteValue(ReadKeySpeed, speed_);
        stream.WriteValue(ReadKeyIsWorldSpace, isWorldSpace_);
        stream.WriteValue(ReadKeyEmitIntervalActive, emitIntervalActive_);
        stream.WriteValue(ReadKeyEmitInterval, emitInterval_);
        stream.WriteValue(ReadKeyStartingLifetime, particleArchetype_.lifetimeRemaining_);
        stream.WriteValue(ReadKeyStartingTranslation, particleArchetype_.translation3_);
        stream.WriteValue(ReadKeyStartingRotation, particleArchetype_.rotation3_);
        stream.WriteValue(ReadKeyStartingScale, particleArchetype_.scale3_);
        stream.WriteValue(ReadKeyStartingVelocity, particleArchetype_.velocity3_);
        stream.WriteValue(ReadKeyStartingAcceleration, particleArchetype_.acceleration3_);
        stream.WriteValue(ReadKeyStartingRotationalVelocity, particleArchetype_.rotationalVelocity3_);
        stream.WriteValue(ReadKeyStartingDrag, particleArchetype_.drag_);
        stream.WriteValue(ReadKeyStartingRotationalDrag, particleArchetype_.rotationalDrag_);
        stream.WriteValue(ReadKeyRandomLifetime, particleArchetypeRandom_.lifetimeRemaining_);
        stream.WriteValue(ReadKeyRandomTranslation, particleArchetypeRandom_.translation3_);
        stream.WriteValue(ReadKeyRandomRotation, particleArchetypeRandom_.rotation3_);
        stream.WriteValue(ReadKeyRandomScale, particleArchetypeRandom_.scale3_);
        stream.WriteValue(ReadKeyRandomVelocity, particleArchetypeRandom_.velocity3_);
        stream.WriteValue(ReadKeyRandomAcceleration, particleArchetypeRandom_.acceleration3_);
        stream.WriteValue(ReadKeyRandomRotationalVelocity, particleArchetypeRandom_.rotationalVelocity3_);
        stream.WriteValue(ReadKeyRandomDrag, particleArchetypeRandom_.drag_);
        stream.WriteValue(ReadKeyRandomRotationalDrag, particleArchetypeRandom_.rotationalDrag_);
    }

    void ParticleEmitter::Update(float dt)
    {
        float dtScaled = dt * speed_;

        transform_ = isWorldSpace_ ? nullptr : EntityHas(Transform);

        ParticleBatch::PreUpdate();

        if (emitIntervalActive_ && emitInterval_ > 0.000001f)
        {
            emitIntervalCurrent_ -= dtScaled;
            if (emitIntervalCurrent_ <= 0)
            {
                static constexpr float MaxEmitTime = 1.0f;
                emitIntervalCurrent_ = std::max(emitIntervalCurrent_, -MaxEmitTime * speed_);
                unsigned emitCount = (unsigned)(-emitIntervalCurrent_ / emitInterval_) + 1;
                EmitInterval(emitCount, -emitIntervalCurrent_, -emitInterval_);
                emitIntervalCurrent_ += (float)emitCount * emitInterval_;
            }
        }

        // Update then emit so that expired particles can be overwritten.
        ParticleBatch::Update(dtScaled, anim_, animBuffer_);
    }

    void ParticleEmitter::Render() const
    {
        ParticleBatch::Render(mesh_);
    }

    void ParticleEmitter::Emit(unsigned int num)
    {
        EmitInterval(num, 0.0f, 0.0f);
    }

    const Mesh* ParticleEmitter::GetMesh(void) const
    {
        return mesh_;
    }

    void ParticleEmitter::SetMesh(const Mesh* mesh)
    {
        mesh_ = mesh;
    }

    Material* ParticleEmitter::GetMaterial(void) const
    {
        return particleArchetype_.GetMaterial();
    }

    void ParticleEmitter::SetMaterial(Material* material)
    {
        particleArchetype_.material_ = material;
    }

    bool ParticleEmitter::GetIsWorldSpace(void) const
    {
        return isWorldSpace_;
    }

    void ParticleEmitter::SetIsWorldSpace(bool worldSpace)
    {
        isWorldSpace_ = worldSpace;
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

    void ParticleEmitter::EmitInterval(unsigned int num, float base, float interval)
    {
        size_t oldTotal = particlesCount_;
        ParticleBatch::Emit(num, particleArchetype_);
        for (size_t i = oldTotal; i < particlesCount_; ++i)
        {
            Particle& particle = operator[](i);
#define RAND(value) particle.value = Random::Variance(particleArchetypeRandom_.value, particle.value)
            RAND(lifetimeRemaining_);
            RAND(translation3_);
            RAND(rotation3_);
            RAND(scale3_);
            RAND(velocity3_);
            RAND(acceleration3_);
            RAND(rotationalVelocity3_);
            RAND(drag_);
            RAND(rotationalDrag_);
            particle.SetParentMatrix(transform_);
            base += interval;
            if (IsBatch())
            {
                particle.SetLifeTime(particle.GetLifeTime() - base);
            }
            else
            {
                particle.Update(base, anim_, animBuffer_);
            }
        }
        if (IsBatch())
        {
            unsigned newCount = static_cast<unsigned>(particlesCount_ - oldTotal);
            if (newCount > 0)
            {
                batchMesh_->SetParticles(&particles_[oldTotal], newCount, particlesRaw_.data(), static_cast<unsigned>(oldTotal));
            }
        }
    }

#pragma endregion Private Functions

}	// namespace
