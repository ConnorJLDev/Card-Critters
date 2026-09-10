//------------------------------------------------------------------------------
//
// File Name:	Factory.h
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
	// Forward Declarations:

	// Typedefs:

	// Class Definition:
	template <typename T>
	class Factory
	{
		// Public Constants, Enums, and Structs:
	public:

		// Constructors/Destructors:
	public:
		Factory() = delete;
		
		~Factory() = delete;

		// Public Static Functions:
	public:

		static T* Build(const T*& other, Container<T>* container = nullptr);

		static T* Build(Library<T>& library, string_view_arg name, Container<T>* container = nullptr);

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

		// Private Static Variables:
	private:

		// Private Variables:
	private:
	};

}	// namespace

#include "Factory.inl"
