//------------------------------------------------------------------------------
//
// File Name:	DataCharacter.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class for containing, saving, and loading the state of a character.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "DataCard.h"
#include "BehaviorCharacter.h"

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
	class DataCharacter : public NamedObject
	{
		// Public Constants, Enums, and Structs:
	public:

		// Constructors/Destructors:
	public:
		DataCharacter(void);

		DataCharacter(string_view_arg name);

		DataCharacter(const DataCharacter& other);

		DataCharacter(DataCharacter&& other) noexcept;

		DataCharacter& operator=(const DataCharacter& other);

		~DataCharacter(void);

		// Public Static Functions:
	public:

		// Public Functions:
	public:
		//
		void Read(StreamIn& stream);

		//
		void Write(StreamOut& stream) const;

		//
		void ApplyToCharacter(BehaviorCharacter* character) const; // save cache -> runtime

		//
		void CaptureFromCharacter(const BehaviorCharacter* character);   // runtime -> save cache

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
		int health_;
		int maxHealth_;
		BehaviorCharacter::CharacterStatusEffect status_;
		vector_t<DataCard> cards_;
	};

}	// namespace
