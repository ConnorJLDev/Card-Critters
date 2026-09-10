//------------------------------------------------------------------------------
//
// File Name:	CallbackWrapper.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Struct representing an identifiable callback function that can
//              be registered and unregistered.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Object.h"

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
    using CallbackID = ObjectID;

    // Struct Definition:
    template <typename CallbackReturn, typename... CallbackArgs>
    struct CallbackWrapper : public Object
    {
        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    public:
        CallbackWrapper(void) = delete;
        CallbackWrapper(const std::function<CallbackReturn(CallbackArgs...)>& func);
        CallbackWrapper(const CallbackWrapper& other) = delete;
        CallbackWrapper(CallbackWrapper&& other) noexcept;
        CallbackWrapper& operator=(const CallbackWrapper& other);
        CallbackWrapper& operator=(CallbackWrapper&& other) noexcept;

        ~CallbackWrapper(void);

        // Public Static Functions:
    public:

        // Public Functions:
    public:
        // The function call of the wrapped function.
        CallbackReturn operator()(CallbackArgs...) const;

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
        // The underlying function this wrapper represents.
        std::function<CallbackReturn(CallbackArgs...)> func;
    };

}	// namespace
