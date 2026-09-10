//------------------------------------------------------------------------------
//
// File Name:	BehaviorSlider.cpp
// Author(s):	Andrew Lamb (andrew.lamb)
// Course:		GAM250S26
// Project:		Card Critters
// Purpose:		Behavior class for sliders
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Entity.h"
#include "Transform.h"
#include "Sprite.h"
#include "Collider.h"
#include "BehaviorSlider.h"
#include "InputSystem.h"
#include "Scene.h"
#include "Camera.h"
#include "AudioSystem.h"
#include "SpriteText.h"

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
    constexpr StreamKey BehaviorSlider::ReadFunction = "SaveValue";
    constexpr StreamKey BehaviorSlider::ReadTitle = "SliderTitle";
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

    BehaviorSlider::BehaviorSlider()
        : BehaviorInteractable(cBehaviorSlider), _minVal(-.4), _maxVal(0.4), _currVal(0.0), _interval(10.0), _scale(1.0), _storeVal(nullptr), _titleString("")
    {
    }

    BehaviorSlider::BehaviorSlider(const BehaviorSlider& other)
        : BehaviorInteractable(other)
    {
    }

    //--------------------------------------------------------------------------

    BehaviorSlider::~BehaviorSlider()
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

    BehaviorSlider* BehaviorSlider::Clone() const
    {
        return new BehaviorSlider(*this);
    }

    void BehaviorSlider::Read(StreamIn& stream)
    {
        stream.TryReadValue(ReadFunction, _storeValString);
        stream.TryReadValue(ReadTitle, _titleString);
    }

    void BehaviorSlider::Write(StreamOut& stream)
    {
        UNREFERENCED_PARAMETER(stream);
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

    void BehaviorSlider::OnInit()
    {
        Transform* parentT = GetParent()->Has(Transform);
        int parentZ = GetParent()->Has(Sprite)->GetZlayer();
        Vec2 camScale = GetParent()->GetScene()->GetCamera()->GetViewSize();
        _scale = parentT->GetScale().x / camScale.x;
        if (_knob == nullptr)
            _knob = GetOrMakeChildEntity("Knob");
        if (_titleText == nullptr)
            _titleText = GetOrMakeChildEntity("SliderTitleText");
        _titleText->Has(SpriteText)->SetText(_titleString);
        if (_valueText == nullptr)
            _valueText = GetOrMakeChildEntity("SliderValueText");
        if (_storeValString == "Music")
        {
            _storeVal = AudioSystem::SetVolumeMusic;
            _currVal = AudioSystem::GetVolumeMusic();
        }
        else if (_storeValString == "SFX")
        {
            _storeVal = AudioSystem::SetVolumeSFX;
            _currVal = AudioSystem::GetVolumeSFX();
        }
        _knob->Has(Transform)->SetTranslation(Vec2(((_currVal)) * (_maxVal - _minVal) + _minVal, .01));
        _knob->Has(Sprite)->SetZLayer(parentZ + 1);
        _titleText->Has(SpriteText)->SetZLayer(parentZ);
        _valueText->Has(SpriteText)->SetZLayer(parentZ + 2);
        FORMAT_MAKE(output, "{:.0f}%", (_currVal * 100));
        _valueText->Has(SpriteText)->SetText(output);
        switch (GetStateCurrent())
        {
        case States::cIdle:
            break;
        }
    }

    void BehaviorSlider::OnUpdate(float dt)
    {
        UNREFERENCED_PARAMETER(dt);

        
        if (_isClicked)
        {
            float xPos = (float)std::clamp(static_cast<double>(InputSystem::GetMousePos().x * (GetParent()->Has(Transform)->GetScale().x / 150.f)), _minVal, _maxVal);
            _knob->Has(Transform)->SetTranslation(Vec2(xPos, _knob->Has(Transform)->GetTranslation().y));
        }

        switch (GetStateCurrent())
        {
        case States::cIdle:
            break;
        }
    }

    void BehaviorSlider::OnExit()
    {
        switch (GetStateCurrent())
        {
        case States::cIdle:
            break;
        }
    }

    void BehaviorSlider::OnHoverStart()
    {
    }

    void BehaviorSlider::OnHoverEnd()
    {
    }

    void BehaviorSlider::OnClickStart()
    {
        _isClicked = true;
    }

    void BehaviorSlider::OnClickEnd(bool contained)
    {
        UNREFERENCED_PARAMETER(contained);
        if (!_parentX)
            _parentX = GetParent()->Has(Transform)->GetTranslation().x;
        _isClicked = false;
        /*double mouseVal = std::clamp(knob->Has(Transform)->GetTranslation().x * _scale, _minVal, _maxVal);
        _currVal = mouseVal / _maxVal;
        _currVal = _interval * round(_currVal / _interval);
        if (_storeVal != nullptr)
            _storeVal(_currVal);*/
        _currVal = std::clamp(((_knob->Has(Transform)->GetTranslation().x - _minVal) / (_maxVal - _minVal)), 0.0, 1.0);
        _currVal = (_interval * (int)(_currVal * 100 / _interval)) / 100;
        _knob->Has(Transform)->SetTranslation(Vec2(((_currVal)) * (_maxVal - _minVal) + _minVal, 0.01));
        if (_storeVal != nullptr)
            _storeVal(static_cast<float>(_currVal));
        FORMAT_MAKE(output, "{:.0f}%", (_currVal * 100));
        _valueText->Has(SpriteText)->SetText(output);
        TraceInfo("Slider value: {}", _currVal);
    }

    void BehaviorSlider::DisplayIMGUI()
    {

    }

#pragma endregion Private Functions

}	// namespace
