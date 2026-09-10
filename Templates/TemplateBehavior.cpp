//------------------------------------------------------------------------------
//
// File Name:	$safeitemname$.cpp
// Author(s):	$username$
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Template class for a new behavior.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Entity.h"
#include "Transform.h"
#include "Sprite.h"
#include "Collider.h"
#include "$safeitemname$.h"

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

    $safeitemname$::$safeitemname$(void)
        : Behavior()
    {
    }

    $safeitemname$::$safeitemname$(const $safeitemname$& other)
        : Behavior(other)
    {
    }

    //--------------------------------------------------------------------------

    $safeitemname$::~$safeitemname$(void)
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

    $safeitemname$* $safeitemname$::Clone() const
    {
        return new $safeitemname$(*this);
    }

    void $safeitemname$::Read(StreamIn& stream)
    {
        UNREFERENCED_PARAMETER(stream);
    }

    void $safeitemname$::Write(StreamOut& stream)
    {
        UNREFERENCED_PARAMETER(stream);
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

    void $safeitemname$::OnStartup()
    {
        Collider* collider = GetParent()->Has(Collider);
        if (collider != nullptr)
        {
            collider->SetCollisionHandler([this](const CollisionRecord& record) { CollisionHandler(record); });
        }
    }

    void $safeitemname$::OnInit()
    {
        switch (GetStateCurrent())
        {
        case States::cIdle:
            break;
        }
    }

    void $safeitemname$::OnUpdate(float dt)
    {
        UNREFERENCED_PARAMETER(dt);

        switch (GetStateCurrent())
        {
        case States::cIdle:
            break;
        }
    }

    void $safeitemname$::OnExit()
    {
        switch (GetStateCurrent())
        {
        case States::cIdle:
            break;
        }
    }

    void $safeitemname$::CollisionHandler(const CollisionRecord& record)
    {
    }

#pragma endregion Private Functions

}	// namespace
