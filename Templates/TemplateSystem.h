//------------------------------------------------------------------------------
//
// File Name:	$safeitemname$.h
// Author(s):	$username$
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Template class for a new system.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "BaseSystem.h"

//------------------------------------------------------------------------------
// Preprocessor Directives:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    // Forward Declarations:
    class BaseSystem;

    // Typedefs:

    // Class Definition:
    class $safeitemname$
    {
        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    private:
        $safeitemname$(void) = delete;

        // Public Static Functions:
    public:
        /// <summary>
        /// Gets the instance of the $safeitemname$.
        /// </summary>
        /// <returns>A pointer to the system.</returns>
        static BaseSystem* GetInstance(void);

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
