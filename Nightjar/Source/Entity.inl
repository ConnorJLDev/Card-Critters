//------------------------------------------------------------------------------
//
// File Name:	Entity.inl
// Author(s):	Connor Lariviere (connor.lariviere)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:      Class representing an Entity.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Entity.h"
#include "Component.h"

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

	template<typename T>
	T* Entity::GetComponent(Component::TypeEnum typeId) const
	{
		//return dynamic_cast<T*>(ComponentGet(typeId));
		return static_cast<T*>(ComponentGet(typeId));
	}

	template<typename type, typename subtype>
	subtype* Entity::GetComponentSub(Component::TypeEnum typeId, type::SubTypeEnum subtypeId) const
	{
		type* mainType = GetComponent<type>(typeId);
		//return dynamic_cast<subtype*>(mainType);
		return mainType != nullptr && mainType->GetSubType() == subtypeId ? static_cast<subtype*>(mainType) : nullptr;
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
