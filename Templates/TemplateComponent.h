//------------------------------------------------------------------------------
//
// File Name:	$safeitemname$.h
// Author(s):	$username$
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Template class for a new component.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Component.h"

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

    // Typedefs:

    // Class Definition:
    class $safeitemname$ COMPONENT_FINAL : public Component
    {
        // Public Constants, Enums, and Structs:
    public:
        // Common component data.
        COMPONENT_COMMON($safeitemname$);

        // Constructors/Destructors:
    public:
        $safeitemname$(void);

        $safeitemname$(const $safeitemname$& other);

        ~$safeitemname$(void);

        // Public Static Functions:
    public:

        // Public Functions:
    public:
        /// <summary>
        /// Clone the $safeitemname$.
        /// </summary>
        /// <returns>A clone of this $safeitemname$.</returns>
        $safeitemname$* Clone(void) const override;
        
        /// <summary>
        /// Read $safeitemname$ data from a stream.
        /// </summary>
        /// <param name="stream">The stream to read from.</param>
        void Read(StreamIn& stream) override;

        /// <summary>
        /// Writes the $safeitemname$ data into the stream.
        /// </summary>
        /// <param name="stream">The stream to write into.</param>
        void Write(StreamOut& stream) override;

        /// <summary>
        /// Update this $safeitemname$.
        /// </summary>
        /// <param name="dt">The time elapsed since the previous tick.</param>
        void Update(float dt) override;
        
        /// <summary>
        /// Render this $safeitemname$.
        /// </summary>
        void Render(void) const override;

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
