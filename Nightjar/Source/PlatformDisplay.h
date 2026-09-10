//------------------------------------------------------------------------------
//
// File Name:	PlatformDisplay.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class representing a display connected to the computer.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "PlatformSystem.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    // Forward Declarations:

    // Typedefs:

    // Class Definition:
    class PlatformDisplay : public NamedObject
    {
        friend class PlatformSystem;
        friend class PlatformSystem_;

        // Public Constants, Enums, and Structs:
    public:
        //
        struct VideoMode
        {
            IVec2 resolution;
            IVec3 bits;
            int refreshRate;
            VideoMode(VideoModePtr videoMode);
            VideoMode(const VideoMode& other) = delete;
            VideoMode(VideoMode&& other) noexcept;
            VideoMode& operator=(const VideoMode& other) = delete;
            ~VideoMode(void);

            bool operator==(const VideoMode& other);
            bool operator!=(const VideoMode& other);
        };

        // Constructors/Destructors:
    public:
        PlatformDisplay(DisplayHandle display);
        
        PlatformDisplay(const PlatformDisplay& other) = delete;

        PlatformDisplay(PlatformDisplay&& other) noexcept;

        PlatformDisplay& operator=(const PlatformDisplay& other) = delete;

        ~PlatformDisplay(void);

        // Public Static Functions:
    public:

        // Public Functions:
    public:
        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        DisplayHandle GetHandle(void) const;

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        unsigned int GetDisplayIndex(void) const;

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        const vector_t<VideoMode>& GetVideoModes(void) const;

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        const VideoMode& GetCurrentVideoMode(void) const;

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        const IVec2& GetPhysicalSize(void) const;

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        const Vec2& GetContentScale(void) const;

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        const IVec2& GetVirtualPosition(void) const;

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        const IVec2& GetWorkAreaPosition(void) const;

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        const IVec2& GetWorkAreaSize(void) const;

        // Public Event Handlers
    public:

        // Private Constants, Enums, and Structs:
    private:

        // Private Static Functions:
    private:

        // Private Functions:
    private:

        // Private Static Variables:
    private:

        // Private Variables:
    private:
        DisplayHandle displayHandle_;
        IVec2 physicalSize_;
        Vec2 contentScale_;
        IVec2 virtualPosition_;
        IVec2 workAreaPos_;
        IVec2 workAreaSize_;
        float gammaRegular_;
        unsigned short videoMode_;
        unsigned char monitorIndex_;
        bool gammaIsRegular_;
        vector_t<UVec3> gammaRamp_;
        vector_t<VideoMode> videoModes_;
    };

}	// namespace
