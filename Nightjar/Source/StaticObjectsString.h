//------------------------------------------------------------------------------
//
// File Name:	StaticObjectsString.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Static container implementation for string.
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
    // Static container for string.
    template<typename T>
    class BasicStringWrapper : public ContainerBase
    {
        // Constructors/Destructors:
    public:
        BasicStringWrapper(void);
        BasicStringWrapper(basic_string_t<T>&& data);

    public:
        ~BasicStringWrapper(void);

        // Public Static Functions:
    public:
        /// <summary>
        /// Creates a default-constucted static string container.
        /// </summary>
        /// <returns>The newly constructed contained string.</returns>
        static basic_string_t<T>& Create(void);
        
        /// <summary>
        /// Creates an empty static string container with the given capacity.
        /// </summary>
        /// <returns>The newly constructed contained string.</returns>
        static basic_string_t<T>& Create(size_t capacity);

        /// <summary>
        /// Creates a static string container initialized with given data.
        /// </summary>
        /// <param name="data">The data to initialize with.</param>
        /// <returns>The newly constructed contained string.</returns>
        static basic_string_t<T>& Create(basic_string_t<T>&& data);

        // Public Functions:
    public:
        /// <summary>
        /// Gets the contained string.
        /// </summary>
        operator basic_string_t<T>&(void);

        /// <summary>
        /// Gets the contained string.
        /// </summary>
        operator const basic_string_t<T>&(void) const;

        /// <summary>
        /// Clears the string of all its contents and frees unused memory.
        /// </summary>
        void Clear(void) override;

        // Private Variables:
    private:
        // The contained string.
        basic_string_t<T> data;
    };

    // Common macro for implementing static string container functions.
#define INTERNAL_STATIC_STRING(modifier, type, funcname, name, ...) INTERNAL_STATIC_BASE_TEMPLATE(modifier, basic_string_t, BasicStringWrapper, (type), funcname, name, __VA_ARGS__)

    // Implements a static template class string container.
#define STATIC_STRING_CLASS_TEMPLATE(className, name, ...) INTERNAL_STATIC_STRING( , char, (UNPAREN className ::name), name, __VA_ARGS__)

    // Implements a static class string container.
#define STATIC_STRING_CLASS(className, name, ...) INTERNAL_STATIC_STRING( , char, (className::name), name, __VA_ARGS__)

    // Implements a static global string container.
#define STATIC_STRING(name, ...) INTERNAL_STATIC_STRING(static, char, (name), name, __VA_ARGS__)

    // Implements a static template class wide string container.
#define STATIC_WSTRING_CLASS_TEMPLATE(className, name, ...) INTERNAL_STATIC_STRING( , wchar_t, (UNPAREN className ::name), name, __VA_ARGS__)

    // Implements a static class wide string container.
#define STATIC_WSTRING_CLASS(className, name, ...) INTERNAL_STATIC_STRING( , wchar_t, (className::name), name, __VA_ARGS__)

    // Implements a static global wide string container.
#define STATIC_WSTRING(name, ...) INTERNAL_STATIC_STRING(static, wchar_t, (name), name, __VA_ARGS__)

    using StringWrapper = BasicStringWrapper<char>;
    using WStringWrapper = BasicStringWrapper<wchar_t>;

}	// namespace
