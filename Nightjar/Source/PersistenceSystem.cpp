//------------------------------------------------------------------------------
//
// File Name:	PersistenceSystem.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		System that manages persistent data, such as settings.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "PersistenceSystem.h"
#include "BaseSystem.h"

#include "PersistenceEntry.h"
#include "PersistenceGroup.h"

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
    //--------------------------------------------------------------------------
    // Class Implementation:
    //--------------------------------------------------------------------------

#pragma region Class Implementation

    class PersistenceSystem_ SYSTEM_FINAL : public BaseSystem
    {
        friend PersistenceSystem;

        SYSTEM_COMMON(PersistenceSystem_);

        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    private:
        PersistenceSystem_(void);

        // All systems need a virtual destructor to have their destructor called 
        ~PersistenceSystem_(void) override;

        // Public Static Functions:
    public:

        // Public Functions:
    public:

        // Public Event Handlers
    public:

        // Private Constants, Enums, and Structs:
    private:

        // Private Static Functions:
    private:

        // Private Functions:
    private:
        /// <summary>
        /// Initializes the system.
        /// </summary>
        /// <returns>True if initialization succeeded, false otherwise.</returns>
        bool Init(void) override;

        /// <summary>
        /// Exits and shuts down the system.
        /// </summary>
        void Exit(void) override;

        // Private Static Variables:
    private:
        // The instance of the system.
        static PersistenceSystem_* instance;

        // Private Variables:
    private:
        // Container of the entries.
        unordered_map_string_t<PersistenceGroup> groups_;
    };

#pragma endregion Class Implementation

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

    PersistenceSystem_* PersistenceSystem_::instance = nullptr;

#define inst PersistenceSystem_::instance

    //--------------------------------------------------------------------------
    // Private Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Constructors/Destructors:
    //--------------------------------------------------------------------------

#pragma region Constructors

    PersistenceSystem_::PersistenceSystem_(void)
        : BaseSystem("PersistenceSystem", LoopFlags::None)
    {
        // Raise an assert if this system has already been created.
        assert(instance == nullptr);

        // Store this system's instance for use by static functions.
        instance = this;
    }

    //--------------------------------------------------------------------------

    PersistenceSystem_::~PersistenceSystem_(void)
    {
    }

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

    BaseSystem* PersistenceSystem::GetInstance(void)
    {
        return inst != nullptr ? inst : new PersistenceSystem_();
    }

    PersistenceGroup* PersistenceSystem::GetGroup(string_view_arg groupName)
    {
        auto iter = inst->groups_.find(groupName);
        return iter != inst->groups_.end() ? &iter->second : nullptr;
    }

    PersistenceGroup* PersistenceSystem::CreateGroup(string_view_arg groupName)
    {
        auto existIter = inst->groups_.find(groupName);
        bool found = existIter != inst->groups_.end();
        if (found)
        {
            return nullptr;
        }
        auto iter = inst->groups_.emplace(std::piecewise_construct,
            std::forward_as_tuple(groupName),
            std::forward_as_tuple(groupName));
        return &iter.first->second;
    }

    bool PersistenceSystem::DestroyGroup(string_view_arg groupName)
    {
        auto iter = inst->groups_.find(groupName);
        bool found = iter != inst->groups_.end();
        if (found)
        {
            inst->groups_.erase(iter);
        }
        return found;
    }

#pragma endregion Public Static Functions

    //--------------------------------------------------------------------------
    // Public Functions:
    //--------------------------------------------------------------------------

#pragma region Public Functions

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

    bool PersistenceSystem_::Init(void)
    {
        // Add system-specific initialization code here.

        // Return true if the initialization completed successfully.
        return true;
    }

    void PersistenceSystem_::Exit(void)
    {
    }

#pragma endregion Private Functions

}	// namespace
