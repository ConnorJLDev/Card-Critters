//------------------------------------------------------------------------------
//
// File Name:	Component.cpp
// Author(s):	Jason Nguyen (jason.nguyen)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Implementation of the Component class.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Component.h"
#include "Entity.h"

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

    Component::Component(TypeEnum type)
        : type_(type)
        , parent_(nullptr)
    {
    }

    Component::Component(const Component& other)
        : type_(other.type_)
        , parent_(nullptr)
    {
    }

    Component::Component(Component&& other) noexcept
        : type_(other.type_)
        , parent_(std::exchange(other.parent_, nullptr))
    {
    }

    Component& Component::operator=(Component&& other) noexcept
    {
        if (this != &other)
        {
            assert(type_ == other.type_ && "Move assignment can only be performed between components of the same type!");
            parent_ = std::exchange(other.parent_, nullptr);
        }

        return *this;
    }

    //--------------------------------------------------------------------------

    Component::~Component(void)
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

    void Component::Read([[maybe_unused]] StreamIn& stream)
    {
    }

    void Component::Write([[maybe_unused]] StreamOut& stream)
    {
    }

    void Component::Update([[maybe_unused]] float dt)
    {
    }

    void Component::Render(void) const
    {
    }

    Component::TypeEnum Component::GetType(void) const
    {
        return type_;
    }

    Entity* Component::GetParent(void) const
    {
        return parent_;
    }

    void Component::SetEntity(Entity* entity)
    {
        parent_ = entity;
    }

    const string_t& Component::EntityName(void) const
    {
        return GetParent()->GetName();
    }

    bool Component::EntityNamed(string_view_arg name) const
    {
        return GetParent()->IsNamed(name);
    }

    Scene* Component::GetScene(void) const
    {
        return GetParent()->GetScene();
    }

#pragma endregion Public Functions

    //--------------------------------------------------------------------------	
    // Private Static Functions:
    //--------------------------------------------------------------------------

#pragma region Private Static Functions

#pragma endregion Private Static Functions

    //--------------------------------------------------------------------------	
    // Protected Functions:
    //--------------------------------------------------------------------------

#pragma region Protected Functions

#pragma endregion Protected Functions

}	// namespace
