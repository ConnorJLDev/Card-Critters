//------------------------------------------------------------------------------
//
// File Name:	PlatformWindow.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class representing a window as part of the operating system
//              desktop.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "PlatformWindow.h"

#include "PlatformSystem.h"

#include "GraphicsSystem.h"

#include GRAPHICS_INCLUDE
#include PLATFORM_INCLUDE

#include "PlatformDisplay.h"

#include "CallbackWrapper.inl"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <Windows.h>

#undef max
#undef min

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

    enum class OpenGLProfile : unsigned char
    {
        Core,
        Compatibility,
        NUM_ENTRIES,
    };

    ENUM_MAPS_SEQUENTIAL(OpenGLProfile, Core, Compatibility);

#define glProfile_ glVersion_.w

    //--------------------------------------------------------------------------
    // Private Static Variables:
    //--------------------------------------------------------------------------

    // Internal counter for generating unique callback IDs.
    static CallbackID CallbackIDNext = 1;

    //--------------------------------------------------------------------------
    // Private Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Constructors/Destructors:
    //--------------------------------------------------------------------------

#pragma region Constructors

    PlatformWindow::PlatformWindow(void)
        : NamedObject()
        , windowSize_(1080, 720)
        , glVersion_(3, 3, 0, (decltype(glProfile_))OpenGLProfile::Core)
        , isClosing_(false)
        , isIconified_(false)
        , isFocused_(true)
        , isMaximized_(false)
        , isFullscreen_(false)
        , isBorderlessMaximized_(true)
        , vsync_(true)
        , resizeable_(true)
        , visible_(true)
        , isDecorated_(true)
        , autoIconify_(true)
        , floating_(false)
        , centerCursor_(false)
        , transparentFramebuffer_(false)
        , focusOnShow_(true)
        , scaleToMonitor_(true)
        , scaleFramebuffer_(true)
        , mousePassthrough_(false)
        , fullscreenIndex_(-1)
        , lastWindowPos_()
        , lastWindowSize_(windowSize_)
        , contentScale_(1.0f, 1.0f)
        , windowHandle_(nullptr)
        , resizeCallbacks_()
        , icons_()
    {
    }

    PlatformWindow::PlatformWindow(PlatformWindow&& other) noexcept
        : NamedObject(std::move(other))
        , windowSize_(std::move(other.windowSize_))
        , glVersion_(std::move(other.glVersion_))
        , flags_(std::move(other.flags_))
        , fullscreenIndex_(std::move(other.fullscreenIndex_))
        , lastWindowPos_(std::move(other.lastWindowPos_))
        , lastWindowSize_(std::move(other.lastWindowSize_))
        , contentScale_(std::move(other.contentScale_))
        , windowHandle_(std::exchange(other.windowHandle_, nullptr))
        , resizeCallbacks_(std::move(other.resizeCallbacks_))
        , icons_(std::move(other.icons_))
    {
        if (windowHandle_ != nullptr)
        {
            glfwSetWindowUserPointer(windowHandle_, this);
        }
    }

    //--------------------------------------------------------------------------

    PlatformWindow::~PlatformWindow(void)
    {
        Destroy();
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

#define FlagGLFW(flag) (flag ? GLFW_TRUE : GLFW_FALSE)

    bool PlatformWindow::Create(PlatformWindow* sharedWindow)
    {
        // Window setup hints
        {
#define WindowFlag(hint, flag) glfwWindowHint(hint, FlagGLFW(flag))

            TraceInfo("Initializing window \"{}\" with OpenGL {}.{} {}", GetName(), glVersion_.x, glVersion_.y, GetGraphicsProfile());

            // Graphics libraries hints
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glVersion_.x);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glVersion_.y);
            glfwWindowHint(GLFW_OPENGL_PROFILE, (OpenGLProfile)glProfile_ == OpenGLProfile::Core ? GLFW_OPENGL_CORE_PROFILE : GLFW_OPENGL_COMPAT_PROFILE);
#ifdef GraphicsDebugCallback
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#else
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_FALSE);
#endif
            WindowFlag(GLFW_RESIZABLE, resizeable_);
            WindowFlag(GLFW_VISIBLE, visible_);
            WindowFlag(GLFW_DECORATED, isDecorated_);
            WindowFlag(GLFW_FOCUSED, isFocused_);
            WindowFlag(GLFW_AUTO_ICONIFY, autoIconify_);
            WindowFlag(GLFW_FLOATING, floating_);
            WindowFlag(GLFW_MAXIMIZED, isMaximized_);
            WindowFlag(GLFW_CENTER_CURSOR, centerCursor_);
            WindowFlag(GLFW_TRANSPARENT_FRAMEBUFFER, transparentFramebuffer_);
            WindowFlag(GLFW_FOCUS_ON_SHOW, focusOnShow_);
            WindowFlag(GLFW_SCALE_TO_MONITOR, scaleToMonitor_);
            WindowFlag(GLFW_SCALE_FRAMEBUFFER, scaleFramebuffer_);
            WindowFlag(GLFW_MOUSE_PASSTHROUGH, mousePassthrough_);
            glfwWindowHint(GLFW_STEREO, GLFW_FALSE);
            glfwWindowHint(GLFW_SAMPLES, GLFW_FALSE);
            glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_FALSE);
            glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
            glfwWindowHint(GLFW_POSITION_X, GLFW_ANY_POSITION);
            glfwWindowHint(GLFW_POSITION_Y, GLFW_ANY_POSITION);
        }

        // Get the monitor to display this window as fullscreen on (if enabled)
        DisplayHandle display;
        if (isFullscreen_)
        {
            const PlatformDisplay* monitor = PlatformSystem::GetDisplay(fullscreenIndex_);
            if (monitor == nullptr)
            {
                if (fullscreenIndex_ >= 0)
                {
                    TraceWarning("Failed to find monitor index {} to create fullscreen window on, defaulting to monitor 0!", fullscreenIndex_);
                }
                monitor = PlatformSystem::GetDisplay(0);
                if (monitor == nullptr)
                {
                    TraceWarning("Failed to find any monitor create fullscreen window on!", fullscreenIndex_);
                }
            }
            if (monitor != nullptr)
            {
                lastWindowSize_ = (Vec2)windowSize_ * monitor->GetContentScale();
                const auto& mode = monitor->GetCurrentVideoMode();
                lastWindowPos_ = (windowSize_ - lastWindowSize_) / 2;

                if (isBorderlessMaximized_)
                {
                    windowSize_ = (Vec2)mode.resolution / monitor->GetContentScale();
                    display = nullptr;
                    IVec2 windowPos = monitor->GetVirtualPosition();
                    glfwWindowHint(GLFW_POSITION_X, windowPos.x);
                    glfwWindowHint(GLFW_POSITION_Y, windowPos.y);
                    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
                }
                else
                {
                    windowSize_ = mode.resolution;
                    display = monitor->GetHandle();
                    glfwWindowHint(GLFW_RED_BITS, mode.bits.r);
                    glfwWindowHint(GLFW_GREEN_BITS, mode.bits.g);
                    glfwWindowHint(GLFW_BLUE_BITS, mode.bits.b);
                    glfwWindowHint(GLFW_REFRESH_RATE, mode.refreshRate);
                }
            }
            else
            {
                display = nullptr;
            }
        }
        else
        {
            display = nullptr;
        }

        isFullscreen_ = display != nullptr || isBorderlessMaximized_;

        TraceInfo("Creating window {}x{} titled: \"{}\"", windowSize_.x, windowSize_.y, GetName());

        WindowHandle sharedWindowHandle = sharedWindow == nullptr ? nullptr : sharedWindow->windowHandle_;

        // Create the window using the window size variables and our window title
        windowHandle_ = glfwCreateWindow(windowSize_.x, windowSize_.y, GetName().c_str(), display, sharedWindowHandle);

        // If we failed to create the window, quit
        if (windowHandle_ == nullptr)
        {
            return false;
        }

        glfwSetWindowUserPointer(windowHandle_, this);

        // Icons
        {
            vector_t<GLFWimage> images{};
            for (const Texture& tex : icons_)
            {
                if (!tex.IsValid())
                {
                    TraceWarning("Cannot set texture \"{}\" as icon for window \"{}\": Invalid dimensions or null data!", tex.GetName(), GetName());
                    continue;
                }
                else if (tex.GetChannelType() != Texture::ChannelType::UByte)
                {
                    TraceWarning("Cannot set texture \"{}\" as icon for window \"{}\": Invalid channel type, must be UByte!", tex.GetName(), GetName());
                    continue;
                }
                else if (tex.GetFormat() != Texture::Format::RGBA)
                {
                    TraceWarning("Cannot set texture \"{}\" as icon for window \"{}\": Invalid color format, must be RGBA!", tex.GetName(), GetName());
                    continue;
                }
                const UVec2& texDim = tex.GetDimensions();
                GLFWimage& image = images.emplace_back();
                image.pixels = tex.Data();
                image.width = (int)texDim.x;
                image.height = (int)texDim.y;
            }

            if (!images.empty())
            {
                glfwSetWindowIcon(windowHandle_, (int)images.size(), images.data());
            }
        }

        if (!isFullscreen_ && !isBorderlessMaximized_)
        {
            glfwGetWindowPos(windowHandle_, &lastWindowPos_.x, &lastWindowPos_.y);
            glfwGetWindowSize(windowHandle_, &lastWindowSize_.x, &lastWindowSize_.y);
        }
        glfwGetWindowSize(windowHandle_, &windowSize_.x, &windowSize_.y);
        isFocused_ = (bool)glfwGetWindowAttrib(windowHandle_, GLFW_FOCUSED);
        isIconified_ = (bool)glfwGetWindowAttrib(windowHandle_, GLFW_ICONIFIED);
        isMaximized_ = (bool)glfwGetWindowAttrib(windowHandle_, GLFW_MAXIMIZED);

        glfwMakeContextCurrent(windowHandle_);

