//------------------------------------------------------------------------------
//
// File Name:	PlatformSystem.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200
// Project:		Card Critters
// Purpose:		System for handling platform-related tasks like window management.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "PlatformSystem.h"

#include "BaseSystem.h"

#include "GraphicsSystem.h"

#include GRAPHICS_INCLUDE
#include PLATFORM_INCLUDE

#include "PlatformWindow.h"
#include "PlatformDisplay.h"

#include "CallbackWrapper.inl"

#include "Engine.h"
#include "EngineLoop.h"

#include "Windows.h"
#undef max
#undef min
#include <psapi.h>

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

    class PlatformSystem_ SYSTEM_FINAL : public BaseSystem
    {
        friend PlatformSystem;

        SYSTEM_COMMON(PlatformSystem_);

        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    private:
        PlatformSystem_(void);

        // All systems need a virtual destructor to have their destructor called 
        ~PlatformSystem_(void) override;

        // Public Static Functions:
    public:

        // Public Functions:
    public:

        // Public Event Handlers
    public:

        // Private Constants, Enums, and Structs:
    private:
        //
        static constexpr size_t memId_glfw = MEM_ID("GLFW");

        // Private Static Functions:
    private:
        //
        static void MonitorCallback(GLFWmonitor* monitor, int event) noexcept;

        //
        static void* AllocatorAllocate(size_t size, void* user) noexcept;

        //
        static void* AllocatorReallocate(void* block, size_t size, void* user) noexcept;

        //
        static void AllocatorDeallocate(void* block, void* user) noexcept;

        //
        static size_t GetMemoryUsage(void);

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

        /// <summary>
        /// Gets the priority of the system for a given engine loop.
        /// </summary>
        /// <param name="loop">The engine loop to check.</param>
        /// <returns>An int indicating how early (relative to other systems) to run this system in the given loop.</returns>
        int LoopPriority(EngineLoop loop) const override;

        // @breif Refresh monitor data.
        void RefreshMonitors(void);

        // Private Static Variables:
    private:
        //
        static PlatformSystem_* instance;

        //
        static CallbackID CallbackID_;

        // Private Variables:
    private:
        double timeLast_;
        float timeDelta_;
        size_t frameCount_;
        vector_t<PlatformDisplay> displays_;
        vector_t<PlatformWindow> windows_;
        vector_t<CallbackWrapper<void, const PlatformDisplay&>> displayCallbacks_;
        GLFWallocator allocator_;
        // This bool captures the window's focus state at the beginning of each
        // update loop, to prevent changes mid-update or mid-render.
        bool delayedFocus_;
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

    //--------------------------------------------------------------------------
    // Private Static Variables:
    //--------------------------------------------------------------------------

    PlatformSystem_* PlatformSystem_::instance = nullptr;

#define inst PlatformSystem_::instance

#define instWindow PlatformSystem_::instance->windows_[0]

    CallbackID PlatformSystem_::CallbackID_ = 1;

    //--------------------------------------------------------------------------
    // Private Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Constructors/Destructors:
    //--------------------------------------------------------------------------

#pragma region Constructors

    PlatformSystem_::PlatformSystem_(void)
        : BaseSystem("PlatformSystem", LoopFlags::HasUpdate | LoopFlags::HasRender)
        , timeLast_()
        , timeDelta_()
        , frameCount_(0)
        , displays_()
        , windows_(1)
        , displayCallbacks_()
        , allocator_()
        , delayedFocus_(false)
    {
        // Raise an assert if this system has already been created.
        assert(instance == nullptr);

        // Store this system's instance for use by static functions.
        instance = this;
    }

    //--------------------------------------------------------------------------

    PlatformSystem_::~PlatformSystem_(void)
    {
    }

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

    BaseSystem* PlatformSystem::GetInstance()
    {
        return inst != nullptr ? inst : new PlatformSystem_();
    }

    CallbackID PlatformSystem::ViewportResizeCallbackAdd(const std::function<void(const IVec2&)>& func)
    {
        return instWindow.ViewportResizeCallbackAdd(func);
    }

    void PlatformSystem::ViewportResizeCallbackRemove(CallbackID funcId)
    {
        instWindow.ViewportResizeCallbackRemove(funcId);
    }

    CallbackID PlatformSystem::DisplayChangeCallbackAdd(const std::function<void(const PlatformDisplay&)>& func)
    {
        const auto& entry = inst->displayCallbacks_.emplace_back(func);

        return entry.GetGUID();
    }

    void PlatformSystem::DisplayChangeCallbackRemove(CallbackID funcId)
    {
        std::erase_if(inst->displayCallbacks_, [funcId](const auto& entry) { return entry.GetGUID() == funcId; });
    }

    bool PlatformSystem::WindowShouldClose()
    {
        return instWindow.WindowShouldClose();
    }

    void PlatformSystem::SwapBuffers()
    {
        instWindow.SwapBuffers();
    }

    float PlatformSystem::GetDeltaTime()
    {
        return inst->timeDelta_;
    }

    const size_t& PlatformSystem::GetFrameCount()
    {
        return inst->frameCount_;
    }

    double PlatformSystem::GetRunTime()
    {
        return glfwGetTime();
    }

    const IVec4& PlatformSystem::GetGraphicsVersion()
    {
        return instWindow.GetGraphicsVersion();
    }

    void PlatformSystem::SetGraphicsVersion(const IVec4& version)
    {
        instWindow.SetGraphicsVersion(version);
    }

    string_view_arg PlatformSystem::GetGraphicsProfile()
    {
        return instWindow.GetGraphicsProfile();
    }

    void PlatformSystem::SetGraphicsProfile(string_view_arg profile)
    {
        instWindow.SetGraphicsProfile(profile);
    }

    const IVec2& PlatformSystem::GetWindowSize()
    {
        return instWindow.GetWindowSize();
    }

    void PlatformSystem::SetWindowSize(const IVec2& size)
    {
        instWindow.SetWindowSize(size);
    }

    const string_t& PlatformSystem::GetWindowTitle()
    {
        return instWindow.GetWindowTitle();
    }

    void PlatformSystem::SetWindowTitle(string_view_arg title)
    {
        instWindow.SetWindowTitle(title);
    }

    bool PlatformSystem::GetVSync()
    {
        return instWindow.GetVSync();
    }

    void PlatformSystem::SetVSync(bool vsync)
    {
        instWindow.SetVSync(vsync);
    }

    bool PlatformSystem::GetFullscreen(void)
    {
        return instWindow.GetFullscreen();
    }

    void PlatformSystem::SetFullscreen(bool fullscreen, int displayIndex)
    {
        instWindow.SetFullscreen(fullscreen, displayIndex);
    }

    int PlatformSystem::GetDisplayIndex(void)
    {
        return instWindow.GetDisplayIndex();
    }

    int PlatformSystem::GetDisplayCount(void)
    {
        return (int)inst->displays_.size();
    }

    PlatformDisplay* PlatformSystem::GetDisplay(int index)
    {
        unsigned i = (unsigned)index;
        if (i >= inst->displays_.size())
        {
            return nullptr;
        }
        PlatformDisplay* display = &inst->displays_[i];
        return display;
    }

    PlatformDisplay* PlatformSystem::GetDisplay(string_view_arg name)
    {
        for (auto& display : inst->displays_)
        {
            if (display.IsNamed(name))
            {
                return &display;
            }
        }
        return nullptr;
    }

    PlatformWindow* PlatformSystem::GetWindow(int index)
    {
        unsigned i = (unsigned)index;
        if (i >= inst->windows_.size())
        {
            return nullptr;
        }
        PlatformWindow* window = &inst->windows_[i];
        return window;
    }

    PlatformWindow* PlatformSystem::GetWindow(string_view_arg name)
    {
        for (auto& window : inst->windows_)
        {
            if (window.IsNamed(name))
            {
                return &window;
            }
        }
        return nullptr;
    }

    bool PlatformSystem::GetIsIconified(void)
    {
        return instWindow.GetIsIconified();
    }

    bool PlatformSystem::GetIsFocused(void)
    {
        return inst->delayedFocus_;
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

    WindowHandle PlatformSystem::GetWindowHandle()
    {
        return instWindow.windowHandle_;
    }

    void PlatformSystem_::MonitorCallback(GLFWmonitor* monitor, int event) noexcept
    {
        UNREFERENCED_PARAMETER(monitor);
        UNREFERENCED_PARAMETER(event); // GLFW_CONNECTED GLFW_DISCONNECTED

        inst->RefreshMonitors();
    }

    void* PlatformSystem_::AllocatorAllocate(size_t size, void*) noexcept
    {
        return MemoryManager::AllocateNoexcept(size, memId_glfw);
    }

    void* PlatformSystem_::AllocatorReallocate(void* block, size_t size, void*) noexcept
    {
        return MemoryManager::ReallocateNoexcept(block, size, memId_glfw);
    }

    void PlatformSystem_::AllocatorDeallocate(void* block, void*) noexcept
    {
        MemoryManager::Deallocate(block, memId_glfw);
    }

    size_t PlatformSystem_::GetMemoryUsage(void)
    {
        PROCESS_MEMORY_COUNTERS pmc;
        if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
        {
            return static_cast<size_t>(pmc.WorkingSetSize);
        }
        return 0;
    }

#pragma endregion Private Static Functions

    //--------------------------------------------------------------------------
    // Private Functions:
    //--------------------------------------------------------------------------

#pragma region Private Functions

    bool PlatformSystem_::Init(void)
    {
        // Initialize glfw

        allocator_.allocate = AllocatorAllocate;
        allocator_.reallocate = AllocatorReallocate;
        allocator_.deallocate = AllocatorDeallocate;

        glfwInitAllocator(&allocator_);

        glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WIN32);
        // If glfw did not initialize correctly, quit
        if (glfwInit() == GLFW_FALSE)
        {
            return false;
        }

        // 
        RefreshMonitors();
        glfwSetMonitorCallback(MonitorCallback);
#ifndef _DEBUG
        instWindow.SetFullscreen(true, 0);
#endif
        // Initialize the window
        if (!instWindow.Create())
        {
            return false;
        }

        timeLast_ = PlatformSystem::GetRunTime();

        // Return true if the initialization completed successfully.
        return true;
    }

    void PlatformSystem_::Update(void)
    {
        instWindow.CheckChildFocus();
        bool oldFocus = delayedFocus_;
        delayedFocus_ = instWindow.GetIsFocused();

        // Update delta time
        double timeCurrent = PlatformSystem::GetRunTime();
        timeDelta_ = static_cast<float>(timeCurrent - timeLast_);
        timeLast_ = timeCurrent;

        ++frameCount_;

        // Handle the window events
        glfwPollEvents();

        if (!delayedFocus_)
        {
            static double SleepUntil(0.0);
            if (oldFocus)
            {
                SleepUntil = timeCurrent;
            }
            static constexpr int SleepUpdatesPerSecond = 20;
            static constexpr float SleepInterval = 1.0f / (float)SleepUpdatesPerSecond;
            SleepUntil += SleepInterval;
            double sleepTime = SleepUntil - timeCurrent;
            if (sleepTime > 0.0)
            {
                std::this_thread::sleep_for(secd_t(sleepTime));
            }
        }

        // If the window isn't visible or isn't focused, don't render anything.
        const bool shouldRender = !PlatformSystem::GetIsIconified() && PlatformSystem::GetIsFocused();
        if (Engine::GetLoopEnabled(EngineLoop::Render))
        {
            if (!shouldRender)
            {
                Engine::SetLoopEnabled(EngineLoop::Render, shouldRender);
            }
        }
        else if (shouldRender)
        {
            Engine::SetLoopEnabled(EngineLoop::Render, shouldRender);
        }
    }

    void PlatformSystem_::Render(void)
    {
        // Complete the draw process for the current game loop.
        PlatformSystem::SwapBuffers();
    }

    void PlatformSystem_::Exit(void)
    {
        windows_.clear();
        windows_.shrink_to_fit();
        displayCallbacks_.clear();
        displayCallbacks_.shrink_to_fit();
        displays_.clear();
        displays_.shrink_to_fit();

        glfwMakeContextCurrent(NULL);

        // Shut down glfw and OpenGL
        glfwTerminate();
    }

    int PlatformSystem_::LoopPriority(EngineLoop loop) const
    {
        switch (loop)
        {
        case EngineLoop::Update:
            return std::numeric_limits<int>::max();
        case EngineLoop::Render:
            return std::numeric_limits<int>::min();
        default:
            return 0;
        }
    }

    void PlatformSystem_::RefreshMonitors(void)
    {
        int count;
        GLFWmonitor** monitors = glfwGetMonitors(&count);

        displays_.clear();
        displays_.reserve((size_t)count);

        for (int i = 0; i < count; ++i)
        {
            GLFWmonitor* monitor = monitors[(unsigned)i];
            assert(monitor != nullptr && "GLFW should never return a NULL monitor!");

            auto& data = displays_.emplace_back(monitor);
            data.monitorIndex_ = (decltype(data.monitorIndex_))i;
        }
    }

#pragma endregion Private Functions

}	// namespace
