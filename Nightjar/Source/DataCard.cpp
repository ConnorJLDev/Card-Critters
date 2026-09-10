//------------------------------------------------------------------------------
//
// File Name:	DataCard.cpp
// Author(s):	barend.brink, jason.nguyen
// Course:		GAM200F25
// Project:		Critter Cards
// Purpose:		Card class data object
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "DataCard.h"
#include "BehaviorCharacter.h"
#include "Sound.h"
#include "Random.h"
#include "BehaviorParty.h"

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

	#define OVERHEALLIMIT 10

	//--------------------------------------------------------------------------
	// Public Static Variables:
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// Public Variables:
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// Private Constants:
	//--------------------------------------------------------------------------

	constexpr StreamKey
		DataCard::ReadKeyType = "Card Type",
		DataCard::ReadKeyCost = "Cost",
		DataCard::ReadKeyDamage = "Damage",
		DataCard::ReadKeyHitCount = "Hit Count",
		DataCard::ReadKeyHeal = "Heal Amount",
		DataCard::ReadKeyMove = "Move",
		DataCard::ReadKeyDefense = "Defense",
		DataCard::ReadKeyCombo = "Combo",
		DataCard::ReadKeyAccuracy = "Accuracy",
		DataCard::ReadKeyCriticalHitChance = "Critical Hit Chance",
		DataCard::ReadKeySplashDirection = "Splash Direction",
		DataCard::ReadKeyStatusEffect = "Status Effect";

	constexpr StreamKey
		DataCard::ReadValueCombo = "COMBO",
		DataCard::ReadValueAttack = "ATTACK",
		DataCard::ReadValueAttackAOE = "AOE ATTACK",
		DataCard::ReadValueSplashDamage = "SPLASH ATTACK",
		DataCard::ReadValueDefend = "DEFENSE",
		DataCard::ReadValueHeal = "HEAL",
		DataCard::ReadValueMove = "MOVE",
		DataCard::ReadValueAccuracy = "ACCURACY",
		DataCard::ReadValueCriticalHitChance = "CRITICAL",
		DataCard::ReadValueStatusEffect = "STATUS";

	ENUM_MAP_WRITE(DataCard::, CardType,
		Damage,
		DamageAOE,
		Defense,
		Heal,
		Move,
		Accuracy,
		CriticalHitChance,
		SplashDamage,
		StatusEffect
		);

	ENUM_MAP_READ_BACKUP(DataCard::, CardType,
		Damage, "ATTACK",
		DamageAOE, "AOE ATTACK",
		Defense, "DEFENSE",
		Heal, "HEAL",
		Move, "MOVE",
		Accuracy, "ACCURACY",
		CriticalHitChance, "CRITICAL",
		SplashDamage, "SPLASH ATTACK",
		StatusEffect, "STATUS"
		);

	ENUM_IMPLEMENT_OPS(DataCard::CardType);

	//--------------------------------------------------------------------------
	// Private Static Variables:
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// Private Variables:
	//--------------------------------------------------------------------------
	
	//--------------------------------------------------------------------------
	// Constructors/Destructors:
	//--------------------------------------------------------------------------

	DataCard::DataCard(void)
		: NamedObject()
		, cost(0)
		, damage(0)
		, heal(0)
		, move(0)
		, defense(0)
		, accuracy(0)
		, criticalhitchance(0)
		, hitcount(1)
		, splashdir(1)
		, statuseffect(CardStatusEffect::None)
		, statusduration(0)
		, statusdamage(0)
		, type(CardType::Invalid)
		, character_(nullptr)
		, sound_(nullptr)
	{
	}

	DataCard::DataCard(const DataCard& other)
		: NamedObject(other)
		, cost(other.cost)
		, damage(other.damage)
		, heal(other.heal)
		, move(other.move)
		, defense(other.defense)
		, accuracy(other.accuracy)
		, criticalhitchance(other.criticalhitchance)
		, hitcount(other.hitcount)
		, splashdir(other.splashdir)
		, statuseffect(other.statuseffect)
		, statusduration(other.statusduration)
		, statusdamage(other.statusdamage)
		, type(other.type)
		, character_(other.character_)
		, sound_(other.sound_)
	{
	}

	DataCard::DataCard(DataCard&& other) noexcept
		: NamedObject(other)
		, cost(std::move(other.cost))
		, damage(std::move(other.damage))
		, heal(std::move(other.heal))
		, move(std::move(other.move))
		, defense(std::move(other.defense))
		, accuracy(std::move(other.accuracy))
		, criticalhitchance(std::move(other.criticalhitchance))
		, hitcount(std::move(other.hitcount))
		, splashdir(std::move(other.splashdir))
		, statuseffect(other.statuseffect)
		, statusduration(other.statusduration)
		, statusdamage(other.statusdamage)
		, type(std::move(other.type))
		, character_(std::move(other.character_))
		, sound_(std::move(other.sound_))
	{
	}

	DataCard& DataCard::operator=(const DataCard& other)
	{
		SetName(other.GetName());
		cost = other.cost;
		damage = other.damage;
		heal = other.heal;
		move = other.move;
		defense = other.defense;
		accuracy = other.accuracy;
		criticalhitchance = other.criticalhitchance;
		hitcount = other.hitcount;
		splashdir = other.splashdir;
		statuseffect = other.statuseffect;
		statusduration = other.statusduration;
		statusdamage = other.statusdamage;
		type = other.type;
		character_ = other.character_;
		sound_ = other.sound_;
		return *this;
	}

	//--------------------------------------------------------------------------

	DataCard::~DataCard(void)
	{
	}

