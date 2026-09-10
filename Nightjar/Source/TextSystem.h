//------------------------------------------------------------------------------
//
// File Name:	TextSystem.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		System that handles TrueType text.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#define TEXT_INCLUDE "TextInclude.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

struct FT_LibraryRec_;
struct FT_FaceRec_;
struct FT_GlyphRec_;

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    // Forward Declarations:
    class BaseSystem;

    // Typedefs:

    // Class Definition:
    class TextSystem
    {
        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    private:
        TextSystem(void) = delete;

        // Public Static Functions:
    public:
        /// <summary>
        /// Gets the instance of the TextSystem.
        /// </summary>
        /// <returns>A pointer to the system.</returns>
        static BaseSystem* GetInstance(void);

        /// <summary>
        /// Gets the underlying FreeType library.
        /// </summary>
        /// <returns>A pointer to the implementation library.</returns>
        static FT_LibraryRec_* GetLibrary(void);

        // Public Functions:
    public:

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
    };

}	// namespace
