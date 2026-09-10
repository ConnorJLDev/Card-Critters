//------------------------------------------------------------------------------
//
// File Name:	StaticObjectsUnorderedMap.h
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

#include "StaticObjectsBase.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar::StaticManagement
{
    // Static container for unordered map.
    template<typename K, typename V, typename H = std::hash<K>, typename E = std::equal_to<K>>
    class UnorderedMapWrapper : public ContainerBase
    {
        // Constructors/Destructors:
    public:
        UnorderedMapWrapper(void);
        UnorderedMapWrapper(unordered_map_t<K, V, H, E>&& data);

    public:
        ~UnorderedMapWrapper(void);

        // Public Static Functions:
    public:
        /// <summary>
        /// Creates a default-constucted static unordered map container.
        /// </summary>
        /// <returns>The newly constructed contained unordered map.</returns>
        static unordered_map_t<K, V, H, E>& Create(void);

        /// <summary>
        /// Creates a static unordered map container initialized with given data.
        /// </summary>
        /// <param name="data">The data to initialize with.</param>
        /// <returns>The newly constructed contained unordered map.</returns>
        static unordered_map_t<K, V, H, E>& Create(unordered_map_t<K, V, H, E>&& data);

        /// <summary>
        /// Creates an immutable static unordered map container initialized with given data.
        /// </summary>
        /// <param name="data">The data to initialize with.</param>
        /// <returns>The newly constructed contained unordered map.</returns>
        static unordered_map_t<K, V, H, E>& CreateConstant(unordered_map_t<K, V, H, E>&& data);

        // Public Functions:
    public:
        /// <summary>
        /// Gets the contained unordered map.
        /// </summary>
        operator unordered_map_t<K, V, H, E>&(void);

        /// <summary>
        /// Gets the contained unordered map.
        /// </summary>
        operator const unordered_map_t<K, V, H, E>&(void) const;

        /// <summary>
        /// Clears the unordered map of all its contents and frees unused memory.
        /// </summary>
        void Clear(void) override;

        // Private Variables:
    private:
        // The contained unordered map.
        unordered_map_t<K, V, H, E> data;
    };

    // Common macro for implementing static unordered map container functions.
#define INTERNAL_STATIC_UNORDERED_MAP(modifier, type, funcname, name, ...) INTERNAL_STATIC_BASE_TEMPLATE(modifier, unordered_map_t, UnorderedMapWrapper, type, funcname, name, __VA_ARGS__)

    // Implements a static template class unordered map container.
#define STATIC_UNORDERED_MAP_CLASS_TEMPLATE(type, className, name, ...) INTERNAL_STATIC_UNORDERED_MAP( , type, (UNPAREN className ::name), name, __VA_ARGS__)

    // Implements a static class unordered map container.
#define STATIC_UNORDERED_MAP_CLASS(type, className, name, ...) INTERNAL_STATIC_UNORDERED_MAP( , type, (className::name), name, __VA_ARGS__)

    // Implements a static global unordered map container.
#define STATIC_UNORDERED_MAP(type, name, ...) INTERNAL_STATIC_UNORDERED_MAP(static, type, (name), name, __VA_ARGS__)

    // Implements a static template class unordered map container with string keys.
#define STATIC_UNORDERED_MAP_STRING_CLASS_TEMPLATE(type, className, name, ...) STATIC_UNORDERED_MAP_CLASS_TEMPLATE((string_t, UNPAREN type , Utility::StringHash, std::equal_to<>), className, name, __VA_ARGS__)

    // Implements a static class unordered map container with string keys.
#define STATIC_UNORDERED_MAP_STRING_CLASS(type, className, name, ...) STATIC_UNORDERED_MAP_CLASS((string_t, UNPAREN type , Utility::StringHash, std::equal_to<>), className, name, __VA_ARGS__)

    // Implements a static global unordered map container with string keys.
#define STATIC_UNORDERED_MAP_STRING(type, name, ...) STATIC_UNORDERED_MAP((string_t, UNPAREN type , Utility::StringHash, std::equal_to<>), name, __VA_ARGS__)

}	// namespace

#include "StaticObjectsUnorderedMap.inl"
