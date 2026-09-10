//------------------------------------------------------------------------------
//
// File Name:	BehaviorCard.h
// Author(s):	Barend Brink (barend.brink)
// Project:		Card Critters
// Course:		GAM200
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
#include "BehaviorInteractable.h"
#include "DataCard.h"
#include "MessageSystem.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	// Forward Declarations:
	class BehaviorHand;
	class BehaviorCharacter;
	struct CollisionRecord;
	enum class Message;

	// Typedefs:

	// Class Definition:
	class BehaviorCard : public BehaviorInteractable
	{
		friend class BehaviorHand;
		friend class BehaviorCharacterSelector;
		friend class BehaviorParty;

		// Public Constants, Enums, and Structs:
	public:
		// Common component data.
		COMPONENT_COMMON(BehaviorCard);

		// Constructors/Destructors:
	public:
		BehaviorCard();

		BehaviorCard(const BehaviorCard& other);

		~BehaviorCard();

		void CardSetDamage(int dmg);
		void CardHeal(int heal);
		void CardSetCost(int cost);
		void CardSetMove(int move);
		void CardSetDefense(int def);



		DataCard::CardType CardGetCType();
		int CardGetDamage();
		int CardGetHeal();
		int CardGetCost();
		int CardGetMove();
		int CardGetDefense();
		int GetIndex();

		// Public Static Functions:
	public:

		// Public Functions:
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name=""></param>
		/// <returns></returns>
		BehaviorCard* Clone(void) const override;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="stream"></param>
		void Read(StreamIn& stream) override;

		/// <summary>
		/// Writes the BehaviorCard data into the stream.
		/// </summary>
		/// <param name="stream">The stream to write into.</param>
		void Write(StreamOut& stream) override;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="hand"></param>
		void SetParentHand(class BehaviorHand* hand);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		BehaviorHand* GetParentHand() const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		bool GetIsHovered() const;

		/// <summary>
		/// Helped function called when the card is clicked.
		/// </summary>
		void Use();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		bool IsPlayable() const;

		// Public Event Handlers
	public:

		// Private Constants, Enums, and Structs:
	private:

		enum CardStates
		{
			cCardInvalid = -1,	// Card has not yet been initialized.
			cCardIdle,			// Card is idle.
			cCardHover,			// Card is being hovered over.
			cCardSelect,		// Card is clicked on.
			cCardLocked,		// Card is locked for the enemy turn.
			cCardInactive,		// Card is not in the active hand.
		};

		enum class CardGroup : char
		{
			cInvalid,
			cHand,
			cDeck,
			cDiscard,
			cBanished,
			cGroupCount_,
		};

		static const StreamKey NameMouse, ChildText, ChildCostText, ChildAccuracyText, ChildCritText, CardAccuracyIcon, CardCritIcon, ChildTooltip;

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
		/// Callback for when the parent entity of this behavior experiences a collision.
		/// </summary>
		/// <param name="record">The record of the collision.</param>
		void CollisionHandler(const CollisionRecord& record);

		/// <summary>
		/// Callback receiver for the message system.
		/// </summary>
		/// <param name="msg">The message payload.</param>
		void Receive(Message msg);

		/// <summary>
		/// Callback when the mouse enters the collider.
		/// </summary>
		void OnHoverStart() override;

		/// <summary>
		/// Callback when the mouse exits the collider.
		/// </summary>
		void OnHoverEnd() override;

		/// <summary>
		/// Callback when a mouse button is pressed while in the collider.
		/// </summary>
		void OnClickStart() override;

		/// <summary>
		/// Callback when a mouse button is released while in the collider.
		/// </summary>
		/// <param name="contained">True if the click stayed entirely contained within the collider, false otherwise.</param>
		void OnClickEnd(bool contained) override;

		void CreateTooltip();
		void DestroyTooltip();

		// Private Static Variables:
	private:

		// Private Variables:
	private:
		bool isHovered_;
		bool isClicked_;
		bool hasClicked_;
		bool isLocked_;

		BehaviorHand* hand_;
		CardGroup group_;
		Vec2 tweenTarget_;

		DataCard* data_;

		CallbackID CID_;

		// Tooltip helpers
		Entity* tooltipEntity_;
		Vec2 tooltipOffset_;
	};

}	// namespace