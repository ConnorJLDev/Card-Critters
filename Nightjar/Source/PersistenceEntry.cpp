//------------------------------------------------------------------------------
//
// File Name:	PersistenceEntry.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Describes a data entry within a persistence group.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "PersistenceEntry.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    //--------------------------------------------------------------------------
    // Public Constants:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Public Static Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Public Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Private Constants:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Private Static Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Private Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Constructors/Destructors:
    //--------------------------------------------------------------------------

#pragma region Constructors

    PersistenceEntry::PersistenceEntry(void)
        : NamedObject()
        , Variant()
        , getter(nullptr)
        , setter(nullptr)
    {
    }

    PersistenceEntry::PersistenceEntry(string_view_arg name)
        : NamedObject(name)
        , Variant()
        , getter(nullptr)
        , setter(nullptr)
    {
    }

    PersistenceEntry::PersistenceEntry(const PersistenceEntry& other)
        : NamedObject(other)
        , Variant(other)
        , getter(other.getter)
        , setter(other.setter)
    {
    }

    PersistenceEntry::PersistenceEntry(PersistenceEntry&& other) noexcept
        : NamedObject(std::move(other))
        , Variant(std::move(other))
        , getter(std::exchange(other.getter, nullptr))
        , setter(std::exchange(other.setter, nullptr))
    {
    }

    PersistenceEntry& PersistenceEntry::operator=(const PersistenceEntry& other)
    {
        NamedObject::operator=(other);
        Variant::operator=(other);
        getter = other.getter;
        setter = other.setter;
        return *this;
    }

    PersistenceEntry& PersistenceEntry::operator=(PersistenceEntry&& other) noexcept
    {
        NamedObject::operator=(std::move(other));
        Variant::operator=(std::move(other));
        getter = std::exchange(other.getter, nullptr);
        setter = std::exchange(other.setter, nullptr);
        return *this;
    }

    //--------------------------------------------------------------------------

    PersistenceEntry::~PersistenceEntry(void)
    {
    }

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

#pragma endregion Public Static Functions

    //--------------------------------------------------------------------------
    // Public Functions:
    //--------------------------------------------------------------------------

#pragma region Public Functions

    bool PersistenceEntry::Read([[maybe_unused]] StreamIn& stream)
    {
        return false;
    }

    bool PersistenceEntry::Write([[maybe_unused]] StreamOut& stream)
    {
        stream.WriteObjectStart(GetName());

        stream.WriteObjectEnd();
        return false;
    }

    PersistenceEntry::getter_t PersistenceEntry::GetGetter(void) const
    {
        return getter;
    }

    void PersistenceEntry::SetGetter(getter_t getter_)
    {
        getter = getter_;
    }

    PersistenceEntry::setter_t PersistenceEntry::GetSetter(void) const
    {
        return setter;
    }

    void PersistenceEntry::SetSetter(setter_t setter_)
    {
        setter = setter_;
    }

#pragma endregion Public Functions

    //--------------------------------------------------------------------------
    // Private Static Functions:
    //--------------------------------------------------------------------------

#pragma region Private Static Functions

#pragma endregion Private Static Functions

    //--------------------------------------------------------------------------
    // Private Functions:
    //--------------------------------------------------------------------------

#pragma region Private Functions

#pragma endregion Private Functions

}	// namespace
