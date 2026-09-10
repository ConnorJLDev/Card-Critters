//------------------------------------------------------------------------------
//
// File Name:	NamedObject.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Base class for objects with a name.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "NamedObject.h"

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

	constexpr StreamKey NamedObject::ReadKeyName = "Name";

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

	NamedObject::NamedObject(void)
		: Object()
		, name_()
	{
	}

    NamedObject::NamedObject(string_view_arg name)
        : Object()
        , name_(name)
    {
    }

	NamedObject::NamedObject(const NamedObject& other)
		: Object(other)
		, name_(other.name_)
	{
	}

	NamedObject::NamedObject(NamedObject&& other) noexcept
		: Object(std::move(other))
		, name_(std::move(other.name_))
	{
	}

	NamedObject& NamedObject::operator=(const NamedObject& other)
	{
		Object::operator=(other);
		name_ = other.name_;
		return *this;
	}

	NamedObject& NamedObject::operator=(NamedObject&& other) noexcept
	{
        Object::operator=(std::move(other));
        name_ = std::move(other.name_);
        return *this;
	}

	//--------------------------------------------------------------------------

	NamedObject::~NamedObject()
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

	const string_t& NamedObject::GetName(void) const
	{
		assert(this != nullptr);
		return name_;
	}

	bool NamedObject::IsNamed(string_view_arg name) const
	{
		assert(this != nullptr);
		return name_ == name;
	}

#pragma endregion Public Functions

	//--------------------------------------------------------------------------
	// Protected Static Functions:
	//--------------------------------------------------------------------------

#pragma region Protected Static Functions

	bool NamedObject::TryReadName(StreamIn& stream, string_t& name)
	{
		bool hasKey = stream.StreamHas(ReadKeyName);
		if (hasKey)
		{
			name = stream.ReadString(ReadKeyName);
		}
		return hasKey;
	}

	template <> StreamValue StreamOut::WriteValueRaw<NamedObject>(const NamedObject& value)
	{
		return WriteValueRaw(value.GetName());
	}

	template <> StreamValue StreamOut::WriteValueRaw<const NamedObject*>(const NamedObject* const& value)
	{
		return value == nullptr ? WriteValueRaw(string_view_t()) : WriteValueRaw(value->GetName());
	}

#pragma endregion Protected Static Functions

	//--------------------------------------------------------------------------
	// Protected Functions:
	//--------------------------------------------------------------------------

#pragma region Protected Functions

	void NamedObject::SetName(string_view_arg name)
	{
		assert(this != nullptr);
		name_ = name;
	}

	bool NamedObject::ReadName(StreamIn& stream)
	{
		assert(this != nullptr);
		bool hasKey = stream.StreamHas(ReadKeyName);
		if (hasKey)
		{
			SetName(stream.ReadString(ReadKeyName));
#ifdef _DEBUG
			string_t filepath = stream.GetPath(), filename, subdir;
			size_t index = filepath.find_first_of('/') + 1;
			size_t index2 = filepath.find_first_of('/', index) + 1;
			size_t indexEnd = filepath.find_last_of('.');
			filename = filepath.substr(index2, indexEnd - index2);
			subdir = filepath.substr(index, index2 - index);
			if (subdir != "Scenes/" && name_ != filename)
			{
				TraceWarning("Object has different name than file: \"{}\"|\"{}\" in file \"{}\"", name_, filename, filepath);
			}
#endif
		}
		else
		{
			TraceWarning("Failed to read name of object!");
		}
		return hasKey;
	}

#pragma endregion Private Functions

}	// namespace
