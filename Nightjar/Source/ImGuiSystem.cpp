//------------------------------------------------------------------------------
//
// File Name:	ImGuiSystem.cpp
// Author(s):	Barend Brink (barend.brink), Connor Lariviere (connor.lariviere)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		DEAR IMGUI system.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiSystem.h"

#include "ImGuiInclude.h"
#include "imgui_internal.h"

#include "PlatformSystem.h"
#include PLATFORM_INCLUDE

#include "ImGuiWindowManager.h"
#include "ImGuiLayoutManager.h"
#include "ImGuiMenuBar.h"
#include "ImGuiToolbar.h"
#include "ImGuiStatusBar.h"

#include "SceneSystem.h"

#include "PerformanceSystem.h"

//------------------------------------------------------------------------------
// Preprocessor Directives:
//------------------------------------------------------------------------------

#define WINDOW PlatformSystem::GetWindowHandle()

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

    class ImGuiSystem_ SYSTEM_FINAL : public BaseSystem
    {
        friend ImGuiSystem;

        SYSTEM_COMMON(ImGuiSystem_);

        // Public Constants and Enums:
    public:

        // Constructors/Destructors:
    private:
        ImGuiSystem_(void);

        // All systems need a virtual destructor to have their destructor called 
        ~ImGuiSystem_(void) override;

        // Public Static Functions:
    public:

        // Public Functions:
    public:

        // Private Constants, Enums, and Structs:
    private:
        //
        static constexpr size_t memId_imgui = MEM_ID("IMGUI");

        // Dummy object struct used for tracking 
        struct DummyObject : public NamedObject
        {
            DummyObject(string_view_arg name)
                : NamedObject(name)
            {
            }
        };

        // Private Static Functions:
    private:
        /// <summary>
        /// 
        /// </summary>
        /// <param name="size"></param>
        /// <param name="userData"></param>
        /// <returns></returns>
        static void* AllocatorAllocate(size_t size, void* userData) noexcept;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="ptr"></param>
        /// <param name="userData"></param>
        static void AllocatorDeallocate(void* ptr, void* userData) noexcept;

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
        /// 
        /// </summary>
        void ShowDockspace(void);

        // Private Static Variables:
    private:
        //
        static ImGuiSystem_* instance;

        // Private Variables:
    private:
        // 
        ImGuiContext* MainContext;
        // 
        ImGuiID g_DockspaceId;
        // 
        bool ShowDebug;
        // LayoutManager singleton instance.
        ImGuiLayoutManager settings_;
        // WindowManager singleton instance.
        ImGuiWindowManager windows_;
        // MenuBar singleton instance.
        ImGuiMenuBar menuBar_;
        // Toolbar singleton instance.
        ImGuiToolbar toolbar_;
        // StatusBar singleton instance.
        ImGuiStatusBar statusBar_;
        //
        array_t<DummyObject, 4> trackers_;
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

    ImGuiSystem_* ImGuiSystem_::instance = nullptr;

#define inst ImGuiSystem_::instance

    //--------------------------------------------------------------------------
    // Private Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Constructors/Destructors:
    //--------------------------------------------------------------------------

#pragma region Constructors

    ImGuiSystem_::ImGuiSystem_(void)
        : BaseSystem("ImGuiSystem", LoopFlags::HasUpdate | LoopFlags::HasRender)
        , MainContext()
        , g_DockspaceId(0)
#ifdef _DEBUG
        , ShowDebug(true)
#else
        , ShowDebug(false)
#endif
        , settings_()
        , windows_()
        , menuBar_()
        , toolbar_()
        , statusBar_()
        , trackers_{
            DummyObject{"MenuBar"},
            DummyObject{"Toolbar"},
            DummyObject{"StatusBar"},
            DummyObject{"Docker"},
        }
    {
        // Raise an assert if this system has already been created.
        assert(instance == nullptr);

        // Store this system's instance for use by static functions.
        instance = this;
    }

    //--------------------------------------------------------------------------

    ImGuiSystem_::~ImGuiSystem_(void)
    {
    }

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

    BaseSystem* ImGuiSystem::GetInstance()
    {
        return inst != nullptr ? inst : new ImGuiSystem_();
    }

    bool ImGuiSystem::GetEnabled(void)
    {
        return inst->ShowDebug;
    }

    void ImGuiSystem::SetEnabled(bool enable)
    {
        inst->ShowDebug = enable;
    }

    bool ImGuiSystem::IsKeyboardCaptured(void)
    {
        // WantCaptureKeyboard: True if a window is active (for navigation, etc).
        // WantTextInput: True if a text field is being edited.
        return inst != nullptr && inst->ShowDebug && ImGui::GetIO().WantTextInput;
    }

    bool ImGuiSystem::IsMouseCaptured(void)
    {
        return inst != nullptr && inst->ShowDebug && ImGui::GetIO().WantCaptureMouse;
    }

    void ImGuiSystem::SetUiSize(float uiscale)
    {
        ImGui::GetIO().FontGlobalScale = uiscale;
    }

    void ImGuiSystem::SetUiStyle(ColorStyle style)
    {
        switch (style)
        {
        case ColorStyle::Dark:
            ImGui::StyleColorsDark();
            break;
        case ColorStyle::Light:
            ImGui::StyleColorsLight();
            break;
        case ColorStyle::Classic:
            ImGui::StyleColorsClassic();
            break;
        }
    }

    void ImGuiSystem_::ShowDockspace()
    {
        ImGui::SetCurrentContext(MainContext);
        static bool dockspaceOpen = true;
        //IF THIS DOESNT HAVE 	ImGuiDockNodeFlags_PassthruCentralNode THIS MAKES IT NOT TRANSPARENT/WORKS
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_AutoHideTabBar;

        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x, viewport->WorkPos.y));
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        // Styles
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

        ImGuiWindowFlags window_flags =
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoNavFocus |
            ImGuiWindowFlags_NoBackground;

        // Make THIS window truly transparent (alpha = 0)
        // WindowBg
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));

        ImGui::Begin("DockSpaceWindow", &dockspaceOpen, window_flags);

        g_DockspaceId = ImGui::GetID("MyDockSpace");

        ImGui::DockSpace(g_DockspaceId, ImVec2(0.0f, 0.0f), dockspace_flags);

        ImGui::End();

        ImGui::PopStyleColor();
        // WindowBg
        ImGui::PopStyleVar(3);
        // Styles
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

    void* ImGuiSystem_::AllocatorAllocate(size_t size, [[maybe_unused]] void* userData) noexcept
    {
        return MemoryManager::AllocateNoexcept(size, memId_imgui);
    }

    void ImGuiSystem_::AllocatorDeallocate(void* ptr, [[maybe_unused]] void* userData) noexcept
    {
        MemoryManager::Deallocate(ptr, memId_imgui);
    }

