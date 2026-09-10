//------------------------------------------------------------------------------
//
// File Name:	StaticObjectsMap.h
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

#include "StaticObjectsBase.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar::StaticManagement
{
    // Static container for map.
    template<typename K, typename V, typename L = std::less<K>>
    class MapWrapper : public ContainerBase
    {
        // Constructors/Destructors:
    public:
        MapWrapper(void);
        MapWrapper(map_t<K, V, L>&& data);

    public:
        ~MapWrapper(void);

        // Public Static Functions:
    public:
        /// <summary>
        /// Creates a default-constucted static map container.
        /// </summary>
        /// <returns>The newly constructed contained map.</returns>
        static map_t<K, V, L>& Create(void);

        /// <summary>
        /// Creates a static map container initialized with given data.
        /// </summary>
        /// <param name="data">The data to initialize with.</param>
        /// <returns>The newly constructed contained map.</returns>
        static map_t<K, V, L>& Create(map_t<K, V, L>&& data);

        /// <summary>
        /// Creates an immutable static map container initialized with given data.
        /// </summary>
        /// <param name="data">The data to initialize with.</param>
        /// <returns>The newly constructed contained map.</returns>
        static map_t<K, V, L>& CreateConstant(map_t<K, V, L>&& data);

        // Public Functions:
    public:
        /// <summary>
        /// Gets the contained map.
        /// </summary>
        operator map_t<K, V, L>&(void);

        /// <summary>
        /// Gets the contained map.
        /// </summary>
        operator const map_t<K, V, L>&(void) const;

        /// <summary>
        /// Clears the map of all its contents and frees unused memory.
        /// </summary>
        void Clear(void) override;

        // Private Variables:
    private:
        // The contained map.
        map_t<K, V, L> data;
    };

    // Common macro for implementing static map container functions.
#define INTERNAL_STATIC_MAP(modifier, type, funcname, name, ...) INTERNAL_STATIC_BASE_TEMPLATE(modifier, map_t, MapWrapper, type, funcname, name, __VA_ARGS__)

    // Implements a static template class map container.
#define STATIC_MAP_CLASS_TEMPLATE(type, className, name, ...) INTERNAL_STATIC_MAP( , type, (UNPAREN className ::name), name, __VA_ARGS__)

    // Implements a static class map container.
#define STATIC_MAP_CLASS(type, className, name, ...) INTERNAL_STATIC_MAP( , type, (className::name), name, __VA_ARGS__)

    // Implements a static global map container.
#define STATIC_MAP(type, name, ...) INTERNAL_STATIC_MAP(static, type, (name), name, __VA_ARGS__)

    // Implements a static template class map container with string keys.
#define STATIC_MAP_STRING_CLASS_TEMPLATE(type, className, name, ...) STATIC_UNORDERED_MAP_CLASS_TEMPLATE((string_t, UNPAREN type , std::less<>), className, name, __VA_ARGS__)

    // Implements a static class map container with string keys.
#define STATIC_MAP_STRING_CLASS(type, className, name, ...) STATIC_MAP_CLASS((string_t, UNPAREN type , std::less<>), className, name, __VA_ARGS__)

    // Implements a static global map container with string keys.
#define STATIC_MAP_STRING(type, name, ...) STATIC_MAP((string_t, UNPAREN type , std::less<>), name, __VA_ARGS__)

}	// namespace

#include "StaticObjectsMap.inl"
