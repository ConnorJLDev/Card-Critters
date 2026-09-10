//------------------------------------------------------------------------------
//
// File Name:	Factory.inl
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Base class for an object factory.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Library.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	//--------------------------------------------------------------------------
	// Public Static Functions:
	//--------------------------------------------------------------------------

#pragma region Public Static Functions

	template<typename T>
	T* Factory<T>::Build(const T*& other, Container<T>* container)
	{
		if (other == nullptr)
		{
			return nullptr;
		}
		T* objNew = new T(*other);
		if (container != nullptr && objNew != nullptr)
		{
			container->Add(objNew);
		}
		return objNew;
	}

	template<typename T>
	T* Factory<T>::Build(Library<T>& library, string_view_arg name, Container<T>* container)
	{
		const T* obj = library.Get(name);
		return Build(obj, container);
	}

#pragma endregion Public Static Functions

}	// namespace
