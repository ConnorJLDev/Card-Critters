//------------------------------------------------------------------------------
//
// File Name:	StaticObjectsBase.h
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

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar::StaticManagement
{
    class ConstContainerBase;
    class ContainerBase;

    // Accessor for the vector of immutable static containers.
    std::unique_ptr<vector_t<std::unique_ptr<ConstContainerBase>>>& StaticConstantStorage(void);
    // Destroys the vector of immutable static containers.
    void StaticConstantDestroy(void);

    // Accessor for the vector of static containers.
    std::unique_ptr<vector_t<std::unique_ptr<ContainerBase>>>& StaticStorage(void);
    // Clears all static containers.
    void StaticClear(void);
    // Destroys the vector of static containers.
    void StaticDestroy(void);

    // Abstract base class for immutable static containers (typically map and unordered map).
    class ConstContainerBase
    {
    public:
        virtual ~ConstContainerBase(void);
    protected:
        /// <summary>
        /// Constructs a container within StaticConstantStorage.
        /// </summary>
        /// <typeparam name="TContainer">The container type being constructed.</typeparam>
        /// <typeparam name="Args">The container constructor argument types.</typeparam>
        /// <param name="args">The container constructor arguments.</param>
        /// <returns>The newly constructed container.</returns>
        template <class TContainer, typename... Args>
        static TContainer& Create(Args&&... args) requires (std::is_base_of_v<ConstContainerBase, TContainer>);
    };

    // Abstract base class for static containers.
    class ContainerBase : public ConstContainerBase
    {
    public:
        /// <summary>
        /// Clears the container of all its contents and frees unused memory.
        /// </summary>
        virtual void Clear(void) = 0;
    protected:
        /// <summary>
        /// Constructs a container within StaticStorage.
        /// </summary>
        /// <typeparam name="TContainer">The container type being constructed.</typeparam>
        /// <typeparam name="Args">The container constructor argument types.</typeparam>
        /// <param name="args">The container constructor arguments.</param>
        /// <returns>The newly constructed container.</returns>
        template <class TContainer, typename... Args>
        static TContainer& Create(Args&&... args) requires (std::is_base_of_v<ContainerBase, TContainer>);
    };
    
    // Macro for creating a global or class static container accessor function.
#define INTERNAL_STATIC_BASE(modifier, typeContainer, typeWrap, funcname, name, ...)\
    modifier UNPAREN typeContainer & UNPAREN funcname (void)\
    {\
        static UNPAREN typeContainer & name##_\
        (\
            StaticManagement::UNPAREN typeWrap ::Create(__VA_OPT__(UNPAREN typeContainer {) __VA_ARGS__ __VA_OPT__(}))\
        );\
        return name##_;\
    }\

    // Macro for creating a global or class static template container accessor function.
#define INTERNAL_STATIC_BASE_TEMPLATE(modifier, typeContainer, typeWrap, typeInternal, funcname, name, ...) INTERNAL_STATIC_BASE(modifier, (typeContainer<UNPAREN typeInternal >), (typeWrap<UNPAREN typeInternal >), funcname, name, __VA_ARGS__)

}	// namespace

#include "StaticObjectsBase.inl"
