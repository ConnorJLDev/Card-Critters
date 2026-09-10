//------------------------------------------------------------------------------
//
// File Name:	BehaviorMessageBox.cpp
// Author(s):	Jonathan
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
#include "EntityContainer.h"
#include "Collider.h"
#include "BehaviorMessageBox.h"
#include "SceneSystem.h"

#include "Transform.h"
#include "Sprite.h"
#include "SpriteText.h"
#include "TurnSystem.h"
#include "PlatformSystem.h"

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
		BehaviorMessageBox::ReadKeyMessages = "Messages",
		BehaviorMessageBox::ReadKeyOnMessagesEnd = "OnMessagesEnd",
		BehaviorMessageBox::ReadKeyFont = "Font";

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

	BehaviorMessageBox::BehaviorMessageBox()
		: BehaviorInteractable(cBehaviorMessageBox)
		, fontName_("Happy_Monkey/HappyMonkey-Regular.ttf")
		, messages_()
		, onMessageEndStr_()
		, onMessageEnd_(nullptr)
		, childText_(nullptr)
		, messageIndex_(0)
        , onMessageEndCustom_(false)
	{
	}

	BehaviorMessageBox::BehaviorMessageBox(const BehaviorMessageBox& other)
		: BehaviorInteractable(other)
		, fontName_(other.fontName_)
		, messages_(other.messages_)
		, onMessageEndStr_(other.onMessageEndStr_)
		, onMessageEnd_(other.onMessageEnd_)
		, childText_(nullptr)
		, messageIndex_(other.messageIndex_)
        , onMessageEndCustom_(other.onMessageEndCustom_)
	{
	}

	//--------------------------------------------------------------------------

	BehaviorMessageBox::~BehaviorMessageBox()
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

	BehaviorMessageBox* BehaviorMessageBox::Clone() const
	{
		return new BehaviorMessageBox(*this);
	}

	void BehaviorMessageBox::Read(StreamIn& stream)
	{
		if (stream.StreamHas("Messages"))
		{
			stream.ReadVector("Messages", messages_);
		}
		//stream.TryReadValue(ReadKeyMessages, messages_);
		stream.TryReadValue(ReadKeyOnMessagesEnd, onMessageEndStr_);
		stream.TryReadValue(ReadKeyFont, fontName_);
	}

	void BehaviorMessageBox::Write(StreamOut& stream)
	{
		stream.WriteArrayStart(ReadKeyMessages);
		for (const auto& message : messages_)
		{
			stream.WriteValue(message);
		}
		stream.WriteArrayEnd();
		stream.WriteValue(ReadKeyOnMessagesEnd, onMessageEndStr_);
		stream.WriteValue(ReadKeyFont, fontName_);
	}

    void BehaviorMessageBox::SetFinishCallback(std::function<void()> func)
    {
        onMessageEndCustom_ = true;
        onMessageEnd_ = func;
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

	void BehaviorMessageBox::OnInit()
	{
		switch (GetStateCurrent())
		{
		case cBehaviorMessageBoxIdle:
			break;
		}
	}

	void BehaviorMessageBox::OnUpdate(float dt)
	{
		UNREFERENCED_PARAMETER(dt);
		switch (GetStateCurrent())
		{
		case cBehaviorMessageBoxIdle:
			break;
		}
	}

	void BehaviorMessageBox::OnExit()
	{
		switch (GetStateCurrent())
		{
		case cBehaviorMessageBoxInvalid:
		{
			Entity* child = new Entity("MessageBoxText");
			GetParent()->GetContainer()->Add(child);
			Transform* childTfm = new Transform();
			child->ComponentAdd(childTfm);
			childTfm->SetParentTransform(GetParent()->Has(Transform));
			childText_ = new SpriteText();
			child->ComponentAdd(childText_);
			Sprite* sprite = EntityHas(Sprite);
			if (sprite != nullptr)
			{
				childText_->SetZLayer(sprite->GetZlayer() + 1);
			}
			childText_->SetText(messages_.empty() ? "TEXT" : messages_.front());
			childText_->SetFont(fontName_);
		}
			break;
		case cBehaviorMessageBoxIdle:
			break;
		}
	}

	void BehaviorMessageBox::OnHoverStart()
	{
	}

	void BehaviorMessageBox::OnHoverEnd()
	{
	}

	void BehaviorMessageBox::OnClickStart()
	{
	}

	void BehaviorMessageBox::OnClickEnd(bool contained)
	{
		if (contained)
		{
			if (messageIndex_ < (int)messages_.size() - 1)
			{
				++messageIndex_;
				const string_t& msg = messages_[(unsigned)messageIndex_];
				childText_->SetText(msg);

			}
			else
			{
				if (onMessageEnd_ == nullptr)
				{
                    if (onMessageEndCustom_)
                    {
                        return;
                    }
					UpdateMessageEndFunc();
				}
				onMessageEnd_();
			}
		}
	}

	void BehaviorMessageBox::UpdateMessageEndFunc()
	{
		if (onMessageEndStr_ == "Destroy")
		{
			onMessageEnd_ = [this]() { GetParent()->Destroy(); };
		}
		else if (onMessageEndStr_.starts_with("ChangeScene"))
		{
			string_view_t args{ onMessageEndStr_.c_str() + _countof("ChangeScene") - 1 };
			if (args[0] == ':')
			{
				onMessageEnd_ = [this]() { SceneSystem::SetNext(onMessageEndStr_.substr(_countof("ChangeScene:") - 1)); };
			}
			else if (args[0] == '@')
			{
				unsigned int index;
				unsigned int colon = (unsigned)args.find(':');
				std::from_chars(args.data() + 1, args.data() + colon, index);
				onMessageEnd_ = [this, colon, index]() { SceneSystem::SetNext(onMessageEndStr_.substr(colon + _countof("ChangeScene:") - 1), index); };
			}
		}
        else if (onMessageEndStr_.starts_with("TransitionScene"))
        {
            string_view_t args{ onMessageEndStr_.c_str() + _countof("TransitionScene") - 1 };
            if (args[0] == ':')
            {
                onMessageEnd_ = [this]() { TurnSystem::TransitionScene(onMessageEndStr_.substr(_countof("TransitionScene:") - 1)); };
            }
            else if (args[0] == '@')
            {
                unsigned int index;
                unsigned int colon = (unsigned)args.find(':');
                std::from_chars(args.data() + 1, args.data() + colon, index);
                onMessageEnd_ = [this, colon, index]() { TurnSystem::TransitionScene(onMessageEndStr_.substr(colon + _countof("TransitionScene:") - 1), index); };
            }
        }
        else if (onMessageEndStr_.starts_with("ToggleFullscreen"))
        {
            onMessageEnd_ = []() {(PlatformSystem::SetFullscreen(!PlatformSystem::GetFullscreen())); };
        }
		else
		{
			onMessageEnd_ = [this]() { GetParent()->Destroy(); };
		}
	}

#pragma endregion Private Functions

}	// namespace
