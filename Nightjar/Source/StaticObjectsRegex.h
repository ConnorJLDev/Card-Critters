//------------------------------------------------------------------------------
//
// File Name:	StaticObjectsRegex.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Static container implementation for regex.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "StaticObjectsBase.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar::StaticManagement
{
    // Static container for regex.
    class RegexWrapper : public ConstContainerBase
    {
        // Constructors/Destructors:
    public:
        RegexWrapper(regex_t&& data);

    public:
        ~RegexWrapper(void);

        // Public Static Functions:
    public:
        /// <summary>
        /// Creates a static regex container initialized with given data.
        /// </summary>
        /// <param name="data">The data to initialize with.</param>
        /// <returns>The newly constructed contained regex.</returns>
        static regex_t& Create(regex_t&& data);

        // Public Functions:
    public:
        /// <summary>
        /// Gets the contained regex.
        /// </summary>
        operator regex_t&(void);

        /// <summary>
        /// Gets the contained regex.
        /// </summary>
        operator const regex_t&(void) const;

        // Private Variables:
    private:
        // The contained regex.
        regex_t data;
    };

}	// namespace
