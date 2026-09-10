//------------------------------------------------------------------------------
//
// File Name:	PlatformDisplay.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class representing a display connected to the computer.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "PlatformDisplay.h"

#include "PlatformSystem.h"

#include "GraphicsSystem.h"

#include GRAPHICS_INCLUDE
#include PLATFORM_INCLUDE

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

    PlatformDisplay::PlatformDisplay(DisplayHandle display)
        : NamedObject()
        , displayHandle_(display)
        , physicalSize_()
        , contentScale_()
        , virtualPosition_()
        , workAreaPos_()
        , workAreaSize_()
        , gammaRegular_()
        , videoMode_()
        , monitorIndex_()
        , gammaIsRegular_()
        , gammaRamp_()
        , videoModes_()
    {
        // Misc. data
        glfwGetMonitorPhysicalSize(displayHandle_, &physicalSize_.x, &physicalSize_.y);
        glfwGetMonitorContentScale(displayHandle_, &contentScale_.x, &contentScale_.y);
        glfwGetMonitorPos(displayHandle_, &virtualPosition_.x, &virtualPosition_.y);
        glfwGetMonitorWorkarea(displayHandle_, &workAreaPos_.x, &workAreaPos_.y, &workAreaSize_.x, &workAreaSize_.y);
        SetName(glfwGetMonitorName(displayHandle_));
        // Video modes
        {
            // Read all possible video modes.
            int count;
            const GLFWvidmode* modes = glfwGetVideoModes(displayHandle_, &count);
            videoModes_.reserve((size_t)count);
            for (int i = 0; i < count; ++i)
            {
                videoModes_.emplace_back(&modes[(unsigned)i]);
            }
            // Read current video mode.
            const GLFWvidmode* mode = glfwGetVideoMode(displayHandle_);
            if (mode != nullptr)
            {
                videoMode_ = (decltype(videoMode_))-1;
                VideoMode current{ mode };
                for (unsigned short i = 0; i < videoModes_.size(); ++i)
                {
                    if (current == videoModes_[i])
                    {
                        videoMode_ = (decltype(videoMode_))i;
                    }
                }
                if (videoMode_ >= videoModes_.size())
                {
                    TraceWarning("Failed to match active video mode found for display \"{}\"!", GetName());
                    videoMode_ = 0;
                }
            }
            else
            {
                TraceWarning("No active video mode found for display \"{}\"!", GetName());
                videoMode_ = 0;
            }
        }
        // Gamma ramp
        {
            const GLFWgammaramp* ramp = glfwGetGammaRamp(displayHandle_);
            if (ramp != nullptr)
            {
                gammaRamp_.reserve(ramp->size);
                for (unsigned i = 0; i < ramp->size; ++i)
                {
                    gammaRamp_.emplace_back(ramp->red[i], ramp->green[i], ramp->blue[i]);
                }
            }
            else
            {
                TraceInfo("Gamma ramp not found for display \"{}\"!", GetName());
            }
        }
        // Assign user pointer for easy access back to this struct.
        glfwSetMonitorUserPointer(displayHandle_, this);
    }

    PlatformDisplay::PlatformDisplay(PlatformDisplay&& other) noexcept
        : NamedObject(std::move(other))
        , displayHandle_(std::exchange(other.displayHandle_, nullptr))
        , physicalSize_(std::move(other.physicalSize_))
        , contentScale_(std::move(other.contentScale_))
        , virtualPosition_(std::move(other.virtualPosition_))
        , workAreaPos_(std::move(other.workAreaPos_))
        , workAreaSize_(std::move(other.workAreaSize_))
        , gammaRegular_(std::move(other.gammaRegular_))
        , videoMode_(std::move(other.videoMode_))
        , monitorIndex_(std::move(other.monitorIndex_))
        , gammaIsRegular_(std::move(other.gammaIsRegular_))
        , gammaRamp_(std::move(other.gammaRamp_))
        , videoModes_(std::move(other.videoModes_))
    {
        glfwSetMonitorUserPointer(displayHandle_, this);
    }

    PlatformDisplay::VideoMode::VideoMode(VideoModePtr videoMode)
        : resolution(videoMode->width, videoMode->height)
        , bits(videoMode->redBits, videoMode->greenBits, videoMode->blueBits)
        , refreshRate(videoMode->refreshRate)
    {
    }

    PlatformDisplay::VideoMode::VideoMode(VideoMode&& other) noexcept
        : resolution(std::move(other.resolution))
        , bits(std::move(other.bits))
        , refreshRate(std::move(other.refreshRate))
    {
    }

    //--------------------------------------------------------------------------

    PlatformDisplay::~PlatformDisplay(void)
    {
        if (displayHandle_ != nullptr)
        {
            glfwSetMonitorUserPointer(displayHandle_, nullptr);
        }
    }

    PlatformDisplay::VideoMode::~VideoMode(void)
    {
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

    DisplayHandle PlatformDisplay::GetHandle(void) const
    {
        return displayHandle_;
    }

    unsigned int PlatformDisplay::GetDisplayIndex(void) const
    {
        return monitorIndex_;
    }

    const vector_t<PlatformDisplay::VideoMode>& PlatformDisplay::GetVideoModes(void) const
    {
        return videoModes_;
    }

    const PlatformDisplay::VideoMode& PlatformDisplay::GetCurrentVideoMode(void) const
    {
        return videoModes_[videoMode_];
    }

    const IVec2& PlatformDisplay::GetPhysicalSize(void) const
    {
        return physicalSize_;
    }

    const Vec2& PlatformDisplay::GetContentScale(void) const
    {
        return contentScale_;
    }

    const IVec2& PlatformDisplay::GetVirtualPosition(void) const
    {
        return virtualPosition_;
    }

    const IVec2& PlatformDisplay::GetWorkAreaPosition(void) const
    {
        return workAreaPos_;
    }

    const IVec2& PlatformDisplay::GetWorkAreaSize(void) const
    {
        return workAreaSize_;
    }

    bool PlatformDisplay::VideoMode::operator==(const VideoMode& other)
    {
        return resolution == other.resolution && bits == other.bits && refreshRate == other.refreshRate;
    }

    bool PlatformDisplay::VideoMode::operator!=(const VideoMode& other)
    {
        return resolution != other.resolution || bits != other.bits || refreshRate != other.refreshRate;
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
