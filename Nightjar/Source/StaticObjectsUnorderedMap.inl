//------------------------------------------------------------------------------
//
// File Name:	StaticObjects.inl
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Static container implementation for unordered map.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "StaticObjectsUnorderedMap.h"

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

    template<typename K, typename V, typename H, typename E>
    UnorderedMapWrapper<K, V, H, E>::UnorderedMapWrapper(void)
        : data()
    {
    }

    template<typename K, typename V, typename H, typename E>
    UnorderedMapWrapper<K, V, H, E>::UnorderedMapWrapper(unordered_map_t<K, V, H, E>&& _data)
        : data(_data)
    {
    }

    //--------------------------------------------------------------------------

    template<typename K, typename V, typename H, typename E>
    UnorderedMapWrapper<K, V, H, E>::~UnorderedMapWrapper(void)
    {
    }

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

    template<typename K, typename V, typename H, typename E>
    unordered_map_t<K, V, H, E>& UnorderedMapWrapper<K, V, H, E>::Create(void)
    {
        return ContainerBase::Create<UnorderedMapWrapper>();
    }

    template<typename K, typename V, typename H, typename E>
    unordered_map_t<K, V, H, E>& UnorderedMapWrapper<K, V, H, E>::Create(unordered_map_t<K, V, H, E>&& _data)
    {
        return ContainerBase::Create<UnorderedMapWrapper>(std::forward<unordered_map_t<K, V, H, E>>(_data));
    }

    template<typename K, typename V, typename H, typename E>
    unordered_map_t<K, V, H, E>& UnorderedMapWrapper<K, V, H, E>::CreateConstant(unordered_map_t<K, V, H, E>&& _data)
    {
        return ConstContainerBase::Create<UnorderedMapWrapper>(std::forward<unordered_map_t<K, V, H, E>>(_data));
    }

#pragma endregion Public Static Functions

    //--------------------------------------------------------------------------
    // Public Functions:
    //--------------------------------------------------------------------------

#pragma region Public Functions

    template<typename K, typename V, typename H, typename E>
    UnorderedMapWrapper<K, V, H, E>::operator unordered_map_t<K, V, H, E>&(void)
    {
        return data;
    }

    template<typename K, typename V, typename H, typename E>
    UnorderedMapWrapper<K, V, H, E>::operator const unordered_map_t<K, V, H, E>&(void) const
    {
        return data;
    }

    template<typename K, typename V, typename H, typename E>
    void UnorderedMapWrapper<K, V, H, E>::Clear(void)
    {
        data.clear();
        data.rehash(0);
    }

#pragma endregion Public Functions

}	// namespace
