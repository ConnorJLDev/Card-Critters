//------------------------------------------------------------------------------
//
// File Name:	BehaviorCard.cpp
// Author(s):	Barend Brink (barend.brink)
// Project:		Card Critters
// Course:		GAM200
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Behavior.h"
#include "BehaviorCard.h"
#include "BehaviorCharacter.h"
#include "BehaviorHand.h"
#include "BehaviorParty.h"

#include "PlatformSystem.h"
#include "InputSystem.h"
#include "ImGuiSystem.h"
#include "SceneSystem.h"
#include "AudioSystem.h"

#include "Scene.h"
#include "EntityContainer.h"
#include "Entity.h"
#include "Transform.h"
#include "Collider.h"
#include "Sprite.h"
#include "SpriteText.h"
#include "Physics.h"
#include "Animation.h"
#include "DataCard.h"
#include "AudioEmitter.h"
#include "Random.h"

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
		BehaviorCard::NameMouse = "Mouse",
		BehaviorCard::ChildText = "CardText",
		BehaviorCard::ChildCostText = "CardCostText",
		BehaviorCard::ChildAccuracyText = "CardAccuracyText",
		BehaviorCard::ChildCritText = "CardCritText",
		BehaviorCard::CardAccuracyIcon = "CardAccuracyIcon",
		BehaviorCard::CardCritIcon = "CardCritIcon",
		BehaviorCard::ChildTooltip = "CardTooltip";

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

	BehaviorCard::BehaviorCard()
		: BehaviorInteractable(cBehaviorCard)
		, isHovered_(false)
		, isClicked_(false)
		, hasClicked_(false)
		, isLocked_(false)
		, hand_(nullptr)
		, group_(CardGroup::cInvalid)
		, tweenTarget_(0, 0)
		, data_(nullptr)
		, CID_()
		, tooltipEntity_(nullptr)
		, tooltipOffset_(-0.8, 0.95)
	{
		CID_ = MessageBus::Subscribe([this](Message msg){ Receive(msg);});
	}
	
	BehaviorCard::BehaviorCard(const BehaviorCard& other)
		: BehaviorInteractable(other)
		, isHovered_(other.isHovered_)
		, isClicked_(other.isClicked_)
		, hasClicked_(other.hasClicked_)
		, isLocked_(other.isLocked_)
		, hand_(other.hand_)
		, group_(other.group_)
		, tweenTarget_(other.tweenTarget_)
		, data_(other.data_)
		, CID_()
		, tooltipEntity_(nullptr)
		, tooltipOffset_(other.tooltipOffset_)
	{
		CID_ = MessageBus::Subscribe([this](Message msg) { Receive(msg); });
	}

	//--------------------------------------------------------------------------

	BehaviorCard::~BehaviorCard()
	{
		MessageBus::Unsubscribe(CID_);

		Transform* transform = GetParent()->Has(Transform);
		assert(transform != nullptr);
		Transform* child = transform->FindChild(ChildText);
		if (child != nullptr && child->GetParent() != nullptr)
		{
			child->GetParent()->Destroy();
		}
		Transform* costChild = transform->FindChild(ChildCostText);
		if (costChild != nullptr && costChild->GetParent() != nullptr)
		{
			costChild->GetParent()->Destroy();
		}
		Transform* titleChild = transform->FindChild(ChildAccuracyText);
		if (titleChild != nullptr && titleChild->GetParent() != nullptr)
		{
			titleChild->GetParent()->Destroy();
		}
		Transform* critChild = transform->FindChild(ChildCritText);
		if (critChild != nullptr && critChild->GetParent() != nullptr)
		{
			critChild->GetParent()->Destroy();
		}
		Transform* criticonChild = transform->FindChild(CardCritIcon);
		if (criticonChild != nullptr && criticonChild->GetParent() != nullptr)
		{
			criticonChild->GetParent()->Destroy();
		}
		Transform* acciconChild = transform->FindChild(CardAccuracyIcon);
		if (acciconChild != nullptr && acciconChild->GetParent() != nullptr)
		{
			acciconChild->GetParent()->Destroy();
		}

		// Tooltip cleanup
		Transform* tooltipChild = transform->FindChild(ChildTooltip);
		if (tooltipChild != nullptr && tooltipChild->GetParent() != nullptr)
		{
			tooltipChild->GetParent()->Destroy();
			tooltipEntity_ = nullptr;
		}
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

	BehaviorCard* BehaviorCard::Clone() const
	{
		return new BehaviorCard(*this);
	}

	void BehaviorCard::Read(StreamIn& stream)
	{
		UNREFERENCED_PARAMETER(stream);
	}

	void BehaviorCard::Write(StreamOut& stream)
	{
		UNREFERENCED_PARAMETER(stream);
	}

	void BehaviorCard::SetParentHand(BehaviorHand* hand)
	{
		hand_ = hand;
	}

	BehaviorHand* BehaviorCard::GetParentHand() const
	{
		return hand_;
	}

	bool BehaviorCard::GetIsHovered() const
	{
		return isHovered_;
	}

	void BehaviorCard::CardSetDamage(int dmg)
	{
		data_->damage = dmg;
	}

	void BehaviorCard::CardHeal(int cheal)
	{
		data_->heal = cheal;
	}

	void BehaviorCard::CardSetCost(int ccost)
	{
		data_->cost = ccost;
	}

	void BehaviorCard::CardSetMove(int cmove)
	{
		data_->move = cmove;
	}

	void BehaviorCard::CardSetDefense(int def)
	{
		data_->defense = def;
	}

	int BehaviorCard::CardGetDamage()
	{
		return data_->damage;
	}

	DataCard::CardType BehaviorCard::CardGetCType()
	{
		return data_->type;
	}

	int BehaviorCard::CardGetHeal()
	{
		return data_->heal;
	}

	int BehaviorCard::CardGetCost()
	{
		return data_->cost;
	}

	int BehaviorCard::CardGetMove()
	{
		return data_->move;
	}

	int BehaviorCard::CardGetDefense()
	{
		return data_->defense;
	}

	int BehaviorCard::GetIndex()
	{
		if (hand_ == nullptr)
			return -1;

		return hand_->GetCardIndex(this);
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

	void BehaviorCard::CreateTooltip()
	{
		if (!GetParent() || !data_)
			return;

		Transform* parentT = GetParent()->Has(Transform);
		if (!parentT) return;

		// Background
		Transform* bgT = parentT->FindChild(ChildTooltip);
		Entity* bgEnt = nullptr;

		if (bgT == nullptr)
		{
			bgEnt = new Entity(ChildTooltip);
			bgEnt->Read(ChildTooltip);
			bgT = bgEnt->Has(Transform);
			assert(bgT != nullptr);

			GetParent()->GetContainer()->Add(bgEnt);
			bgT->SetParentTransform(parentT);
			bgT->SetTranslation(tooltipOffset_);
		}
		else
		{
			bgEnt = bgT->GetParent();
			bgT->SetTranslation(tooltipOffset_);
		}

		tooltipEntity_ = bgEnt;
		assert(tooltipEntity_ != nullptr);

		// Tooltip Text
		constexpr const char* textName = "CardTooltipText";
		Transform* textT = parentT->FindChild(textName);
		Entity* textEnt = nullptr;

		if (textT == nullptr)
		{
			textEnt = new Entity(textName);
			textEnt->Read(textName);
			textT = textEnt->Has(Transform);
			assert(textT != nullptr);

			GetParent()->GetContainer()->Add(textEnt);

			// Parent text to the card transform
			textT->SetParentTransform(parentT);
			// Text offset
			textT->SetTranslation(tooltipOffset_ + Vec2(-0.6f, 0.3f));
		}
		else
		{
			// Parent text to the card transform
			textT->SetParentTransform(parentT);
			// Text offset
			textT->SetTranslation(tooltipOffset_ + Vec2(-0.6f, 0.3f));
			// Ensure textEnt is set when the transform already existed
			textEnt = textT->GetParent();
		}

		// Build tooltip text
		if (SpriteText* st = textEnt->Has(SpriteText))
		{
			string_t tip;
			vector_t<string_t> types;

			if ((bool)(data_->type & DataCard::CardType::Damage)) types.push_back("ATTACK");
			if ((bool)(data_->type & DataCard::CardType::DamageAOE)) types.push_back("AOE");
			if ((bool)(data_->type & DataCard::CardType::SplashDamage)) types.push_back("SPLASH");
			if ((bool)(data_->type & DataCard::CardType::Move)) types.push_back("MOVE");
			if ((bool)(data_->type & DataCard::CardType::Heal)) types.push_back("HEAL");
			if ((bool)(data_->type & DataCard::CardType::Defense)) types.push_back("DEFENSE");
			if ((bool)(data_->type & DataCard::CardType::Accuracy)) types.push_back("ACCURACY");
			if ((bool)(data_->type & DataCard::CardType::CriticalHitChance)) types.push_back("CRITICAL");

			if (!types.empty())
			{
				tip += "Type:";
				for (size_t i = 0; i < types.size(); ++i)
				{
					if (i) tip += ", ";
					tip += types[i];
				}
				tip += "\n";
			}

			// Core stats
			tip += std::format("Cost:{}\n", data_->cost);

			if (data_->damage != 0) tip += std::format("Damage:{}\n", data_->damage);
			if (data_->heal != 0) tip += std::format("Heal: {}\n", data_->heal);
			if (data_->defense != 0) tip += std::format("Defense:{}\n", data_->defense);
			if (data_->accuracy != 0) tip += std::format("Accuracy:{}\n", data_->accuracy);
			if (data_->criticalhitchance != 0) tip += std::format("Crit Chance:{}\n", data_->criticalhitchance);
			if (data_->move != 0) tip += std::format("Move:{}\n", data_->move);

			// Status effect stats
			if (data_->statuseffect != DataCard::CardStatusEffect::None)
			{
				string_t statusName;
				if (data_->statuseffect == DataCard::CardStatusEffect::Bleed) statusName = "Bleed";
				else if (data_->statuseffect == DataCard::CardStatusEffect::Disarm) statusName = "Disarm";
				else if (data_->statuseffect == DataCard::CardStatusEffect::Poison) statusName = "Poison";
				else statusName = "Unknown";

				tip += std::format("Status:{}\nDuration:{} turn(s)\nDamage:{} per turn\n",
					statusName, data_->statusduration, data_->statusdamage);
			}

            Vec4 grey = { 0.5, 0.5, 0.5 ,0.8 };

			// Apply text
            Vec4 white = { 1, 1, 1 ,1 };
            st->SetTextBorderColor(white);
			st->SetText(tip);
		}
	}

	void BehaviorCard::DestroyTooltip()
	{
		if (!GetParent())
			return;

		Transform* parentT = GetParent()->Has(Transform);
		if (!parentT) return;

		// Destroy text
		Transform* textChild = parentT->FindChild("CardTooltipText");
		if (textChild != nullptr && textChild->GetParent() != nullptr)
		{
			textChild->GetParent()->Destroy();
		}

		// Destroy background
		Transform* tooltipChild = parentT->FindChild(ChildTooltip);
		if (tooltipChild != nullptr && tooltipChild->GetParent() != nullptr)
		{
			tooltipChild->GetParent()->Destroy();
		}

		tooltipEntity_ = nullptr;
	}

	void BehaviorCard::OnInit()
	{
		isLocked_ = false;

		Animation* animation = GetParent()->Has(Animation);
		switch (GetStateCurrent())
		{
		case cCardInvalid:
		{
		}
		break;
		case cCardIdle:
		{
			if (animation != nullptr)
			{
				animation->SetSequence("CardHover");
				animation->SetSpeed(-1);
				animation->SetPlaying(true);
			}
		}
		break;
		case cCardHover:
		{
			if (animation != nullptr)
			{
				animation->SetSequence("CardHover");
				animation->SetSpeed(1);
				animation->SetPlaying(true);
			}
			AudioEmitter* audioEmitter = GetParent()->Has(AudioEmitter);
			if (audioEmitter != nullptr)
			{
				audioEmitter->SetSound("Hover");
				audioEmitter->RandPitch(.8f, 1.2f);
				audioEmitter->Play();
			}
		}
		break;
		case cCardSelect:
		{
			// Keep tooltip if the mouse is currently hovering the selected card
			if (IsHovered() && IsPlayable())
			{
				CreateTooltip();
			}
			else
			{
				DestroyTooltip();
			}
		}
		break;
		case cCardLocked:
		{

		}
		break;
		case cCardInactive:
		{
			if (animation != nullptr)
			{
				animation->SetSequence("CardInactive");
				animation->SetLoop(AnimationLoop::Stop);
				animation->SetSpeed(3);
				animation->SetPlaying(true);
			}
		}
		break;
		}

		Collider* collider = GetParent()->Has(Collider);
		if (collider != nullptr)
		{
			collider->SetCollisionHandler([this](const CollisionRecord& record) { CollisionHandler(record); });
		}
		

		//Add text regardless in idle state/locked state for any cards added in any special states.
		{
			Transform* transform = GetParent()->Has(Transform);
			Transform* childText = transform->FindChild(ChildText);
			Entity* childTextEntity;
			if (childText == nullptr) //Checks to see if child is already made, if not make a copy of child
			{
				childTextEntity = new Entity(ChildText);
				childTextEntity->Read(ChildText);
				childText = childTextEntity->Has(Transform);
				assert(childText != nullptr);
				GetParent()->GetContainer()->Add(childTextEntity);
				childText->SetParentTransform(transform);
			}
			else //Don't create child and set childtextentity
			{
				childTextEntity = childText->GetParent();
			}
			assert(childTextEntity != nullptr);
			SpriteText* childTextSprite = childTextEntity->Has(SpriteText);
			assert(childTextSprite != nullptr);

			string_t text;

			if (data_->damage != 0)
				text += std::format("Dmg:{}\n", data_->damage);

			if (data_->heal != 0)
				text += std::format("Heal:{}\n", data_->heal);

			if (data_->move != 0)
				text += std::format("Move:{}\n", data_->move);

			if (data_->defense != 0)
				text += std::format("Def:{}\n", data_->defense);


			childTextSprite->SetText(text);
		}

		// ---------------------------
		// COST TEXT CHILD
		// ---------------------------
		{
			Transform* transform = GetParent()->Has(Transform);
			Transform* costChild = transform->FindChild(ChildCostText);
			Entity* costChildEntity;

			if (costChild == nullptr)
			{
				costChildEntity = new Entity(ChildCostText);
				costChildEntity->Read(ChildCostText);

				costChild = costChildEntity->Has(Transform);
				assert(costChild != nullptr);

				GetParent()->GetContainer()->Add(costChildEntity);
				costChild->SetParentTransform(transform);
			}
			else
			{
				costChildEntity = costChild->GetParent();
			}

			assert(costChildEntity != nullptr);

			SpriteText* costSprite = costChildEntity->Has(SpriteText);
			assert(costSprite != nullptr);

			FORMAT_MAKE(costText, "{}", data_->cost);
			costSprite->SetText(costText);
		}

		if(data_->accuracy > 0)
		{
			Transform* transform = GetParent()->Has(Transform);
			Transform* Child = transform->FindChild(ChildAccuracyText);
			Entity* ChildEntity;

			if (Child == nullptr)
			{
				ChildEntity = new Entity(ChildAccuracyText);
				ChildEntity->Read(ChildAccuracyText);

				Child = ChildEntity->Has(Transform);
				assert(Child != nullptr);

				GetParent()->GetContainer()->Add(ChildEntity);
				Child->SetParentTransform(transform);
			}
			else
			{
				ChildEntity = Child->GetParent();
			}

			assert(ChildEntity != nullptr);

			SpriteText* AccuracySprite = ChildEntity->Has(SpriteText);
			assert(AccuracySprite != nullptr);
			string_t Text = "";
			if (data_->accuracy > 0)
				Text = std::format("{}", data_->accuracy);
			AccuracySprite->SetText(Text);
		}

		if (data_->criticalhitchance > 0)
		{
			Transform* transform = GetParent()->Has(Transform);
			Transform* CritChild = transform->FindChild(ChildCritText);
			Entity* CritChildEntity;

			if (CritChild == nullptr)
			{
				CritChildEntity = new Entity(ChildCritText);
				CritChildEntity->Read(ChildCritText);

				CritChild = CritChildEntity->Has(Transform);
				assert(CritChild != nullptr);

				GetParent()->GetContainer()->Add(CritChildEntity);
				CritChild->SetParentTransform(transform);
			}
			else
			{
				CritChildEntity = CritChild->GetParent();
			}

			assert(CritChildEntity != nullptr);

			SpriteText* CritSprite = CritChildEntity->Has(SpriteText);
			assert(CritSprite != nullptr);
			string_t Text = "";
			if(data_->criticalhitchance > 0)
			Text = std::format("{}",data_->criticalhitchance);

			CritSprite->SetText(Text);
		}
		//Icons for cards//
		if(data_->accuracy > 0)
		{
			Transform* parentT = GetParent()->Has(Transform);
			if (!parentT) return;

			
			Transform* iconT = parentT->FindChild(CardAccuracyIcon);
			Entity* iconEnt = nullptr;

			if (!iconT)
			{
				iconEnt = new Entity(CardAccuracyIcon);
				iconEnt->Read(CardAccuracyIcon);
				iconT = iconEnt->Has(Transform);
				assert(iconT);

				GetParent()->GetContainer()->Add(iconEnt);
				iconT->SetParentTransform(parentT);

			}
			else
			{
				iconEnt = iconT->GetParent();
			}

	
				if (Sprite* iconS = iconEnt->Has(Sprite))
				{
					iconS->SetSpriteSource("CardAccuracyIcon");
				}
				
		}
		
		
		if(data_->criticalhitchance>0)
		{
			Transform* parentT = GetParent()->Has(Transform);
			if (!parentT) return;

			
			Transform* iconT = parentT->FindChild(CardCritIcon);
			Entity* iconEnt = nullptr;

			if (!iconT)
			{
				iconEnt = new Entity(CardCritIcon);
				iconEnt->Read(CardCritIcon);
				iconT = iconEnt->Has(Transform);
				assert(iconT);

				GetParent()->GetContainer()->Add(iconEnt);
				iconT->SetParentTransform(parentT);

			}
			else
			{
				iconEnt = iconT->GetParent();
			}


			if (Sprite* iconS = iconEnt->Has(Sprite))
			{
				iconS->SetSpriteSource("CardCritIcon");
			}
		}

	}

	void BehaviorCard::OnUpdate(float dt)
	{

		
		if (data_->character_->GetHealth() <= 0 && group_ != CardGroup::cBanished)
		{
			hand_->BanishCard(data_);
		}

		{
			// Interpolation factor
			float tweenSpeed_ = hand_->GetCardMoveSpeed();
			float factor = std::clamp(tweenSpeed_ * dt, 0.0f, 1.0f);

			Transform* transform(GetParent()->Has(Transform));
			// Lerp Formula: 
			// result = a + t * (b - a) 
			// Where a = start value, b = end value, t = interpolation factor (0.0 to 1.0)
			// Reference: https://stackoverflow.com/questions/4353525/floating-point-linear-interpolation
			const Vec2& posCurr = transform->GetTranslation(),
				& posTarget = tweenTarget_;
			Vec2 next(posCurr + (posTarget - posCurr) * factor);

			// Snap
			float tweenSnapDistance_ = hand_->GetCardMoveSnap();
			if (std::fabs(posTarget.x - next.x) < tweenSnapDistance_)
			{
				next.x = posTarget.x;
			}
			if (std::fabs(posTarget.y - next.y) < tweenSnapDistance_)
			{
				next.y = posTarget.y;
			}

			transform->SetTranslation(next);
		}

		Sprite* sprite = GetParent()->Has(Sprite);
		auto collider = GetParent()->Has(Collider);

		switch (GetStateCurrent())
		{
		case cCardIdle:
		{
			if (!IsPlayable())
			{
				SetStateNext(cCardInactive);
			}
			if (sprite != nullptr)
			{
				if (collider != nullptr)
				{
					collider->SetEnabled(true);
				}
				sprite->SetZLayer(3);
			}
		}
		break;
		case cCardHover:
		{
			if (sprite != nullptr)
			{
				sprite->SetZLayer(4);
				if (collider != nullptr)
				{
					collider->SetEnabled(true);
				}
			}
		}
		break;
		case cCardSelect:
		{
			if (sprite != nullptr)
			{
				sprite->SetZLayer(5);
			}
			if (collider != nullptr)
			{
				collider->SetEnabled(true);
			}
			// If we are selected, and the mouse was clicked while not hovering on us, revert to the idle state
			if (InputSystem::MouseButtonIsTriggered(B_Mouse::Right) && !IsHovered())
			{
				SetStateNext(cCardIdle);
			}
		}
		break;
		case cCardLocked:
		{
			if (sprite != nullptr)
			{
				if (collider != nullptr)
				{
					collider->SetEnabled(false);
				}
			}
		}
		break;
		case cCardInactive:
		{
			if (sprite != nullptr)
			{
				if (collider != nullptr)
				{
					collider->SetEnabled(false);
				}
			}
		}
		break;
		default:
		{
		}
		}

		if (group_ != CardGroup::cHand)
		{
			SetStateNext(cCardInactive);
		}
		else if (GetStateCurrent() == cCardInactive)
		{
			SetStateNext(cCardIdle);
		}
	}

	void BehaviorCard::OnExit()
	{
		switch (GetStateCurrent())
		{
		case cCardInvalid:
		{
		}
		break;
		case cCardIdle:
		{
		}
		break;
		case cCardHover:
		{
		}
		break;
		case cCardSelect:
		{
		}
		break;
		case cCardInactive:
		{
		}
		break;
		}
	}

	void BehaviorCard::CollisionHandler(const CollisionRecord& record)
	{
		UNREFERENCED_PARAMETER(record);
	}

	void BehaviorCard::Use()
	{
		bool valid = true;
		BehaviorCharacter* target = hand_->GetPartyTarget();
		
		assert(hand_ != nullptr);
		int energy = hand_->GetParentParty()->GetCurrentEnergy();
		if (energy < data_->cost)
		{
			return;
		}
		Entity* card = GetParent();

		assert(target != nullptr);

		BehaviorCharacter* character = data_->character_;
		assert(character != nullptr);

		// Apply card effect
		if ((bool)(data_->type & DataCard::CardType::Damage))
		{
			valid = target->GetHealth() > 0;
			if (valid)
			{
				character->AttackState();
			}
		}
		else if ((bool)(data_->type & DataCard::CardType::Move))
		{
			BehaviorCharacter* owner = data_->GetOwner();
			assert(owner != nullptr);
			BehaviorParty* party = owner->GetParty();
			assert(party != nullptr);
			valid = owner != target && party->MoveAlly(owner, target);
			if (valid)
			{
				// TODO: character->MoveState();
			}
		}
		else if ((bool)(data_->type & DataCard::CardType::Heal))
		{
			// Heal should target the selected character but will not work if they are already dead.
			valid = target != nullptr && target->GetHealth() > 0;
			if (valid)
			{
				// TODO: character->HealState();
			}
		}

		// Remove the card from the hand/container and play sound
		if (valid)
		{
				TraceInfo("Calling data_->Use() on target: {}, damage: {}, target health before: {}",
					target->GetParent()->GetName(),
					data_->damage,
					target->GetHealth());

				data_->Use(target);

				TraceInfo("Target health after: {}", target->GetHealth());

				hand_->DiscardCard(data_);
				hand_->GetParentParty()->SetCurrentEnergy(energy - data_->cost);
			
			AudioEmitter* audioEmitter = card->Has(AudioEmitter);
			if (audioEmitter)
			{
				audioEmitter->SetSound(data_->sound_);
				audioEmitter->RandPitch(.8f, 1.2f);
				audioEmitter->Play();
			}
		}

		TraceInfo("Current Energy: {}", hand_->GetParentParty()->GetCurrentEnergy());
	}

	bool BehaviorCard::IsPlayable() const
	{
		// Must be in hand and enough energy
		if (group_ != CardGroup::cHand || hand_ == nullptr)
			return false;

		if (hand_->GetParentParty()->GetCurrentEnergy() < data_->cost)
			return false;

		// If the owner is disarmed, damage cards are not playable
		BehaviorCharacter* owner = data_->GetOwner();
		if (owner != nullptr && owner->HasStatus(DataCard::CardStatusEffect::Disarm))
		{
			if ((bool)(data_->type & (DataCard::CardType::Damage | DataCard::CardType::DamageAOE | DataCard::CardType::SplashDamage)))
			{
				return false;
			}
		}

		return true;
	}

	void BehaviorCard::Receive(Message msg)
	{
		switch (msg)
		{
		case Message::IsPlayerTurn:
		{
			if (BehaviorHand* h = GetParentHand())
			{
				BehaviorCharacter* target = h->GetPartyTarget();
				if (target)
				{
					target->SelectedState();
				}
				h->UnlockHand();
			}
		}
		break;
		case Message::IsEnemyTurn:
		{
			isLocked_ = true;
		}
		break;
		default:
			break;
		}
	}

	void BehaviorCard::OnHoverStart()
	{
		if (!IsPlayable())
			return;

		// Create tooltip on hover
		CreateTooltip();

		// Only change visual state to hover if we're not selected.
		if (GetStateCurrent() != cCardSelect)
		{
			SetStateNext(cCardHover);
		}
	}

	void BehaviorCard::OnHoverEnd()
	{
		DestroyTooltip();

		// If the card is not selected, revert visual state to idle.
		if (GetStateCurrent() != cCardSelect)
		{
			SetStateNext(cCardIdle);
		}
	}

	void BehaviorCard::OnClickStart()
	{
	}

	void BehaviorCard::OnClickEnd(bool contained)
	{
		if (contained)
		{
			if (GetStateCurrent() == cCardHover && IsPlayable())
			{
				SetStateNext(cCardSelect);
			}
		}
	}

#pragma endregion Private Functions

}	// namespace