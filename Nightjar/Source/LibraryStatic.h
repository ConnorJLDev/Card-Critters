//------------------------------------------------------------------------------
//
// File Name:	Library.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Base class for a static object library.
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
	class LibraryStatic : public Library<T>
	{
		// Public Constants, Enums, and Structs:
	public:

		// Constructors/Destructors:
	protected:
		LibraryStatic();
	public:
		~LibraryStatic(void);

		// Public Static Functions:
	public:
		//
		static Library<T>* GetInstance() noexcept;

		//
		static const T* Get_S(string_view_arg name);

		//
		static void FreeAll_S();

		//
		static void Init_S();

		//
		static void Shutdown_S();

		//
		static void DisplayIMGUI_S();

		//
		template<typename TPtr, std::enable_if_t<std::is_same_v<std::decay_t<TPtr>, T>, bool> = false>
		static bool DisplayIMGUISelector_S(string_view_arg name, TPtr*& current);

		// Public Functions:
	public:

		// Public Event Handlers
	public:

		// Protected Constants, Enums, and Structs:
	protected:

		// Protected Static Functions:
	protected:

		// Protected Functions:
	protected:

		// Protected Static Variables:
	protected:
		static LibraryStatic* library;

		// Private Variables:
	private:
	};

}	// namespace
