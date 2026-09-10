//------------------------------------------------------------------------------
//
// File Name:	SceneSystem.cpp
// Author(s):	Connor Lariviere (connor.lariviere), Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		System that handles scenes.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "BaseSystem.h"
#include "SceneSystem.h"
#include "PlatformSystem.h"
#include "Scene.h"
#include "Entity.h"
#include "PerformanceSystem.h"
#include "GraphicsStopwatch.h"
#include "CycleCounter.h"
#include "ComponentRegistry.h"

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

    class SceneSystem_ SYSTEM_FINAL : public BaseSystem
    {
        friend SceneSystem;

        SYSTEM_COMMON(SceneSystem_);

        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    private:
        SceneSystem_(void);

        // All systems need a virtual destructor to have their destructor called 
        ~SceneSystem_(void) override;

        // Public Static Functions:
    public:

        // Public Functions:
    public:
        //
        //EntityContainer* FindContainer() const;

        //Scene* GetScene();

        // Public Event Handlers
    public:

        // Private Constants, Enums, and Structs:
    private:
        struct SceneManager;

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
        /// Renders the system.
        /// </summary>
        void Render(void) override;

        /// <summary>
        /// Exits and shuts down the system.
        /// </summary>
        void Exit(void) override;

        // Private Static Variables:
    private:
        static SceneSystem_* instance;

        // Private Variables:
    private:
        vector_t<SceneManager> activeScenes_;
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
    // Private Structures:
    //--------------------------------------------------------------------------

    struct SceneSystem_::SceneManager
    {
        Scene* scene;
        string_t nextScene;
        bool isRestarting;

        SceneManager(void)
            : scene(nullptr)
            , nextScene()
            , isRestarting(false)
        {
        }
        SceneManager(const SceneManager& other) = delete;
        SceneManager(SceneManager&& other) noexcept
            : scene(std::exchange(other.scene, nullptr))
            , nextScene(std::move(other.nextScene))
            , isRestarting(std::move(other.isRestarting))
        {
        }
        SceneManager& operator=(const SceneManager& other) = delete;
        ~SceneManager();

        bool Running() const;
        bool IsNamed(string_view_arg name) const;
        bool IsChanging() const;
        void SetNext(string_view_arg name);

        void Update(float& dt);
        void Render() const;
        void Exit();
    };

    //--------------------------------------------------------------------------
    // Private Constants:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Private Static Variables:
    //--------------------------------------------------------------------------

    SceneSystem_* SceneSystem_::instance = nullptr;

#define inst SceneSystem_::instance

    //--------------------------------------------------------------------------
    // Private Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Constructors/Destructors:
    //--------------------------------------------------------------------------

#pragma region Constructors

    SceneSystem_::SceneSystem_(void)
        : BaseSystem("SceneSystem", LoopFlags::HasUpdate | LoopFlags::HasRender)
        , activeScenes_()
    {
        activeScenes_.resize(10);

        assert(instance == nullptr);

        instance = this;
    }

    //--------------------------------------------------------------------------

    SceneSystem_::~SceneSystem_(void)
    {
    }

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

    BaseSystem* SceneSystem::GetInstance()
    {
        return inst != nullptr ? inst : new SceneSystem_();
    }

    bool SceneSystem::IsRunning()
    {
        //return !inst->activeScenes_.empty();
        auto iter = std::find_if(inst->activeScenes_.begin(), inst->activeScenes_.end(), [](const SceneSystem_::SceneManager& scnMgr) { return scnMgr.Running(); });
        return iter != inst->activeScenes_.end();
    }

    Scene* SceneSystem::FindScene(string_view_arg name)
    {
        auto iter = std::find_if(inst->activeScenes_.begin(), inst->activeScenes_.end(), [&name](const SceneSystem_::SceneManager& scnMgr) { return scnMgr.IsNamed(name); });
        return iter == inst->activeScenes_.end() ? nullptr : iter->scene;
    }

    Scene* SceneSystem::GetScene(unsigned slot)
    {
        assert(slot < inst->activeScenes_.size());
        SceneSystem_::SceneManager& scnMgr = inst->activeScenes_[slot];
        return scnMgr.scene;
    }

    unsigned SceneSystem::GetSceneSlotCount(void)
    {
        return static_cast<unsigned>(inst->activeScenes_.size());
    }

    void SceneSystem::Restart(unsigned slot)
    {
        assert(slot < inst->activeScenes_.size());
        inst->activeScenes_[slot].isRestarting = true;
    }

    void SceneSystem::SetNext(string_view_arg name, unsigned slot)
    {
        assert(slot < inst->activeScenes_.size());
        SceneSystem_::SceneManager& scnMgr = inst->activeScenes_[slot];
        if (scnMgr.IsNamed(name))
        {
            Restart(slot);
        }
        else
        {
            scnMgr.nextScene = name;
        }
    }

    bool SceneSystem::IsChanging(unsigned slot)
    {
        assert(slot < inst->activeScenes_.size());
        SceneSystem_::SceneManager& scnMgr = inst->activeScenes_[slot];
        return scnMgr.IsChanging();
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

    bool SceneSystem_::Init(void)
    {
        EntityLibrary::Init_S();

        // Return true if the initialization completed successfully.
        return true;
    }

    void SceneSystem_::Update(void)
    {
        // If the window isn't visible or isn't focused, don't update anything.
        if (PlatformSystem::GetIsIconified() || !PlatformSystem::GetIsFocused())
        {
            return;
        }

        PerformanceSystem::SetTrackerCount(PerformanceGroup::Entity, EngineLoop::Update, 0);
        PerformanceSystem::SetTrackerCount(PerformanceGroup::Component, EngineLoop::Update, 0);

        /*static constexpr unsigned componentTypeCount = (unsigned)ComponentRegistry::ComponentIndex::NUM_ENTRIES;
        PerformanceSystem::SetTrackerCount(PerformanceGroup::ComponentType, EngineLoop::Update, componentTypeCount);
        for (unsigned i = 0; i < componentTypeCount; ++i)
        {
            PerformanceSystem::SetTrackerTime(PerformanceGroup::ComponentType, EngineLoop::Update, i, nsec_t(0), ComponentRegistry::Entries[i].typeName);
        }*/

        PerformanceSystem::ReserveTrackerCount(PerformanceGroup::Scene, EngineLoop::Update, activeScenes_.size());
        PerformanceSystem::SetTrackerCount(PerformanceGroup::Scene, EngineLoop::Update, 0);

        float dt = PlatformSystem::GetDeltaTime();

        for (SceneManager& scnMgr : activeScenes_ | std::views::reverse)
        {
            scnMgr.Update(dt);
        }
    }

    void SceneSystem_::Render(void)
    {
        // If the window isn't visible or isn't focused, don't render anything.
        if (PlatformSystem::GetIsIconified() || !PlatformSystem::GetIsFocused())
        {
            return;
        }

        PerformanceSystem::SetTrackerCount(PerformanceGroup::Entity, EngineLoop::Render, 0);
        PerformanceSystem::SetTrackerCount(PerformanceGroup::Component, EngineLoop::Render, 0);

        /*static constexpr unsigned componentTypeCount = (unsigned)ComponentRegistry::ComponentIndex::NUM_ENTRIES;
        PerformanceSystem::SetTrackerCount(PerformanceGroup::ComponentType, Engine::Loop::Render, componentTypeCount);
        for (unsigned i = 0; i < componentTypeCount; ++i)
        {
            PerformanceSystem::SetTrackerTime(PerformanceGroup::ComponentType, Engine::Loop::Render, i, nsec_t(0), ComponentRegistry::Entries[i].typeName);
        }*/

        PerformanceSystem::ReserveTrackerCount(PerformanceGroup::Scene, EngineLoop::Render, activeScenes_.size());
        PerformanceSystem::SetTrackerCount(PerformanceGroup::Scene, EngineLoop::Render, 0);

        for (const SceneManager& scnMgr : activeScenes_)
        {
            scnMgr.Render();
        }
    }

    void SceneSystem_::Exit(void)
    {
        for (SceneManager& scnMgr : activeScenes_)
        {
            scnMgr.Exit();
        }
        EntityLibrary::Shutdown_S();
    }

#pragma endregion Private Functions

    SceneSystem_::SceneManager::~SceneManager()
    {
        SafeDeleteCheck(scene);
    }

    bool SceneSystem_::SceneManager::Running() const
    {
        return scene != nullptr || !nextScene.empty();
    }

    bool SceneSystem_::SceneManager::IsNamed(string_view_arg name) const
    {
        return scene != nullptr && scene->IsNamed(name);
    }

    bool SceneSystem_::SceneManager::IsChanging() const
    {
        return isRestarting || !IsNamed(nextScene);
    }

    void SceneSystem_::SceneManager::SetNext(string_view_arg name)
    {
        nextScene = name;
    }

    void SceneSystem_::SceneManager::Update(float& dt)
    {
        const bool changing = IsChanging();
        const bool processing = changing || scene != nullptr;

        Stopwatch stopwatch{};

        COMPILER_BARRIER();
        if (processing)
        {
            stopwatch.Start();
        }
        COMPILER_BARRIER();

        // Check for a scene change.
        if (changing)
        {
            // If the name of the next scene is empty, we're destroying our current scene.
            bool destroyingScene = nextScene.empty();

            // If we have a scene running...
            if (scene != nullptr)
            {
                // Exit the current scene.
                scene->Exit();

                // Unload active scene if we're NOT restarting.
                if (!isRestarting)
                {
                    scene->Unload();
                }
            }
            else
            {
                // If we don't have a scene running, make a new scene.
                scene = new Scene();
            }
            if (destroyingScene)
            {
                // If we're deleting this scene, it should exist.
                assert(scene != nullptr);
                SafeDelete(scene);
                return;
            }
            // Read the data of the next scene.
            scene->Read(nextScene);
            // Initializes the tracker for the scene in the performance system.
            auto& tracker = PerformanceSystem::GetTracker(*scene, *inst);
            if (!isRestarting)
            {
                // If we're not restarting, we need to set the name and call the load.
                scene->SetName(nextScene);
                // Load new scene
                scene->Load();
                // Set the performance tracker name.
                tracker.name = scene->GetName();
            }
            // Clear the children of the performance tracker.
            tracker.children.clear();

            isRestarting = false;

            // Initialize the new scene.
            scene->Init();
        }

        if (scene != nullptr)
        {
            // Update the active scene.
            scene->Update(dt);
        }

        COMPILER_BARRIER();
        if (processing && scene != nullptr)
        {
            stopwatch.Stop();
            size_t index = PerformanceSystem::GetTrackerCount(PerformanceGroup::Scene, EngineLoop::Update);
            PerformanceSystem::SetTrackerCount(PerformanceGroup::Scene, EngineLoop::Update, index + 1);
            PerformanceSystem::AssignTrackerOrder(*scene, PerformanceGroup::Scene, EngineLoop::Update, index);
            auto& tracker = PerformanceSystem::GetTracker(*scene, *inst);
            tracker.Push(stopwatch.Duration<nsec_t>(), EngineLoop::Update, PerformanceSource::CPU);
        }
        COMPILER_BARRIER();
    }

    void SceneSystem_::SceneManager::Render() const
    {
        if (scene != nullptr)
        {
            Stopwatch stopwatch{};
            stopwatch.Start();

            size_t index = PerformanceSystem::GetTrackerCount(PerformanceGroup::Scene, EngineLoop::Render);
            PerformanceSystem::SetTrackerCount(PerformanceGroup::Scene, EngineLoop::Render, index + 1);
            PerformanceSystem::AssignTrackerOrder(*scene, PerformanceGroup::Scene, EngineLoop::Render, index);

            auto& tracker = PerformanceSystem::GetTracker(*scene, *inst);

            if (tracker.gfxTimer.GetResults())
            {
                tracker.Push(tracker.gfxTimer.GetDuration(), EngineLoop::Render, PerformanceSource::Graphics);
            }
            COMPILER_FORCE(tracker.gfxTimer.Start());

            scene->Render();

            COMPILER_FORCE(tracker.gfxTimer.Stop());

            COMPILER_FORCE(stopwatch.Stop());
            tracker.Push(stopwatch.Duration<nsec_t>(), EngineLoop::Render, PerformanceSource::CPU);
        }
    }

    void SceneSystem_::SceneManager::Exit()
    {
        if (scene != nullptr)
        {
            scene->Exit();
            SafeDelete(scene);
        }
    }

}	// namespace
