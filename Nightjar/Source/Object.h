//------------------------------------------------------------------------------
//
// File Name:	Object.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Base class for objects.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

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
    using ObjectID = long long;

    // Class Definition:
    class Object
    {
        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    protected:
        Object(void);

        Object(const Object& other);

        Object(Object&& other) noexcept;

        Object& operator=(const Object& other);

        Object& operator=(Object&& other) noexcept;

    public:
        ~Object(void);

        // Public Static Functions:
    public:

        static void* operator new(size_t size);

        static void* operator new[](size_t size);

        static void operator delete(void* ptr) noexcept;

        static void operator delete[](void* ptr) noexcept;

        // Public Functions:
    public:
        /// <summary>
        /// Gets the ID of this object.
        /// </summary>
        /// <returns>The ID of this object.</returns>
        const ObjectID& GetGUID(void) const;

        //
        void DisplayIMGUI(void);

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
        // The next GUID to assign.
        static ObjectID nextId;

        // Private Variables:
    private:
        // The GUID of this object.
        ObjectID guid_;
    };

}	// namespace
