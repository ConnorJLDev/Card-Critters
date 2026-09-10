//------------------------------------------------------------------------------
//
// File Name:	StaticObjectsBase.inl
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Manages the creation and destruction of objects with static
//              lifetime, to track and manage memory usage.
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

    template <class TContainer, typename... Args>
    TContainer& ConstContainerBase::Create(Args&&... args) requires (std::is_base_of_v<ConstContainerBase, TContainer>)
    {
        TContainer* container = static_cast<TContainer*>(
            StaticConstantStorage()->emplace_back(std::make_unique<TContainer>(std::forward<Args>(args)...)).get()
            );
        return *container;
    }

    template <class TContainer, typename... Args>
    TContainer& ContainerBase::Create(Args&&... args) requires (std::is_base_of_v<ContainerBase, TContainer>)
    {
        TContainer* container = static_cast<TContainer*>(
            StaticStorage()->emplace_back(std::make_unique<TContainer>(std::forward<Args>(args)...)).get()
            );
        return *container;
    }

}	// namespace
