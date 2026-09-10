//------------------------------------------------------------------------------
//
// File Name:	BehaviorFloatingText.cpp
// Author(s):	jason.nguyen
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Floating combat text behavior that auto-destroys.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Entity.h"
#include "Collider.h"
#include "Transform.h"
#include "BehaviorFloatingText.h"
#include "Sprite.h"

//------------------------------------------------------------------------------

namespace Nightjar
{
#pragma region Constructors

	BehaviorFloatingText::BehaviorFloatingText()
		: Behavior(cBehaviorFloatingText)
		, lifetime(1.0f)
		, timer(0.0f)
		, floatSpeed(0.5f)
		, fade(false)
	{
	}

	BehaviorFloatingText::BehaviorFloatingText(const BehaviorFloatingText& other)
		: Behavior(other)
		, lifetime(other.lifetime)
		, timer(0.0f)
		, floatSpeed(other.floatSpeed)
		, fade(other.fade)
	{
	}

	BehaviorFloatingText::~BehaviorFloatingText()
	{
	}

#pragma endregion

	//------------------------------------------------------------------------------

#pragma region Public Functions

	BehaviorFloatingText* BehaviorFloatingText::Clone() const
	{
		return new BehaviorFloatingText(*this);
	}

	void BehaviorFloatingText::Read(StreamIn& stream)
	{
		stream.TryReadValue("Lifetime", lifetime);
		stream.TryReadValue("Speed", floatSpeed);
		stream.TryReadValue("Fade", fade);
	}

	void BehaviorFloatingText::Write(StreamOut& stream)
	{
		stream.WriteValue("Lifetime", lifetime);
		stream.WriteValue("Speed", floatSpeed);
		stream.WriteValue("Fade", fade);
	}

#pragma endregion

	//------------------------------------------------------------------------------

#pragma region Private Functions

	void BehaviorFloatingText::OnInit()
	{
		switch (GetStateCurrent())
		{
		case cBehaviorFloatingTextIdle:
			timer = 0.0f;
			break;
		}

		// Optional collider hookup (probably not needed but template-compatible)
		Collider* collider = GetParent()->Has(Collider);
		if (collider != nullptr)
		{
			collider->SetCollisionHandler(
				[this](const CollisionRecord& record)
				{
					CollisionHandler(record);
				});
		}
	}

	void BehaviorFloatingText::OnUpdate(float dt)
	{
		timer += dt;

		switch (GetStateCurrent())
		{
		case cBehaviorFloatingTextIdle:
		{
			Transform* transform = GetParent()->Has(Transform);
			if (transform != nullptr)
			{
				Vec2 pos = transform->GetTranslation();
				pos.y += floatSpeed * dt;
				transform->SetTranslation(pos);
			}

			// Fade effect
			if (fade)
			{
				Sprite* sprite = GetParent()->Has(Sprite);
				if (sprite != nullptr)
				{
					float progress = timer / lifetime;
					progress = std::clamp(progress, 0.0f, 1.0f);

					float alpha = 1.0f - progress;

					Vec4 tint = sprite->GetTint();
					tint.w = alpha;
					sprite->SetTint(tint);
				}
			}

			if (timer >= lifetime)
			{
				GetParent()->Destroy();
			}

			break;
		}
		}
	}

	void BehaviorFloatingText::OnExit()
	{
		switch (GetStateCurrent())
		{
		case cBehaviorFloatingTextIdle:
			break;
		}
	}

	void BehaviorFloatingText::CollisionHandler(const CollisionRecord& record)
	{
		UNREFERENCED_PARAMETER(record);
		// Floating text should not react to collisions
	}

#pragma endregion

}