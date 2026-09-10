//------------------------------------------------------------------------------
//
// File Name:	$safeitemname$.cpp
// Author(s):	$username$
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Template class for a new system.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "$safeitemname$.h"
#include "BaseSystem.h"

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
    // Class Definition:
    class $safeitemname$_ SYSTEM_FINAL : public BaseSystem
    {
        friend $safeitemname$;

        SYSTEM_COMMON($safeitemname$_);

        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    private:
        $safeitemname$_(void);

        // All systems need a virtual destructor to have their destructor called 
        ~$safeitemname$_(void) override;

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
        /// Updates the system.
        /// </summary>
        void Update(void) override;

        /// <summary>
        /// Renders the system.
        /// </summary>
        void Render(void) override;

        /// <summary>
        /// Exits and shuts down the system.
        /// </summary>
        void Exit(void) override;

        // Private Static Variables:
    private:
        // The instance of the system.
        static $safeitemname$_* instance;

        // Private Variables:
    private:
    };

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

    $safeitemname$_* $safeitemname$_::instance = nullptr;

#define inst $safeitemname$_::instance

    //--------------------------------------------------------------------------
    // Private Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Constructors/Destructors:
    //--------------------------------------------------------------------------

#pragma region Constructors

    $safeitemname$_::$safeitemname$_(void)
        : BaseSystem("$safeitemname$")
    {
        // Raise an assert if this system has already been created.
        assert(instance == nullptr);

        // Store this system's instance for use by static functions.
        instance = this;
    }

    //--------------------------------------------------------------------------

    $safeitemname$_::~$safeitemname$_(void)
    {
    }

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

    BaseSystem* $safeitemname$::GetInstance(void)
    {
        return inst != nullptr ? inst : new $safeitemname$_();
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

    bool $safeitemname$_::Init(void)
    {
        // Add system-specific initialization code here.

        // Return true if the initialization completed successfully.
        return true;
    }

    void $safeitemname$_::Update(void)
    {
    }

    void $safeitemname$_::Render(void)
    {
    }

    void $safeitemname$_::Exit(void)
    {
    }

#pragma endregion Private Functions

}	// namespace
