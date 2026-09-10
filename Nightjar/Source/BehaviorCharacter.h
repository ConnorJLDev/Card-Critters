//------------------------------------------------------------------------------
//
// File Name:	BehaviorCharacter.h
// Author(s):	JSand
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Template class for a new behavior.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "BehaviorHealth.h"
#include "MessageSystem.h"
#include "DataCard.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	// Forward Declarations:
	class DataCard;
	class Animation;
	class Entity;

	// Typedefs:

	// Class Definition:
	class BehaviorCharacter : public BehaviorHealth
	{
		friend class BehaviorParty;

		// Public Constants, Enums, and Structs:
	public:
		// Common component data.
		COMPONENT_COMMON(BehaviorCharacter);

		enum CharacterStatusEffect : unsigned int
		{
			cNone = 0,
			cBleed = 1 << 0,
			cDisarm = 1 << 1,
		};

		// Status effects applied to this character
		struct StatusInstance
		{
			DataCard::CardStatusEffect type = DataCard::CardStatusEffect::None;
			int duration = 0;
			int magnitude = 0; // For bleed: damage per turn. For other statuses use as needed.
		};
		

		// Constructors/Destructors:
	public:
		BehaviorCharacter();

		BehaviorCharacter(const BehaviorCharacter& other);

		~BehaviorCharacter(void);

		// Public Static Functions:
	public:

		// Public Functions:
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		BehaviorCharacter* Clone() const override;

		/// <summary>
		/// Read BehaviorCharacter data from a stream.
		/// </summary>
		/// <param name="stream">The stream to read from.</param>
		void Read(StreamIn& stream) override;

		/// <summary>
		/// Writes the BehaviorCharacter data into the stream.
		/// </summary>
		/// <param name="stream">The stream to write into.</param>
		void Write(StreamOut& stream) override;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		vector_t<DataCard>& GetCards();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const vector_t<DataCard>& GetCardsConst() const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		BehaviorParty* GetParty() const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		int GetPartyPosition();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		BehaviorParty* GetPartyPointer();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="value"></param>
		void SetMaxHealth(int value);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="deck"></param>
		void SetCards(const vector_t<DataCard>& deck);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="amount"></param>
		void DamageText(int amount);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="amount"></param>
		void CritText(int amount);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="amount"></param>
		void HealText(int amount);

		/// <summary>
		/// 
		/// </summary>
		void MissText();

		void StatusText(DataCard::CardStatusEffect type);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		string_t GetCharacterName();

		/// <summary>
		/// 
		/// </summary>
		void IdleState();

		/// <summary>
		/// 
		/// </summary>
		void AttackState();

		/// <summary>
		/// 
		/// </summary>
		void DamagedState();

		/// <summary>
		/// 
		/// </summary>
		void SelectedState();

		void Deselect();

		/// <summary>
		/// 
		/// </summary>
		void LoadCards();

		// Status API
		void AddStatus(DataCard::CardStatusEffect type, int duration, int magnitude = 0);
		void RemoveStatus(DataCard::CardStatusEffect type);
		bool HasStatus(DataCard::CardStatusEffect type) const;
        bool HasNoStatus() const;

		// Called when this character's party becomes active (start of its turn)
		void OnTurnStart();

		// Public Event Handlers
	public:
        void ShowNextTargetArrow(bool show);
		// Protected Constants, Enums, and Structs:
	protected:

		enum BehaviorCharacterStates
		{
			cBehaviorCharacterInvalid = -1,	// BehaviorCharacter has not yet been initialized.
			cBehaviorCharacterIdle,			// BehaviorCharacter is idle.
			cBehaviorCharacterAttack,       // BehaviorCharacter is attacking.
			cBehaviorCharacterDamaged,		// BehaviorCharacter has taken damage.
			cBehaviorCharacterDead,			// BehaviorCharacter is dead.
			cBehaviorCharacterSelected      // BehaviorCharacter is hovered or selected.
		};

        static const StreamKey ChildVisual, ChildSelector, ReadKeyCharacterName, ReadKeyCards, ChildStatusIcon, ChildNextTargetArrow;

		// Protected Static Functions:
	protected:

		// Protected Functions:
	protected:
		/// <summary>
		/// Creates/updates child entities.
		/// </summary>
		void UpdateVisual();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		Animation* GetChildAnimation();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		void UpdateStatusEffect();

        void UpdateStatusEffectDisplay();

		/// <summary>
		/// Called on initializing/entering a new state.
		/// </summary>
		void OnInit() override;

		/// <summary>
		/// Called when the base behavior updates.
		/// </summary>
		/// <param name="dt">The time elapsed since the previous tick.</param>
		void OnUpdate(float dt) override;

		/// <summary>
		/// Called on exiting an old state.
		/// </summary>
		void OnExit() override;

        //void ShowNextTargetArrow(bool show);
	protected:

		// Protected Static Variables:
	protected:

		// Protected Variables:
	protected:
		// Cards this character owns
		vector_t<DataCard> cards_;
		vector_t<string_t> cardNames_;
		string_t characterName_;
		BehaviorParty* party_;

		Vec2 tweenTarget_;

		bool hasAttacked_;
		bool attackWaiting_;
		bool returnToSelected_;

		CallbackID CID_;

		Vec4 originalTint_{ 1.0f, 1.0f, 1.0f, 1.0f };
		bool hasSavedOriginalTint_ = false;

		vector_t<StatusInstance> statuses_;

        // Multiple status icon entities
        vector_t<Entity*> statusIconEntities_;
        Vec2 statusIconOffset_;
        // Spacing to apply between icons
        Vec2 statusIconSpacing_{ 0.2f, 0.0f };
	};
}	// namespace
