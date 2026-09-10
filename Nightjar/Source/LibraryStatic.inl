//------------------------------------------------------------------------------
//
// File Name:	LibraryStatic.inl
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

#include "Container.h"
#include "Library.h"
#include "Library.inl"
#include "ImGuiLibrary.inl"
#include "StreamIn.h"
#include "StreamOut.h"
#include "IOSystem.h"

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

	template <typename T>
	LibraryStatic<T>* LibraryStatic<T>::library{ nullptr };

	//--------------------------------------------------------------------------
	// Private Variables:
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// Constructors/Destructors:
	//--------------------------------------------------------------------------

#pragma region Constructors

	template <typename T>
	LibraryStatic<T>::LibraryStatic()
		: Library<T>()
	{
		assert(library == nullptr);

		library = this;
	}

	//--------------------------------------------------------------------------

	template <typename T>
	LibraryStatic<T>::~LibraryStatic(void)
	{
		assert(Container<T>::Count() == 0 && "FreeAll_S must be called on this library before engine exit!");

		library = nullptr;
	}

#pragma endregion Constructors

	//--------------------------------------------------------------------------
	// Public Static Functions:
	//--------------------------------------------------------------------------

#pragma region Public Static Functions

	template <typename T>
	Library<T>* LibraryStatic<T>::GetInstance() noexcept
	{
		return library == nullptr ? new LibraryStatic<T>() : library;
	}

	template <typename T>
	const T* LibraryStatic<T>::Get_S(string_view_arg name)
	{
		return library->Get(name);
	}

	template <typename T>
	void LibraryStatic<T>::FreeAll_S()
	{
		library->FreeAll();
	}

	template<typename T>
	void LibraryStatic<T>::Init_S()
	{
		GetInstance();
		library->Library<T>::InitDirectoryCallback();
	}

	template<typename T>
	void LibraryStatic<T>::Shutdown_S()
	{
		FreeAll_S();
		SafeDeleteCheck(library);
	}

	template<typename T>
	void LibraryStatic<T>::DisplayIMGUI_S()
	{
		library->Library<T>::DisplayIMGUI();
	}

	template<typename T>
	template<typename TPtr, std::enable_if_t<std::is_same_v<std::decay_t<TPtr>, T>, bool>>
	bool LibraryStatic<T>::DisplayIMGUISelector_S(string_view_arg name, TPtr*& current)
	{
		return library->Library<T>::DisplayIMGUISelector(name, current);
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

#pragma endregion Private Functions

#define INSTANTIATE_LIBRARY_IMGUI_SELECTOR(itemType)\
	template bool LibraryStatic<itemType>::DisplayIMGUISelector_S<itemType>(string_view_arg name, itemType*& current);\
	template bool LibraryStatic<itemType>::DisplayIMGUISelector_S<const itemType>(string_view_arg name, const itemType*& current);\

#define IMPLEMENT_READ(itemType)\
	template <> bool StreamIn::TryReadValueInternal<const itemType*>(const StreamValue& streamValue, const itemType*& value)\
	{\
		string_t name;\
		if (!TryReadValueInternal(streamValue, name))\
		{\
			return false;\
		}\
		if (name.empty())\
		{\
			lastError_ = ReadValueError::InvalidValue;\
			return false;\
		}\
		value = LibraryStatic<itemType>::Get_S(name);\
		return true;\
	}\

#define IMPLEMENT_WRITE(itemType)\
	template <> StreamValue StreamOut::WriteValueRaw<itemType>(itemType const& value) { return WriteValueRaw<NamedObject>(value); }\
	template <> StreamValue StreamOut::WriteValueRaw<itemType*>(itemType* const& value) { return WriteValueRaw<const NamedObject*>(value); }\
	template <> StreamValue StreamOut::WriteValueRaw<const itemType*>(const itemType* const& value) { return WriteValueRaw<const NamedObject*>(value); }\

}	// namespace