#pragma warning(push)
#pragma warning(disable : 4191)
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
#pragma warning(pop)
        {
            return false;
        }

        SetVSync(vsync_);

        // Window callbacks
        glfwSetWindowPosCallback(windowHandle_, CallbackWindowReposition);
        glfwSetWindowSizeCallback(windowHandle_, CallbackWindowResize);
        glfwSetFramebufferSizeCallback(windowHandle_, CallbackBufferResize);
        glfwSetWindowIconifyCallback(windowHandle_, CallbackIconify);
        glfwSetWindowFocusCallback(windowHandle_, CallbackFocus);
        glfwSetWindowCloseCallback(windowHandle_, CallbackClose);
        glfwSetWindowRefreshCallback(windowHandle_, CallbackRefresh);
        glfwSetWindowMaximizeCallback(windowHandle_, CallbackMaximize);
        glfwSetWindowContentScaleCallback(windowHandle_, CallbackContentScale);

        return true;
    }

    void PlatformWindow::Destroy(void)
    {
        if (windowHandle_ != nullptr)
        {
            glfwDestroyWindow(windowHandle_);
        }
    }

    CallbackID PlatformWindow::ViewportResizeCallbackAdd(const std::function<void(const IVec2&)>& func)
    {
        const auto& entry = resizeCallbacks_.emplace_back(func);

        return entry.GetGUID();
    }

    void PlatformWindow::ViewportResizeCallbackRemove(CallbackID funcId)
    {
        std::erase_if(resizeCallbacks_, [funcId](const auto& entry) { return entry.GetGUID() == funcId; });
    }

    bool PlatformWindow::WindowShouldClose() const
    {
        return static_cast<bool>(glfwWindowShouldClose(windowHandle_));
    }

    void PlatformWindow::SwapBuffers() const
    {
        glfwSwapBuffers(windowHandle_);
    }

    const IVec4& PlatformWindow::GetGraphicsVersion() const
    {
        return glVersion_;
    }

    void PlatformWindow::SetGraphicsVersion(const IVec4& version)
    {
        assert(windowHandle_ == nullptr);
        glVersion_ = version;
    }

    string_view_arg PlatformWindow::GetGraphicsProfile() const
    {
        return ENUM_WRITE_VALUE(OpenGLProfile, glProfile_);
    }

    void PlatformWindow::SetGraphicsProfile(string_view_arg version)
    {
        assert(windowHandle_ == nullptr);
        glProfile_ = (decltype(glProfile_))ENUM_READ(OpenGLProfile).at(version);
    }

    const IVec2& PlatformWindow::GetWindowSize() const
    {
        return windowSize_;
    }

    void PlatformWindow::SetWindowSize(const IVec2& size)
    {
        if (windowHandle_ != nullptr)
        {
            CallbackBufferResize(windowHandle_, size.x, size.y);
        }
        else
        {
            windowSize_ = size;
        }
    }

    const string_t& PlatformWindow::GetWindowTitle() const
    {
        return GetName();
    }

    void PlatformWindow::SetWindowTitle(string_view_arg title)
    {
        SetName(title);
        if (windowHandle_ != nullptr)
        {
            glfwSetWindowTitle(windowHandle_, GetName().c_str());
        }
    }

    bool PlatformWindow::GetVSync() const
    {
        return vsync_;
    }

    void PlatformWindow::SetVSync(bool vsync)
    {
        vsync_ = vsync;
        if (windowHandle_ != nullptr)
        {
            glfwMakeContextCurrent(windowHandle_);
            glfwSwapInterval(vsync_ ? 1 : 0);
        }
    }

    bool PlatformWindow::GetFullscreen(void) const
    {
        return isFullscreen_;
    }

    void PlatformWindow::SetFullscreen(bool fullscreen, int displayIndex, bool borderlessMax)
    {
        fullscreenIndex_ = displayIndex;
        if (windowHandle_ != nullptr)
        {
            if (fullscreen)
            {
                const PlatformDisplay* monitor = PlatformSystem::GetDisplay(displayIndex);
                if (monitor == nullptr)
                {
                    if (displayIndex >= 0)
                    {
                        TraceWarning("Failed to fullscreen window \"{}\" to display {}, using current display instead!", GetName(), displayIndex);
                    }
                    displayIndex = GetDisplayIndex();
                    monitor = PlatformSystem::GetDisplay(displayIndex);
                    if (monitor == nullptr)
                    {
                        displayIndex = 0;
                        monitor = PlatformSystem::GetDisplay(displayIndex);
                    }
                    fullscreenIndex_ = displayIndex;
                }
                if (monitor == nullptr)
                {
                    TraceWarning("Failed to fullscreen window \"{}\", no displays found!", GetName());
                    return;
                }
                // If we weren't previously fullscreen, store the last windowed position and size.
                if (!isFullscreen_)
                {
                    glfwGetWindowPos(windowHandle_, &lastWindowPos_.x, &lastWindowPos_.y);
                    glfwGetWindowSize(windowHandle_, &lastWindowSize_.x, &lastWindowSize_.y);
                }
                // Fullscreen with the active video mode.
                const auto& mode = monitor->GetCurrentVideoMode();

                // Set fullscreen flag before setting monitor so callbacks behave correctly.
                isFullscreen_ = fullscreen;
                isBorderlessMaximized_ = borderlessMax;

                IVec2 windowPos, windowSize;
                int refreshRate;
                DisplayHandle displayHandle;

                if (isBorderlessMaximized_)
                {
                    displayHandle = NULL;
#ifdef USE_WORK_AREA
                    windowPos = monitor->GetWorkAreaPosition();
                    windowSize = monitor->GetWorkAreaSize();
#else
                    windowPos = monitor->GetVirtualPosition();
                    windowSize = mode.resolution;
#endif
                    glfwSetWindowAttrib(windowHandle_, GLFW_DECORATED, GLFW_FALSE);
#ifdef WIN32
                    // In Windows, a style flag is set that causes fullscreen optimizations which lead to stale frames on focus loss and black flickers on focus change.
                    HWND hWnd = glfwGetWin32Window(windowHandle_);
                    DWORD style = (DWORD)GetWindowLong(hWnd, GWL_STYLE);
                    // WS_POPUP is the main offender.
                    style &= (DWORD)~(WS_POPUP);
                    SetWindowLong(hWnd, GWL_STYLE, (LONG)style);
#endif
                    refreshRate = GLFW_DONT_CARE;
                }
                else
                {
                    displayHandle = monitor->GetHandle();
                    windowPos = IVec2(0, 0);
                    windowSize = mode.resolution;
                    refreshRate = mode.refreshRate;
                }
                glfwSetWindowMonitor(windowHandle_, displayHandle,
                    windowPos.x, windowPos.y,
                    windowSize.x, windowSize.y,
                    refreshRate);
                // TODO: Repaint window to fix briefly distorted frame.
            }
            else
            {
                // If fullscreen is being disabled, restore the window to its last position and size.
                glfwSetWindowMonitor(windowHandle_, NULL,
                    lastWindowPos_.x, lastWindowPos_.y,
                    lastWindowSize_.x, lastWindowSize_.y,
                    GLFW_DONT_CARE);
                // Make sure the decoration state is restored.
                glfwSetWindowAttrib(windowHandle_, GLFW_DECORATED, FlagGLFW(isDecorated_));
            }
        }
        isFullscreen_ = fullscreen;
    }

    int PlatformWindow::GetDisplayIndex(void)
    {
        // If this window is fullscreen, we can easily get the monitor used.
        GLFWmonitor* monitor = glfwGetWindowMonitor(windowHandle_);
        const PlatformDisplay* data;
        isFullscreen_ = monitor != nullptr;
        if (isFullscreen_)
        {
            data = reinterpret_cast<const PlatformDisplay*>(glfwGetMonitorUserPointer(monitor));
            assert(data != nullptr && "Monitor struct and glfwMonitor should be bound to each other!");
            fullscreenIndex_ = (int)data->GetDisplayIndex(); // Should already be the same, but this makes sure fullscreenIndex_ is accurate.
            return fullscreenIndex_;
        }
        // If we're not fullscreen, find the display with the most overlap with this window. Fun!
        glfwGetWindowPos(windowHandle_, &lastWindowPos_.x, &lastWindowPos_.y);
        glfwGetWindowSize(windowHandle_, &lastWindowSize_.x, &lastWindowSize_.y);
        data = nullptr;
        size_t maxOverlap = 0;
        int count = PlatformSystem::GetDisplayCount();
        for (int i = 0; i < count; ++i)
        {
            const PlatformDisplay& monitorData = *PlatformSystem::GetDisplay(i);
            const IVec2& monitorPos = monitorData.GetVirtualPosition();
            IVec2 windowCorner = lastWindowPos_ + lastWindowSize_, monitorCorner = monitorPos + monitorData.GetCurrentVideoMode().resolution;
            // If the window is outside of the bounds of the monitor, skip.
            if (lastWindowPos_.x >= monitorCorner.x || windowCorner.x < monitorPos.x ||
                lastWindowPos_.y >= monitorCorner.y || windowCorner.y < monitorPos.y)
            {
                continue;
            }
            IVec2 maxPos = MathMax(lastWindowPos_, monitorPos);
            IVec2 minPos = MathMin(windowCorner, monitorCorner);
            IVec2 rect = MathAbs(maxPos - minPos);
            size_t overlap = (size_t)rect.x * (size_t)rect.y;
            if (overlap > maxOverlap || data == nullptr)
            {
                maxOverlap = overlap;
                data = &monitorData;
            }
        }
        return data == nullptr ? -1 : (int)data->GetDisplayIndex();
    }

    bool PlatformWindow::GetIsIconified(void) const
    {
        return isIconified_;
    }

    bool PlatformWindow::GetIsFocused(void) const
    {
        return isFocused_ || isChildFocused_;
    }

    bool PlatformWindow::CheckChildFocus(void)
    {
        bool childFocused =
#ifdef WIN32
            GetActiveWindow() != NULL
#else
            false
#endif
            ;
        isChildFocused_ = childFocused;
        return childFocused;
    }

