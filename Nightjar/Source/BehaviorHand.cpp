//------------------------------------------------------------------------------
//
// File Name:	BehaviorHand.cpp
// Author(s):	barend.brink, jason.nguyen, Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Behavior for hand entity
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Entity.h"
#include "BehaviorHand.h"
#include "BehaviorCard.h"
#include "BehaviorCharacter.h"
#include "EntityContainer.h"
#include "Random.h"
#include "Transform.h"
#include "AudioEmitter.h"
#include "Sprite.h"
#include "SpriteText.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
#define CardGroupLambda(cardGroup) ([](CardRef cardBhv) { return cardBhv->group_ == BehaviorCard::CardGroup::cardGroup; })
#define GetCardsOfGroup(cardGroup) (cards_ | std::views::filter(CardGroupLambda(cardGroup)))
#define CountCardsOfGroup(cardGroup) (std::ranges::count_if(cards_, CardGroupLambda(cardGroup)))

	//--------------------------------------------------------------------------
	// Public Constants:
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// Public Static Variables:
	//--------------------------------------------------------------------------
	static constexpr StreamKey ChildHandBackground = "HandBackground";
	//--------------------------------------------------------------------------
	// Public Variables:
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// Private Constants:
	//--------------------------------------------------------------------------

	static constexpr const char* ChildEnergyText = "EnergyText";
	static constexpr const char* ChildEnergyIcon = "EnergyIcon";
	static constexpr const char* ChildDeckIcon = "DeckIcon";
	static constexpr const char* ChildDiscardText = "HandDeckCountText";

	//--------------------------------------------------------------------------
	// Private Static Variables:
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// Private Variables:
	//--------------------------------------------------------------------------
	static float oldY = 0;
	//--------------------------------------------------------------------------
	// Constructors/Destructors:
	//--------------------------------------------------------------------------

