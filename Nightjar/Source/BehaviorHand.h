//------------------------------------------------------------------------------
//
// File Name:	BehaviorHand.h
// Author(s):	barend.brink, jason.nguyen, Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Behavior for hand entity
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
#include "MessageSystem.h"
#include "BehaviorCard.h"
#include "BehaviorParty.h"

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
	class BehaviorCard;
	class BehaviorCharacter;
	class Transform;
	struct CollisionRecord;

	// Typedefs:

	// Class Definition:
	class BehaviorHand : public Behavior
	{
		friend class BehaviorParty;

		// Public Constants, Enums, and Structs:
	public:
		// Common component data.
		COMPONENT_COMMON(BehaviorHand);

		// Constructors/Destructors:
	public:
		BehaviorHand(void);

		BehaviorHand(const BehaviorHand& other);

		~BehaviorHand(void);

		// Public Static Functions:
	public:

		// Public Functions:
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name=""></param>
		/// <returns></returns>
		BehaviorHand* Clone(void) const override;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="stream"></param>
		void Read(StreamIn& stream) override;

		/// <summary>
		/// Writes the BehaviorHand data into the stream.
		/// </summary>
		/// <param name="stream">The stream to write into.</param>
		void Write(StreamOut& stream) override;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="cardEntity"></param>
		void DiscardCard(DataCard* cardEntity);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="cardEntity"></param>
		void BanishCard(DataCard* cardEntity);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="cardData"></param>
		void AddCard(DataCard* cardData);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="origin"></param>
		/// <param name="destination"></param>
		/// <param name="index"></param>
		void MoveCard(vector_t<Entity*>& origin, vector_t<Entity*>& destination, int index);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="index"></param>
		void DiscardFromHand(int index);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="index"></param>
		void BanishFromHand(int index);

		/// <summary>
		/// 
		/// </summary>
		void FillHand();

		/// <summary>
		/// 
		/// </summary>
		void ClearHand();
		
		/// <summary>
		/// 
		/// </summary>
		void ShuffleDeck();

		/// <summary>
		/// 
		/// </summary>
		void ReturnDiscardPile();

		/// <summary>
		/// 
		/// </summary>
		void InitializeDeck();

		/// <summary>
		/// 
		/// </summary>
		void LockHand();

		/// <summary>
		/// 
		/// </summary>
		void UnlockHand();

		/// <summary>
		/// 
		/// </summary>
		void Energy_Text();

		/// <summary>
		/// 
		/// </summary>
		void Deck_Icon();


		/// <summary>
		/// 
		/// </summary>
		void Discard_Text();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		BehaviorCard* GetCard(int index);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		BehaviorParty* GetParentParty();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		bool HasPlayableCard() const;

		/// <summary>
		/// Gets the speed at which cards move to their target.
		/// </summary>
		/// <returns>The card move speed.</returns>
		float GetCardMoveSpeed() const;

		/// <summary>
		/// Sets the speed at which cards move to their target.
		/// </summary>
		/// <param name="speed">The new speed.</param>
		void SetCardMoveSpeed(float speed);

		/// <summary>
		/// Gets the range within which cards snap to their target.
		/// </summary>
		/// <returns>The card snap range.</returns>
		float GetCardMoveSnap() const;

		/// <summary>
		/// Sets the range within which cards snap to their target.
		/// </summary>
		/// <param name="snap">The new snapping range.</param>
		void SetCardMoveSnap(float snap);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="target"></param>
		void SetPartyTarget(BehaviorCharacter* target);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		BehaviorCharacter* GetPartyTarget();

		/// <summary>
		/// Creates the target indicator if it doesn't exist and positions it over the selected enemy
		/// </summary>
		void TargetIndicator();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="input"></param>
		void SetTargetVisible(bool input);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		BehaviorCard* GetActiveCard() const;

		// Public Event Handlers
	public:
		/// <summary>
		/// Gets the index of a card within the hand's card list.
		/// </summary>
		/// <param name="card">The card to search for.</param>
		/// <returns>The index of the card, or -1 if not found.</returns>
		int GetCardIndex(const BehaviorCard* card) const;
		// Private Constants, Enums, and Structs:
		int GetCardCount() const;
		// BehaviorHand.h
		BehaviorCard* GetCardAt(int index) const;
	private:
		typedef BehaviorCard* CardRef;

		enum BehaviorHandStates
		{
			cHandInvalid = -1,	// BehaviorHand has not yet been initialized.
			cHandIdle,			// BehaviorHand is idle.
		};

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
		/// Sets the card positions for all child cards.
		/// </summary>
		void UpdateCardPositions();

		/// <summary>
		/// Finds a card in our vector of cards.
		/// </summary>
		/// <param name="card">The card to search for.</param>
		/// <returns>An iterator containing the CardPackage with the card if found, else the end iterator.</returns>
		vector_t<CardRef>::iterator FindCard(DataCard* card);

		/// <summary>
		/// Callback receiver for the message system.
		/// </summary>
		/// <param name="msg">The message payload.</param>
		void Receive(Message msg);
		
		
		// Private Static Variables:
	private:

		// Private Variables:
	private:

		vector_t<CardRef> cards_;

		unsigned int handSize_;

		// Card Shift Tuning
		float tweenSpeed_ ;  // increase to make transition quicker
		float tweenSnapDistance_;        // distance to tweenSnapDistance_ to target

		array_t<Vec2, (size_t)BehaviorCard::CardGroup::cGroupCount_> origins_;
		array_t<Vec2, (size_t)BehaviorCard::CardGroup::cGroupCount_> spacings_;

		Entity* handBgEntity_;
		BehaviorParty* parentParty_;
		BehaviorCharacter* partyTarget_;
		CallbackID CID_;
		Transform* targetT_;
		BehaviorCard* hovered_ = nullptr;
		BehaviorCard* selected_ = nullptr;
	};

}	// namespace
