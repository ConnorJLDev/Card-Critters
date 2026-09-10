//------------------------------------------------------------------------------
//
// File Name:	StaticObjectsVector.h
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

#include "StaticObjectsBase.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar::StaticManagement
{
    // Static container for vector.
    template<typename T>
    class VectorWrapper : public ContainerBase
    {
        // Constructors/Destructors:
    public:
        VectorWrapper(void);
        VectorWrapper(vector_t<T>&& data);

    public:
        ~VectorWrapper(void);

        // Public Static Functions:
    public:
        /// <summary>
        /// Creates a default-constucted static vector container.
        /// </summary>
        /// <returns>The newly constructed contained vector.</returns>
        static vector_t<T>& Create(void);
        
        /// <summary>
        /// Creates an empty static vector container with the given capacity.
        /// </summary>
        /// <returns>The newly constructed contained vector.</returns>
        static vector_t<T>& Create(size_t capacity);

        /// <summary>
        /// Creates a static vector container initialized with given data.
        /// </summary>
        /// <param name="data">The data to initialize with.</param>
        /// <returns>The newly constructed contained vector.</returns>
        static vector_t<T>& Create(vector_t<T>&& data);

        // Public Functions:
    public:
        /// <summary>
        /// Gets the contained vector.
        /// </summary>
        operator vector_t<T>&(void);

        /// <summary>
        /// Gets the contained vector.
        /// </summary>
        operator const vector_t<T>&(void) const;

        /// <summary>
        /// Clears the vector of all its contents and frees unused memory.
        /// </summary>
        void Clear(void) override;

        // Private Variables:
    private:
        // The contained vector.
        vector_t<T> data;
    };

    // Common macro for implementing static vector container functions.
#define INTERNAL_STATIC_VECTOR(modifier, type, funcname, name, ...) INTERNAL_STATIC_BASE_TEMPLATE(modifier, vector_t, VectorWrapper, type, funcname, name, __VA_ARGS__)

    // Implements a static template class vector container.
#define STATIC_VECTOR_CLASS_TEMPLATE(type, className, name, ...) INTERNAL_STATIC_VECTOR( , type, (UNPAREN className ::name), name, __VA_ARGS__)

    // Implements a static class vector container.
#define STATIC_VECTOR_CLASS(type, className, name, ...) INTERNAL_STATIC_VECTOR( , type, (className::name), name, __VA_ARGS__)

    // Implements a static global vector container.
#define STATIC_VECTOR(type, name, ...) INTERNAL_STATIC_VECTOR(static, type, (name), name, __VA_ARGS__)

}	// namespace

#include "StaticObjectsVector.inl"