#pragma endregion Private Static Functions

    //--------------------------------------------------------------------------
    // Private Functions:
    //--------------------------------------------------------------------------

#pragma region Private Functions

    bool ImGuiSystem_::Init(void)
    {
        ImGui::SetAllocatorFunctions(AllocatorAllocate, AllocatorDeallocate);

        MainContext = ImGui::CreateContext();
        ImGui::GetIO().UserData = new ImGuiContextData();
        
        if (!ImGuiLayoutManager::Init())
        {
            return false;
        }

        if (!ImGuiWindowManager::Init())
        {
            return false;
        }

        if (!ImGuiMenuBar::Init())
        {
            return false;
        }

        if (!ImGuiToolbar::Init())
        {
            return false;
        }

        if (!ImGuiStatusBar::Init())
        {
            return false;
        }

        ImGuiLayoutManager::LoadLayout();

        bool suc = ImGui_ImplGlfw_InitForOpenGL(WINDOW, true);
        if (!suc)
        {
            TraceError("Failed to initialize ImGui for Glfw/OpenGL!");
            return false;
        }
        ImGui_ImplOpenGL3_Init("#version 330");

        ImGuiIO& io = ImGui::GetIO();
        io.FontGlobalScale = 1.0f;
        io.ConfigFlags |= 
            ImGuiConfigFlags_NavEnableKeyboard |
            ImGuiConfigFlags_DockingEnable |
            ImGuiConfigFlags_ViewportsEnable;

        ImGui::StyleColorsDark();
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f; // fully opaque for platform windows
        style.DockingNodeHasCloseButton = false;

        {
            // In order to properly initialize the window selection, a frame has to be fully rendered.

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            menuBar_.Update();
            toolbar_.Update();
            statusBar_.Update();
            ShowDockspace();

            ImGuiWindowManager::InitWindowSelection();

            ImGui::Render();
            ImGui::UpdatePlatformWindows();

            ImGui::SetCurrentContext(MainContext);
        }

        {
            PerformanceSystem::SetTrackerCount(PerformanceGroup::ImGuiWindow, EngineLoop::Update, 4);
            for (unsigned i = 0; i < trackers_.size(); ++i)
            {
                PerformanceSystem::AssignTrackerOrder(trackers_[i], PerformanceGroup::ImGuiWindow, EngineLoop::Update, i);
            }
        }


        // Return true if the initialization completed successfully.
        return true;
    }

    void ImGuiSystem_::Update(void)
    {
        auto& ctx = ImGuiContextData::GetContext();
        
        if (SceneSystem::IsChanging())
        {
            ctx.selectedEntity_ = nullptr;
            ctx.selectedEntities_.clear();
        }

        // If debug is disabled, or the window isn't visible or isn't focused, don't update anything.
        if (!ShowDebug || PlatformSystem::GetIsIconified() || !PlatformSystem::GetIsFocused())
        {
            return;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        Stopwatch stopwatch{};
        unsigned trackerIndex = 0;
        stopwatch.Start();
        menuBar_.Update();
        {
            auto time = stopwatch.Lap<nsec_t>();
            auto& tracker = PerformanceSystem::GetTracker(trackers_[trackerIndex++], *this);
            tracker.Push(time, EngineLoop::Update, PerformanceSource::CPU);
        }
        toolbar_.Update();
        {
            auto time = stopwatch.Lap<nsec_t>();
            auto& tracker = PerformanceSystem::GetTracker(trackers_[trackerIndex++], *this);
            tracker.Push(time, EngineLoop::Update, PerformanceSource::CPU);
        }
        statusBar_.Update();
        {
            auto time = stopwatch.Lap<nsec_t>();
            auto& tracker = PerformanceSystem::GetTracker(trackers_[trackerIndex++], *this);
            tracker.Push(time, EngineLoop::Update, PerformanceSource::CPU);
        }
        ShowDockspace();
        {
            auto time = stopwatch.Lap<nsec_t>();
            auto& tracker = PerformanceSystem::GetTracker(trackers_[trackerIndex++], *this);
            tracker.Push(time, EngineLoop::Update, PerformanceSource::CPU);
        }

        ImGuiWindowManager::Windows->UpdateWindows();
    }

    void ImGuiSystem_::Render(void)
    {
        // If debug is disabled, or the window isn't visible or isn't focused, don't render anything.
        if (!ShowDebug || PlatformSystem::GetIsIconified() || !PlatformSystem::GetIsFocused())
        {
            return;
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void ImGuiSystem_::Exit(void)
    {
        ImGuiStatusBar::Exit();
        ImGuiToolbar::Exit();
        ImGuiMenuBar::Exit();
        ImGuiWindowManager::Exit();
        ImGuiLayoutManager::Exit();

        ImGuiContextData*& ctx = reinterpret_cast<ImGuiContextData*&>(static_cast<void*&>(ImGui::GetIO().UserData));
        SafeDeleteCheck(ctx);
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        // Normally, about 128 bytes are leaked due to "ImVector<ImGui_ImplGlfw_WindowToContext> g_ContextMap" in imgui_impl_glfw:198, as the vector is never freed.
        // This has been directly patched and surrounded with comment guards indicating the patch.
    }

#pragma endregion Private Functions

}	// namespace