#pragma endregion Public Functions

    //--------------------------------------------------------------------------
    // Private Static Functions:
    //--------------------------------------------------------------------------

#pragma region Private Static Functions

#define CALLBACK_COMMON\
    PlatformWindow* windowPtr = reinterpret_cast<PlatformWindow*>(glfwGetWindowUserPointer(windowHandle));\
    assert(windowPtr != nullptr && "Window user pointer must point to associated PlatformWindow!");\
    PlatformWindow& window = *windowPtr;\
    assert(window.windowHandle_ == windowHandle);\

    void PlatformWindow::CallbackWindowReposition(WindowHandle windowHandle, int xpos, int ypos) noexcept
    {
        CALLBACK_COMMON;

        if (!window.isFullscreen_)
        {
            window.lastWindowPos_.x = xpos;
            window.lastWindowPos_.y = ypos;
        }
    }

    void PlatformWindow::CallbackWindowResize(WindowHandle windowHandle, int width, int height) noexcept
    {
        CALLBACK_COMMON;

        if (!window.isFullscreen_)
        {
            window.lastWindowSize_.x = width;
            window.lastWindowSize_.y = height;
        }
    }

    void PlatformWindow::CallbackBufferResize(WindowHandle windowHandle, int width, int height) noexcept
    {
        CALLBACK_COMMON;

        window.windowSize_.x = width;
        window.windowSize_.y = height;

        for (const auto& resizeCallback : window.resizeCallbacks_)
        {
            resizeCallback(window.windowSize_);
        }
    }

    void PlatformWindow::CallbackIconify(WindowHandle windowHandle, int iconified) noexcept
    {
        CALLBACK_COMMON;

        window.isIconified_ = (bool)iconified;
    }

    void PlatformWindow::CallbackFocus(WindowHandle windowHandle, int focused) noexcept
    {
        CALLBACK_COMMON;

        window.isFocused_ = (bool)focused;
        window.CheckChildFocus();
    }

    void PlatformWindow::CallbackMaximize(WindowHandle windowHandle, int maximized) noexcept
    {
        CALLBACK_COMMON;

        window.isMaximized_ = (bool)maximized;
    }

    void PlatformWindow::CallbackClose(WindowHandle windowHandle) noexcept
    {
        CALLBACK_COMMON;

        window.isClosing_ = true;
    }

    void PlatformWindow::CallbackRefresh(WindowHandle windowHandle) noexcept
    {
        CALLBACK_COMMON;

        UNREFERENCED_PARAMETER(window);
    }

    void PlatformWindow::CallbackContentScale(WindowHandle windowHandle, float xscale, float yscale) noexcept
    {
        CALLBACK_COMMON;

        window.contentScale_.x = xscale;
        window.contentScale_.y = yscale;
    }

#pragma endregion Private Static Functions

    //--------------------------------------------------------------------------
    // Private Functions:
    //--------------------------------------------------------------------------

#pragma region Private Functions

#pragma endregion Private Functions

}	// namespace
