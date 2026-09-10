//------------------------------------------------------------------------------
//
// File Name:	CallbackWrapper.inl
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

#include "CallbackWrapper.h"

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

    template <typename CallbackReturn, typename... CallbackArgs>
    CallbackWrapper<CallbackReturn, CallbackArgs...>::CallbackWrapper(const std::function<CallbackReturn(CallbackArgs...)>& func_)
        : Object()
        , func(func_)
    {
    }

    template <typename CallbackReturn, typename... CallbackArgs>
    CallbackWrapper<CallbackReturn, CallbackArgs...>::CallbackWrapper(CallbackWrapper&& other) noexcept
        : Object(std::move(other))
        , func(std::move(other.func))
    {
    }

    template <typename CallbackReturn, typename... CallbackArgs>
    CallbackWrapper<CallbackReturn, CallbackArgs...>& CallbackWrapper<CallbackReturn, CallbackArgs...>::operator=(const CallbackWrapper& other)
    {
        if (&other != this)
        {
            Object::operator=(other);
            func = other.func;
        }
        return *this;
    }

    template<typename CallbackReturn, typename ...CallbackArgs>
    CallbackWrapper<CallbackReturn, CallbackArgs...>& CallbackWrapper<CallbackReturn, CallbackArgs...>::operator=(CallbackWrapper&& other) noexcept
    {
        if (&other != this)
        {
            Object::operator=(std::move(other));
            func = std::move(other.func);
        }
        return *this;
    }

    //--------------------------------------------------------------------------

    template <typename CallbackReturn, typename... CallbackArgs>
    CallbackWrapper<CallbackReturn, CallbackArgs...>::~CallbackWrapper(void)
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

    template <typename CallbackReturn, typename... CallbackArgs>
    CallbackReturn CallbackWrapper<CallbackReturn, CallbackArgs...>::operator()(CallbackArgs... args) const
    {
        if constexpr (std::is_same_v<CallbackReturn, void>)
        {
            if constexpr (sizeof...(CallbackArgs) > 0)
            {
                func(std::forward<CallbackArgs...>(args...));
            }
            else
            {
                func();
            }
        }
        else
        {
            if constexpr (sizeof...(CallbackArgs) > 0)
            {
                return func(std::forward<CallbackArgs...>(args...));
            }
            else
            {
                return func();
            }
        }
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
