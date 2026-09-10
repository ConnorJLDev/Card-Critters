//------------------------------------------------------------------------------
//
// File Name:	EntityContainer.c
// Author(s):	Connor Lariviere (connor.lariviere), Jason Nguyen (jason.nguyen)
// Project:		Project 4
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "EntityContainer.h"
#include "Entity.h"
#include "Collider.h"
#include "Scene.h"

#include "PerformanceSystem.h"
#include "CycleCounter.h"

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

    EntityContainer::EntityContainer()
        : scene_(nullptr)
    {
    }

    //--------------------------------------------------------------------------

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

    void EntityContainer::UpdateAll(float dt)
    {
        const size_t oldSize = PerformanceSystem::GetTrackerCount(PerformanceGroup::Entity, EngineLoop::Update);
        size_t trackerIndex = oldSize;
        {
            size_t newSize = oldSize + Count();
            PerformanceSystem::ReserveTrackerCount(PerformanceGroup::Entity, EngineLoop::Update, newSize);
        }
        Stopwatch stopwatch{};
        stopwatch.Start();
        for (unsigned int i = 0; i < Count(); ++i)
        {
            Entity* entity = items_[i];

            auto& tracker = PerformanceSystem::GetTracker(*entity, *scene_);

            entity->Update(dt);

            COMPILER_FORCE(auto time = stopwatch.Lap<nsec_t>());

            PerformanceSystem::SetTrackerCount(PerformanceGroup::Entity, EngineLoop::Update, oldSize + Count());
            PerformanceSystem::AssignTrackerOrder(*entity, PerformanceGroup::Entity, EngineLoop::Update, trackerIndex);
            tracker.Push(time, EngineLoop::Update, PerformanceSource::CPU);
            ++trackerIndex;
        }
        std::erase_if(items_, [](const auto& entity)
        {
            const bool destroyed = entity->IsDestroyed();
            if (destroyed)
            {
                delete entity;
            }
            return destroyed;
        });
        stopwatch.Stop();
    }

    void EntityContainer::RenderAll(void)
    {
        size_t trackerIndex = PerformanceSystem::GetTrackerCount(PerformanceGroup::Entity, EngineLoop::Render);
        {
            size_t newSize = trackerIndex + Count();
            PerformanceSystem::SetTrackerCount(PerformanceGroup::Entity, EngineLoop::Render, newSize);
        }
        Stopwatch stopwatch{};
        stopwatch.Start();
        for (const auto& entity : items_)
        {
            auto& tracker = PerformanceSystem::GetTracker(*entity, *scene_);
#ifdef RENDERTIMER_ENTITIES
            tracker.gfxTimer.GetResults();
            tracker.Push(tracker.gfxTimer.GetDuration(), EngineLoop::Render, PerformanceSource::Graphics);
            COMPILER_FORCE(tracker.gfxTimer.Start());
#endif

            entity->Render();

#ifdef RENDERTIMER_ENTITIES
            COMPILER_FORCE(tracker.gfxTimer.Stop());
#endif
            COMPILER_FORCE(auto time = stopwatch.Lap<nsec_t>());
            tracker.Push(time, EngineLoop::Render, PerformanceSource::CPU);
            PerformanceSystem::AssignTrackerOrder(*entity, PerformanceGroup::Entity, EngineLoop::Render, trackerIndex);
            ++trackerIndex;
        }
        stopwatch.Stop();
    }

    void EntityContainer::CheckCollisions(void)
    {
        for (unsigned int i = 0; i < Count(); i++)
        {
            Entity* entity1 = items_[i];
            Collider* collider1 = entity1->Has(Collider);
            if (entity1 != nullptr && collider1 != nullptr)
            {
                for (unsigned int j = i + 1; j < Count(); j++)
                {
                    Entity* entity2 = items_[j];
                    Collider* collider2 = entity2->Has(Collider);

                    //Checks if collider 1 is colling with 2.
                    if (collider1 != nullptr && collider2 != nullptr)
                    {
                        collider1->Check(collider2);
                    }
                }

            }
        }
    }

    Scene* EntityContainer::GetScene(void) const
    {
        return scene_;
    }

    bool EntityContainer::Add(Entity* entity)
    {
        bool success = Container<Entity>::Add(entity);
        if (success)
        {
            entity->parentContainer_ = this;
        }
        return success;
    }

    bool EntityContainer::Add(Entity& entity)
    {
        return Add(&entity);
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
