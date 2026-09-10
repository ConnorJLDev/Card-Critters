//------------------------------------------------------------------------------
//
// File Name:	StaticObjectsString.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Static container implementation for string.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "StaticObjectsString.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar::StaticManagement
{
    //--------------------------------------------------------------------------
    // Constructors/Destructors:
    //--------------------------------------------------------------------------

#pragma region Constructors

    template<typename T>
    BasicStringWrapper<T>::BasicStringWrapper(void)
        : data()
    {
    }

    template<typename T>
    BasicStringWrapper<T>::BasicStringWrapper(basic_string_t<T>&& _data)
        : data(_data)
    {
    }

    //--------------------------------------------------------------------------

    template<typename T>
    BasicStringWrapper<T>::~BasicStringWrapper(void)
    {
    }

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

    template<typename T>
    basic_string_t<T>& BasicStringWrapper<T>::Create(void)
    {
        return ContainerBase::Create<BasicStringWrapper>();
    }

    template<typename T>
    basic_string_t<T>& BasicStringWrapper<T>::Create(size_t capacity)
    {
        basic_string_t<T>& str = Create();
        str.reserve(capacity);
        return str;
    }

    template<typename T>
    basic_string_t<T>& BasicStringWrapper<T>::Create(basic_string_t<T>&& _data)
    {
        return ContainerBase::Create<BasicStringWrapper>(std::forward<basic_string_t<T>>(_data));
    }

#pragma endregion Public Static Functions

    //--------------------------------------------------------------------------
    // Public Functions:
    //--------------------------------------------------------------------------

#pragma region Public Functions

    template<typename T>
    BasicStringWrapper<T>::operator basic_string_t<T>&(void)
    {
        return data;
    }

    template<typename T>
    BasicStringWrapper<T>::operator const basic_string_t<T>&(void) const
    {
        return data;
    }

    template<typename T>
    void BasicStringWrapper<T>::Clear(void)
    {
        data.clear();
    }

#pragma endregion Public Functions

    template BasicStringWrapper<char>;
    template BasicStringWrapper<wchar_t>;

}	// namespace
