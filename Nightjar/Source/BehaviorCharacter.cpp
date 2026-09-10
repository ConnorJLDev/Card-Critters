//------------------------------------------------------------------------------
//
// File Name:	BehaviorCharacter.cpp
// Author(s):	JSand
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Template class for a new behavior.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Entity.h"
#include "BehaviorCharacter.h"
#include "Sprite.h"
#include "SpriteText.h"
#include "Model.h"
#include "Physics.h"
#include "Transform.h"
#include "Animation.h"
#include "EntityContainer.h"
#include "BehaviorHand.h"

#include "DataCard.h"
#include "BehaviorParty.h"
#include "AudioEmitter.h"
#include "BehaviorCharacterSelector.h"
#include "BehaviorFloatingText.h"
#include "ParticleEmitter.h"
#include "Material.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	//--------------------------------------------------------------------------

	constexpr StreamKey
		BehaviorCharacter::ChildVisual = "CharacterVisual",
		BehaviorCharacter::ChildSelector = "CharacterSelector",
		BehaviorCharacter::ReadKeyCharacterName = "CharacterName",
		BehaviorCharacter::ReadKeyCards = "Cards",
        BehaviorCharacter::ChildStatusIcon = "CharacterStatusIcon",
        BehaviorCharacter::ChildNextTargetArrow = "NextTargetArrow";

	//--------------------------------------------------------------------------

#pragma region Constructors

	BehaviorCharacter::BehaviorCharacter()
		: BehaviorHealth(cBehaviorCharacter)
		, cards_()
		, cardNames_()
		, characterName_()
		, party_()
		, tweenTarget_()
		, hasAttacked_(false)
		, attackWaiting_(false)
		, CID_()
        , statusIconEntities_()
        , statusIconOffset_(-0.55, 0.93)
	{
	}

	BehaviorCharacter::BehaviorCharacter(const BehaviorCharacter& other)
		: BehaviorHealth(other)
		, cards_()
		, cardNames_(other.cardNames_)
		, characterName_(other.characterName_)
		, party_(nullptr)
		, tweenTarget_(other.tweenTarget_)
		, hasAttacked_(false)
		, attackWaiting_(false)
		, CID_()
        , statusIconEntities_()
        , statusIconOffset_(other.statusIconOffset_)
	{
	}

	BehaviorCharacter::~BehaviorCharacter()
	{
		MessageBus::Unsubscribe(CID_);

        // Ensure any created status icon entities are destroyed
        for (Entity* e : statusIconEntities_)
        {
            if (e != nullptr)
            {
                e->Destroy();
            }
        }
        statusIconEntities_.clear();
	}

#pragma endregion Constructors

