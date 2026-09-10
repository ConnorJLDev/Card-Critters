//------------------------------------------------------------------------------
//
// File Name:	BehaviorParty.h
// Author(s):	Barend Brink (barend.brink)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Behavior class for party
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Behavior.h"
#include "BehaviorCharacter.h"
#include "MessageSystem.h"
#include "BehaviorCard.h"
#include "DataParty.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	// Forward Declarations:
	class Entity;
	struct CollisionRecord;

	// Typedefs:

	// Class Definition:
	class BehaviorParty : public Behavior
	{
		friend class BehaviorCharacterSelector;
		friend class BehaviorHand;
		friend class TurnSystem;
		friend class DataParty;
		friend class CheatSystem;

		// Public Constants, Enums, and Structs:
	public:
		// Common component data.
		COMPONENT_COMMON(BehaviorParty);

		// Constructors/Destructors:
	public:
		BehaviorParty(void);

		BehaviorParty(const BehaviorParty& other);

		~BehaviorParty(void);

		// Public Static Functions:

	public:

		// Public Functions:
	public:
		/// <summary>
		/// Clone the BehaviorParty.
		/// </summary>
		/// <returns>A clone of this BehaviorParty.</returns>
		BehaviorParty* Clone() const override;

		/// <summary>
		/// Read BehaviorParty data from a stream.
		/// </summary>
		/// <param name="stream">The stream to read from.</param>
		void Read(StreamIn& stream) override;

		/// <summary>
		/// Writes the BehaviorParty data into the stream.
		/// </summary>
		/// <param name="stream">The stream to write into.</param>
		void Write(StreamOut& stream) override;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const vector_t<BehaviorCharacter*>& GetParty() const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		bool HasPlayableCards() const;

		/// <summary>
		/// 
		/// </summary>
		void UpdateCharacterPositions() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="character"></param>
		void RemoveFromParty(BehaviorCharacter* character);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="character"></param>
		void MoveAlly(BehaviorCharacter* character);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="character"></param>
		bool MoveAlly(BehaviorCharacter* character, BehaviorCharacter* target);
		
		/// <summary>
		/// 
		/// </summary>
		/// <param name="msg"></param>
		void Receive(Message msg);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		int GetCurrentEnergy() const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		int GetMaxEnergy() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="inp"></param>
		void SetCurrentEnergy(int inp);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="inp"></param>
		void SetHand(BehaviorHand* inp);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		BehaviorHand* GetHand() const;

		/// <summary>
		/// 
		/// </summary>
		void Turn_Text();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		int GetTotalHealth();

		bool AnyAnimationsPlaying();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		bool GetEnergyChanged();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="changed"></param>
		void SetEnergyChanged(bool changed);

		/// <summary>
		/// Gets the speed at which characters move to their target.
		/// </summary>
		/// <returns>The character move speed.</returns>
		float GetCharacterMoveSpeedScaled() const;

		/// <summary>
		/// Sets the speed at which characters move to their target.
		/// </summary>
		/// <param name="speed">The new speed.</param>
		void SetCharacterMoveSpeed(float speed);

		/// <summary>
		/// Gets the range within which characters snap to their target.
		/// </summary>
		/// <returns>The character snap range.</returns>
		float GetCharacterMoveSnap() const;

		/// <summary>
		/// Sets the range within which characters snap to their target.
		/// </summary>
		/// <param name="snap">The new snapping range.</param>
		void SetCharacterMoveSnap(float snap);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		bool GetActive();

		void RecruitMember(const string_t& name);
		void ClearPlannedAction();
		void PlanTurnActions(BehaviorParty* targetParty);
		BehaviorCharacter* GetCharacterAtSlot(int slot) const;
		void DebugPrintPlannedAction(const char* context) const;
		int GetCardPriority(BehaviorCard* card) const; //Helper to get what cards to prio...
		// Public Event Handlers
	public:



		// Private Constants, Enums, and Structs:
		struct PlannedAction
		{
			BehaviorCharacter* attacker = nullptr;
			BehaviorCard* plannedCard = nullptr;
			int targetSlot = -1;
			int energyCost = 0;
			bool targetsAllies = false;
			bool valid = false;
		};

	private:

		enum BehaviorPartyStates
		{
			cPartyInvalid = -1,	// Party has not yet been initialized.
			cPartyIdle,			// Party is idle.
			cPartyWaiting,		// Enemy attack in process
			cPartyNext,			// Enemy attack finished
			cPartyDone,
			cPartyRecruit,
			cPartyTransition,
		};

		static const StreamKey ReadKeyCharacters, ReadKeyNextScene, ReadKeyIsPlayer, ReadKeyPreset;

		static const StreamKey ChildHand;

		// Private Static Functions:
	private:

		// Private Functions:
	private:
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

		/// <summary>
		/// 
		/// </summary>
		/// <param name="character"></param>
		void AddToParty(BehaviorCharacter* character);

		/// <summary>
		/// Automatic choices for the AI.
		/// </summary>
		void AIPlay();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="isActive"></param>
		void SetActive(bool isActive);


        void UpdateNextTargetIndicators();
        void ClearNextTargetIndicators();


		// Private Static Variables:
	private:

		// Private Variables:
	private:
		vector_t<BehaviorCharacter*> characters_;
		vector_t<string_t> characterNames_;

		float partyStartPos_;
		float partyOffsetPos_;
		float tweenSpeed_;  // increase to make transition quicker
		float tweenSpeedScaled_;
		float tweenSnapDistance_;        // distance to tweenSnapDistance_ to target

		int maxEnergy_;		  // maximum energy value
		int currentEnergy_;    // current energy value
		bool energyChanged_;
		CallbackID CID_;
		BehaviorHand* hand_;
		bool playerControlled_;
		float aiTimer_;
		float aiTimerCurrent_;
		string_t nextScene_;
		vector_t<PlannedAction> plannedActions_;
		size_t plannedActionIndex_ = 0;
		bool nextTurnPrepared_ = false;
        static constexpr const char* ChildNextTargetArrow = "NextTargetArrow";
	};


}	// namespace
