//------------------------------------------------------------------------------
//
// File Name:	Engine.cpp
// Author(s):	Doug Schilling (dschilling), Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Main engine.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Engine.h"
#include "BaseSystem.h"
#include "Random.h"
#include "PerformanceTracker.h"
#include "CycleCounter.h"
#include "Systems.h"
#include "Systems.inl"
#include "EngineLoop.h"

#ifdef WINDOWS_THREADS
#include <windows.h>
#include <tlhelp32.h>
#endif

#undef TRACE_CATEGORY
#define TRACE_CATEGORY ENGINE

//------------------------------------------------------------------------------
// Preprocessor Directives:
//------------------------------------------------------------------------------

#pragma warning(push)
//#pragma warning(disable : 4371)

#ifdef _MSC_VER
#define LOOPSYSTEMS_USE_LAMBDA
#endif

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

    class Engine_
    {
        friend Engine;

        // Public Constants, Enums, and Structs:
    public:
        // Struct representing all systems that are called in a given loop.
        struct LoopSystems
        {
#ifdef LOOPSYSTEMS_USE_LAMBDA
            typedef void (* invoke_t)(BaseSystem&);
            inline void exec(BaseSystem& target) const { func(target); }
#define BASESYSTEM_FUNC(func) [](BaseSystem& t) { t.func(); }
#else
            typedef void (BaseSystem::* invoke_t)(void);
            inline void exec(BaseSystem& target) const { (target.*func)(); }
#define BASESYSTEM_FUNC(func) &BaseSystem::func
#endif
            invoke_t func;
            unsigned short begin, end;
            EngineLoop loop;
            bool enabled;
            constexpr LoopSystems(EngineLoop loop_, invoke_t func_)
                : func(func_)
                , begin(static_cast<unsigned short>(static_cast<unsigned short>(Systems::NUM_ENTRIES)* static_cast<unsigned short>(loop_)))
                , end(begin)
                , loop(loop_)
                , enabled(true)
            {
            }
            LoopSystems(const LoopSystems& other) = delete;
            constexpr LoopSystems(LoopSystems&& other) noexcept
                : func(std::exchange(other.func, invoke_t{}))
                , begin(std::exchange(other.begin, (unsigned short)0))
                , end(std::exchange(other.end, (unsigned short)0))
                , loop(std::exchange(other.loop, EngineLoop{}))
                , enabled(std::exchange(other.enabled, false))
            {
            }
            LoopSystems& operator=(const LoopSystems& other) = delete;

            unsigned short size(void) const { return static_cast<unsigned short>(end - begin); }
            void SortLoop(void);
            void DoLoop(void) const;
        };

        // Constructors/Destructors:
    private:
        constexpr Engine_(void)
            : systemCount_(0)
            , systems_()
            , systemLoops_({
                LoopSystems(EngineLoop::Update, BASESYSTEM_FUNC(Update)),
                LoopSystems(EngineLoop::Render, BASESYSTEM_FUNC(Render)),
                })
            , systemLoopIndices_()
        {
            assert(&engine == this);
        }
    public:
        Engine_(const Engine_& other) = delete;

        Engine_(Engine_&& other) noexcept = delete;

        Engine_& operator=(const Engine_& other) = delete;

        ~Engine_(void)
        {
        }

        // Public Static Functions:
    public:

        // Public Functions:
    public:

        // Public Event Handlers
    public:

        // Private Constants, Enums, and Structs:
    private:
        //
        static const string_view_t
              ConfigFileName
            , ConfigTraceConsole
            , ConfigTraceLog
            , ConfigWindowSize
            , ConfigWindowTitle
            , ConfigGraphicsVersion
            , ConfigGraphicsProfile
            , ConfigSceneInit
            , ConfigVSync
            ;

        // Private Static Functions:
    private:
        // Add a system to the game engine.
        // Params:
        //	 system = Pointer to the system to be added.
        static void AddSystem(BaseSystem* system);

        // Private Functions:
    private:
        bool InitConfig(string_view_arg path);

        // Private Static Variables:
    private:
        static Engine_ engine;
        static LoopSystems lsTest;

        // Private Variables:
    private:
        unsigned systemCount_;
        array_t<BaseSystem*, Systems::NUM_ENTRIES> systems_;
        array_t<LoopSystems, EngineLoop::NUM_ENTRIES> systemLoops_;
        array_t<unsigned char, static_cast<size_t>(Systems::NUM_ENTRIES) * static_cast<size_t>(EngineLoop::NUM_ENTRIES)> systemLoopIndices_;
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

    constexpr string_view_t
          Engine_::ConfigFileName = "config.json"
        , Engine_::ConfigTraceConsole = "TraceConsole"
        , Engine_::ConfigTraceLog = "TraceLog"
        , Engine_::ConfigWindowSize = "WindowSize"
        , Engine_::ConfigWindowTitle = "WindowTitle"
        , Engine_::ConfigGraphicsVersion = "GraphicsVersion"
        , Engine_::ConfigGraphicsProfile = "GraphicsProfile"
        , Engine_::ConfigSceneInit = "SceneInit"
        , Engine_::ConfigVSync = "VSync"
        ;

    //--------------------------------------------------------------------------
    // Private Static Variables:
    //--------------------------------------------------------------------------

    constinit Engine_ Engine_::engine;

#define engine Engine_::engine

    //--------------------------------------------------------------------------
    // Private Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Constructors/Destructors:
    //--------------------------------------------------------------------------

#pragma region Constructors

    //--------------------------------------------------------------------------

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

    bool Engine::Init(void)
    {
        Stopwatch stopwatch{};
        stopwatch.Start();

        //----------------------------------------------------------------------
        // Initialize the memory manager, and exit if it failed to initialize.
        //----------------------------------------------------------------------

        if (!MemoryManager::Init())
        {
            assert(false && "Memory manager failed to initialize!");
            return false;
        }

        // Certain modules need to be initialized first.
        // Other systems can be initialized later and in any order.
#define REGISTER_SYSTEM(system) Engine_::AddSystem(system::GetInstance());
#include "Systems.def"
#undef REGISTER_SYSTEM
        
        assert(engine.systemCount_ == engine.systems_.size());

        // Initialize the Tracing/Logging module.
        Trace::Init();

        // Initialize the Random module.
        Random::Init();

        TraceInfo("Engine: Init");

        if (!engine.InitConfig(Engine_::ConfigFileName))
        {
            TraceWarning("Failed to find engine config file: \"{}\"", Engine_::ConfigFileName);
        }
        engine.InitConfig(IOSystem::GetUserDataPath() + string_t(Engine_::ConfigFileName));

        {
            auto initStartTime = stopwatch.Lap<msecd_t>();
            TraceVerbose("Engine started initialization in {}", initStartTime);
        }

        //----------------------------------------------------------------------
        // Initialize all systems.
        //----------------------------------------------------------------------

        bool success;

        for (unsigned i = 0; i < engine.systemCount_; ++i)
        {
            BaseSystem* systemPtr = engine.systems_[i];

            // Verify that the current system is valid.
            if (systemPtr == nullptr)
            {
                TraceError("Failed to initialize engine: Null system pointer at index {}.", i);
                assert(systemPtr != nullptr && "EngineInit Error: A NULL system has been added to the engine.");
                return false;
            }

            BaseSystem& system = *systemPtr;

            // Write a trace message as each system is initialized.
            TraceInfo("{}: Init", system.GetName());

            // Call the current system's Init function.
            success = system.Init();
            if (!success)
            {
                return false;
            }

            auto systemTime = stopwatch.Lap<msecd_t>();
            TraceVerbose("{} initialized in {}", system.GetName(), systemTime);
        }

        {
            auto initSysTime = stopwatch.Lap<msecd_t>();
            TraceVerbose("Engine initialized systems in {}", initSysTime);
        }

        {
            for (auto& loopContainer : engine.systemLoops_)
            {
                loopContainer.SortLoop();
            }

            auto initTimerTime = stopwatch.Lap<msecd_t>();
            TraceVerbose("Engine initialized system loops in {}", initTimerTime);
        }

        stopwatch.Stop();

        auto initTime = stopwatch.Duration<msecd_t>();
        TraceVerbose("Engine finished initialization in {}", initTime);

        return true;
    }

    void Engine::Update(void)
    {
        for (const auto& loopContainer : engine.systemLoops_)
        {
            if (loopContainer.enabled)
            {
                loopContainer.DoLoop();
            }
        }
    }

    void Engine::Exit(void)
    {
        TraceVerbose("Engine: Exit");

        //--------------------------------------------------------------------------
        // NOTE: Shutdown the systems in reverse order.
        //--------------------------------------------------------------------------

        Stopwatch stopwatch{};
        stopwatch.Start();

        // systemCount is used here in case initialization failed. In that case,
        // we only want to shutdown the systems that actually initialized.
        for (int i = static_cast<int>(engine.systemCount_) - 1; i >= 0; --i)
        {
            BaseSystem* system = engine.systems_[(unsigned)i];

            // Write a trace message as each system is exited.
            TraceInfo("{}: Exit", system->GetName());

            // Call the current system's Exit function.
            system->Exit();

            string_t systemName = system->GetName();

            SafeDelete(system);

            // Check how long it took for the system to exit.
            auto systemTime = stopwatch.Lap<msecd_t>();
            TraceVerbose("{} exit in {}", systemName, systemTime);
        }

        stopwatch.Stop();
        TraceVerbose("Engine finished exiting in {}", stopwatch.Duration<msecd_t>());

        // Shutdown the Tracing/Logging module.
        Trace::Shutdown();

        // Shutdown the memory manager. Any memory allocations after this point are likely to cause errors.
        MemoryManager::Shutdown();

#ifdef WINDOWS_THREADS
        DWORD pid = GetCurrentProcessId();
        DWORD threadId = GetCurrentThreadId();
        HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, pid);
        if (h != INVALID_HANDLE_VALUE)
        {
            THREADENTRY32 te;
            te.dwSize = sizeof(te);
            if (Thread32First(h, &te))
            {
                do
                {
                    if (te.dwSize >= FIELD_OFFSET(THREADENTRY32, th32OwnerProcessID) + sizeof(te.th32OwnerProcessID)
                        && te.th32OwnerProcessID == pid)
                    {
                        if (te.th32ThreadID == threadId)
                        {
                            printf("Process 0x%04x Main Thread 0x%04x\n",
                                (unsigned int)te.th32OwnerProcessID, (unsigned int)te.th32ThreadID);
                        }
                        else
                        {
                            printf("Process 0x%04x Thread 0x%04x\n",
                                (unsigned int)te.th32OwnerProcessID, (unsigned int)te.th32ThreadID);
                        }
                    }
                    te.dwSize = sizeof(te);
                } while (Thread32Next(h, &te));
            }
            CloseHandle(h);
        }
#endif
    }

    bool Engine::ShouldExit(void)
    {
        return PlatformSystem::WindowShouldClose() || !SceneSystem::IsRunning();
    }

    const BaseSystem& Engine::GetSystem(Systems system)
    {
        return *engine.systems_[static_cast<unsigned>(system)];
    }

    bool Engine::GetLoopEnabled(EngineLoop loop)
    {
        return engine.systemLoops_[ENUM_VAL(loop)].enabled;
    }

    void Engine::SetLoopEnabled(EngineLoop loop, bool enabled)
    {
        engine.systemLoops_[ENUM_VAL(loop)].enabled = enabled;
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

    bool Engine_::InitConfig(string_view_arg path)
    {
        StreamIn stream{ path };
        if (!stream.IsValid())
        {
            return false;
        }

        string_t tempStr;
        bool tempBool;
        union
        {
            IVec4 vec4;
            IVec3 vec3;
            IVec2 vec2;
        };

        if (stream.TryReadValue(ConfigTraceConsole, tempStr))
        {
            const Trace::Severity& severity = ENUM_READ(Trace::Severity).at(tempStr);
            Trace::SetSeverity(severity);
        }
        if (stream.TryReadValue(ConfigTraceLog, tempStr))
        {
            const Trace::Severity& severity = ENUM_READ(Trace::Severity).at(tempStr);
            Trace::SetSeverityConsole(severity);
        }
        if (stream.StreamHas(ConfigWindowSize))
        {
            IVec2 windowSize{ 1280, 720 };
            stream.ReadValue(ConfigWindowSize, windowSize);
            PlatformSystem::SetWindowSize(windowSize);
        }
        if (stream.TryReadValue(ConfigWindowTitle, tempStr))
        {
            PlatformSystem::SetWindowTitle(tempStr);
        }
        if (stream.StreamHas(ConfigGraphicsVersion))
        {
            vec4 = PlatformSystem::GetGraphicsVersion();
            if (stream.TryReadValue(ConfigGraphicsVersion, vec2) || stream.TryReadValue(ConfigGraphicsVersion, vec3) || stream.TryReadValue(ConfigGraphicsVersion, vec4))
            {
                PlatformSystem::SetGraphicsVersion(vec4);
            }
        }
        if (stream.TryReadValue(ConfigGraphicsProfile, tempStr))
        {
            PlatformSystem::SetGraphicsProfile(tempStr);
        }
        if (stream.TryReadValue(ConfigSceneInit, tempStr))
        {
            SceneSystem::SetNext(tempStr);
        }
        if (stream.TryReadValue(ConfigVSync, tempBool))
        {
            PlatformSystem::SetVSync(tempBool);
        }
        return true;
    }

    void Engine_::AddSystem(BaseSystem* system)
    {
        // Verify that a valid system pointer has been passed.
        assert(system != nullptr && "EngineAddSystem Error: A NULL system pointer has been passed.");

        // Verify that the system container size will not be exceeded.
        assert((engine.systemCount_ < SystemMax) && "EngineAddSystem Error: The maximum container size has been exceeded.");

        // Add the new system to the system container.
        engine.systems_[engine.systemCount_++] = system;
    }

#pragma endregion Private Static Functions

    //--------------------------------------------------------------------------
    // Private Functions:
    //--------------------------------------------------------------------------

#pragma region Private Functions

    void Engine_::LoopSystems::SortLoop(void)
    {
        // Get all systems that participate in the loop.
        end = begin;
        for (unsigned i = 0; i < engine.systemCount_; ++i)
        {
            BaseSystem* system = engine.systems_[i];
            if (system->LoopActive(loop))
            {
                engine.systemLoopIndices_[end++] = static_cast<unsigned char>(i);
            }
        }
        // Sort the systems by their loop priority.
        std::span<unsigned char> sysIndices(engine.systemLoopIndices_.begin() + begin, size());
        std::ranges::sort(sysIndices, [this](const auto& a, const auto& b)
        {
            return engine.systems_[a]->LoopPriority(loop) > engine.systems_[b]->LoopPriority(loop);
        });
        // Update PerformanceSystem.
        PerformanceSystem::SetTrackerCount(PerformanceGroup::System, loop, size());
        for (unsigned i = begin; i < end; ++i)
        {
            BaseSystem& system = *engine.systems_[engine.systemLoopIndices_[i]];

            PerformanceSystem::AssignTrackerOrder(system, PerformanceGroup::System, loop, i - static_cast<unsigned>(begin));
        }
    }

    void Engine_::LoopSystems::DoLoop(void) const
    {
        const string_view_t loopName = ENUM_WRITE_VALUE(EngineLoop, loop);

        TraceDebug("Engine: {}", loopName);

        Stopwatch stopwatch{};
        stopwatch.Start();

        auto& engineTracker = PerformanceSystem::GetEngineTracker();
        if (engineTracker.gfxTimer.IsDone())
        {
            engineTracker.Push(engineTracker.gfxTimer.GetDuration(), loop, PerformanceSource::Graphics);
        }
        const bool gfxTimerReady = engineTracker.gfxTimer.IsReady();

        COMPILER_FORCE(if (gfxTimerReady) { engineTracker.gfxTimer.Start(); });

        //----------------------------------------------------------------------
        // Update all systems.
        //----------------------------------------------------------------------

        for (unsigned i = begin; i != end; ++i)
        {
            unsigned systemIndex = engine.systemLoopIndices_[i];
            BaseSystem& system = *engine.systems_[systemIndex];

            // Write a trace message as each system is updated.
            TraceDebug("{}: {}", system.GetName(), loopName);

            // Call the current system's loop function, surrounded by render timer calls.
            PerformanceTracker& systemTracker = PerformanceSystem::GetSystemTracker(static_cast<Systems>(systemIndex));
            if (systemTracker.gfxTimer.GetResults())
            {
                systemTracker.Push(systemTracker.gfxTimer.GetDuration(), loop, PerformanceSource::Graphics);
            }
            COMPILER_FORCE(systemTracker.gfxTimer.Start());
            COMPILER_FORCE(exec(system));
            COMPILER_FORCE(systemTracker.gfxTimer.Stop());

            // Check how long it took for the system to update.
            auto systemTime = stopwatch.Lap<nsec_t>();
            systemTracker.Push(systemTime, loop, PerformanceSource::CPU);
        }
        COMPILER_FORCE(if (gfxTimerReady) { engineTracker.gfxTimer.Stop(); });

        stopwatch.Stop();
        engineTracker.Push(stopwatch.Duration<nsec_t>(), loop, PerformanceSource::CPU);
    }

#pragma endregion Private Functions

}	// namespace

#pragma warning(pop)