#pragma region Public Functions

	BehaviorCharacter* BehaviorCharacter::Clone() const
	{
		return new BehaviorCharacter(*this);
	}

	void BehaviorCharacter::Read(StreamIn& stream)
	{
		BehaviorHealth::Read(stream);

		if (stream.StreamHas(ReadKeyCards))
		{
			cardNames_.clear();
			stream.ReadVector(ReadKeyCards, cardNames_);
		}

		stream.TryReadValue(ReadKeyCharacterName, characterName_);
	}

	void BehaviorCharacter::Write(StreamOut& stream)
	{
		BehaviorHealth::Write(stream);
		stream.WriteValue(ReadKeyCharacterName, characterName_);

		// Write cards.
		stream.WriteArrayStart(ReadKeyCards);
		for (const auto& card : cards_)
		{
			stream.WriteValue<NamedObject>(card);
		}
		stream.WriteArrayEnd();
	}

	vector_t<DataCard>& BehaviorCharacter::GetCards()
	{
		return cards_;
	}

	const vector_t<DataCard>& BehaviorCharacter::GetCardsConst() const
	{
		return cards_;
	}

	BehaviorParty* BehaviorCharacter::GetParty() const
	{
		return party_;
	}

	int BehaviorCharacter::GetPartyPosition()
	{
		for (size_t i = 0; i < party_->GetParty().size(); ++i)
		{
			if (this == party_->GetParty().at(i))
			{
				return (int)i;
			}
		}
		return -1;
	}

	BehaviorParty* BehaviorCharacter::GetPartyPointer()
	{
		return party_;
	}

	void BehaviorCharacter::SetMaxHealth(int value)
	{
		healthMax = value;
	}

	void BehaviorCharacter::SetCards(const vector_t<DataCard>& deck)
	{
		cards_ = deck;
		for (auto& card : cards_)
		{
			card.SetOwner(this);
		}
	}

	void BehaviorCharacter::DamageText(int amount)
	{
		Entity* parent = GetParent();
		Transform* parentTransform = parent->Has(Transform);
		if (!parentTransform) return;

		// ALWAYS create new entity
		Entity* textEntity = new Entity("DamageText");
		textEntity->Read("DamageText");

		Transform* textTransform = textEntity->Has(Transform);

		parent->GetContainer()->Add(textEntity);
		textTransform->SetParentTransform(parentTransform);
		textTransform->SetTranslation({ 0.0f, 0.8f });

        FORMAT_MAKE(textValue, "-{}", amount);

		if (SpriteText* stext = textEntity->Has(SpriteText))
		{
			stext->SetText(textValue);
		}
	}

	void BehaviorCharacter::CritText(int amount)
	{
		Entity* parent = GetParent();
		Transform* parentTransform = parent->Has(Transform);
		if (!parentTransform) return;

		Entity* textEntity = new Entity("CritText");
		textEntity->Read("CritText");

		Transform* textTransform = textEntity->Has(Transform);

		parent->GetContainer()->Add(textEntity);
		textTransform->SetParentTransform(parentTransform);
		textTransform->SetTranslation({ -0.3f, 0.8f });

        FORMAT_MAKE(textValue, "-{} CRIT!", amount);

		if (SpriteText* stext = textEntity->Has(SpriteText))
		{
			stext->SetText(textValue);
		}
	}

	void BehaviorCharacter::HealText(int amount)
	{
		Entity* parent = GetParent();
		Transform* parentTransform = parent->Has(Transform);
		if (!parentTransform) return;

		Entity* textEntity = new Entity("HealText");
		textEntity->Read("HealText");

		Transform* textTransform = textEntity->Has(Transform);

		parent->GetContainer()->Add(textEntity);
		textTransform->SetParentTransform(parentTransform);
		textTransform->SetTranslation({ 0.0f, 0.8f });

        FORMAT_MAKE(textValue, "+{}", amount);

		if (SpriteText* stext = textEntity->Has(SpriteText))
		{
			stext->SetText(textValue);
		}
	}

	void BehaviorCharacter::MissText()
	{
		Entity* parent = GetParent();
		Transform* parentTransform = parent->Has(Transform);
		if (!parentTransform) return;

		Entity* textEntity = new Entity("MissText");
		textEntity->Read("MissText");

		Transform* textTransform = textEntity->Has(Transform);

		parent->GetContainer()->Add(textEntity);
		textTransform->SetParentTransform(parentTransform);
		textTransform->SetTranslation({ 0.0f, 0.8f });

		string_t textValue = "MISSED!";

		if (SpriteText* stext = textEntity->Has(SpriteText))
		{
			stext->SetText(textValue);
		}
	}

	void BehaviorCharacter::StatusText(DataCard::CardStatusEffect type)
	{
		Entity* parent = GetParent();
		Transform* parentTransform = parent->Has(Transform);
		if (!parentTransform) return;

		Entity* textEntity = new Entity("StatusText");
		textEntity->Read("StatusText");

		Transform* textTransform = textEntity->Has(Transform);

		Sprite* textSprite = textEntity->Has(Sprite);
		SpriteText* textSpriteText = textEntity->Has(SpriteText);
		Vec4 greyTint{ 0.5f, 0.5f, 0.5f, 1.0f };
		Vec4 redTint{ 1.0f, 0.0f, 0.0f, 1.0f };
		Vec4 greenTint{ 0.0f, 1.0f, 0.0f, 1.0f };

		parent->GetContainer()->Add(textEntity);
		textTransform->SetParentTransform(parentTransform);
		textTransform->SetTranslation({ 0.0f, 0.6f });

		string_t textValue = "";
		if (type == DataCard::CardStatusEffect::Bleed)
		{
			textValue = "BLEEDING!";
			if (textSprite) textSprite->SetTint(redTint);
			if (textSpriteText) textSpriteText->SetTint(redTint);
		}
		else if (type == DataCard::CardStatusEffect::Disarm)
		{
			textValue = "DISARMED!";
			if (textSprite) textSprite->SetTint(greyTint);
			if (textSpriteText) textSpriteText->SetTint(greyTint);
		}
		else if (type == DataCard::CardStatusEffect::Poison)
		{
			textValue = "POISONED!";
			if (textSprite) textSprite->SetTint(greenTint);
			if (textSpriteText) textSpriteText->SetTint(greenTint);
		}

		if (SpriteText* stext = textEntity->Has(SpriteText))
		{
			stext->SetText(textValue);
		}
	}

	string_t BehaviorCharacter::GetCharacterName()
	{
		return characterName_;
	}

	// STATUS RELATED
	void BehaviorCharacter::AddStatus(DataCard::CardStatusEffect type, int duration, int magnitude)
	{

		// Merge with existing status of same type when possible to avoid duplicates
		for (auto &s : statuses_)
		{
			if (s.type == type)
			{
				// Extend duration to the larger value, update magnitude to the provided one
				s.duration = std::max(s.duration, duration);
				s.magnitude = magnitude;
				TraceInfo("BehaviorCharacter '{}' merged existing status {} -> duration {}, magnitude {}",
					EntityName(), static_cast<int>(type), s.duration, s.magnitude);
				// Immediately update visuals so the change is visible right away.
				UpdateStatusEffect();
				return;
			}
		}

		// If not found, add a new status instance
		StatusInstance newStatus;
		newStatus.type = type;
		newStatus.duration = duration;
		newStatus.magnitude = magnitude;
		if (newStatus.duration < 1) newStatus.duration = 1;

		statuses_.push_back(newStatus);

		// Log and immediately update visuals so tint/indicators refresh this frame.
		TraceInfo("BehaviorCharacter '{}' added status {} duration {}, magnitude {}",
			EntityName(), static_cast<int>(type), newStatus.duration, newStatus.magnitude);

		UpdateStatusEffect();
	}

	void BehaviorCharacter::RemoveStatus(DataCard::CardStatusEffect type)
	{
		statuses_.erase(std::remove_if(statuses_.begin(), statuses_.end(),
			[type](const StatusInstance& s) { return s.type == type; }), statuses_.end());

		TraceInfo("BehaviorCharacter '{}' removed status {}", EntityName(), static_cast<int>(type));

		// Ensure visuals update immediately after removal
		UpdateStatusEffect();
	}

	bool BehaviorCharacter::HasStatus(DataCard::CardStatusEffect type) const
	{
		for (const auto& s : statuses_)
		{
			if (s.type == type)
			{
				return true;
			}
		}
		return false;
	}

    bool BehaviorCharacter::HasNoStatus() const
    {
        if (statuses_.empty())
            return true;
        else
            return false;
    }

