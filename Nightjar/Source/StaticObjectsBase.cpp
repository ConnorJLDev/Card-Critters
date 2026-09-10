//------------------------------------------------------------------------------
//
// File Name:	StaticObjectsBase.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Manages the creation and destruction of objects with static
//              lifetime, to track and manage memory usage.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

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

    ConstContainerBase::~ConstContainerBase(void)
    {
    }

    std::unique_ptr<vector_t<std::unique_ptr<ConstContainerBase>>>& StaticConstantStorage(void)
    {
        static std::unique_ptr<vector_t<std::unique_ptr<ConstContainerBase>>> storage{ std::make_unique<vector_t<std::unique_ptr<ConstContainerBase>>>() };
        return storage;
    }

    void StaticConstantDestroy(void)
    {
        auto ptr = StaticConstantStorage().release();
        delete ptr;
    }

    std::unique_ptr<vector_t<std::unique_ptr<ContainerBase>>>& StaticStorage(void)
    {
        static std::unique_ptr<vector_t<std::unique_ptr<ContainerBase>>> storage{ std::make_unique<vector_t<std::unique_ptr<ContainerBase>>>() };
        return storage;
    }

    void StaticClear(void)
    {
        auto& containers = StaticStorage();
        if (containers == nullptr)
        {
            return;
        }
        for (auto& container : *containers)
        {
            container->Clear();
        }
    }

    void StaticDestroy(void)
    {
        auto ptr = StaticStorage().release();
        delete ptr;
    }

}	// namespace
