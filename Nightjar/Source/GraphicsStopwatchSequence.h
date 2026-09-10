//------------------------------------------------------------------------------
//
// File Name:	GraphicsStopwatchSequence.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Utilizes several GraphicsStopwatches to avoid losing timing
//              data due to delayed completion.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "GraphicsStopwatch.h"

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
    template <auto N> requires (array_size_t<decltype(N)>)
    class GraphicsStopwatchSequence : public dearray_t<GraphicsStopwatchCore, N, true>
    {
        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    public:
        GraphicsStopwatchSequence(void);

        ~GraphicsStopwatchSequence(void);

        // Public Static Functions:
    public:

        // Public Functions:
    public:
        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        bool Initialize(void);

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        bool Shutdown(void);

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        bool IsValid(void) const;

        /// <summary>
        /// 
        /// </summary>
        void Start(void);

        /// <summary>
        /// 
        /// </summary>
        void Stop(void);

        // Private Constants, Enums, and Structs:
    private:
        //
        using base_t = dearray_t<GraphicsStopwatchCore, N, true>;

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

#include "GraphicsStopwatchSequence.inl"
