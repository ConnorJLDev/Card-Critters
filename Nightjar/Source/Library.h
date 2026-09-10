//------------------------------------------------------------------------------
//
// File Name:	Library.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Base class for an object library.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Container.h"

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
	class Library : public Container<T>
	{
		// Public Constants, Enums, and Structs:
	public:

		// Constructors/Destructors:
	public:
		Library(void);

		~Library(void);

		// Public Static Functions:
	public:

		// Public Functions:
	public:
		//
		const T* Get(string_view_arg name);

		//
		void InitDirectoryCallback();

		//
		void DisplayIMGUI();

		//
		template<typename TPtr, std::enable_if_t<std::is_same_v<std::decay_t<TPtr>, T>, bool> = false>
		bool DisplayIMGUISelector(string_view_arg name, TPtr*& current);

		// Public Event Handlers
	public:

		// Protected Constants, Enums, and Structs:
	protected:

		// Protected Static Functions:
	protected:

		// Protected Functions:
	protected:
		//
		T* Get_(string_view_arg name);

		//
		T* Build(string_view_arg name);

		//
		T* BuildDefault(string_view_arg name);

		// Protected Static Variables:
	protected:

		// Private Variables:
	private:
#ifdef _DEBUG
		string_t validatorStr_;
#endif
		CallbackID dirCallback_;
        unordered_map_t<string_t, T*> entries_;
	};

}	// namespace
