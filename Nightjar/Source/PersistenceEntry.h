//------------------------------------------------------------------------------
//
// File Name:	PersistenceEntry.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Describes a data entry within a persistence group.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Variant.h"

//------------------------------------------------------------------------------
// Preprocessor Directives:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    // Forward Declarations:

    // Typedefs:

    // Class Definition:
    class PersistenceEntry : public NamedObject, Variant
    {
        friend class PersistenceGroup;

        // Public Constants, Enums, and Structs:
    public:
        // The type used for getting an arbitrary value.
        using getter_t = void*(*)(void);

        // The type used for setting an arbitrary value.
        using setter_t = void(*)(const void*);

        // Constructors/Destructors:
    public:
        PersistenceEntry(void);
        PersistenceEntry(string_view_arg name);
        PersistenceEntry(const PersistenceEntry& other);
        PersistenceEntry(PersistenceEntry&& other) noexcept;
        PersistenceEntry& operator=(const PersistenceEntry& other);
        PersistenceEntry& operator=(PersistenceEntry&& other) noexcept;

        ~PersistenceEntry(void);

        // Public Static Functions:
    public:

        // Public Functions:
    public:
        using Variant::GetType;
        using Variant::SetType;
        using Variant::GetLocked;
        using Variant::SetLocked;

        /// <summary>
        /// Reads a persistent data entry from a stream.
        /// </summary>
        /// <param name="stream">The stream being read from.</param>
        /// <returns>True if the value was read successfully, false otherwise.</returns>
        bool Read(StreamIn& stream);

        /// <summary>
        /// Writes this persistent data entry to a stream.
        /// </summary>
        /// <param name="stream">The stream being written to.</param>
        /// <returns>True if the value was written successfully, false otherwise.</returns>
        bool Write(StreamOut& stream);

        /// <summary>
        /// Gets the getter function of this entry.
        /// </summary>
        /// <returns>The current getter function in use.</returns>
        getter_t GetGetter(void) const;

        /// <summary>
        /// Sets the getter function of this entry.
        /// </summary>
        /// <param name="getter">The new getter function to use.</param>
        void SetGetter(getter_t getter);
        
        /// <summary>
        /// Gets the setter function of this entry.
        /// </summary>
        /// <returns>The current setter function in use.</returns>
        setter_t GetSetter(void) const;

        /// <summary>
        /// Sets the setter function of this entry.
        /// </summary>
        /// <param name="setter">The new setter function to use.</param>
        void SetSetter(setter_t setter);

        /// <summary>
        /// Tries to get the value of this entry as the given type.
        /// </summary>
        /// <typeparam name="T">The type to get the value as.</typeparam>
        /// <param name="value">A reference that is set to the value if the type is valid, unchanged otherwise.</param>
        /// <returns>True if the type is valid, false otherwise.</returns>
        template<variants_t T>
        bool GetValue(T& value);

        /// <summary>
        /// Tries to set the value of this entry.
        /// </summary>
        /// <typeparam name="T">The type to get the value as.</typeparam>
        /// <param name="valuePtr">A pointer reference that is set to the value if the type is valid, or nullptr otherwise.</param>
        /// <returns>True if the type is correct, false otherwise.</returns>
        template<variants_t T>
        bool SetValue(const T& value);

        // Public Event Handlers
    public:

        // Private Constants, Enums, and Structs:
    private:

        // Private Static Functions:
    private:

        // Private Functions:
    private:

        // Private Static Variables:
    private:

        // Private Variables:
    private:
        // Optional function pointer for getting the value of this entry.
        getter_t getter;
        // Optional function pointer for setting the value of this entry.
        setter_t setter;
    };

}	// namespace

#include "PersistenceEntry.inl"
