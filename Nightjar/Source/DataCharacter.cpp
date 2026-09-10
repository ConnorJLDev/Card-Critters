//------------------------------------------------------------------------------
//
// File Name:	DataCharacter.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class for containing, saving, and loading the state of a character.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

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

	DataCharacter::DataCharacter(void)
		: NamedObject()
		, health_(-1)
		, maxHealth_(-1)
		, status_()
		, cards_()
	{
	}

	DataCharacter::DataCharacter(string_view_arg name)
		: NamedObject(name)
		, health_(-1)
		, maxHealth_(-1)
		, status_()
		, cards_()
	{
	}

	DataCharacter::DataCharacter(const DataCharacter& other)
		: NamedObject(other)
		, health_(other.health_)
		, maxHealth_(other.maxHealth_)
		, status_(other.status_)
		, cards_(other.cards_)
	{
	}

	DataCharacter::DataCharacter(DataCharacter&& other) noexcept
		: NamedObject(std::move(other))
		, health_(std::move(other.health_))
		, maxHealth_(std::move(other.maxHealth_))
		, status_(std::move(other.status_))
		, cards_(std::move(other.cards_))
	{
	}

	DataCharacter& DataCharacter::operator=(const DataCharacter& other)
	{
		NamedObject::operator=(other);
		health_ = other.health_;
		maxHealth_ = other.maxHealth_;
		status_ = other.status_;
		cards_ = other.cards_;
		return *this;
	}

	//--------------------------------------------------------------------------

	DataCharacter::~DataCharacter(void)
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

	static vector_t<DataCard> BuildDeckFromNames(const vector_t<string_t>& cardNames)
	{
		vector_t<DataCard> deck;
		deck.reserve(cardNames.size());

		for (const string_t& name : cardNames)
		{
			StreamIn cardStream("Data/Cards/" + name + ".json");
			if (!cardStream.IsValid())
			{
				TraceWarning("PlayerSaveState: Failed to read card \"{}\" from {}", name, cardStream.GetPath());
				continue;
			}

			DataCard& card = deck.emplace_back();
			card.Read(cardStream);
		}
		return deck;
	}

	void DataCharacter::Read(StreamIn& stream)
	{
		NamedObject::ReadName(stream);
		stream.TryReadValue("Health", health_);
		stream.TryReadValue("MaxHealth", maxHealth_);

		if (stream.StreamHas("CardsRaw"))
		{
			cards_.clear();
			cards_.reserve((size_t)stream.GetArraySize("CardsRaw"));
			stream.ReadArrayObjects("CardsRaw", [this](StreamIn& stream, int)
			{
				DataCard& card = cards_.emplace_back();
				card.Read(stream);
			});
		}
		else if (stream.StreamHas("Cards"))
		{
			vector_t<string_t> cardNames;
			stream.ReadVector("Cards", cardNames);
			cards_ = BuildDeckFromNames(cardNames);
		}
		else
		{
			TraceWarning("Missing Cards array in {}", stream.GetHierarchy());
		}
	}

	void DataCharacter::Write(StreamOut& stream) const
	{
		stream.WriteValue("Name", GetName());
		stream.WriteValue("Health", health_);
		stream.WriteValue("MaxHealth", maxHealth_);

		stream.WriteArrayStart("CardsRaw");
		for (const auto& card : cards_)
		{
			stream.WriteObjectStart();
			card.Write(stream);
			stream.WriteObjectEnd();
		}
		stream.WriteArrayEnd();

		stream.WriteArrayStart("Cards");
		for (const auto& card : cards_)
		{
			stream.WriteValue(card.GetName());
		}
		stream.WriteArrayEnd();
	}

	void DataCharacter::ApplyToCharacter(BehaviorCharacter* character) const
	{
		if (maxHealth_ >= 0)
		{
			character->SetMaxHealth(maxHealth_);
		}
		if (health_ >= 0)
		{
			character->SetHealth(health_);
		}
		if (!cards_.empty())
		{
			character->SetCards(cards_);
		}
	}

	void DataCharacter::CaptureFromCharacter(const BehaviorCharacter* character)
	{
		SetName(character->EntityName());
		health_ = character->GetHealth();
		maxHealth_ = character->GetMaxHealth();
		cards_ = character->GetCardsConst();
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

#pragma endregion Private Functions

}	// namespace
