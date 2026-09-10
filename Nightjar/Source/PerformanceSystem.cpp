//------------------------------------------------------------------------------
//
// File Name:	PerformanceSystem.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		System that gathers and processes performance information.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "PerformanceSystem.h"
#include "PlatformSystem.h"

#include "Systems.inl"

#include "PerformanceTracker.h"

#include "Engine.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    //--------------------------------------------------------------------------
    // Class Implementation:
    //--------------------------------------------------------------------------

#pragma region Class Implementation

    class PerformanceSystem_ SYSTEM_FINAL : public BaseSystem
    {
        friend PerformanceSystem;

        SYSTEM_COMMON(PerformanceSystem_);

        // Public Constants and Enums:
    public:

        // Constructors/Destructors:
    private:
        PerformanceSystem_(void);

        // All systems need a virtual destructor to have their destructor called 
        ~PerformanceSystem_(void);

        // Public Static Functions:
    public:

        // Public Functions:
    public:

        // Private Constants, Enums, and Structs:
    private:
        //
        struct EngineDummy : public NamedObject
        {
            EngineDummy(void)
                : NamedObject("Engine")
            {
            }
        };

        // 
        struct VariableLengthTrackers
        {
        public:
            size_t size(void) const;
            void resize(size_t size);
            size_t capacity(void) const;
            void reserve(size_t capacity);
            ObjectID& operator[](size_t id);
            const ObjectID& operator[](size_t id) const;
        private:
            size_t current;
            vector_t<ObjectID> trackers;
        };

        // Private Static Functions:
    private:

        // Private Functions:
    private:
        /// <summary>
        /// Initializes the system.
        /// </summary>
        /// <returns>True if initialization succeeded, false otherwise.</returns>
        bool Init(void) override;

        /// <summary>
        /// Updates the system.
        /// </summary>
        void Update(void) override;

        /// <summary>
        /// Exits and shuts down the system.
        /// </summary>
        void Exit(void) override;

        // Private Static Variables:
    private:
        static PerformanceSystem_* instance;

        // Private Variables:
    private:
        //
        float systemTimesRefresherReset_;
        //
        float systemTimesRefresher_;
        // 
        PerformanceTracker frameTimes_;
        ////
        //PerformanceTracker engineTracker_;
        ////
        //array_t<PerformanceTracker, SystemMax> systemTrackers_;
        //
        unordered_map_t<ObjectID, PerformanceTracker> trackers_;
        //
        array_t<array_t<VariableLengthTrackers, EngineLoop::NUM_ENTRIES>, PerformanceGroup::NUM_ENTRIES> trackersOrdered_;
        //
        EngineDummy engineDummy_;
        //
        unordered_set_t<ObjectID> active_;
    };

#pragma endregion Class Implementation

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

    static constexpr size_t DefaultCapacity = 300;

    //--------------------------------------------------------------------------
    // Private Static Variables:
    //--------------------------------------------------------------------------

    PerformanceSystem_* PerformanceSystem_::instance = nullptr;

#define inst PerformanceSystem_::instance

    //--------------------------------------------------------------------------
    // Private Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Constructors/Destructors:
    //--------------------------------------------------------------------------

#pragma region Constructors

    PerformanceSystem_::PerformanceSystem_(void)
        : BaseSystem("PerformanceSystem", LoopFlags::HasUpdate)
        , systemTimesRefresherReset_(1.0f)
        , systemTimesRefresher_(systemTimesRefresherReset_)
        , frameTimes_(false)
        //, engineTracker_()
        //, systemTrackers_()
        , trackers_()
        , trackersOrdered_()
        , engineDummy_()
        , active_()
    {
        // Raise an assert if this system has already been created.
        assert(instance == nullptr);

        // Store this system's instance for use by static functions.
        instance = this;
    }

    //--------------------------------------------------------------------------

    PerformanceSystem_::~PerformanceSystem_(void)
    {
    }

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

