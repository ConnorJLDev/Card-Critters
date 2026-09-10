//------------------------------------------------------------------------------
//
// File Name:	DataParty.cpp
// Author(s):	Connor Lariviere, Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class for containing, saving, and loading the state of a party.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "DataParty.h"

#include "BehaviorParty.h"
#include "BehaviorCharacter.h"
#include "DataCard.h"
#include "Entity.h"
#include "DataParty.h"

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

    DataParty::DataParty(void)
        : NamedObject()
        , members_()
    {
    }

    DataParty::DataParty(const DataParty& other)
        : NamedObject(other)
        , members_(other.members_)
    {
    }

    DataParty::DataParty(DataParty&& other) noexcept
        : NamedObject(std::move(other))
        , members_(std::move(other.members_))
    {
    }

    DataParty& DataParty::operator=(const DataParty& other)
    {
        NamedObject::operator=(other);
        members_ = other.members_;
        return *this;
    }

    //--------------------------------------------------------------------------

    DataParty::~DataParty(void)
    {
    }

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

    // Add member by name (if not already present). New members have no saved health/maxHealth by default
    void DataParty::AddMember(const string_t& name)
    {
        EnsureLoaded();

        if (Find(name) != nullptr)
        {
            TraceInfo("PlayerSaveState: Member '{}' already exists in save, skipping AddMember", name);
            return;
        }

        members_.emplace_back(name);
        //dirty_ = true;
        TraceInfo("PlayerSaveState: Added new member '{}' to save cache", name);
    }

    bool DataParty::HasParty()
    {
        EnsureLoaded();
        return !members_.empty();
    }

    vector_t<string_t> DataParty::PartyNames() const
    {
        EnsureLoaded();
        vector_t<string_t> names;
        names.reserve(members_.size());
        for (const auto& member : members_)
        {
            names.push_back(member.GetName());
        }
        return names;
    }

    void DataParty::ApplyToCharacter(BehaviorCharacter* character) const
    {
        assert(character != nullptr);

        EnsureLoaded();

        // Use entity name as the key since we will name everything the same
        const string_t keyName = character->EntityName();

        const DataCharacter* saved = Find(keyName);
        if (saved == nullptr)
        {
            return;
        }

        saved->ApplyToCharacter(character);
    }

    void DataParty::CaptureFromParty(const BehaviorParty* party)
    {
        if (party == nullptr)
        {
            return;
        }
        EnsureLoaded();

        for (BehaviorCharacter* bc : party->GetParty())
        {
            assert(bc != nullptr);

            const string_t& name = bc->EntityName();

            DataCharacter* slot = FindMutable(name);
            if (slot == nullptr)
            {
                slot = &members_.emplace_back(name);
            }
            slot->CaptureFromCharacter(bc);
        }
    }

    void DataParty::ApplyToParty(BehaviorParty* party) const
    {
        party->characterNames_ = PartyNames();
    }

#pragma endregion Public Static Functions

    //--------------------------------------------------------------------------
    // Public Functions:
    //--------------------------------------------------------------------------

#pragma region Public Functions
    
    void DataParty::Read(StreamIn& stream)
    {
        if (!stream.StepInto("SavedParty"))
        {
            TraceWarning("PlayerSaveState: Missing 'SavedParty' object in {}", stream.GetHierarchy());
            return;
        }

        //Read Members from file
        if (stream.StreamHas("Members"))
        {
            members_.clear();
            members_.reserve((size_t)stream.GetArraySize("Members"));
            stream.ReadArrayObjects("Members", [this](StreamIn& stream, int)
            {
                DataCharacter& member = members_.emplace_back();
                member.Read(stream);
            });
        }
        else
        {
            TraceWarning("PlayerSaveState: Missing 'Members' array in {}", stream.GetHierarchy());
        }

        stream.StepBack();
    }

    void DataParty::Write(StreamOut& stream) const
    {
        stream.WriteObjectStart("SavedParty");
        stream.WriteArrayStart("Members");
        for (const auto& member : members_)
        {
            stream.WriteObjectStart();
            member.Write(stream);
            stream.WriteObjectEnd();
        }
        stream.WriteArrayEnd();
        stream.WriteObjectEnd();
    }

#pragma endregion Public Functions

    //--------------------------------------------------------------------------
    // Private Static Functions:
    //--------------------------------------------------------------------------

#pragma region Private Static Functions

    void DataParty::EnsureLoaded() const
    {
        /*if (!loaded_ || dirty_)
        {
            LoadPartyFromFile();
        }*/
    }

    DataCharacter* DataParty::FindMutable(const string_t& name)
    {
        const auto iter = std::find_if(members_.begin(), members_.end(), [name](auto& member) { return member.IsNamed(name); });
        return iter == members_.end() ? nullptr : &(*iter);
    }

    const DataCharacter* DataParty::Find(const string_t& name) const
    {
        const auto iter = std::find_if(members_.cbegin(), members_.cend(), [name](auto& member) { return member.IsNamed(name); });
        return iter == members_.end() ? nullptr : &(*iter);
    }

#pragma endregion Private Static Functions

    //--------------------------------------------------------------------------
    // Private Functions:
    //--------------------------------------------------------------------------

#pragma region Private Functions

#pragma endregion Private Functions

}	// namespace
