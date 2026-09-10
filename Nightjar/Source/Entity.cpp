//------------------------------------------------------------------------------
//
// File Name:	 Entity.cpp
// Author(s):	 Doug Schilling (dschilling), Connor Lariviere (connor.lariviere)
// Course:       GAM200F25
// Project:      Card Critters
// Purpose:      Class representing an Entity.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Entity.h"
#include "Component.h"

#include "Transform.h"
#include "Animation.h"
#include "EntityContainer.h"
#include "ComponentRegistry.h"
#include "SceneSystem.h"

#include "Library.inl"
#include "LibraryStatic.inl"

#include "PerformanceSystem.h"
#include "PerformanceTracker.h"
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

    constexpr StreamKey
        Entity::ReadKeyArchetype = "Archetype",
        Entity::ReadKeyWritable = "Scene Write",
        Entity::ReadKeyComponents = "Components";

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

    Entity::Entity()
        : NamedObject()
        , components_()
        , parentContainer_(nullptr)
        , archetypeName_()
        , isDestroyed_(false)
        , writeable_(false)
    {
    }

    Entity::Entity(string_view_arg name)
        : NamedObject(name)
        , components_()
        , parentContainer_(nullptr)
        , archetypeName_()
        , isDestroyed_(false)
        , writeable_(false)
    {
    }

    Entity::Entity(const Entity& other)
        : NamedObject(other)
        , components_()
        , parentContainer_(other.parentContainer_)
        , archetypeName_(other.archetypeName_)
        , isDestroyed_(other.isDestroyed_)
        , writeable_(other.writeable_)
    {
        for (Component* c : other.components_)
        {
            ComponentAdd(c->Clone());
        }
        other.parentContainer_->Add(this);
    }

    Entity::Entity(Entity&& other) noexcept
        : NamedObject(other)
        , components_(std::move(other.components_))
        , parentContainer_(std::move(other.parentContainer_))
        , archetypeName_(std::move(other.archetypeName_))
        , isDestroyed_(std::move(other.isDestroyed_))
        , writeable_(std::move(other.writeable_))
    {
        // Clear other's components so that they aren't deleted when other is destroyed.
        other.components_.clear();
    }

    //--------------------------------------------------------------------------

    Entity::~Entity()
    {
        DestroyComponents();
    }

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

    Entity* Entity::TryRead(StreamIn& stream)
    {
        string_t name;
        if (!TryReadName(stream, name))
        {
            TraceWarning("No name when trying to read entity in {}|{}", stream.GetPath(), stream.GetHierarchy());
            return nullptr;
        }
        Entity* entity = new Entity(name);
        entity->Read(stream);
        return entity;
    }

#pragma endregion Public Static Functions

    //--------------------------------------------------------------------------
    // Public Functions:
    //--------------------------------------------------------------------------

