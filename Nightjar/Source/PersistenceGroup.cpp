//------------------------------------------------------------------------------
//
// File Name:	PersistenceGroup.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Describes a group of data entries within the persistence system.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "PersistenceGroup.h"

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

    PersistenceGroup::PersistenceGroup(void)
        : NamedObject()
        , path_()
        , entries_()
    {
    }

    PersistenceGroup::PersistenceGroup(string_view_arg name)
        : NamedObject(name)
        , path_()
        , entries_()
    {
    }

    PersistenceGroup::PersistenceGroup(const PersistenceGroup& other)
        : NamedObject(other)
        , path_()
        , entries_(other.entries_)
    {
    }

    PersistenceGroup::PersistenceGroup(PersistenceGroup&& other) noexcept
        : NamedObject(std::move(other))
        , path_(std::move(other.path_))
        , entries_(std::move(other.entries_))
    {
    }

    PersistenceGroup& PersistenceGroup::operator=(const PersistenceGroup& other)
    {
        NamedObject::operator=(other);
        entries_ = other.entries_;
        return *this;
    }

    PersistenceGroup& PersistenceGroup::operator=(PersistenceGroup&& other) noexcept
    {
        NamedObject::operator=(std::move(other));
        path_ = std::move(other.path_);
        entries_ = std::move(other.entries_);
        return *this;
    }

    //--------------------------------------------------------------------------

    PersistenceGroup::~PersistenceGroup(void)
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

    bool PersistenceGroup::Read(void)
    {
        StreamIn stream(path_);
        if (!stream.IsValid() || !stream.StreamHas(ReadKey))
        {
            return false;
        }

        stream.ReadObjectKeyValues(ReadKey, [this](string_view_arg key, StreamIn& stream, [[maybe_unused]] int index)
        {
            PersistenceEntry entry(key);
            if (entry.Read(stream))
            {
                entries_.emplace(std::piecewise_construct, std::forward_as_tuple(key), std::forward_as_tuple(std::move(entry)));
            }
        });

        // TODO: Loop through all objects and call PersistenceEntry::Read on them.

        return true;
    }

    bool PersistenceGroup::Write(void)
    {
        StreamOut stream(path_);
        if (!stream.IsValid())
        {
            return false;
        }

        stream.WriteObjectStart(ReadKey);

        for (auto& [entryName, entry] : entries_)
        {
            entry.Write(stream);
        }

        stream.WriteObjectEnd();

        stream.Write();

        return true;
    }

    void PersistenceGroup::Clear(void)
    {
        entries_.clear();
    }

    string_view_t PersistenceGroup::GetPath(void) const
    {
        return path_;
    }

    void PersistenceGroup::SetPath(string_view_arg path)
    {
        path_ = path;
    }

    PersistenceEntry* PersistenceGroup::GetEntry(string_view_arg entryName)
    {
        auto iter = entries_.find(entryName);
        return iter != entries_.end() ? &iter->second : nullptr;
    }

    PersistenceEntry* PersistenceGroup::CreateEntry(string_view_arg entryName)
    {
        auto existIter = entries_.find(entryName);
        bool found = existIter != entries_.end();
        if (found)
        {
            return nullptr;
        }
        auto iter = entries_.emplace(std::piecewise_construct,
            std::forward_as_tuple(entryName),
            std::forward_as_tuple());
        return &iter.first->second;
    }

    bool PersistenceGroup::DestroyEntry(string_view_arg entryName)
    {
        auto iter = entries_.find(entryName);
        bool found = iter != entries_.end();
        if (found)
        {
            entries_.erase(iter);
        }
        return found;
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
