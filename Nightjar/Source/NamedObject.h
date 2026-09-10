//------------------------------------------------------------------------------
//
// File Name:	NamedObject.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Base class for objects with a name.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include <string>

#include "Object.h"

#include "StreamIn.h"

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
	class NamedObject : public Object
	{
		// Public Constants, Enums, and Structs:
	public:
		// The key used to read the name of an object from a stream.
		static const StreamKey ReadKeyName;

		// Constructors/Destructors:
	protected:
		NamedObject(void);

        NamedObject(string_view_arg name);

		NamedObject(const NamedObject& other);

		NamedObject(NamedObject&& other) noexcept;

		NamedObject& operator=(const NamedObject& other);

		NamedObject& operator=(NamedObject&& other) noexcept;

	public:
		~NamedObject(void);

		// Public Static Functions:
	public:

		// Public Functions:
	public:
		/// <summary>
		/// Gets the name of this object.
		/// </summary>
		/// <returns>The name of this object.</returns>
		const string_t& GetName(void) const;

		/// <summary>
		/// Checks if this object has a particular name.
		/// </summary>
		/// <param name="name">The name to compare.</param>
		/// <returns>True if the names match, false otherwise.</returns>
		bool IsNamed(string_view_arg name) const;

		//
		void DisplayIMGUI(bool editableName = false);

		// Public Event Handlers
	public:

		// Private Constants, Enums, and Structs:
	private:

		// Protected Static Functions:
	protected:
		/// <summary>
		/// Tries to read a name from a stream.
		/// </summary>
		/// <param name="stream">The stream to read from.</param>
		/// <param name="name">The string to receive the name.</param>
		/// <returns>True if the key was found, false otherwise.</returns>
		static bool TryReadName(StreamIn& stream, string_t& name);

		// Protected Functions:
	protected:
		/// <summary>
		/// Reads the name of this object from a stream.
		/// </summary>
		/// <param name="stream">The stream to read from.</param>
		/// <returns>True if the name was successfully read, false otherwise.</returns>
		bool ReadName(StreamIn& stream);

		/// <summary>
		/// Sets the name of this object.
		/// </summary>
		/// <param name="name">The new name for this object.</param>
		void SetName(string_view_arg name);

		// Private Static Variables:
	private:

		// Private Variables:
	private:
		// The name of this object.
		string_t name_;
	};

}	// namespace