#pragma region Public Functions

    Entity* Entity::Clone() const
    {
        return new Entity(*this);
    }

    void Entity::Read(string_view_arg filepath)
    {
        const Entity* archetype = EntityLibrary::Get_S(filepath);
        if (archetype != nullptr)
        {
            CopyFromArchetype(archetype);
        }
        else
        {
            TraceError("Failed to read entity \"{}\"", filepath);
        }
    }

    void Entity::Read(StreamIn& stream)
    {
        ReadName(stream);

        if (stream.TryReadValue(ReadKeyArchetype, archetypeName_) && !IsNamed(archetypeName_) && !archetypeName_.empty())
        {
            const Entity* archetype = EntityLibrary::Get_S(archetypeName_);
            if (archetype != nullptr)
            {
                CopyFromArchetype(archetype);
            }
        }

        stream.TryReadValue(ReadKeyWritable, writeable_);

        if (!stream.StreamHas(ReadKeyComponents))
        {
            TraceError("EntityRead: Failed to find Components in \"{}\"", stream.GetHierarchy());
            return;
        }
        
        stream.ReadObjectKeyValues(ReadKeyComponents, [this](string_view_t key, StreamIn& stream, int)
        {
            /*for (const auto& componentEntry : ComponentRegistry::Entries)
            {
                if (key == componentEntry.typeName)
                {
                    Component* component = ComponentGet(componentEntry.typeId);
                    if (component == nullptr)
                    {
                        // Creates a new component with base values
                        component = componentEntry.generator();
                        ComponentAdd(component);
                    }
                    component->Read(stream);
                    return;
                }
            }*/
            static const auto& mapKey(ComponentRegistry::MapKey());
            auto iter = mapKey.find(key);
            if (iter != mapKey.end())
            {
                const auto& componentEntry = ComponentRegistry::Entries[iter->second];
                Component* component = ComponentGet(componentEntry.typeId);
                try
                {
                    if (component == nullptr)
                    {
                        // Creates a new component with base values
                        component = componentEntry.generator();
                        ComponentAdd(component);
                    }
                    component->Read(stream);
                }
                catch (std::runtime_error err)
                {
                    TraceError("Error reading component with key \"{}\" at \"{}\": {}", key, stream.GetHierarchy(), err.what());
                }
                catch (...)
                {
                    TraceError("Unexpected error reading component with key \"{}\" at \"{}\"", key, stream.GetHierarchy());
                }
            }
            else
            {
                TraceWarning("Unknown component \"{}\" when reading entity \"{}\": \"{}\"", key, GetName(), stream.GetHierarchy());
            }
        });

        Animation* anim = Has(Animation);
        if (anim != nullptr && anim->GetPlaying())
        {
            anim->SetPlaying(true);
        }
    }

    void Entity::Write(StreamOut& stream)
    {
        if (!GetWriteable())
        {
            return;
        }

        // TODO: Save differences from archetype.
        //const Entity* archetype = EntityLibrary::Get_S(archetypeName_);

        stream.WriteObjectStart();
        stream.WriteValue(ReadKeyName, GetName());
        stream.WriteValue(ReadKeyArchetype, archetypeName_);
        stream.WriteValue(ReadKeyWritable, writeable_);

        stream.WriteObjectStart(ReadKeyComponents);
        for (Component* component : components_)
        {
            assert(component != nullptr);
            stream.WriteObjectStart(component->GetReadKey());
            component->Write(stream);
            stream.WriteObjectEnd();
        }
        stream.WriteObjectEnd();

        stream.WriteObjectEnd();
    }

    void Entity::Destroy(bool destroyChildren)
    {
        isDestroyed_ = true;
        if (destroyChildren)
        {
            Transform* tfm = Has(Transform);
            if (tfm != nullptr)
            {
                const auto& children = tfm->GetChildren();
                for (const auto& childTfm : children)
                {
                    childTfm->GetParent()->Destroy(true);
                }
            }
        }
    }

    bool Entity::IsDestroyed() const
    {
        return isDestroyed_;
    }

    void Entity::ComponentAdd(Component* other)
    {
        assert(other != nullptr);
        assert(other->GetParent() == nullptr);
        other->SetEntity(this);
        components_.push_back(other);
        std::sort(components_.begin(), components_.end(), ComponentSorter());
    }

    void Entity::ComponentRemove(Component* other)
    {
        assert(other != nullptr);
        assert(other->GetParent() == this);
        auto it = std::find(components_.begin(), components_.end(), other);
        assert(it != components_.end());
        components_.erase(it);
    }

    Component* Entity::ComponentGet(Component::TypeEnum type) const
    {
        size_t begin = 0;
        size_t end = components_.size();

        while (begin < end)
        {
            size_t mid = (begin + end) / 2;
            if (components_[mid]->GetType() < type)
                begin = mid + 1;
            else
                end = mid;
        }
        if ((begin < components_.size()) && (components_[begin]->GetType() == type))
        {
            // Return the first component found that matches the specified type.
            return components_[begin];
        }
        else
        {
            // No component matches the specified type.
            return nullptr;
        }
    }

    Entity* Entity::FindEntity(string_view_arg name) const
    {
        return parentContainer_ != nullptr ? parentContainer_->Find(name) : nullptr;
    }

    void Entity::Update(float dt)
    {
        size_t trackerIndex = PerformanceSystem::GetTrackerCount(PerformanceGroup::Component, EngineLoop::Update);
        {
            size_t newSize = trackerIndex + components_.size();
            PerformanceSystem::SetTrackerCount(PerformanceGroup::Component, EngineLoop::Update, newSize);
        }

        Stopwatch stopwatch{};
        stopwatch.Start();
        for (Component* c : components_)
        {
            //size_t typeIndex = static_cast<size_t>(ComponentRegistry::MapEnum().at(c->GetType()));

            auto& tracker = PerformanceSystem::GetTracker(*c, c->GetReadKey(), *this);

            c->Update(dt);

            COMPILER_FORCE(auto time = stopwatch.Lap<nsec_t>());
            PerformanceSystem::AssignTrackerOrder(*c, PerformanceGroup::Component, EngineLoop::Update, trackerIndex);
            tracker.Push(time, EngineLoop::Update, PerformanceSource::CPU);
            ++trackerIndex;
        }
        stopwatch.Stop();
    }

    void Entity::Render() const
    {
        size_t trackerIndex = PerformanceSystem::GetTrackerCount(PerformanceGroup::Component, EngineLoop::Render);
        {
            size_t newSize = trackerIndex + components_.size();
            PerformanceSystem::SetTrackerCount(PerformanceGroup::Component, EngineLoop::Render, newSize);
        }
        Stopwatch stopwatch{};
        stopwatch.Start();
        for (Component* c : components_)
        {
            //size_t typeIndex = static_cast<size_t>(ComponentRegistry::MapEnum().at(c->GetType()));

            auto& tracker = PerformanceSystem::GetTracker(*c, c->GetReadKey(), *this);

#ifdef RENDERTIMER_COMPONENTS
            tracker.gfxTimer.GetResults();
            tracker.Push(tracker.gfxTimer.GetDuration(), EngineLoop::Render, PerformanceSource::Graphics);
            COMPILER_FORCE(tracker.gfxTimer.Start());
#endif

            c->Render();

#ifdef RENDERTIMER_COMPONENTS
            COMPILER_FORCE(tracker.gfxTimer.Stop());
#endif

            COMPILER_FORCE(auto time = stopwatch.Lap<nsec_t>());
            tracker.Push(time, EngineLoop::Render, PerformanceSource::CPU);
            PerformanceSystem::AssignTrackerOrder(*c, PerformanceGroup::Component, EngineLoop::Render, trackerIndex);
            ++trackerIndex;
        }
        stopwatch.Stop();
    }

    bool Entity::GetWriteable() const
    {
        return writeable_;
    }

    const vector_t<Component*>& Entity::GetComponentList() const
    {
        return components_;
    }

    EntityContainer* Entity::GetContainer() const
    {
        return parentContainer_;
    }

    Scene* Entity::GetScene() const
    {
        return parentContainer_->GetScene();
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
    
    void Entity::DestroyComponents()
    {
        for (Component*& c : components_ | std::views::reverse)
        {
            assert(c != nullptr);
            SafeDelete(c);
        }
    }

    void Entity::CopyFromArchetype(const Entity* archetype)
    {
        DestroyComponents();
        for (const Component* c : archetype->components_)
        {
            ComponentAdd(c->Clone());
        }
    }

    bool Entity::ComponentSorter::operator()(Component* left, Component* right) const
    {
        return left->GetType() < right->GetType();
    }

#pragma endregion Private Functions

    //--------------------------------------------------------------------------
    // Library Implementation:
    //--------------------------------------------------------------------------

#pragma region Library Implementation

    template EntityLibrary;

    INSTANTIATE_LIBRARY_IMGUI_SELECTOR(Entity);

#pragma endregion Library Implementation

}	// namespace
