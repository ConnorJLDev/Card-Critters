//------------------------------------------------------------------------------
// 
// File Name:	BehaviorHealth.cpp
// Author(s):	Jason Nguyen (jason.nguyen)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Health behavior.
//
// Copyright ? 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Entity.h"
#include "BehaviorHealth.h"
#include "Transform.h"
#include "Sprite.h"
#include "SpriteText.h"
#include "EntityContainer.h"

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
		BehaviorHealth::ReadKeyHealth = "Health",
		BehaviorHealth::ReadKeyHealthMax = "HealthMax",
		BehaviorHealth::ChildHealthText = "HealthText",
		BehaviorHealth::ChildHealthBarBG = "HealthBarBG",
		BehaviorHealth::ChildHealthBarFill = "HealthBarFill",
		BehaviorHealth::ChildHealthBarBorder = "HealthBarBorder"; 
	
	constexpr float HealthBarBaseWidth = 150.0f;
	constexpr float HealthBarBaseHeight = 30.0f;
	constexpr float HealthBarY = 0.8f;
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

	BehaviorHealth::BehaviorHealth(SubTypeEnum type)
		: Behavior(type)
		, health(5)
		, healthMax(5)
		, healthPrev(5)
		, displayedHealthRatio(1.0f)
	{
	}

	BehaviorHealth::BehaviorHealth(const BehaviorHealth& other)
		: Behavior(other)
		, health(other.health)
		, healthMax(other.healthMax)
		, healthPrev(other.healthPrev)
		, displayedHealthRatio(other.displayedHealthRatio)
	{
	}

	//--------------------------------------------------------------------------

	BehaviorHealth::~BehaviorHealth()
	{
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

	void BehaviorHealth::Read(StreamIn& stream)
	{
		stream.TryReadValue(ReadKeyHealth, health);
		if (!stream.TryReadValue(ReadKeyHealthMax, healthMax))
		{
			healthMax = health;
		}
		healthPrev = health;
		displayedHealthRatio = (healthMax > 0) ? static_cast<float>(health) / static_cast<float>(healthMax) : 0.0f;
	}

	void BehaviorHealth::Write(StreamOut& stream)
	{
		stream.WriteValue(ReadKeyHealth, health);
		stream.WriteValue(ReadKeyHealthMax, healthMax);
	}

	void BehaviorHealth::Update(float dt)
	{
		Behavior::Update(dt);

		if (IsDead())
		{
			health = 0; // Clamp health to 0
		}

		// Health counter display
		UpdateChildren();

		// Ensure derived classes can reliably detect a health change by
		// exposing the previous-frame value here.
		healthPrev = health;
	}

	void BehaviorHealth::ModifyHealth(int amount)
	{
		health += amount;
		if (health < 0)
			health = 0;
	}
	void BehaviorHealth::SetHealth(int amount)
	{
		health = amount;
		if (health < 0)
			health = 0;
	}

	int BehaviorHealth::GetHealth() const
	{
		return health;
	}

	int BehaviorHealth::GetMaxHealth() const
	{
		return healthMax;
	}

	bool BehaviorHealth::IsDead() const
	{
		return health <= 0;
	}

#pragma endregion Public Functions

	//--------------------------------------------------------------------------
	// Protected Static Functions:
	//--------------------------------------------------------------------------

#pragma region Protected Static Functions

#pragma endregion Protected Static Functions

	//--------------------------------------------------------------------------
	// Protected Functions:
	//--------------------------------------------------------------------------

#pragma region Protected Functions

	void BehaviorHealth::UpdateChildren(void)
	{
		Transform* parentTransform = GetParent()->Has(Transform);
		if (parentTransform == nullptr)
			return;

		UpdateHealthBarBackground(parentTransform);
		UpdateHealthBarFill(parentTransform);
		UpdateHealthBarBorder(parentTransform);
		UpdateHealthText(parentTransform);
	}

	void BehaviorHealth::UpdateHealthText(Transform* parentTransform)
	{
		Entity* childEntity = GetOrMakeChildEntity(ChildHealthText);
		if (childEntity == nullptr)
			return;

		Transform* childTransform = childEntity->Has(Transform);
		SpriteText* s = childEntity->Has(SpriteText);
		if (childTransform == nullptr || s == nullptr || parentTransform == nullptr)
			return;

		float xOffset = 0.0f;
		if (GetParent() != nullptr && GetParent()->GetName() == "Nero") //nero still fat
			xOffset = 0.25f;

		Vec2 parentScale = parentTransform->GetScale();

		float parentScaleX = std::abs(parentScale.x);
		float parentScaleY = std::abs(parentScale.y);

		if (parentScaleX == 0.0f) parentScaleX = 1.0f;
		if (parentScaleY == 0.0f) parentScaleY = 1.0f;

		Vec2 desiredScale = { 160.0f, 140.0f }; //chamge this value to change text size globally

		Vec2 correctedScale =
		{
			desiredScale.x / parentScaleX,
			desiredScale.y / parentScaleY
		};

		childTransform->SetTranslation({ xOffset, 0.8f });
		childTransform->SetScale(correctedScale);

		FORMAT_MAKE(text, "{}/{}", health, healthMax);
		s->SetText(text);
	}

	void BehaviorHealth::UpdateHealthBarBackground(Transform* parentTransform)
	{
		Entity* childEntity = GetOrMakeChildEntity(ChildHealthBarBG);
		if (childEntity == nullptr)
			return;

		Transform* childTransform = childEntity->Has(Transform);
		Sprite* s = childEntity->Has(Sprite);
		if (childTransform == nullptr || s == nullptr || parentTransform == nullptr) //failure = die
			return;

		float xOffset = 0.0f;
		if (GetParent() != nullptr && GetParent()->GetName() == "Nero") //nero is a fat boy and need hard coding.
			xOffset = 0.25f;

		Vec2 parentScale = parentTransform->GetScale();

	
		if (parentScale.x == 0.0f) parentScale.x = 1.0f;
		if (parentScale.y == 0.0f) parentScale.y = 1.0f;

		//same logic as stated in update, uses for undoing parent scale
		Vec2 correctedScale =
		{
			HealthBarBaseWidth / parentScale.x,
			HealthBarBaseHeight / parentScale.y
		};

		childTransform->SetTranslation({ xOffset, HealthBarY });
		childTransform->SetScale(correctedScale);

		s->SetSpriteSource("HealthBarBG");
		s->SetZLayer(9);
	}

	void BehaviorHealth::UpdateHealthBarFill(Transform* parentTransform)
	{
		Entity* childEntity = GetOrMakeChildEntity(ChildHealthBarFill);
		if (childEntity == nullptr)
			return;

		Transform* childTransform = childEntity->Has(Transform);
		Sprite* s = childEntity->Has(Sprite);
		if (childTransform == nullptr || s == nullptr || parentTransform == nullptr)
			return;

		float targetRatio = 0.0f;
		if (healthMax > 0)
			targetRatio = static_cast<float>(health) / static_cast<float>(healthMax);

		targetRatio = std::clamp(targetRatio, 0.0f, 1.0f);

		//Determines speed for losing hp bar
		float lerpSpeed = 0.025f; // Bigger = faster, lower = slower 
		displayedHealthRatio += (targetRatio - displayedHealthRatio) * lerpSpeed;
		displayedHealthRatio = std::clamp(displayedHealthRatio, 0.0f, 1.0f);

		float xOffset = 0.0f;
		if (GetParent() != nullptr && GetParent()->GetName() == "Nero")
			xOffset = 0.25f;

		//Counter's parent scale, asked chat GPT for an idea and gave this this, actually worked lol. 
			Vec2 parentScale = parentTransform->GetScale();
			if (parentScale.x == 0.0f) parentScale.x = 1.0f;
			if (parentScale.y == 0.0f) parentScale.y = 1.0f;

			float currentWidth = HealthBarBaseWidth * displayedHealthRatio;

			Vec2 correctedScale =
			{
				currentWidth / parentScale.x,
				HealthBarBaseHeight / parentScale.y
			};
		//End of chat GPT code fragment (used in all pieces)
		
		// Anchored to the right of the health bar and is moved left, because we are drawing from the center (THANKS JONATHAN!) 
		float lostWidth = HealthBarBaseWidth - currentWidth;
		float xShift = -(lostWidth * 0.5f) / parentScale.x;

		childTransform->SetTranslation({ xOffset + xShift, HealthBarY }); //Uses predifined number to shift depending on either specific creature or just y value offset from top.
		childTransform->SetScale(correctedScale);

		s->SetSpriteSource("HealthBarFill"); 
		s->SetZLayer(10);

		Vec3 color;
		if (displayedHealthRatio > 0.5f)
		{
			float t = (displayedHealthRatio - 0.5f) / 0.5f;
			color = Vec3(1.0f - t, 1.0f, 0.0f);
		}
		else
		{
			float t = displayedHealthRatio / 0.5f;
			color = Vec3(1.0f, t, 0.0f);
		}

		s->SetTint(color);
	}

	void BehaviorHealth::UpdateHealthBarBorder(Transform* parentTransform)
	{
		Entity* childEntity = GetOrMakeChildEntity(ChildHealthBarBorder);
		if (childEntity == nullptr)
			return;

		Transform* childTransform = childEntity->Has(Transform);
		Sprite* s = childEntity->Has(Sprite);
		if (childTransform == nullptr || s == nullptr || parentTransform == nullptr) //safety check
			return;

		float xOffset = 0.0f;
		if (GetParent() != nullptr && GetParent()->GetName() == "Nero") //Nero is just fucked (fat), so i'm hardcoding this don't ask.
			xOffset = 0.25f;

		//same logic as stated in update, uses for undoing parent scale
		Vec2 parentScale = parentTransform->GetScale();

		// Prevent divide-by-zero oops lots of crashes #thanksfutureme
		if (parentScale.x == 0.0f) parentScale.x = 1.0f;
		if (parentScale.y == 0.0f) parentScale.y = 1.0f;
	
		Vec2 correctedScale =
		{
			HealthBarBaseWidth / parentScale.x,
			HealthBarBaseHeight / parentScale.y
		};

		childTransform->SetTranslation({ xOffset, HealthBarY });
		childTransform->SetScale(correctedScale);

		s->SetSpriteSource("HealthBarBorder");
		s->SetZLayer(11);
	}

#pragma endregion Protected Functions

}	// namespace