#pragma endregion Public Functions

#pragma region Protected Functions

	void BehaviorCharacter::UpdateVisual()
	{
		Entity* childVisualEntity = GetOrMakeChildEntity(ChildVisual);
		Sprite* childVisualSprite = childVisualEntity->Has(Sprite);
		if (characterName_.empty())
		{
			TraceWarning("Entity \"{}\" missing CharacterName in BehaviorCharacter!", EntityName());
		}
		else if (childVisualSprite != nullptr)
		{
			childVisualSprite->SetSpriteSource(characterName_);
		}
		GetOrMakeChildEntity(ChildSelector);
	}

	Animation* BehaviorCharacter::GetChildAnimation()
	{
		Entity* childVisualEntity = GetOrMakeChildEntity(ChildVisual);
		return childVisualEntity->Has(Animation);
	}

	void BehaviorCharacter::UpdateStatusEffect()
	{
        UpdateStatusEffectDisplay();

		// Always try to update visual tint
		Entity* visualEntity = GetOrMakeChildEntity(ChildVisual);
		if (!visualEntity)
			return;

		Sprite* charSprite = visualEntity->Has(Sprite);
		if (!charSprite)
			return;

		static const Vec4 defaultTint{ 1.0f, 1.0f, 1.0f, 1.0f };
		if (!hasSavedOriginalTint_)
		{
			originalTint_ = charSprite->GetTint();
			hasSavedOriginalTint_ = true;
		}

		// STATUS PRIORITY TINT
		// 1. DISARM
		if (HasStatus(DataCard::CardStatusEffect::Disarm))
		{
			Vec4 greyTint{ 0.5f, 0.5f, 0.5f, 1.0f };
			charSprite->SetTint(greyTint);
			return;
		}
		// 2. BLEED
		if (HasStatus(DataCard::CardStatusEffect::Bleed))
		{
			Vec4 redTint{ 1.0f, 0.0f, 0.0f, 1.0f };
			charSprite->SetTint(redTint);
			return;
		}
		// 2. Poison
		if (HasStatus(DataCard::CardStatusEffect::Poison))
		{
			Vec4 redTint{ 0.0f, 1.0f, 0.0f, 1.0f };
			charSprite->SetTint(redTint);
			return;
		}

		// Restore original/default tint
		charSprite->SetTint(hasSavedOriginalTint_ ? originalTint_ : defaultTint);
	}

    void BehaviorCharacter::UpdateStatusEffectDisplay()
    {
        // If the character is dead, remove any status icons
        if (IsDead())
        {
            for (Entity* e : statusIconEntities_)
            {
                if (e != nullptr)
                    e->Destroy();
            }
            statusIconEntities_.clear();
            return;
        }

        // If no statuses, destroy any existing icon entities and return
        if (HasNoStatus())
        {
            for (Entity* e : statusIconEntities_)
            {
                if (e != nullptr)
                    e->Destroy();
            }
            statusIconEntities_.clear();
            return;
        }

        Transform* parentT = GetParent()->Has(Transform);
        if (!parentT) return;

        vector_t<DataCard::CardStatusEffect> effects;
        effects.reserve(statuses_.size());
        for (const auto& s : statuses_) effects.push_back(s.type);
        std::sort(effects.begin(), effects.end(), [](DataCard::CardStatusEffect a, DataCard::CardStatusEffect b) {
            return static_cast<int>(a) < static_cast<int>(b);
            });

        // Ensure we have an icon entity for each effect, create if missing
        for (size_t i = 0; i < effects.size(); ++i)
        {
            Entity* iconEnt = nullptr;

            if (i < statusIconEntities_.size())
            {
                iconEnt = statusIconEntities_[i];
                // If somehow the stored pointer is invalid, recreate
                if (iconEnt == nullptr)
                {
                    statusIconEntities_[i] = nullptr;
                }
            }

            if (iconEnt == nullptr)
            {
                FORMAT_MAKE(runtimeName, "{}{}", ChildStatusIcon, i);
                Entity* newEnt = new Entity(runtimeName);
                newEnt->Read(ChildStatusIcon);
                Transform* iconT = newEnt->Has(Transform);
                assert(iconT != nullptr);

                GetParent()->GetContainer()->Add(newEnt);
                iconT->SetParentTransform(parentT);
                // Position will be set below
                iconEnt = newEnt;

                if (i < statusIconEntities_.size())
                    statusIconEntities_[i] = iconEnt;
                else
                    statusIconEntities_.push_back(iconEnt);
            }

            // Position icon relative to base offset and spacing
            Transform* iconT = iconEnt->Has(Transform);
            if (iconT != nullptr)
            {
                Vec2 pos = statusIconOffset_ + Vec2(static_cast<float>(i) * statusIconSpacing_.x, static_cast<float>(i) * statusIconSpacing_.y);
                iconT->SetTranslation(pos);
            }

            Vec2 parentScale = parentT->GetScale();

            if (parentScale.x == 0.0f) parentScale.x = 1.0f;
            if (parentScale.y == 0.0f) parentScale.y = 1.0f;

            Vec2 correctedScale =
            {
                25.0 / parentScale.x,
                25.0 / parentScale.y
            };

            iconT->SetScale2D(correctedScale);
            // Set sprite source
            Sprite* statusS = iconEnt->Has(Sprite);
            if (statusS)
            {
                switch (effects[i])
                {
                case DataCard::CardStatusEffect::Poison:
                    statusS->SetSpriteSource("StatusPoison");
                    break;
                case DataCard::CardStatusEffect::Bleed:
                    statusS->SetSpriteSource("StatusBleed");
                    break;
                case DataCard::CardStatusEffect::Disarm:
                    statusS->SetSpriteSource("StatusDisarm");
                    break;
                default:
                    statusS->SetSpriteSource("BLANK");
                    break;
                }
            }
        }

        // If we have more existing icons than current effects, remove the extras
        if (statusIconEntities_.size() > effects.size())
        {
            for (size_t i = effects.size(); i < statusIconEntities_.size(); ++i)
            {
                Entity* e = statusIconEntities_[i];
                if (e != nullptr)
                    e->Destroy();
            }
            statusIconEntities_.resize(effects.size());
        }
    }

	void BehaviorCharacter::IdleState()
	{
		UpdateVisual();

		if (IsDead())
		{
			SetStateNext(cBehaviorCharacterDead);

			AudioEmitter* audioEmitter = GetParent()->Has(AudioEmitter);
			if (audioEmitter)
			{
				if (audioEmitter->GetIsPaused())
					audioEmitter->Play();
			}
		}
		if (health < healthPrev)
		{
			SetStateNext(cBehaviorCharacterDamaged);
		}
	}

	void BehaviorCharacter::AttackState()
	{
		SetStateNext(cBehaviorCharacterAttack);
	}

	void BehaviorCharacter::DamagedState()
	{
		SetStateNext(cBehaviorCharacterDamaged);
	}

	void BehaviorCharacter::SelectedState()
	{
		if (IsDead())
			return;
		returnToSelected_ = true;
		if (GetStateCurrent() == cBehaviorCharacterIdle)
			SetStateNext(cBehaviorCharacterSelected);
	}

	void BehaviorCharacter::Deselect()
	{
		if (GetStateCurrent() == cBehaviorCharacterSelected)
		{
			returnToSelected_ = false;
			SetStateNext(cBehaviorCharacterIdle);
		}
	}

	void BehaviorCharacter::LoadCards()
	{
		cards_.clear();
		for (const string_t& name : cardNames_)
		{
			StreamIn cardStream("Data/Cards/" + name + ".json");
			if (cardStream.IsValid())
			{
				DataCard& card = cards_.emplace_back();
				card.Read(cardStream);
				card.SetOwner(this);
			}
			else
			{
				TraceWarning("Failed to read card \"{}\" from path \"{}\"", name, cardStream.GetPath());
			}
		}
	}