#pragma region Public Functions

	void DataCard::Read(StreamIn& stream)
	{
		if (!stream.StreamHas(ReadKey))
		{
			TraceWarning("Failed to find root key \"{}\" in stream \"{}\"", ReadKey, stream.GetPath());
			return;
		}
		stream.StepInto(ReadKey);

		ReadName(stream);

		if (stream.StreamHas(ReadKeyType))
		{
			string_t cardType = stream.ReadString(ReadKeyType);
			if (cardType == ReadValueCombo)
			{
				if (stream.StreamHas(ReadKeyCombo))
				{
					stream.TryReadEnumFlags(ReadKeyCombo, ENUM_READ(CardType), type);
				}
				else
				{
					TraceWarning("Card failed to read combo data after declaring combo type!");
				}
			}
			else
			{
				stream.TryReadEnum(ReadKeyType, ENUM_READ(CardType), type);
			}
		}

		stream.TryReadValue(ReadKeyCost, cost);
		stream.TryReadValue(ReadKeyDamage, damage);
		stream.TryReadValue(ReadKeyHeal, heal);
		stream.TryReadValue(ReadKeyMove, move);
		stream.TryReadValue(ReadKeyDefense, defense);
		stream.TryReadValue(ReadKeyAccuracy, accuracy);
		stream.TryReadValue(ReadKeyCriticalHitChance, criticalhitchance);
		stream.TryReadValue(ReadKeyHitCount, hitcount);
		stream.TryReadValue(ReadKeySplashDirection, splashdir);
			
		string_t statusStr;
		// Use the declared key and add logging so we can see what's being read.
		if (stream.TryReadValue(ReadKeyStatusEffect, statusStr) && !statusStr.empty())
		{
			TraceInfo("DataCard \"{}\" read Status Effect: \"{}\"", GetName(), statusStr);
			//"BLEED", "DISARM", "POISON"
			if (statusStr == "BLEED")
				statuseffect = CardStatusEffect::Bleed;
			else if (statusStr == "DISARM")
				statuseffect = CardStatusEffect::Disarm;
			else if (statusStr == "POISON")
				statuseffect = CardStatusEffect::Poison;
			else statuseffect = CardStatusEffect::None;
		}
		else
		{
			// If missing or empty, keep None but log for debugging.
			TraceInfo("DataCard \"{}\" has no Status Effect key or it is empty", GetName());
		}
		stream.TryReadValue("Status Duration", statusduration);
		stream.TryReadValue("Status Damage", statusdamage);

		string_t sound;
		if (stream.TryReadValue("Sound", sound) && !sound.empty())
		{
			sound_ = SoundLibrary::Get_S(sound);
		}

		stream.StepBack();
	}

	void DataCard::Write(StreamOut& stream) const
	{
		stream.WriteValue(ReadKeyType, ReadValueCombo);

		stream.WriteArrayStart(ReadKeyCombo);
		if ((bool)(type & DataCard::CardType::Damage))
		{
			stream.WriteValue(ReadValueAttack);
		}
		if ((bool)(type & DataCard::CardType::Defense))
		{
			stream.WriteValue(ReadValueDefend);
		}
		if ((bool)(type & DataCard::CardType::Heal))
		{
			stream.WriteValue(ReadValueHeal);
		}
		if ((bool)(type & DataCard::CardType::Move))
		{
			stream.WriteValue(ReadValueMove);
		}
		stream.WriteArrayEnd();

		stream.WriteValue(ReadKeyDamage, damage);
		stream.WriteValue(ReadKeyHeal, heal);
		stream.WriteValue(ReadKeyMove, move);
		stream.WriteValue(ReadKeyDefense, defense);
		stream.WriteValue(ReadKeyCost, cost);
	}

	void DataCard::Use(BehaviorCharacter* target)
	{
		assert(target != nullptr);
		assert(type != CardType::Invalid);

		if ((bool)(type & CardType::Damage))
		{
			TraceInfo("Used damage card for {}", damage);

			// 1. Check for Hit (Accuracy)
			for (int i = 0; i < hitcount; i++)
			{
				int finalDamage = damage;
				if (accuracy < 0 || Random::Range(0, 100) <= accuracy)
				{
					// 2. Check for Critical Hit
					if (criticalhitchance >= 0 && Random::Range(0, 100) <= criticalhitchance)
					{
						finalDamage *= 2;
						TraceInfo("CRITICAL HIT!");

						target->ModifyHealth(-finalDamage);
						target->CritText(finalDamage);
					}
					else
					{
						target->ModifyHealth(-finalDamage);
						target->DamageText(finalDamage);
					}
					// 3. Apply status effect if there is one
					if (statuseffect == CardStatusEffect::Bleed)
					{
						target->AddStatus(CardStatusEffect::Bleed, statusduration, statusdamage);
						target->StatusText(CardStatusEffect::Bleed);
					}
					else if (statuseffect == CardStatusEffect::Disarm)
					{
						target->AddStatus(CardStatusEffect::Disarm, statusduration, 0);
						target->StatusText(CardStatusEffect::Disarm);
					}
					else if (statuseffect == CardStatusEffect::Poison)
					{
						target->AddStatus(CardStatusEffect::Poison, statusduration, statusdamage);
						target->StatusText(CardStatusEffect::Poison);
					}
				}
				else
				{
					TraceInfo("Missed!");
					target->MissText();
				}
			}
		}
		if ((bool)(type & CardType::DamageAOE))
		{
			TraceInfo("Used damage card for {}", damage);
			BehaviorParty* party = target->GetPartyPointer();
			// 1. Check for Hit (Accuracy)
			for (int t = 0; t < hitcount; t++)
			{
				if (accuracy < 0 || Random::Range(0, 100) <= accuracy)
				{
					for (size_t x = 0; x < party->GetParty().size(); x++)
					{
						int finalDamage = damage;
						if (criticalhitchance >= 0 && Random::Range(0, 100) <= criticalhitchance)
						{
							finalDamage *= 2;
							TraceInfo("CRITICAL HIT!");
							if (!party->GetParty()[x]->IsDead())
								party->GetParty()[x]->CritText(finalDamage);
						}
						else
						{
							if (!party->GetParty()[x]->IsDead())
							{
								party->GetParty()[x]->DamageText(finalDamage);
							}
						}
						if (!party->GetParty()[x]->IsDead())
						{
							party->GetParty()[x]->ModifyHealth(-finalDamage);
						}
						// 3. Apply status effect if there is one
						if (statuseffect == CardStatusEffect::Bleed)
							party->GetParty()[x]->AddStatus(CardStatusEffect::Bleed, statusduration, statusdamage);
						else if (statuseffect == CardStatusEffect::Disarm)
							party->GetParty()[x]->AddStatus(CardStatusEffect::Disarm, statusduration, 0);
						else if (statuseffect == CardStatusEffect::Poison)
							party->GetParty()[x]->AddStatus(CardStatusEffect::Poison, statusduration, statusdamage);
					}
				}
				else
				{
					TraceInfo("Missed!");
					target->MissText();
				}
			}
		}
		if ((bool)(type & CardType::SplashDamage))
		{
			TraceInfo("Used damage card for {}", damage);
			BehaviorParty* party = target->GetPartyPointer();
			int x = target->GetPartyPosition();
			bool splashactive = false;
			size_t splash = 0;
			if (x + splashdir >= 0 && (size_t)(x + splashdir) < party->GetParty().size())
			{
				splash = (size_t)(x + (int)splashdir);
				splashactive = true;
			}
				// 1. Check for Hit (Accuracy)
			if (accuracy < 0 || Random::Range(0, 100) <= accuracy)
			{
				int finalDamage = damage;
				if (criticalhitchance >= 0 && Random::Range(0, 100) <= criticalhitchance)
				{
					finalDamage *= 2;
					TraceInfo("CRITICAL HIT!");
					if (!target->IsDead())
					{
						target->CritText(finalDamage);
					}
					if (splashactive && !party->GetParty()[splash]->IsDead())
					{
						party->GetParty()[splash]->CritText(finalDamage);
					}
				}
				else
				{
					if (!target->IsDead())
					{
						target->DamageText(finalDamage);
					}
					if (splashactive && !party->GetParty()[splash]->IsDead())
					{
						party->GetParty()[splash]->DamageText(finalDamage);
					}
				}
				if (!target->IsDead())
				{
					target->ModifyHealth(-finalDamage);
				}
				if (splashactive && !party->GetParty()[splash]->IsDead())
				{
					party->GetParty()[splash]->ModifyHealth(-finalDamage);
				}
				// 3. Apply status effect if there is one
				if (statuseffect == CardStatusEffect::Bleed)
				{
					if (!target->IsDead())
						target->AddStatus(CardStatusEffect::Bleed, statusduration, statusdamage);
					if (splashactive && !party->GetParty()[splash]->IsDead())
						party->GetParty()[splash]->AddStatus(CardStatusEffect::Bleed, statusduration, statusdamage);
				}
				else if (statuseffect == CardStatusEffect::Disarm)
				{
					if (!target->IsDead())
						target->AddStatus(CardStatusEffect::Disarm, statusduration, 0);
					if (splashactive && !party->GetParty()[splash]->IsDead())
						party->GetParty()[splash]->AddStatus(CardStatusEffect::Disarm, statusduration, 0);
				}
				else if (statuseffect == CardStatusEffect::Poison)
				{
					if (!target->IsDead())
						target->AddStatus(CardStatusEffect::Poison, statusduration, statusdamage);
					if (splashactive && !party->GetParty()[splash]->IsDead())
						party->GetParty()[splash]->AddStatus(CardStatusEffect::Poison, statusduration, statusdamage);
				}

				// And in the "miss" branch for splash, guard the splash MissText call:
				else
				{
					TraceInfo("Missed!");
					target->MissText();
					if (splashactive)
						party->GetParty()[splash]->MissText();
				}
			}
		}
		if ((bool)(type & CardType::Defense))
		{
			TraceInfo("Used defense card for {}", defense);
			//assert(false);
		}
		if ((bool)(type & CardType::Move))
		{
			TraceInfo("Used move card for {}", move);
			//assert(false);
		}
		if ((bool)(type & CardType::Heal))
		{
			TraceInfo("Used heal card for {}", heal);
			target->ModifyHealth(heal);
			target->HealText(heal);
			if (target->GetHealth() > target->GetMaxHealth() + OVERHEALLIMIT)
			{
				target->SetHealth(target->GetMaxHealth() + OVERHEALLIMIT);
			}
		}
	}

	DataCard::CardType DataCard::GetType() const
	{
		return type;
	}

	void DataCard::SetOwner(BehaviorCharacter* owner)
	{
		character_ = owner;
	}

	BehaviorCharacter* DataCard::GetOwner(void) const
	{
		return character_;
	}

#pragma endregion Public Functions

}	// namespace
