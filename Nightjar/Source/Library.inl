//------------------------------------------------------------------------------
//
// File Name:	Library.inl
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
#include "StreamIn.h"
#include "StreamInValidator.h"
#include "IOSystem.h"
#include "IOSystem.inl"

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

	template <typename T>
	Library<T>::Library()
		: Container<T>()
#ifdef _DEBUG
		, validatorStr_()
#endif
		, dirCallback_()
	{
#ifdef _DEBUG
		bool success = IOSystem::ReadFileAsString(IOSystem::GetDataPath("Schemas") + "Base.json", validatorStr_);

		assert(success);

		StringReplaceAll(validatorStr_, "ReadKey", T::ReadKey);
#endif
	}

	//--------------------------------------------------------------------------

	template <typename T>
	Library<T>::~Library(void)
	{
		IOSystem::UnregisterChangeCallback(dirCallback_);
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

	template <typename T>
	const T* Library<T>::Get(string_view_arg name)
	{
		return Get_(name);
	}

	template<typename T>
	void Library<T>::InitDirectoryCallback()
	{
		IOSystem::RegisterChangeCallback([this](string_view_arg filepath)
		{
			if (filepath.starts_with(T::ReadDir))
			{
				string_view_t name = filepath.substr(T::ReadDir.size() + 1);
				TraceInfo("Library {} recognized edit of file \"{}\" as \"{}\"", T::ReadKey, filepath, name);
				size_t count = std::erase_if(Container<T>::items_, [name](const auto& item) { return item->IsNamed(name); });
				TraceInfo("Library {} erased {} items named \"{}\"", T::ReadKey, count, name);
			}
		});
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

	template<typename T>
	T* Library<T>::Get_(string_view_arg name)
	{
		if (name.empty())
		{
			return nullptr;
		}
		T* obj;
		if (!Container<T>::Find(name, obj))
		{
			obj = Build(name);
			if (obj != nullptr)
			{
				Container<T>::Add(obj);
			}
		}
		return obj;
	}

	template <typename T>
	T* Library<T>::Build(string_view_arg name)
	{
		return BuildDefault(name);
	}

	template <typename T>
	T* Library<T>::BuildDefault(string_view_arg name)
	{
		bool success{};
		StreamIn stream = IOSystem::OpenStreamIn<T>(name, success);
		if (!success)
		{
			return nullptr;
		}

#ifdef _DEBUG
		string_t validatorStr = validatorStr_;
		StringReplaceAll(validatorStr, "RequiredName", name);
		StreamIn::Validator validator = StreamIn::Validator::Create(validatorStr);

		if (!validator.Validate(stream))
		{
			TraceError("{} file \"{}\" does not follow schema: \"{}\"", T::ReadKey, name, stream.GetPath());
			assert(false);
			return nullptr;
		}
#endif

		T* item = new T();
		item->Read(stream);

		return item;
	}

#pragma endregion Private Functions

}	// namespace

#include "ImGuiLibrary.inl"
