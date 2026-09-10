//------------------------------------------------------------------------------
//
// File Name:	DataParty.h
// Author(s):	Connor Lariviere, Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class for containing, saving, and loading the state of a party.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "DataCharacter.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	// Forward Declarations:
	class BehaviorParty;
	class BehaviorCharacter;

	// Typedefs:

	// Class Definition:
	class DataParty : public NamedObject
	{
		// Public Constants, Enums, and Structs:
	public:

		// Constructors/Destructors:
	public:
		DataParty(void);

		DataParty(const DataParty& other);

		DataParty(DataParty&& other) noexcept;

		DataParty& operator=(const DataParty& other);

		~DataParty(void);

		// Public Static Functions:
	public:

		// Public Functions:
	public:
		//
		void Read(StreamIn& stream);

		//
		void Write(StreamOut& stream) const;

		/// <summary>
		/// Checks if the party is empty.
		/// </summary>
		/// <returns>True if there are any members in the party, false otherwise.</returns>
		bool HasParty();

		//
		vector_t<string_t> PartyNames() const;

		// runtime -> save cache
		void CaptureFromParty(const BehaviorParty* party);

		// save cache -> runtime
		void ApplyToParty(BehaviorParty* party) const;

		// save cache -> runtime
		void ApplyToCharacter(BehaviorCharacter* character) const;

		//
		void AddMember(const string_t& name);

		// Public Event Handlers
	public:

		// Private Constants, Enums, and Structs:
	private:

		// Private Static Functions:
	private:
		// Check to see if we loaded
		void EnsureLoaded() const;

		// Finds non const version to change
		DataCharacter* FindMutable(const string_t& name);

		// CONST ONLY DO NOT CHANGE READ ONLY FIND HELPS WITH DEBUG
		const DataCharacter* Find(const string_t& name) const;

		// Private Functions:
	private:

		// Private Static Variables:
	private:

		// Private Variables:
	private:
		vector_t<DataCharacter> members_;
	};

}	// namespace
