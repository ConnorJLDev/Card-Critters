//------------------------------------------------------------------------------
//
// File Name:	TransformBaseComponents.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Base transform class for handling parenting and world space.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "TransformBase.h"

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
    // Protected Constants:
    //--------------------------------------------------------------------------

    constexpr StreamKey
        TransformComponents::ReadKeyParent = "Parent",
        TransformComponents::ReadKeyTranslation = "Translation",
        TransformComponents::ReadKeyRotation = "Rotation",
        TransformComponents::ReadKeyScale = "Scale";

    constexpr Mat4 TransformComponents::Identity = glm::identity<Mat4>();

    //--------------------------------------------------------------------------
    // Protected Static Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Protected Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Constructors/Destructors:
    //--------------------------------------------------------------------------

#pragma region Constructors

    TransformComponents::TransformComponents()
        : translation3_(0, 0, 0)
        , rotation4_(0, 0, 0, 0)
        , scale3_(1, 1, 1)
        , useQuaternion_(false)
        , use2D_(false)
        , dirtyFlags_(DirtyFlags::All)
    {
    }

    TransformComponents::TransformComponents(const TransformComponents& other)
        : translation3_(other.translation3_)
        , rotation4_(other.rotation4_)
        , scale3_(other.scale3_)
        , useQuaternion_(other.useQuaternion_)
        , use2D_(other.use2D_)
        , dirtyFlags_(DirtyFlags::All)
    {
    }

    TransformComponents::TransformComponents(TransformComponents&& other) noexcept
        : translation3_(std::move(other.translation3_))
        , rotation4_(std::move(other.rotation4_))
        , scale3_(std::move(other.scale3_))
        , useQuaternion_(std::move(other.useQuaternion_))
        , use2D_(std::move(other.use2D_))
        , dirtyFlags_(std::move(other.dirtyFlags_))
    {
    }

    TransformComponents& TransformComponents::operator=(const TransformComponents& other)
    {
        translation3_ = other.translation3_;
        rotation4_ = other.rotation4_;
        scale3_ = other.scale3_;
        useQuaternion_ = other.useQuaternion_;
        use2D_ = other.use2D_;
        dirtyFlags_ = DirtyFlags::All;
        return *this;
    }

    //--------------------------------------------------------------------------

    TransformComponents::~TransformComponents(void)
    {
    }

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

    ENUM_IMPLEMENT_OPS(TransformComponents::DirtyFlags);

#pragma endregion Public Static Functions

    //--------------------------------------------------------------------------
    // Public Functions:
    //--------------------------------------------------------------------------

#pragma region Public Functions

    const Vec3& TransformComponents::GetTranslation3D() const
    {
        return translation3_;
    }

    void TransformComponents::SetTranslation3D(const Vec3& translation)
    {
        translation3_ = translation;
        dirtyFlags_ |= DirtyFlags::Translation;
    }

    const Vec2& TransformComponents::GetTranslation2D() const
    {
        return translation2_;
    }

    void TransformComponents::SetTranslation2D(const Vec2& translation)
    {
        translation2_ = translation;
        dirtyFlags_ |= DirtyFlags::Translation;
    }

    const Vec3& TransformComponents::GetRotation3D() const
    {
        return rotation3_;
    }

    void TransformComponents::SetRotation3D(const Vec3& rotation)
    {
        rotation3_ = rotation;
        dirtyFlags_ |= DirtyFlags::Rotation;
    }

    const float& TransformComponents::GetRotation2D() const
    {
        return rotation2_;
    }

    void TransformComponents::SetRotation2D(const float& rotation)
    {
        rotation2_ = rotation;
        dirtyFlags_ |= DirtyFlags::Rotation;
    }

    const Quat& TransformComponents::GetRotationQuaternion() const
    {
        return rotation4_;
    }

    void TransformComponents::SetRotationQuaternion(const Quat& rotation)
    {
        rotation4_ = rotation;
        dirtyFlags_ |= DirtyFlags::Rotation;
    }

    const Vec3& TransformComponents::GetScale3D() const
    {
        return scale3_;
    }

    void TransformComponents::SetScale3D(const Vec3& scale)
    {
        scale3_ = scale;
        dirtyFlags_ |= DirtyFlags::Scale;
    }

    const Vec2& TransformComponents::GetScale2D() const
    {
        return scale2_;
    }

    void TransformComponents::SetScale2D(const Vec2& scale)
    {
        scale2_ = scale;
        dirtyFlags_ |= DirtyFlags::Scale;
    }
    
    Mat4 TransformComponents::GetTranslationMatrix() const
    {
        return glm::translate(translation3_);
    }

    Mat4 TransformComponents::GetRotationMatrix() const
    {
        return use2D_ ? glm::toMat4(glm::angleAxis(rotation2_, Vec3(0, 0, -1))) : glm::eulerAngleXYZ(rotation3_.x, rotation3_.y, rotation3_.z);
    }

    Mat4 TransformComponents::GetScaleMatrix() const
    {
        return glm::scale(scale3_);
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

#pragma endregion Protected Functions

}	// namespace
