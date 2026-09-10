//------------------------------------------------------------------------------
//
// File Name:	ParticleBatch.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class describing batches of particles.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ParticleBatch.h"

#include "Material.h"
#include "Mesh.h"
#include "ParticleBatchMesh.h"

#include GRAPHICS_INCLUDE

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

    ParticleBatch::ParticleBatch(void)
        : Object()
        , batchMesh_(nullptr)
        , batchMaterial_(nullptr)
        , batchMaterialRender_(nullptr)
        , particles_()
        , particlesRaw_()
        , particlesCount_(0)
        , particlesMax_(1000)
    {
    }

    ParticleBatch::ParticleBatch(const ParticleBatch& other)
        : Object(other)
        , batchMesh_(other.batchMesh_ == nullptr ? nullptr : new ParticleBatchMesh())
        , batchMaterial_(GraphicsSystem::BuildMaterial(other.batchMaterial_))
        , batchMaterialRender_(GraphicsSystem::BuildMaterial(other.batchMaterialRender_))
        , particles_(other.particles_)
        , particlesRaw_(other.particlesRaw_)
        , particlesCount_(0)
        , particlesMax_(other.particlesMax_)
    {
        if (batchMesh_ != nullptr)
        {
            SetMaxParticleCount(particlesMax_);
        }
    }

    //--------------------------------------------------------------------------

    ParticleBatch::~ParticleBatch(void)
    {
        GraphicsSystem::DestroyMaterial(batchMaterial_);
        SafeDeleteCheck(batchMesh_);
    }

    Particle& ParticleBatch::operator[](size_t index)
    {
        return particlesRaw_[particles_[index]];
    }

    const Particle& ParticleBatch::operator[](size_t index) const
    {
        return particlesRaw_[particles_[index]];
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

    void ParticleBatch::PreUpdate(void)
    {
        if (IsBatch())
        {
            batchMesh_->PreUpdate();
            particlesCount_ = batchMesh_->GetCountParticlesActive();
        }
    }

    void ParticleBatch::Update(float dt, const AnimationSequence* anim, vector_t<float>& animBuffer)
    {
        if (IsBatch())
        {
            batchMesh_->Update(*batchMaterial_);
            particlesCount_ = batchMesh_->GetCountParticlesActive();
        }
        else
        {
            for (unsigned i = 0; i < particlesCount_; ++i)
            {
                auto& particleIndex = particles_[i];
                auto& particle = particlesRaw_[particleIndex];
                particle.Update(dt, anim, animBuffer);
                if (particle.Expired())
                {
                    std::swap(particleIndex, particles_[--particlesCount_]);
                }
            }
        }
    }

    void ParticleBatch::Render(const Mesh* mesh) const
    {
        if (IsBatch())
        {
            batchMesh_->Render(TransformComponents::Identity, *batchMaterialRender_);
        }
        else
        {
            for (unsigned i = 0; i < particlesCount_; ++i)
            {
                const Particle& particle = particlesRaw_[particles_[i]];
                particle.Render(mesh);
            }
        }
    }

    void ParticleBatch::Emit(unsigned int num, const Particle& archetype)
    {
        size_t countTotal = MathMin(particlesCount_ + num, particlesMax_);
        if (countTotal > particles_.size() || countTotal > particlesRaw_.size())
        {
            size_t oldSize = particles_.size();
            particles_.resize(countTotal);
            std::iota(particles_.begin() + static_cast<int64_t>(oldSize), particles_.end(), oldSize);
            particlesRaw_.resize(countTotal, archetype);
        }
        for (; particlesCount_ < countTotal; ++particlesCount_)
        {
            particlesRaw_[particles_[particlesCount_]] = archetype;
        }
        if (IsBatch())
        {
            batchMesh_->SetCountParticlesActive(static_cast<unsigned int>(particlesCount_));
        }
    }

    void ParticleBatch::EnableBatching(const Mesh& mesh, Material& material)
    {
        if (batchMesh_ == nullptr)
        {
            batchMesh_ = new ParticleBatchMesh();
        }
        SetMaxParticleCount(particlesMax_);
        ParticleBatchMesh::ApplyMeshToMaterial(mesh, material);
    }

    unsigned int ParticleBatch::GetMaxParticleCount(void) const
    {
        return static_cast<unsigned int>(particlesMax_);
    }

    void ParticleBatch::SetMaxParticleCount(size_t num)
    {
        particlesMax_ = num;
        particles_.reserve(num);
        particlesRaw_.reserve(num);
        if (!IsBatch())
        {
            return;
        }
        static constexpr size_t ParticleSize = sizeof(Vec4) * 6; // Particles_vert.glsl
        batchMesh_->Resize(particlesMax_);
        batchMaterial_->SetOutputSize(particlesMax_ * ParticleSize);
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

    bool ParticleBatch::IsBatch() const
    {
        return batchMesh_ != nullptr && batchMaterial_ != nullptr;
    }

#pragma endregion Private Functions

}	// namespace
