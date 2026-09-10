//------------------------------------------------------------------------------
//
// File Name:	Particle.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class describing particle behavior.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Particle.h"

#include GRAPHICS_INCLUDE

#include "Mesh.h"
#include "Material.h"
#include "AnimationSequence.h"
#include "AnimationLink.h"
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

	Particle::Particle(void)
		: Object()
        , TransformInheritance()
        , PhysicsBase()
		, emitter_(nullptr)
		, material_(nullptr)
		, lifetimeRemaining_()
        , randSeed_()
	{
	}

	Particle::Particle(const Particle& other)
        : Object(other)
        , TransformInheritance(other)
        , PhysicsBase(other)
		, emitter_(other.emitter_)
		, material_(GraphicsSystem::BuildMaterial(other.material_))
		, lifetimeRemaining_(other.lifetimeRemaining_)
        , randSeed_(other.randSeed_)
	{
	}

	Particle::Particle(Particle&& other) noexcept
        : Object(std::move(other))
        , TransformInheritance(std::move(other))
        , PhysicsBase(std::move(other))
		, emitter_(std::move(other.emitter_))
		, material_(std::exchange(other.material_, nullptr))
		, lifetimeRemaining_(std::move(other.lifetimeRemaining_))
        , randSeed_(std::move(other.randSeed_))
	{
	}

	Particle& Particle::operator=(const Particle& other)
	{
        Object::operator=(other);
        TransformInheritance::operator=(other);
        PhysicsBase::operator=(other);
		emitter_ = other.emitter_;
        if (material_ != nullptr && other.material_ != nullptr)
        {
            assert(material_->IsNamed(other.material_->GetName()));
            *material_ = *other.material_;
        }
		lifetimeRemaining_ = other.lifetimeRemaining_;
        randSeed_ = other.randSeed_;
		return *this;
	}

	//--------------------------------------------------------------------------

	Particle::~Particle(void)
	{
		GraphicsSystem::DestroyMaterial(material_);
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

	void Particle::Update(float dt, const AnimationSequence* anim, vector_t<float>& animBuffer)
	{
		lifetimeRemaining_ -= dt;
		if (Expired())
		{
			return;
		}

        Random::Rand localRand{randSeed_};

		UpdateInternal(*this, dt);

		if (anim != nullptr)
		{
			anim->UpdateGroups(lifetimeRemaining_, animBuffer);
			for (auto animLink : anim->links_)
			{
                float ratio = localRand.Range(0.0f, 1.0f);
                UNREFERENCED_PARAMETER(ratio);
				animLink->Update(this, animBuffer);
			}
		}

		GetMatrixFinal();
	}

	void Particle::Render(const Mesh* mesh) const
	{
		GraphicsSystem::DrawCall(mtxFinal_, *mesh, *material_);
	}

	Material* Particle::GetMaterial() const
	{
		return material_;
	}

	bool Particle::Expired() const
	{
		return lifetimeRemaining_ <= 0;
	}

    float Particle::GetLifeTime() const
    {
        return lifetimeRemaining_;
    }

    void Particle::SetLifeTime(float lifeTime)
    {
        lifetimeRemaining_ = lifeTime;
    }

    unsigned int Particle::GetRandomSeed() const
    {
        return randSeed_;
    }

    void Particle::SetRandomSeed(unsigned int seed)
    {
        randSeed_ = seed;
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

#pragma endregion Private Functions

}	// namespace
