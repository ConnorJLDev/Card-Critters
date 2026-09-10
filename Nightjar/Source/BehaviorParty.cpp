//------------------------------------------------------------------------------
//
// File Name:	BehaviorParty.cpp
// Author(s):	Barend Brink (barend.brink)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Behavior class for party
//
// Copyright � 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "InputSystem.h"
#include "Entity.h"
#include "Collider.h"
#include "BehaviorParty.h"
#include "Transform.h"
#include "EntityContainer.h"
#include "Scene.h"
#include "SceneSystem.h"
#include "BehaviorCharacter.h"
#include "Sprite.h"
#include "SpriteText.h"
#include "BehaviorHand.h"
#include "Random.h"

#include "TurnSystem.h"
#include "SaveSystem.h"
#include "BehaviorTransition.h"
#include "Animation.h"

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

	constexpr StreamKey
		  BehaviorParty::ReadKeyCharacters = "Characters"
		, BehaviorParty::ReadKeyNextScene = "Next Scene"
		, BehaviorParty::ReadKeyIsPlayer = "IsPlayer"
		, BehaviorParty::ReadKeyPreset = "Preset"
		;

	constexpr StreamKey
		  BehaviorParty::ChildHand = "Hand"
		;

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

	BehaviorParty::BehaviorParty()
		: Behavior(cBehaviorParty)
		, characters_()
		, characterNames_()
		, partyStartPos_()
		, partyOffsetPos_(-175.0f)
		, tweenSpeed_(10.0f)
		, tweenSpeedScaled_()
		, tweenSnapDistance_(0.5f)
		, maxEnergy_(5)
		, currentEnergy_(5)
		, energyChanged_()
		, CID_()
		, hand_(nullptr)
		, playerControlled_(false)
		, aiTimer_(1.5f)
		, aiTimerCurrent_(1.5f)
		, nextScene_()
	{
		CID_ = MessageBus::Subscribe([this](Message msg) { Receive(msg); });
	}

	BehaviorParty::BehaviorParty(const BehaviorParty& other)
		: Behavior(other)
		, characters_()
		, characterNames_(other.characterNames_)
		, partyStartPos_(other.partyStartPos_)
		, partyOffsetPos_(other.partyOffsetPos_)
		, tweenSpeed_(other.tweenSpeed_)
		, tweenSpeedScaled_()
		, tweenSnapDistance_(other.tweenSnapDistance_)
		, maxEnergy_(other.maxEnergy_)
		, currentEnergy_(other.currentEnergy_)
		, energyChanged_()
		, CID_()
		, hand_()
		, playerControlled_(other.playerControlled_)
		, aiTimer_(other.aiTimer_)
		, aiTimerCurrent_(other.aiTimerCurrent_)
		, nextScene_(other.nextScene_)
	{
		CID_ = MessageBus::Subscribe([this](Message msg) { Receive(msg); });
	}

	//--------------------------------------------------------------------------

	BehaviorParty::~BehaviorParty()
	{
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

	void BehaviorParty::Receive(Message msg)
	{
		switch (msg)
		{
		case Message::IsPlayerTurn:
			break;
		case Message::IsEnemyTurn:
			break;
		}
	}

	int BehaviorParty::GetCurrentEnergy() const
	{
		return currentEnergy_;
	}

	int BehaviorParty::GetMaxEnergy() const
	{
		return maxEnergy_;
	}

	void BehaviorParty::SetCurrentEnergy(int inp)
	{
		currentEnergy_ = inp;
		energyChanged_ = true;
	}

	void BehaviorParty::SetHand(BehaviorHand* inp)
	{
		hand_ = inp;
	}

	BehaviorHand* BehaviorParty::GetHand() const
	{
		return hand_;
	}

	BehaviorParty* BehaviorParty::Clone() const
	{
		return new BehaviorParty(*this);
	}

	void BehaviorParty::Read(StreamIn& stream)
	{
		stream.TryReadValue(ReadKeyIsPlayer, playerControlled_);

		string_t presetName;
		if (stream.TryReadValue(ReadKeyPreset, presetName))
		{
			const DataParty* preset = SaveSystem::LoadPreset(presetName);
			assert(preset != nullptr && "Preset wasn't found!");
			preset->ApplyToParty(this);
			if (playerControlled_)
			{
				DataParty* saveParty = SaveSystem::GetCurrentParty();
				assert(saveParty != nullptr);
				*saveParty = *preset;
				SaveSystem::SaveCurrentParty();
			}
		}

		if (playerControlled_)
		{
			auto saveParty = SaveSystem::GetCurrentParty();
			assert(saveParty != nullptr);
			if (saveParty->HasParty()) //CHECK TO SEE IF HAVE PARTY STORED SOMEWHERE SAFE
			{
				characterNames_ = saveParty->PartyNames();
			}
			else
			{
				if (stream.StreamHas(ReadKeyCharacters))
				{
					characterNames_.clear();
					stream.ReadVector(ReadKeyCharacters, characterNames_);
					for (const auto& name : characterNames_)
					{
						saveParty->AddMember(name);
					}
					
					SaveSystem::SaveCurrentParty();
				}
			}
		}
		else
		{
			if (stream.StreamHas(ReadKeyCharacters))
			{
				characterNames_.clear();
				stream.ReadVector(ReadKeyCharacters, characterNames_);
			}
		}

		stream.TryReadValue(ReadKeyNextScene, nextScene_);
	}

	void BehaviorParty::Write(StreamOut& stream)
	{
		stream.WriteValue(ReadKeyIsPlayer, playerControlled_);
		stream.WriteValue(ReadKeyNextScene, nextScene_);

		if (!playerControlled_)
		{
			stream.WriteArrayStart(ReadKeyCharacters);
			for (BehaviorCharacter* e : characters_)
			{
				assert(e != nullptr);
				stream.WriteValue(e->GetParent()->GetName());
			}
			stream.WriteArrayEnd();
		}
	}

	const vector_t<BehaviorCharacter*>& BehaviorParty::GetParty() const
	{
		return characters_;
	}

	void BehaviorParty::AddToParty(BehaviorCharacter* character)
	{
		assert(character != nullptr);

		// If already in activeParty, do nothing
		assert(std::find(characters_.begin(), characters_.end(), character) == characters_.end());

		characters_.push_back(character);
		character->party_ = this;

		Entity* characterEnt = character->GetParent();
		Transform* charTfm = characterEnt->Has(Transform);
		assert(charTfm != nullptr);

		Entity* partyEnt = GetParent();
		Transform* partyTfm = partyEnt->Has(Transform);
		assert(partyTfm != nullptr);

		charTfm->SetParentTransform(partyTfm);
	}

	bool BehaviorParty::HasPlayableCards() const
	{
		return currentEnergy_ > 0 && hand_ != nullptr && hand_->HasPlayableCard();
	}

	void BehaviorParty::UpdateCharacterPositions() const
	{
		// Interpolation & set transforms
		int i = 0;
		for (auto& charBehavior : characters_)
		{
			charBehavior->tweenTarget_ = Vec2(partyStartPos_, 200) + static_cast<float>(i++) * Vec2(partyOffsetPos_, 0);
		}
	}

	void BehaviorParty::RemoveFromParty(BehaviorCharacter* character)
	{
		assert(character != nullptr);

		auto charIterator = std::find(characters_.begin(), characters_.end(), character);			

		if (charIterator != characters_.end())
		{
			characters_.erase(charIterator);
		}
	}

	bool BehaviorParty::MoveAlly(BehaviorCharacter* character, BehaviorCharacter* target)
	{
		auto currAlly = std::find(characters_.begin(), characters_.end(), character);
		auto targetAlly = std::find(characters_.begin(), characters_.end(), target);

		if (targetAlly == characters_.end())
		{
			return false;
		}

		std::swap(*currAlly, *targetAlly);

		return true;
	}

	bool BehaviorParty::GetEnergyChanged()
	{
		return energyChanged_;
	}

	void BehaviorParty::SetEnergyChanged(bool changed)
	{
		energyChanged_ = changed;
	}

	void BehaviorParty::SetCharacterMoveSpeed(float speed)
	{
		tweenSpeed_ = speed;
	}

	float BehaviorParty::GetCharacterMoveSpeedScaled() const
	{
		return tweenSpeedScaled_;
	}

	void BehaviorParty::SetCharacterMoveSnap(float snap)
	{
		tweenSnapDistance_ = snap;
	}

	float BehaviorParty::GetCharacterMoveSnap() const
	{
		return tweenSnapDistance_;
	}

	bool BehaviorParty::GetActive()
	{
		return TurnSystem::IsPartyActive(this);
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

	void BehaviorParty::OnInit()
	{
		switch (GetStateCurrent())
		{
		case cPartyIdle:
		{
			if (playerControlled_ || !nextTurnPrepared_)
			{
				SetCurrentEnergy(maxEnergy_);
				if (hand_ != nullptr)
				{
					hand_->FillHand();
				}
			}
			else
			{
				TraceInfo("Using preprepared enemy turn state.");
			}

			if (playerControlled_)
				MessageBus::Broadcast(Message::IsPlayerTurn);
			else
				MessageBus::Broadcast(Message::IsEnemyTurn);

			for (auto& character : characters_)
			{
				if (character)
					character->OnTurnStart();
			}

			Transform* form = hand_->GetParent()->Has(Transform);
			Vec2 handPos = form->GetTranslation();
			handPos.y = playerControlled_ ? 0 : -100000.0f;
			form->SetTranslation(handPos);
		}
		break;
		case cPartyRecruit:
		{
			Transform* form = hand_->GetParent()->Has(Transform);
			Vec2 handPos = form->GetTranslation();
			handPos.y = -100000.0;
			form->SetTranslation(handPos);
		}
		break;
		}
	}

	void BehaviorParty::OnUpdate(float dt)
	{
		tweenSpeedScaled_ = std::clamp(tweenSpeed_ * dt, 0.0f, 1.0f);

		UpdateCharacterPositions();


		if (playerControlled_ && GetActive())
		{
			for (BehaviorParty* partyCheck : TurnSystem::Parties())
			{
				if (partyCheck != nullptr && partyCheck != this &&
					partyCheck->plannedActions_.empty() &&
					!partyCheck->nextTurnPrepared_)
				{
					// Build the actual next-turn state NOW
					partyCheck->SetCurrentEnergy(partyCheck->GetMaxEnergy());

					if (partyCheck->GetHand() != nullptr)
					{
						partyCheck->GetHand()->FillHand();
					}

					partyCheck->PlanTurnActions(this);
					partyCheck->nextTurnPrepared_ = true;
					partyCheck->DebugPrintPlannedAction("Prepared On Player Turn");
				}
			}
		}
        if (playerControlled_)
        {
            UpdateNextTargetIndicators();
        }

		Turn_Text();
		
		if (GetTotalHealth() <= 0)
		{
			// If the player party died, capture/save current player state as before.
			auto saveParty = SaveSystem::GetCurrentParty();
			assert(saveParty != nullptr);
			if (playerControlled_)
			{
				saveParty->CaptureFromParty(this);
				SaveSystem::SaveCurrentParty();
			}
			else
			{
				BehaviorParty* playerParty = nullptr;
				for (const auto& p : TurnSystem::Parties())
				{
					if (p->playerControlled_)
					{
						playerParty = p;
						break;
					}
				}

				if (playerParty)
				{
					// Heal everyone in the player's active party to full
					for (BehaviorCharacter* bc : playerParty->GetParty())
					{
						if (!bc) continue;
						bc->SetHealth(bc->GetMaxHealth());
					}

					// Persist healed state
					saveParty->CaptureFromParty(playerParty);
					SaveSystem::SaveCurrentParty();
				}

				// Recruit allowed characters
				static const vector_t<string_t> recruitable = { "Milo", "Katrina" };
				vector_t<string_t> playerNames = saveParty->PartyNames();

				for (const string_t& candidate : characterNames_)
				{
					if (std::find(recruitable.begin(), recruitable.end(), candidate) == recruitable.end())
						continue;
					if (std::find(playerNames.begin(), playerNames.end(), candidate) != playerNames.end())
						continue;

					if (playerParty)
						playerParty->RecruitMember(candidate);
					break;
				}
			}
			//if all animations are done
			//if (!AnyAnimationsPlaying() && GetStateCurrent() != cPartyTransition)

			if (GetStateCurrent() != cPartyTransition)
			{
				SetStateNext(cPartyTransition);
                TurnSystem::TransitionScene(nextScene_);
			}
		}

		switch (GetStateCurrent())
		{
		case cPartyIdle:
		{
			aiTimerCurrent_ -= dt;
			if (aiTimerCurrent_ <= 0.0f)
			{
				aiTimerCurrent_ = aiTimer_;
			}
			else
			{
				return;
			}
			if (!HasPlayableCards())
			{
				if (!playerControlled_)
				{
					nextTurnPrepared_ = false;
					ClearPlannedAction();
				}
				TurnSystem::NextTurn();
			}
			else if (!playerControlled_)
			{
				AIPlay();
			}
		}
		break;
		case cPartyWaiting:
			break;
		case cPartyNext:
			break;
		case cPartyDone:
			break;
		case cPartyTransition:
			break;
		}
	}

	void BehaviorParty::OnExit()
	{
		switch (GetStateCurrent())
		{
		case cPartyInvalid:
		{
			{
				auto party = SaveSystem::GetCurrentParty();
				assert(party != nullptr);
/*				if (playerControlled_)
				{
					if (party->HasParty()) //CHECK TO SEE IF HAVE PARTY STORED SOMEWHERE SAFE
					{
						characterNames_ = party->PartyNames();
					}
				}*/
				characters_.clear();
				unsigned char charIndex = 0;
				// Characters
				for (const string_t& name : characterNames_)
				{
					TraceInfo("Spawning party member entity from file: Data/Entities/{}.json", name);

					Entity* entity = new Entity(name);
					entity->Read(name);

					BehaviorCharacter* charBehavior = entity->HasSub(Behavior, BehaviorCharacter);
					if (charBehavior == nullptr)
					{
						TraceError("Spawned entity \"{}\" but it has no BehaviorCharacter!", name);
						continue;
					}

					AddToParty(charBehavior);

					if (playerControlled_)
					{
						party->ApplyToCharacter(charBehavior);
					}

					if (charBehavior->GetCards().empty())
					{
						charBehavior->LoadCards();
					}

					// Initialize character locations.
					Transform* charTransform = entity->Has(Transform);
					assert(charTransform != nullptr);

					charTransform->SetTranslation(Vec2(partyStartPos_ + static_cast<float>(charIndex++) * partyOffsetPos_, 200));
				}

			}
			{
				// Hand
				Entity* handEnt = GetOrMakeChildEntity(ChildHand);
				Transform* handTfm = handEnt->Has(Transform);
				assert(handTfm != nullptr);
				Vec2 handPos = handTfm->GetTranslation();
				if (!playerControlled_)
				{
					handPos.y = -100000.0;
				}
				handTfm->SetTranslation(handPos);
				hand_ = handEnt->HasSub(Behavior, BehaviorHand);
				assert(hand_ != nullptr);
				hand_->parentParty_ = this;
				hand_->InitializeDeck();
				// Initialize card locations.
				int i[(size_t)BehaviorCard::CardGroup::cGroupCount_] = { 0 };
				for (auto& card : hand_->cards_)
				{
					size_t group = (size_t)card->group_;
					int& groupIndex = i[group];
					Transform* cardTfm = card->EntityHas(Transform);
					assert(cardTfm != nullptr);
					cardTfm->SetTranslation(hand_->origins_[group] + (float)(groupIndex++) * hand_->spacings_[group]);
				}
			}
			EntityContainer* sceneCont = GetParent()->GetContainer();
			for (size_t i = 0; i < characters_.size(); ++i)
			{
				sceneCont->Add(characters_[i]->GetParent());
			}
			TurnSystem::RegisterParty(this);
			SetActive(GetActive());
			break;
		}
		case cPartyIdle:
			break;
		case cPartyWaiting:
			break;
		case cPartyNext:
			break;
		case cPartyDone:
			break;
		}
	}

	void BehaviorParty::AIPlay()
	{
		BehaviorParty* targetParty = nullptr;
		for (const auto& partyCheck : TurnSystem::Parties())
		{
			if (partyCheck != this)
			{
				targetParty = partyCheck;
				break;
			}
		}

		if (targetParty == nullptr)
		{
			TurnSystem::NextTurn();
			return;
		}

		// Replan if needed
		if (plannedActions_.empty())
		{
			PlanTurnActions(targetParty);
		}

		if (plannedActionIndex_ >= (int)plannedActions_.size())
		{
			TraceInfo("Finished all planned enemy actions.");
			ClearPlannedAction();
			nextTurnPrepared_ = false;
			TurnSystem::NextTurn();
			return;
		}

		PlannedAction& action = plannedActions_[plannedActionIndex_];

		if (!action.valid)
		{
			++plannedActionIndex_;

			if (plannedActionIndex_ >= (int)plannedActions_.size())
			{
				TraceInfo("Finished all planned enemy actions.");
				ClearPlannedAction();
				nextTurnPrepared_ = false;
				TurnSystem::NextTurn();
			}
			return;
		}

		BehaviorCard* card = action.plannedCard;
		if (card == nullptr)
		{
			TraceInfo("Planned card pointer is null, skipping.");
			++plannedActionIndex_;

			if (plannedActionIndex_ >= (int)plannedActions_.size())
			{
				ClearPlannedAction();
				nextTurnPrepared_ = false;
				TurnSystem::NextTurn();
			}
			return;
		}

		if (card->group_ != BehaviorCard::CardGroup::cHand)
		{
			TraceInfo("Planned card [{}] is no longer in hand, skipping.",
				card->GetParent()->GetName());
			++plannedActionIndex_;

			if (plannedActionIndex_ >= (int)plannedActions_.size())
			{
				ClearPlannedAction();
				nextTurnPrepared_ = false;
				TurnSystem::NextTurn();
			}
			return;
		}

		BehaviorCharacter* cardOwner = card->data_->GetOwner();
		if (cardOwner == nullptr || cardOwner->GetHealth() <= 0)
		{
			TraceInfo("Planned card owner is dead, skipping.");
			++plannedActionIndex_;

			if (plannedActionIndex_ >= (int)plannedActions_.size())
			{
				ClearPlannedAction();
				nextTurnPrepared_ = false;
				TurnSystem::NextTurn();
			}
			return;
		}

		// If the owner is currently disarmed, and the planned card is a damage-type, skip this planned action
		{
			DataCard::CardType plannedType = card->CardGetCType();
			bool plannedIsDamage = ((plannedType == DataCard::CardType::Damage) || (plannedType == DataCard::CardType::DamageAOE) || (plannedType == DataCard::CardType::SplashDamage));
			if (plannedIsDamage && cardOwner->HasStatus(DataCard::CardStatusEffect::Disarm))
			{
				TraceInfo("Skipping planned [{}] because owner '{}' is disarmed.", card->GetParent()->GetName(), cardOwner->GetParent()->GetName());
				++plannedActionIndex_;
				if (plannedActionIndex_ >= (int)plannedActions_.size())
				{
					TraceInfo("Finished all planned enemy actions.");
					ClearPlannedAction();
					nextTurnPrepared_ = false;
					TurnSystem::NextTurn();
				}
				return;
			}
		}

		BehaviorParty* actionTargetParty = action.targetsAllies ? this : targetParty;
		BehaviorCharacter* target = actionTargetParty->GetCharacterAtSlot(action.targetSlot);

		if (target == nullptr || target->GetHealth() <= 0)
		{
			// fallback: pick first living valid target from the intended side
			target = nullptr;
			for (BehaviorCharacter* candidate : actionTargetParty->GetParty())
			{
				if (candidate != nullptr && candidate->GetHealth() > 0)
				{
					target = candidate;
					break;
				}
			}

			if (target == nullptr)
			{
				TraceInfo("No living targets available for planned action {}, skipping.",
					plannedActionIndex_);
				++plannedActionIndex_;

				if (plannedActionIndex_ >= (int)plannedActions_.size())
				{
					ClearPlannedAction();
					nextTurnPrepared_ = false;
					TurnSystem::NextTurn();
				}
				return;
			}
		}

		hand_->SetPartyTarget(target);

		TraceInfo("{} executes planned [{}] on {} slot {} and targets {}",
			action.attacker ? action.attacker->GetParent()->GetName() : "Enemy",
			card->GetParent()->GetName(),
			action.targetsAllies ? "ally" : "enemy",
			action.targetSlot,
			target->GetParent()->GetName());

		{
			DataCard::CardType execType = card->CardGetCType();
			bool execIsDamage = ((execType == DataCard::CardType::Damage) || (execType == DataCard::CardType::DamageAOE) || (execType == DataCard::CardType::SplashDamage));
			if (execIsDamage && cardOwner->HasStatus(DataCard::CardStatusEffect::Disarm))
			{
				TraceInfo("Execution blocked: owner '{}' became disarmed, cannot execute damage card '{}'.",
					cardOwner->GetParent()->GetName(),
					card->GetParent()->GetName());
				// skip this action
				++plannedActionIndex_;
				if (plannedActionIndex_ >= (int)plannedActions_.size())
				{
					TraceInfo("Finished all planned enemy actions.");
					ClearPlannedAction();
					nextTurnPrepared_ = false;
					TurnSystem::NextTurn();
				}
				return;
			}
		}

		card->Use();
		++plannedActionIndex_;

		if (plannedActionIndex_ >= (int)plannedActions_.size())
		{
			TraceInfo("Finished all planned enemy actions.");
			ClearPlannedAction();
			nextTurnPrepared_ = false;
			TurnSystem::NextTurn();
		}
	}


	void BehaviorParty::SetActive(bool isActive)
	{
		SetStateNext(isActive ? cPartyIdle : cPartyWaiting);
	}


    void BehaviorParty::ClearNextTargetIndicators()
    {
        for (BehaviorCharacter* character : characters_)
        {
            if (character != nullptr)
            {
                character->ShowNextTargetArrow(false);
            }
        }
    }

    void BehaviorParty::UpdateNextTargetIndicators()
    {
        if (!playerControlled_)
            return;

        ClearNextTargetIndicators();

        if (!GetActive())
            return;

        vector_t<bool> targeted(characters_.size(), false);

        for (BehaviorParty* enemyParty : TurnSystem::Parties())
        {
            if (enemyParty == nullptr || enemyParty == this)
                continue;

            for (const PlannedAction& action : enemyParty->plannedActions_)
            {
                if (!action.valid)
                    continue;

                // skip heal / move / defense / ally-target actions
                if (action.targetsAllies)
                    continue;

                if (action.targetSlot < 0 || action.targetSlot >= (int)characters_.size())
                    continue;

                BehaviorCharacter* target = GetCharacterAtSlot(action.targetSlot);
                if (target == nullptr || target->GetHealth() <= 0)
                    continue;

                targeted[(size_t)action.targetSlot] = true;
            }
        }

        for (size_t i = 0; i < targeted.size(); ++i)
        {
            if (!targeted[i])
                continue;

            BehaviorCharacter* target = characters_[i];
            if (target != nullptr && target->GetHealth() > 0)
            {
                target->ShowNextTargetArrow(true);
            }
        }
    }
  

	void BehaviorParty::Turn_Text()
	{
		Transform* transform = GetParent()->Has(Transform);
		Transform* childText = transform ? transform->FindChild("TurnText") : nullptr;
		Entity* childTextEntity = nullptr;

		if (childText == nullptr)
		{
			childTextEntity = new Entity("TurnText");
			childTextEntity->Read("TurnText");
			childText = childTextEntity->Has(Transform);

			GetParent()->GetContainer()->Add(childTextEntity);
			childText->SetParentTransform(transform);
		}
		else
		{
			childTextEntity = childText->GetParent();
		}

		// Update the Turn text every frame.
		// Use the SpriteText component (your TurnText.json uses "SpriteText", not "Sprite").
        FORMAT_MAKE(text, "Turn: {}", TurnSystem::RoundCount() + 1); // +1 if you want turns displayed starting at 1

		if (SpriteText* childTextSprite = childTextEntity->Has(SpriteText))
		{
			childTextSprite->SetText(text);
		}
	}

	int BehaviorParty::GetTotalHealth()
	{
		int hp = 0;
		for (auto d : this->GetParty())
		{
			hp += d->GetHealth();
		}
		return hp;
	}

	bool BehaviorParty::AnyAnimationsPlaying()
	{
		for (auto d : this->GetParty())
		{
			Animation* characterAnimation = d->GetChildAnimation();
			if (characterAnimation)
			{
				if (characterAnimation->GetLoop() != AnimationLoop::Stop && !characterAnimation->IsDone())
					return true;
			}
		}
		return false;
	}

	void BehaviorParty::RecruitMember(const string_t& name)
	{
		if (!playerControlled_)
			return; // only recruit into player's party

		// avoid duplicates in the active names list
		if (std::find(characterNames_.begin(), characterNames_.end(), name) == characterNames_.end())
		{
			characterNames_.push_back(name);
		}

		// Add to the persistent save state so future scenes will spawn them
		auto saveParty = SaveSystem::GetCurrentParty();
		assert(saveParty != nullptr);
		saveParty->AddMember(name);
		SaveSystem::SaveCurrentParty();
		TraceInfo("BehaviorParty: Recruited '{}' and saved party", name);
	}
    
    void BehaviorParty::ClearPlannedAction()
    {
        plannedActions_.clear();
        plannedActionIndex_ = 0;

    }

	void BehaviorParty::PlanTurnActions(BehaviorParty* targetParty)
	{
		ClearPlannedAction();

		if (targetParty == nullptr || hand_ == nullptr)
			return;

		if (characters_.empty() || targetParty->GetParty().empty())
			return;

		vector_t<BehaviorCharacter*> livingAttackers;
		for (BehaviorCharacter* character : characters_)
		{
			if (character != nullptr && character->GetHealth() > 0)
				livingAttackers.push_back(character);
		}

		if (livingAttackers.empty())
			return;

		int simulatedEnergy = currentEnergy_;
		int cardCount = hand_->GetCardCount();

		vector_t<BehaviorCard*> sortedCards;
		sortedCards.reserve((size_t)cardCount);

		for (int i = 0; i < cardCount; ++i)
		{
			BehaviorCard* card = hand_->GetCardAt(i);
			if (card == nullptr || card->group_ != BehaviorCard::CardGroup::cHand)
				continue;

			sortedCards.push_back(card);
		}
		
		std::sort(sortedCards.begin(), sortedCards.end(),[&](BehaviorCard* a, BehaviorCard* b) //sorts based on priority
		{
		return GetCardPriority(a) > GetCardPriority(b);
		});

		for (BehaviorCard* card : sortedCards) //simulate selecting a card
		{
			if (simulatedEnergy <= 0) //if energy is 0 don't select an action
				break;

			if (card == nullptr || card->group_ != BehaviorCard::CardGroup::cHand) //check to see if card is selected then make an action
				continue;

			BehaviorCharacter* cardOwner = card->data_->GetOwner(); //get owner of card to find the character (helps with print value and later target showing)

			int cost = card->CardGetCost(); //gets cost, if energy is more, we can use the card.
			if (cost > simulatedEnergy)
				continue;

			DataCard::CardType type = card->CardGetCType();

			bool isHeal = (type == DataCard::CardType::Heal);
			bool isMove = (type == DataCard::CardType::Move);
			bool isDefense = (type == DataCard::CardType::Defense); //not build dunno if we will use
			bool isDamage = ((type == DataCard::CardType::Damage) || (type == DataCard::CardType::DamageAOE));//check to see dmg

			// If cardOwner is disarmed, do not plan damage cards
			if (isDamage && cardOwner != nullptr && cardOwner->HasStatus(DataCard::CardStatusEffect::Disarm))
			{
				TraceInfo("Not planning damage card [{}] for owner '{}' because they are disarmed.", card->GetParent()->GetName(), cardOwner->GetParent()->GetName());
				continue;
			}

			bool targetsAllies = isHeal || isMove || isDefense; //if we should target allies
			BehaviorParty* actionTargetParty = targetsAllies ? this : targetParty; 
			const auto& actionTargets = actionTargetParty->GetParty();

			vector_t<int> validSlots; 

			if (isHeal)
			{
				// Heals only living wounded allies (doesn't effect dead)
				float lowestRatio = 2.0f;

				for (unsigned int s = 0; s < static_cast<unsigned int>(actionTargets.size()); ++s)
				{
					BehaviorCharacter* c = actionTargets[s];
					if (c == nullptr || c->GetHealth() <= 0)
						continue;

					if (c->GetHealth() >= c->GetMaxHealth())
						continue; // don't heal full-health allies, no point

					float ratio = 1.0f;
					if (c->GetMaxHealth() > 0)
						ratio = static_cast<float>(c->GetHealth()) / static_cast<float>(c->GetMaxHealth());

					if (ratio < lowestRatio)
					{
						lowestRatio = ratio;
						validSlots.clear();
						validSlots.push_back((int)s);
					}
					else if (ratio == lowestRatio)
					{
						validSlots.push_back((int)s);
					}
				}
			}
			else if (isMove)
			{
				// Move only living allies... honestly though enemies shouldn't have a move? imo. idk not good strategy
				float lowestRatio = 2.0f;

				for (unsigned int s = 0; s < static_cast<unsigned int>(actionTargets.size()); ++s)
				{
					BehaviorCharacter* c = actionTargets[s];
					if (c == nullptr || c->GetHealth() <= 0)
						continue;

					float ratio = 1.0f;
					if (c->GetMaxHealth() > 0)
						ratio = static_cast<float>(c->GetHealth()) / static_cast<float>(c->GetMaxHealth());

					if (ratio < lowestRatio)
					{
						lowestRatio = ratio;
						validSlots.clear();
						validSlots.push_back((int)s);
					}
					else if (ratio == lowestRatio)
					{
						validSlots.push_back((int)s);
					}
				}
			}
			else if (isDefense)
			{
				// Defense only living allies, prefer hurt ones not in the game yet uses same code as heal tbh.
				float lowestRatio = 2.0f;

				for (unsigned int s = 0; s < static_cast<unsigned int>(actionTargets.size()); ++s)
				{
					BehaviorCharacter* c = actionTargets[s];
					if (c == nullptr || c->GetHealth() <= 0)
						continue;

					float ratio = 1.0f;
					if (c->GetMaxHealth() > 0)
						ratio = static_cast<float>(c->GetHealth()) / static_cast<float>(c->GetMaxHealth());

					if (ratio < lowestRatio)
					{
						lowestRatio = ratio;
						validSlots.clear();
						validSlots.push_back((int)s);
					}
					else if (ratio == lowestRatio)
					{
						validSlots.push_back((int)s);
					}
				}
			}
			else if (isDamage)
			{
				// Damage only living enemies (NOT IMPLIMENTED BUT SHOULD PREFER TARGET ON LOW HEALTH ALLY)
				for (unsigned int s = 0; s < static_cast<unsigned int>(actionTargets.size()); ++s)
				{
					BehaviorCharacter* c = actionTargets[s];
					if (c != nullptr && c->GetHealth() > 0)
						validSlots.push_back((int)s);
				}
			}
			else
			{
				continue; //card type wasn't found... did we mess up???
			}

			if (validSlots.empty())
				continue;

			int rolledSlot = validSlots[(size_t)Random::Range(0, (int)validSlots.size() - 1)];

			// NEVER plan onto dead targets
			BehaviorCharacter* chosenTarget = actionTargets[(size_t)rolledSlot];
			if (chosenTarget == nullptr || chosenTarget->GetHealth() <= 0)
				continue;

			PlannedAction action;
			action.attacker = cardOwner; //checks to see who owned the card using
			action.plannedCard = card; //checks the card info
			action.targetSlot = rolledSlot; //what slot are we attacking NOT CHARACTER.
			action.energyCost = cost; //Cost of card
			action.targetsAllies = targetsAllies; //is this an ally target ? helpful for inevideble "TARGET" system.
			action.valid = true; //is this move valid? 

			plannedActions_.push_back(action); //Push onto action stack. 
			simulatedEnergy -= cost;

			//ADDED DEBUG TO SEE WHAT IS GOING WRONG?????? HELP?????
			TraceInfo("{} plans [{}] on {} slot {} (cost {}, simulated energy left {})", 
				cardOwner ? cardOwner->GetParent()->GetName() : "Enemy",
				card->GetParent()->GetName(),
				targetsAllies ? "ally" : "enemy",
				rolledSlot,
				cost,
				simulatedEnergy);
		}
	}

	BehaviorCharacter* BehaviorParty::GetCharacterAtSlot(int slot) const //returns character slot
	{
		if (slot < 0 || slot >= static_cast<int>(characters_.size()))
			return nullptr;

		return characters_[(size_t)slot];
	}



	void BehaviorParty::DebugPrintPlannedAction(const char* context) const //Debug To show my terrible code working/not working. AI helped generating this debug issue.
	{
		//ADDED DEBUG TO SEE WHAT IS GOING WRONG?????? HELP?????
		TraceInfo("[{}] Planned action count: {}", context, (int)plannedActions_.size()); 
		//ADDED DEBUG TO SEE WHAT IS GOING WRONG?????? HELP?????
		for (size_t i = 0; i < plannedActions_.size(); ++i)
		{
			const PlannedAction& action = plannedActions_[i];

			string_t cardName = "nullptr";
			string_t cardTypeStr = "unknown";

			if (action.plannedCard != nullptr)
			{
				cardName = action.plannedCard->GetParent()->GetName();

				DataCard::CardType type = action.plannedCard->CardGetCType();
				cardTypeStr.clear();

				if (type == DataCard::CardType::Damage)       cardTypeStr += "Damage ";
				if (type == DataCard::CardType::Heal)         cardTypeStr += "Heal ";
				if (type == DataCard::CardType::Move)         cardTypeStr += "Move ";
				if (type == DataCard::CardType::Defense)      cardTypeStr += "Defense ";
				if (type == DataCard::CardType::DamageAOE)    cardTypeStr += "DamageAOE ";
				if (type == DataCard::CardType::SplashDamage) cardTypeStr += "SplashDamage ";

				if (cardTypeStr.empty())
					cardTypeStr = "none";
			}
			//ADDED DEBUG TO SEE WHAT IS GOING WRONG?????? HELP?????
			TraceInfo("  [{}] attacker={}, card={}, slot={}, cost={}, targetsAllies={}, valid={}, type=[{}]",
				(int)i,
				action.attacker ? action.attacker->GetParent()->GetName() : "nullptr",
				cardName,
				action.targetSlot,
				action.energyCost,
				action.targetsAllies ? 1 : 0,
				action.valid ? 1 : 0,
				cardTypeStr);
		}
	}

	int BehaviorParty::GetCardPriority(BehaviorCard* card) const
	{
		if (card == nullptr)
			return -1;

		DataCard::CardType type = card->CardGetCType();

		if (type == DataCard::CardType::Damage)  return 3;
		if (type == DataCard::CardType::Heal)    return 2;
		if (type == DataCard::CardType::Defense) return 1;
		if (type == DataCard::CardType::Move)    return 0;

		return 0;
	}

#pragma endregion Private Functions

}	// namespace