#define ENUM_INDEX(enumVar) static_cast<std::make_unsigned_t<std::underlying_type_t<decltype(enumVar)>>>(enumVar)
#define DUR_CAST(dur) std::chrono::duration_cast<nsec_t>(dur)

    BaseSystem* PerformanceSystem::GetInstance(void)
    {
        return inst != nullptr ? inst : new PerformanceSystem_();
    }

    PerformanceTracker& PerformanceSystem::GetFrameTime(void)
    {
        return inst->frameTimes_;
    }

    size_t PerformanceSystem::GetTrackerCount(PerformanceGroup group, EngineLoop engineLoop)
    {
        return inst->trackersOrdered_[ENUM_INDEX(group)][ENUM_INDEX(engineLoop)].size();
    }

    void PerformanceSystem::SetTrackerCount(PerformanceGroup group, size_t count)
    {
        auto& groupOrders = inst->trackersOrdered_[ENUM_INDEX(group)];
        for (unsigned i = 0; i < static_cast<unsigned>(EngineLoop::NUM_ENTRIES); ++i)
        {
            groupOrders[i].resize(count);
        }
    }

    void PerformanceSystem::SetTrackerCount(PerformanceGroup group, EngineLoop engineLoop, size_t count)
    {
        inst->trackersOrdered_[ENUM_INDEX(group)][ENUM_INDEX(engineLoop)].resize(count);
    }

    void PerformanceSystem::ReserveTrackerCount(PerformanceGroup group, size_t count)
    {
        auto& groupOrders = inst->trackersOrdered_[ENUM_INDEX(group)];
        for (unsigned i = 0; i < static_cast<unsigned>(EngineLoop::NUM_ENTRIES); ++i)
        {
            groupOrders[i].reserve(count);
        }
    }

    void PerformanceSystem::ReserveTrackerCount(PerformanceGroup group, EngineLoop engineLoop, size_t count)
    {
        inst->trackersOrdered_[ENUM_INDEX(group)][ENUM_INDEX(engineLoop)].reserve(count);
    }

    PerformanceTracker& PerformanceSystem::GetTracker(const Object& obj)
    {
        inst->active_.insert(obj.GetGUID());
        return inst->trackers_.at(obj.GetGUID());
    }

    PerformanceTracker& PerformanceSystem::GetTracker(const NamedObject& obj, const Object& parent)
    {
        return GetTracker(obj, obj.GetName(), parent);
    }

    PerformanceTracker& PerformanceSystem::GetTracker(const Object& obj, string_view_arg name, const Object& parent)
    {
        inst->active_.insert(obj.GetGUID());
        auto result = inst->trackers_.try_emplace(obj.GetGUID(), obj, name, parent);
        if (result.second)
        {
            result.first->second.SetCapacity(DefaultCapacity);
        }
        return result.first->second;
    }

    void PerformanceSystem::AssignTrackerOrder(const Object& obj, PerformanceGroup group, EngineLoop engineLoop, size_t index)
    {
        inst->trackersOrdered_[ENUM_INDEX(group)][ENUM_INDEX(engineLoop)][index] = obj.GetGUID();
    }

    PerformanceTracker& PerformanceSystem::GetEngineTracker(void)
    {
        return GetTracker(inst->engineDummy_, inst->engineDummy_);
    }

    PerformanceTracker& PerformanceSystem::GetSystemTracker(Systems systemIndex)
    {
        const BaseSystem& system = Engine::GetSystem(systemIndex);
        return GetTracker(system, inst->engineDummy_);
    }

    const PerformanceTracker& PerformanceSystem::GetTrackerOrdered(PerformanceGroup group, EngineLoop engineLoop, size_t index)
    {
        return inst->trackers_[inst->trackersOrdered_[ENUM_INDEX(group)][ENUM_INDEX(engineLoop)][index]];
    }

#pragma endregion Public Static Functions

    //--------------------------------------------------------------------------
    // Public Functions:
    //--------------------------------------------------------------------------

#pragma region Public Functions

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

    bool PerformanceSystem_::Init(void)
    {
        // Add system-specific initialization code here.

        frameTimes_.SetCapacity(DefaultCapacity);

        PerformanceSystem::SetTrackerCount(PerformanceGroup::Engine, 1);
        PerformanceSystem::SetTrackerCount(PerformanceGroup::System, SystemMax);
        {
            auto& engineTracker = PerformanceSystem::GetEngineTracker();
            engineTracker.name = engineDummy_.GetName();
        }
        for (unsigned i = 0; i < static_cast<unsigned>(EngineLoop::NUM_ENTRIES); ++i)
        {
            const auto loop = static_cast<EngineLoop>(i);
            PerformanceSystem::AssignTrackerOrder(engineDummy_, PerformanceGroup::Engine, loop, 0);
        }
        for (unsigned i = 0; i < SystemMax; ++i)
        {
            const BaseSystem& system = Engine::GetSystem(static_cast<Systems>(i));
            auto& tracker = PerformanceSystem::GetSystemTracker(static_cast<Systems>(i));
            tracker.name = system.GetName();
        }

        // Return true if the initialization completed successfully.
        return true;
    }

    void PerformanceSystem_::Update(void)
    {
        float dt = PlatformSystem::GetDeltaTime();

        frameTimes_.Push(std::chrono::duration_cast<nsec_t>(secf_t(dt)));

        systemTimesRefresher_ -= dt;
        const bool doCalc = systemTimesRefresher_ <= 0.0f;
        frameTimes_.CalculateSingle(false);
        if (doCalc)
        {
            for (auto id : active_)
            {
                auto& tracker = trackers_.at(id);
                tracker.CalculateAll(false);
                tracker.Next();
            }
            systemTimesRefresher_ = systemTimesRefresherReset_;
        }
        else
        {
            for (auto id : active_)
            {
                auto& tracker = trackers_.at(id);
                tracker.Next();
            }
        }
        active_.clear();
    }

    void PerformanceSystem_::Exit(void)
    {
    }

#pragma endregion Private Functions

    size_t PerformanceSystem_::VariableLengthTrackers::size(void) const
    {
        return current;
    }

    void PerformanceSystem_::VariableLengthTrackers::resize(size_t size)
    {
        reserve(size);
        current = size;
    }

    size_t PerformanceSystem_::VariableLengthTrackers::capacity(void) const
    {
        return trackers.size();
    }

    void PerformanceSystem_::VariableLengthTrackers::reserve(size_t capacity)
    {
        size_t oldSize = trackers.size();
        if (capacity > oldSize)
        {
            trackers.resize(capacity);
        }
    }

    ObjectID& PerformanceSystem_::VariableLengthTrackers::operator[](size_t index)
    {
        return trackers[index];
    }

    const ObjectID& PerformanceSystem_::VariableLengthTrackers::operator[](size_t index) const
    {
        return trackers[index];
    }

}	// namespace
