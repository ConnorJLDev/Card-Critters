//------------------------------------------------------------------------------
//
// File Name:	StaticObjectsVector.inl
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Static container implementation for vector.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "StaticObjectsVector.h"

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
    VectorWrapper<T>::VectorWrapper(void)
        : data()
    {
    }

    template<typename T>
    VectorWrapper<T>::VectorWrapper(vector_t<T>&& _data)
        : data(_data)
    {
    }

    //--------------------------------------------------------------------------

    template<typename T>
    VectorWrapper<T>::~VectorWrapper(void)
    {
    }

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

    template<typename T>
    vector_t<T>& VectorWrapper<T>::Create(void)
    {
        return ContainerBase::Create<VectorWrapper>();
    }

    template<typename T>
    vector_t<T>& VectorWrapper<T>::Create(size_t capacity)
    {
        vector_t<T>& vec = Create();
        vec.reserve(capacity);
        return vec;
    }

    template<typename T>
    vector_t<T>& VectorWrapper<T>::Create(vector_t<T>&& _data)
    {
        return ContainerBase::Create<VectorWrapper>(std::forward<vector_t<T>>(_data));
    }

#pragma endregion Public Static Functions

    //--------------------------------------------------------------------------
    // Public Functions:
    //--------------------------------------------------------------------------

#pragma region Public Functions

    template<typename T>
    VectorWrapper<T>::operator vector_t<T>&(void)
    {
        return data;
    }

    template<typename T>
    VectorWrapper<T>::operator const vector_t<T>&(void) const
    {
        return data;
    }

    template<typename T>
    void VectorWrapper<T>::Clear(void)
    {
        data.clear();
        data.shrink_to_fit();
    }

#pragma endregion Public Functions

}	// namespace