#pragma endregion Protected Functions

#pragma region Private Functions

	void BehaviorCharacter::OnInit()
	{
		Animation* visualAnim{ GetChildAnimation() };

		// States
		switch (GetStateCurrent())
		{
		case cBehaviorCharacterIdle:
			if (visualAnim != nullptr)
			{
				visualAnim->SetSequence("AllyIdle");
				visualAnim->SetPlaying(true, true);
			}
			break;
		case cBehaviorCharacterDamaged:
			if (visualAnim != nullptr)
			{
				visualAnim->SetSequence("AllyDamaged");
				visualAnim->SetPlaying(true, true);
			}
			break;
		case cBehaviorCharacterAttack:
			if (visualAnim != nullptr)
			{
				visualAnim->SetSequence("AllyAttack");
				visualAnim->SetPlaying(true, true);
			}
			break;
		case cBehaviorCharacterDead:
        {
            if (visualAnim != nullptr)
            {
                visualAnim->SetPlaying(false, true);
            }
            ParticleEmitter* emitter = EntityHas(ParticleEmitter);
            if (emitter != nullptr)
            {
                Material* particleMat = emitter->GetMaterial();
                assert(particleMat != nullptr);
                const Texture* tex = GraphicsSystem::GetTexture("Tombstone");
                particleMat->SetTexture("inTexture", tex);
                particleMat->SetUniform("colorTint", Vec4(1, 1, 1, 1));
                particleMat->SetUniform("texCoordScale", Vec2(1, 1));
                particleMat->SetUniform("texCoordOffset", Vec2(0, 0));
                particleMat->SetUniform("alphaClip", 0.01f);
                particleMat->SetUniform("zLayer", 0.9f);
                emitter->Emit(10);
            }
            Entity* childVisualEntity = GetOrMakeChildEntity(ChildVisual);
            if (childVisualEntity != nullptr)
            {
                Sprite* sprite = childVisualEntity->Has(Sprite);
                if (sprite != nullptr)
                {
                    sprite->SetSpriteSource("Tombstone");
                }
            }
        }
			break;
		case cBehaviorCharacterSelected:
			if (visualAnim != nullptr)
			{
				visualAnim->SetSequence("AllySelected");
				visualAnim->SetPlaying(true, true);
			}
			break;
		}
	}

	void BehaviorCharacter::OnUpdate(float)
	{
		UpdateStatusEffect();

		Animation* visualAnim{ GetChildAnimation() };
		Transform* transform = GetParent()->Has(Transform);

		{
			const Transform* partytransform = transform->GetParentTransform();
			float xScale = partytransform->GetScale().x;

			Vec2 xScaleVec = transform->GetScale();
			xScaleVec.x = std::copysign(xScaleVec.x, xScale);
			transform->SetScale(xScaleVec);

			// Interpolation factor
			const float factor = party_->GetCharacterMoveSpeedScaled();

			// Lerp Formula: 
			// result = a + t * (b - a) 
			// Where a = start value, b = end value, t = interpolation factor (0.0 to 1.0)
			// Reference: https://stackoverflow.com/questions/4353525/floating-point-linear-interpolation
			const Vec2& posCurr = transform->GetTranslation();
			Vec2 next(posCurr + (tweenTarget_ - posCurr) * factor);

			// Snap
			if (std::fabs(tweenTarget_.x - next.x) < party_->GetCharacterMoveSnap())
			{
				next.x = tweenTarget_.x;
			}
			if (std::fabs(tweenTarget_.y - next.y) < party_->GetCharacterMoveSnap())
			{
				next.y = tweenTarget_.y;
			}

			transform->SetTranslation(next);
		}

		// States
		switch (GetStateCurrent())
		{
		case cBehaviorCharacterIdle:
			IdleState();
			break;

		case cBehaviorCharacterAttack:
		{
			if (party_->GetHand())
			{
				if (!party_->GetHand()->HasPlayableCard())
				{
					party_->GetHand()->LockHand();
					BehaviorCharacter* target = party_->GetHand()->GetPartyTarget();
					if (target)
						target->Deselect();
				}
			}
			if (visualAnim->IsDone())
			{
				if (returnToSelected_)
					SetStateNext(cBehaviorCharacterSelected);
				else
					SetStateNext(cBehaviorCharacterIdle);
			}
		}
			break;

		case cBehaviorCharacterDamaged:
			if (visualAnim->IsDone())
			{
				if (returnToSelected_)
					SetStateNext(cBehaviorCharacterSelected);
				else
					SetStateNext(cBehaviorCharacterIdle);
			}
			break;

		case cBehaviorCharacterDead:
			if (visualAnim->IsDone())
			{
				// DO PASS GO DO COLLECT $200
			}
			break;

		case cBehaviorCharacterSelected:
			IdleState();
			break;
		}
	}

	void BehaviorCharacter::OnExit()
	{
		switch (GetStateCurrent())
		{
		case cBehaviorCharacterInvalid:
		{
			//LoadCards();
		}
			break;
		}
	}

    void BehaviorCharacter::ShowNextTargetArrow(bool show)
    {
        Entity* arrowEntity = GetOrMakeChildEntity(ChildNextTargetArrow);
        if (arrowEntity == nullptr)
            return;

        Transform* arrowTransform = arrowEntity->Has(Transform);
        if (arrowTransform == nullptr)
            return;

        Sprite* arrowSprite = arrowEntity->Has(Sprite);
        Entity* parent = arrowTransform->GetParent();
        Transform* parentT = parent->Has(Transform);
        //same logic as stated in update, uses for undoing parent scale
        Vec2 correctedScale =
        {
            .1f/ parentT->GetScale().x,
            .15f/ parentT->GetScale().y
        };

        arrowSprite->SetTint({ 1.0f,0.0f,0.0f });
        float xOffset = 0.0f;
        if (GetParent() != nullptr && GetParent()->GetName() == "Nero") //nero is a fat boy and need hard coding.
            xOffset = 0.25f;

        arrowTransform->SetTranslation({ xOffset, 1.2f });
         arrowTransform->SetScale(correctedScale);
        if (!show || IsDead())
        {
            if (arrowSprite)
                arrowSprite->SetAlpha(0);
            return;
        }
     
        if (arrowSprite)
            arrowSprite->SetAlpha(1);
    }

#pragma endregion Private Functions

	void BehaviorCharacter::OnTurnStart()
	{
		if (IsDead())
			return;
		for (size_t i = 0; i < statuses_.size(); )
		{
			StatusInstance& s = statuses_[i];

			// Bleed damage
			if (s.type == DataCard::CardStatusEffect::Bleed)
			{
				int dmg = s.magnitude;
				if (dmg > 0)
				{
					ModifyHealth(-dmg);
					DamageText(dmg);
				}
			}

			// Poison damage
			if (s.type == DataCard::CardStatusEffect::Poison)
			{
				int dmg = s.magnitude;
				if (dmg > 0)
				{
					ModifyHealth(-dmg);
					DamageText(dmg);
				}
			}

			// Decrement duration and remove if expired
			s.duration -= 1;
			if (s.duration <= 0)
			{
				statuses_.erase(statuses_.begin() + (unsigned int)i);
			}
			else
			{
				++i;
			}
		}

		// Update visuals/tinting for status changes
		UpdateStatusEffect();
	}

}	// namespace