#pragma region Constructors

	BehaviorHand::BehaviorHand()
		: Behavior(cBehaviorHand)
		, cards_()
		, handSize_(6)
		, tweenSpeed_(10.0f)
		, tweenSnapDistance_(0.5f)
		//				Invalid,	Hand,			Deck,			Discard,		Banish
		, origins_({	{0, 0},		{-300, -200},	{-400, -500},	{-400, -525},	{-400, -5000} })
		, spacings_({	{10, 0},	{160, 0},		{75, 0},		{75, 0},		{75, 0} })
		, handBgEntity_()
		, parentParty_()
		, partyTarget_()
		, CID_()
		, targetT_()
	{
		CID_ = MessageBus::Subscribe([this](Message msg) { Receive(msg); });
	}

	BehaviorHand::BehaviorHand(const BehaviorHand& other)
		: Behavior(other)
		, cards_(other.cards_)
		, handSize_(other.handSize_)
		, tweenSpeed_(other.tweenSpeed_)
		, tweenSnapDistance_(other.tweenSnapDistance_)
		, origins_(other.origins_)
		, spacings_(other.spacings_)
		, handBgEntity_()
		, parentParty_()
		, partyTarget_()
		, CID_()
		, targetT_()
	{
		CID_ = MessageBus::Subscribe([this](Message msg) { Receive(msg); });
	}

	BehaviorHand* BehaviorHand::Clone() const
	{
		return new BehaviorHand(*this);
	}

	//--------------------------------------------------------------------------

	BehaviorHand::~BehaviorHand()
	{
		AudioEmitter* audioEmitter = GetParent()->Has(AudioEmitter);
		if (audioEmitter)
		{
			audioEmitter->Pause();
		}
		MessageBus::Unsubscribe(CID_);
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

	void BehaviorHand::Read(StreamIn& stream)
	{
		UNREFERENCED_PARAMETER(stream);
	}

	void BehaviorHand::Write(StreamOut& stream)
	{
		UNREFERENCED_PARAMETER(stream);
	}

	void BehaviorHand::DiscardCard(DataCard* card)
	{
		auto cardIter = FindCard(card);
		if (cardIter != cards_.end())
		{
			(*cardIter)->group_ = BehaviorCard::CardGroup::cDiscard;
		}
	}

	void BehaviorHand::BanishCard(DataCard* card)
	{
		auto cardIter = FindCard(card);
		if (cardIter != cards_.end())
		{
			(*cardIter)->group_ = BehaviorCard::CardGroup::cBanished;
			FillHand();
		}
	}

	void BehaviorHand::AddCard(DataCard* card)
	{
		// Emplace a new entity with a name that starts with "Card" to our vector of card entities, and read from the file of the same name.
		string_t cardName = "Card" + card->GetOwner()->EntityName();
		Entity* cardEnt = new Entity(cardName);
		cardEnt->Read(cardName);

		// Card entities should always at least have a Transform and BehaviorCard.
		Transform* cardTfm = cardEnt->Has(Transform);
		assert(cardTfm != nullptr);
		BehaviorCard* cardBhv = cardEnt->HasSub(Behavior, BehaviorCard);
		assert(cardBhv != nullptr);

		// Add
		cards_.push_back(cardBhv);

		// This hand should always have a parent Entity with a Transform.
		Entity* handEnt = GetParent();
		Transform* handTfm = handEnt->Has(Transform);
		assert(handTfm != nullptr);

		// Adds the card entity to the same EntityContainer as this hand is in.
		handEnt->GetContainer()->Add(cardEnt);

		// Assigns the card transform as a child of the hand transform.
		cardTfm->SetParentTransform(handTfm);

		// Assign card pointer and hand pointer.
		cardBhv->data_ = card;
		cardBhv->hand_ = this;

		// A newly added card shall go to the deck.
		cardBhv->group_ = BehaviorCard::CardGroup::cDeck;
	}

	void BehaviorHand::MoveCard(vector_t<Entity*>& origin, vector_t<Entity*>& destination, int index)
	{
		if (index < 0 || index >= (int)origin.size())
		{
			return;
		}

		destination.push_back(origin[(size_t)index]);
		origin.erase(origin.begin() + index);
	}
	void BehaviorHand::DiscardFromHand(int index)
	{
		cards_[static_cast<size_t>(index)]->group_ = BehaviorCard::CardGroup::cDiscard;
	}

	void BehaviorHand::BanishFromHand(int index)
	{
		cards_[static_cast<size_t>(index)]->group_ = BehaviorCard::CardGroup::cBanished;
		this->FillHand();
	}

	void BehaviorHand::FillHand()
	{
		size_t handSize = static_cast<size_t>(CountCardsOfGroup(cHand));
		for (size_t i = handSize; i < handSize_; ++i)
		{
			if (CountCardsOfGroup(cDeck) <= 0)
			{
				ReturnDiscardPile();
				// If the deck failed to regain any cards, break.
				if (CountCardsOfGroup(cDeck) <= 0)
				{
					break;
				}
			}
			auto deckCards = GetCardsOfGroup(cDeck);
			CardRef& card = deckCards.front();
			card->group_ = BehaviorCard::CardGroup::cHand;
		}
	}

	void BehaviorHand::ClearHand()
	{
		auto handCards = GetCardsOfGroup(cHand);
		for (auto& card : handCards)
		{
			card->group_ = BehaviorCard::CardGroup::cDiscard;
		}
	}

	void BehaviorHand::ShuffleDeck()
	{
		for (int i = static_cast<int>(cards_.size() - 1); i > 0; --i)
		{
			int swapPos = Random::Range(0, i);
			std::swap(cards_[static_cast<size_t>(i)], cards_[static_cast<size_t>(swapPos)]);
		}
	}

	void BehaviorHand::ReturnDiscardPile()
	{
		auto discardCards = GetCardsOfGroup(cDiscard);
		for (auto& card : discardCards)
		{
			card->group_ = BehaviorCard::CardGroup::cDeck;
		}
		ShuffleDeck();
	}

	void BehaviorHand::InitializeDeck()
	{
		const auto& characters = parentParty_->GetParty();
		for (auto& character : characters)
		{
			assert(character != nullptr);
			auto& characterCards = character->GetCards();
			for (auto& card : characterCards)
			{
				AddCard(&card);
			}
		}
		ShuffleDeck();
	}

	void BehaviorHand::SetCardMoveSpeed(float speed)
	{
		tweenSpeed_ = speed;
	}

	float BehaviorHand::GetCardMoveSpeed() const
	{
		return tweenSpeed_;
	}

	void BehaviorHand::SetCardMoveSnap(float snap)
	{
		tweenSnapDistance_ = snap;
	}

	float BehaviorHand::GetCardMoveSnap() const
	{
		return tweenSnapDistance_;
	}

	void BehaviorHand::UpdateCardPositions()
	{
		int i[(size_t)BehaviorCard::CardGroup::cGroupCount_] = { 0 };
		hovered_ = nullptr;
		selected_ = nullptr;

		for (auto& card : cards_)
		{
			size_t group = (size_t)card->group_;
			int& groupIndex = i[group];
			card->tweenTarget_ = origins_[group] + (float)(groupIndex++) * spacings_[group];

			if (card->GetStateCurrent() == BehaviorCard::CardStates::cCardSelect)
			{
				if (selected_ == nullptr)
				{
					selected_ = card;
					card->tweenTarget_ += Vec2(0, 100);
				}
				else
				{
					card->SetStateNext(BehaviorCard::CardStates::cCardIdle);
				}
			}

			if (hovered_ == nullptr && card->GetStateCurrent() == BehaviorCard::CardStates::cCardHover)
			{
				hovered_ = card;
			}
		}
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

	void BehaviorHand::OnInit()
	{
		switch (GetStateCurrent())
		{
		case cHandIdle:
		{
			AudioEmitter* audioEmitter = GetParent()->Has(AudioEmitter);
			if (audioEmitter)
			{
				audioEmitter->Play();
			}
			ShuffleDeck();
			FillHand();
			SetPartyTarget(0);
			Deck_Icon();
		}
		break;
		}
	}

	void BehaviorHand::OnUpdate(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		BehaviorParty* curparty = GetParentParty();

		switch (GetStateCurrent())
		{
		case cHandIdle:
			Energy_Text();
			SetTargetVisible(parentParty_->GetActive());
			UpdateCardPositions();
			for (auto& card : cards_)
			{
				assert(card != nullptr);

				if (selected_ && card != selected_)
				{
					card->SetStateNext(BehaviorCard::CardStates::cCardLocked);
					SetTargetVisible(true);
					continue;
				}

				if (!selected_ && card->GetStateCurrent() == BehaviorCard::CardStates::cCardLocked)
				{
					card->SetStateNext(BehaviorCard::CardStates::cCardIdle);
				}

				if (card->CardGetCost() > curparty->GetCurrentEnergy())
				{
					card->SetStateNext(BehaviorCard::CardStates::cCardInactive);
				}
			}
			Discard_Text();
			break;
		}
	}

	void BehaviorHand::OnExit()
	{
		switch (GetStateCurrent())
		{
		case cHandInvalid:
			handBgEntity_ = GetOrMakeChildEntity(ChildHandBackground);
			targetT_ = nullptr;
			break;
		}
	}

	vector_t<BehaviorHand::CardRef>::iterator BehaviorHand::FindCard(DataCard* card)
	{
		return std::find_if(cards_.begin(), cards_.end(), [card](CardRef bhvCard)
		{
			return bhvCard->data_ == card;
		});
	}

	void BehaviorHand::Receive(Message msg)
	{
		switch (msg)
		{
		case Message::IsPlayerTurn:
			FillHand();
			SetTargetVisible(true);
			break;
		case Message::IsEnemyTurn:
			SetTargetVisible(false);
			break;
		}
	}

	int BehaviorHand::GetCardIndex(const BehaviorCard* card) const
	{
		for (unsigned int i = 0; i < (int)cards_.size(); ++i)
		{
			if (cards_[i] == card)
				return (int)i;
		}
		return -1;
	}

	int BehaviorHand::GetCardCount() const
	{
		return (int)cards_.size();
	}

	BehaviorCard* BehaviorHand::GetCardAt(int index) const
	{
		if (index < 0 || index >= (int)cards_.size())
			return nullptr;
		return cards_[(size_t)index];
	}

	void BehaviorHand::SetPartyTarget(BehaviorCharacter* target)
	{
		//assert(target != nullptr);

		if (target != partyTarget_)
		{
			if (partyTarget_)
				partyTarget_->Deselect();
			partyTarget_ = target;
			TargetIndicator();
		}
	}

	BehaviorCharacter* BehaviorHand::GetPartyTarget()
	{
		return partyTarget_;
	}

    void BehaviorHand::TargetIndicator()
    {
        Entity* targetEntity = nullptr;

        if (targetT_ == nullptr)
        {
            targetEntity = new Entity("Target");
            targetEntity->Read("Target");
            targetT_ = targetEntity->Has(Transform);

            GetParent()->GetContainer()->Add(targetEntity);
        }
        else
        {
            targetEntity = targetT_->GetParent();
        }

        Transform* enemyT = partyTarget_->GetParent()->Has(Transform);
        if (enemyT)
        {
            targetT_->SetParentTransform(enemyT);

            // Counter-scale the parent's scale so the target appears at world-space size
            Vec2 parentScale = enemyT->GetScale();
            if (parentScale.x == 0.0f) parentScale.x = 1.0f;
            if (parentScale.y == 0.0f) parentScale.y = 1.0f;

            // Replace these with whatever world-space size you want the target to be
            constexpr float TargetDesiredWidth = 160.0f;
            constexpr float TargetDesiredHeight = 100.0f;

            targetT_->SetScale({
                TargetDesiredWidth / parentScale.x,
                TargetDesiredHeight / parentScale.y
                });

            // Counter the parent translation's inherited offset by setting translation to {0,0}
            // (or whatever local offset you want relative to the enemy in world units)
            targetT_->SetTranslation({ 0.0f, -0.45f});
        }

        if (parentParty_->GetActive())
            partyTarget_->SelectedState();
    }

	void BehaviorHand::SetTargetVisible(bool input)
	{
		int index = input ? 3 : -1;
		if (targetT_)
		{
			Sprite* targetSprite = targetT_->GetParent()->Has(Sprite);
			if (targetSprite)
			{
				targetSprite->SetZLayer(index);
			}
		}
	}

	BehaviorCard* BehaviorHand::GetActiveCard() const
	{
		return selected_;
	}

	bool BehaviorHand::HasPlayableCard() const
	{
		for (auto& card : cards_)
		{
			assert(card != nullptr);
			if (card->IsPlayable())
			{
				return true;
			}
		}
		return false;
	}

	void BehaviorHand::LockHand()
	{
		for (auto& card : cards_)
		{
			assert(card != nullptr);
			if (card->GetStateCurrent() != BehaviorCard::CardStates::cCardSelect)
			{
				card->SetStateNext(BehaviorCard::CardStates::cCardLocked);
			}
		}
	}

	void BehaviorHand::UnlockHand()
	{
		for (auto& card : cards_)
		{
			assert(card != nullptr);
			if (card->GetStateCurrent() == BehaviorCard::CardStates::cCardLocked)
			{
				card->SetStateNext(BehaviorCard::CardStates::cCardIdle);
			}
		}
	}

	BehaviorCard* BehaviorHand::GetCard(int index)
	{
		int cardIndex = 0;
		CardRef card = nullptr;
		if (!HasPlayableCard())
		{
			return nullptr;
		}
		while (index >= 0)
		{
			CardRef& cardCheck = cards_[(size_t)cardIndex];
			assert(cardCheck != nullptr);
			if (cardCheck->IsPlayable())
			{
				card = cardCheck;
				--index;
			}
			cardIndex = (1 + cardIndex) % (int)cards_.size();
		}
		return card;
	}

	BehaviorParty* BehaviorHand::GetParentParty()
	{
		return parentParty_;
	}

	void BehaviorHand::Energy_Text()
	{
		// ---------- ENERGY ICON SPRITE ----------
		Entity* iconEnt = GetOrMakeChildEntity(ChildEnergyIcon);

		if (parentParty_->GetEnergyChanged())
		{
			if (Sprite* iconS = iconEnt->Has(Sprite))
			{
				if (parentParty_->GetCurrentEnergy() == parentParty_->GetMaxEnergy())
				{
					iconS->SetSpriteSource("EnergyIconFull");
				}
				else if (parentParty_->currentEnergy_ > 0)
				{
					iconS->SetSpriteSource("EnergyIconHalf");
				}
				else
				{
					iconS->SetSpriteSource("EnergyIconEmpty");
				}
			}
			parentParty_->SetEnergyChanged(false);
		}

		// ---------- ENERGY TEXT ----------
		Entity* textEnt = GetOrMakeChildEntity(ChildEnergyText);

		// IMPORTANT: text lifetime must persist after this function returns
		string_t energyText;
		energyText = std::to_string(parentParty_->GetCurrentEnergy()) + "/" + std::to_string(parentParty_->GetMaxEnergy());

		if (SpriteText* textS = textEnt->Has(SpriteText))
		{
			textS->SetZLayer(5);
			textS->SetTint({ 0,0,0 });
			textS->SetText(energyText);
		}
	}

	void BehaviorHand::Deck_Icon()
	{
		GetOrMakeChildEntity(ChildDeckIcon);
	}

	void BehaviorHand::Discard_Text()
	{
		Entity* textEnt = GetOrMakeChildEntity(ChildDiscardText);
		string_t discardText;
		discardText = " TOTAL\n\nCARDS:" + std::to_string(cards_.size());

		if (SpriteText* textS = textEnt->Has(SpriteText))
		{
			textS->SetText(discardText);
		}
	}

#pragma endregion Private Functions

}	// namespace

