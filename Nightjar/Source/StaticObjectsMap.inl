//------------------------------------------------------------------------------
//
// File Name:	StaticObjectsMap.inl
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Static container implementation for map.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "StaticObjectsMap.h"

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

    template<typename K, typename V, typename L>
    MapWrapper<K, V, L>::MapWrapper(void)
        : data()
    {
    }

    template<typename K, typename V, typename L>
    MapWrapper<K, V, L>::MapWrapper(map_t<K, V, L>&& _data)
        : data(_data)
    {
    }

    //--------------------------------------------------------------------------

    template<typename K, typename V, typename L>
    MapWrapper<K, V, L>::~MapWrapper(void)
    {
    }

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

    template<typename K, typename V, typename L>
    map_t<K, V, L>& MapWrapper<K, V, L>::Create(void)
    {
        return ContainerBase::Create<MapWrapper>();
    }

    template<typename K, typename V, typename L>
    map_t<K, V, L>& MapWrapper<K, V, L>::Create(map_t<K, V, L>&& _data)
    {
        return ContainerBase::Create<MapWrapper>(std::forward<map_t<K, V, L>>(_data));
    }

    template<typename K, typename V, typename L>
    map_t<K, V, L>& MapWrapper<K, V, L>::CreateConstant(map_t<K, V, L>&& _data)
    {
        return ConstContainerBase::Create<MapWrapper>(std::forward<map_t<K, V, L>>(_data));
    }

#pragma endregion Public Static Functions

    //--------------------------------------------------------------------------
    // Public Functions:
    //--------------------------------------------------------------------------

#pragma region Public Functions

    template<typename K, typename V, typename L>
    MapWrapper<K, V, L>::operator map_t<K, V, L>&(void)
    {
        return data;
    }

    template<typename K, typename V, typename L>
    MapWrapper<K, V, L>::operator const map_t<K, V, L>&(void) const
    {
        return data;
    }

    template<typename K, typename V, typename L>
    void MapWrapper<K, V, L>::Clear(void)
    {
        data.clear();
    }

#pragma endregion Public Functions

}	// namespace
