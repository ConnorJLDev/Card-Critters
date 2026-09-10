//------------------------------------------------------------------------------
//
// File Name:	Object.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Base class for objects.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Object.h"

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

    ObjectID Object::nextId = 0;

    //--------------------------------------------------------------------------
    // Private Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Constructors/Destructors:
    //--------------------------------------------------------------------------

#pragma region Constructors

    Object::Object(void)
        : guid_(++nextId)
    {
    }

    Object::Object(const Object&)
        : guid_(++nextId)
    {
    }

    Object::Object(Object&& other) noexcept
        : guid_(std::exchange(other.guid_, 0))
    {
    }

    Object& Object::operator=(const Object&)
    {
        return *this;
    }

    Object& Object::operator=(Object&& other) noexcept
    {
        if (&other != this)
        {
            guid_ = std::exchange(other.guid_, 0);
        }
        return *this;
    }

    //--------------------------------------------------------------------------

    Object::~Object(void)
    {
    }

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

    static constexpr string_view_t memName = "Object";
    static constexpr size_t memId = MEM_ID(memName);

    void* Object::operator new(size_t size)
    {
        return MemoryManager::Allocate(size, memId);
    }

    void* Object::operator new[](size_t size)
    {
        return MemoryManager::Allocate(size, memId);
    }

    void Object::operator delete(void* ptr) noexcept
    {
        MemoryManager::Deallocate(ptr, memId);
    }

    void Object::operator delete[](void* ptr) noexcept
    {
        MemoryManager::Deallocate(ptr, memId);
    }

#pragma endregion Public Static Functions

    //--------------------------------------------------------------------------
    // Public Functions:
    //--------------------------------------------------------------------------

#pragma region Public Functions

    const ObjectID& Object::GetGUID(void) const
    {
        assert(this != nullptr);
        return guid_;
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
