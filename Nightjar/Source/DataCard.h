//------------------------------------------------------------------------------
//
// File Name:	DataCard.h
// Author(s):	barend.brink, jason.nguyen
// Course:		GAM200F25
// Project:		Critter Cards
// Purpose:		Card class data object
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	// Forward Declarations:
	class BehaviorCharacter;
	class Sound;

	// Typedefs:
	
	// Class Definition:
	class DataCard : public NamedObject
	{
		friend class BehaviorCard;

		// Public Constants and Enums:
	public:

		DECLARE_READ_KEY(Card);

		enum class CardType : unsigned int
		{
			Invalid	= 0,
			Damage				= 1 << 0,
			Move				= 1 << 1,
			Defense				= 1 << 2,
			Heal				= 1 << 3,
			Accuracy			= 1 << 4,
			CriticalHitChance	= 1 << 5,
			DamageAOE			= 1 << 6,
			SplashDamage		= 1 << 7,
			StatusEffect		= 1 << 8,
		};

		enum class CardStatusEffect : unsigned int
		{
			None = 0,
			Bleed		= 1 << 0,
			Disarm		= 1 << 1,
			Poison      = 1 << 2,
		};

		// Constructors/Destructors:
	public:
		DataCard(void);

		DataCard(const DataCard& other);

		DataCard(DataCard&& other) noexcept;

		DataCard& operator=(const DataCard& other);

		virtual ~DataCard(void);

		// Public Static Functions:
	public:
		ENUM_DECLARE_OPS(CardType);

		// Public Functions:
	public:
		void Read(StreamIn& stream);

		void Write(StreamOut& stream) const;

		void Use(BehaviorCharacter* target);

		CardType GetType() const;

		void SetOwner(BehaviorCharacter* owner);

		BehaviorCharacter* GetOwner(void) const;

#ifdef _EDITOR
		void DisplayIMGUI();
#endif
		// Public Event Handlers
	public:

		// Private Functions:
	private:

		// Private Constants:
	private:
		// Keys for reading from stream.
		static const StreamKey ReadKeyType, ReadKeyCost, ReadKeyDamage, ReadKeyHitCount, ReadKeySplashDirection, ReadKeyHeal, ReadKeyMove, ReadKeyDefense, ReadKeyAccuracy, ReadKeyCriticalHitChance, ReadKeyCombo, ReadKeyStatusEffect;

		// Value for reading type.
		static const StreamKey ReadValueCombo, ReadValueAttack, ReadValueHitCount, ReadValueSplashDamage, ReadValueAttackAOE, ReadValueDefend, ReadValueMove, ReadValueHeal, ReadValueAccuracy, ReadValueCriticalHitChance, ReadValueStatusEffect;

		// Map of CardType values for reading from stream.
		ENUM_MAPS_CLASS_DECLARE(CardType);

		// Private Static Variables:
	private:

		// Private Variables:
	private:
		int cost;
		int damage;
		int heal;
		int move;
		int defense;
		int accuracy;
		int criticalhitchance;
		int hitcount;
		int splashdir;
		CardStatusEffect statuseffect;
		int statusduration;
		int statusdamage;
		
		CardType type;
		BehaviorCharacter* character_;
		const Sound* sound_;
	};

}	// namespace
