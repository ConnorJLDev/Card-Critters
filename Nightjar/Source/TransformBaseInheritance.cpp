//------------------------------------------------------------------------------
//
// File Name:	TransformBaseInheritance.cpp
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

    TransformInheritance::TransformInheritance()
        : TransformComponents()
        , parent_()
        , mtxIntermediate_()
        , mtxLocal_()
        , mtxFinal_()
        , finalDirty_(true)
        , optimizeFlags_(OptimizeFlags::Rotation)
    {
    }

    TransformInheritance::TransformInheritance(const TransformInheritance& other)
        : TransformComponents(other)
        , parent_(other.parent_)
        , mtxIntermediate_(other.mtxIntermediate_)
        , mtxLocal_(other.mtxLocal_)
        , mtxFinal_(other.mtxFinal_)
        , finalDirty_(true)
        , optimizeFlags_(other.optimizeFlags_)
    {
    }

    TransformInheritance::TransformInheritance(TransformInheritance&& other) noexcept
        : TransformComponents(std::move(other))
        , parent_(std::move(other.parent_))
        , mtxIntermediate_(std::move(other.mtxIntermediate_))
        , mtxLocal_(std::move(other.mtxLocal_))
        , mtxFinal_(std::move(other.mtxFinal_))
        , finalDirty_(std::move(other.finalDirty_))
        , optimizeFlags_(std::move(other.optimizeFlags_))
    {
    }

    TransformInheritance& TransformInheritance::operator=(const TransformInheritance& other)
    {
        TransformComponents::operator=(other);
        parent_ = other.parent_;
        mtxIntermediate_ = other.mtxIntermediate_;
        mtxLocal_ = other.mtxLocal_;
        mtxFinal_ = other.mtxFinal_;
        finalDirty_ = other.finalDirty_;
        optimizeFlags_ = other.optimizeFlags_;
        return *this;
    }

    //--------------------------------------------------------------------------

    TransformInheritance::~TransformInheritance(void)
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

    TransformInheritance::OptimizeFlags TransformInheritance::GetOptimizeMode() const
    {
        return optimizeFlags_;
    }

    void TransformInheritance::SetOptimizeMode(OptimizeFlags flags)
    {
        optimizeFlags_ = flags;
    }

    const Mat4& TransformInheritance::GetMatrixLocal()
    {
        if (!IsDirty())
        {
            return mtxLocal_;
        }

        Mat4 mtxPos, mtxRot, mtxScl;
        switch (optimizeFlags_)
        {
        case OptimizeFlags::Translation:
            if ((bool)(dirtyFlags_ & DirtyFlags::Translation))
            {
                mtxIntermediate_ = GetTranslationMatrix();
            }
            mtxRot = GetRotationMatrix();
            mtxScl = GetScaleMatrix();
            mtxLocal_ = mtxIntermediate_ * mtxRot * mtxScl;
            break;
        case OptimizeFlags::Rotation:
            if ((bool)(dirtyFlags_ & DirtyFlags::Rotation))
            {
                mtxIntermediate_ = GetRotationMatrix();
            }
            mtxPos = GetTranslationMatrix();
            mtxScl = GetScaleMatrix();
            mtxLocal_ = mtxPos * mtxIntermediate_ * mtxScl;
            break;
        case OptimizeFlags::Scale:
            if ((bool)(dirtyFlags_ & DirtyFlags::Scale))
            {
                mtxIntermediate_ = GetScaleMatrix();
            }
            mtxPos = GetTranslationMatrix();
            mtxRot = GetRotationMatrix();
            mtxLocal_ = mtxPos * mtxRot * mtxIntermediate_;
            break;
        case OptimizeFlags::RotationScale:
            if ((bool)(dirtyFlags_ & (DirtyFlags::Rotation | DirtyFlags::Scale)))
            {
                mtxIntermediate_ = GetRotationMatrix() * GetScaleMatrix();
            }
            mtxPos = GetTranslationMatrix();
            mtxLocal_ = mtxPos * mtxIntermediate_;
            break;
        default:
            mtxPos = GetTranslationMatrix();
            mtxRot = GetRotationMatrix();
            mtxScl = GetScaleMatrix();
            mtxLocal_ = mtxPos * mtxRot * mtxScl;
            break;
        }

        dirtyFlags_ = DirtyFlags::None;

        finalDirty_ = true;

        return mtxLocal_;
    }

    const Mat4& TransformInheritance::GetMatrixFinal()
    {
        const Mat4& local = GetMatrixLocal();

        if (finalDirty_)
        {
            mtxFinal_ = parent_ != nullptr ? parent_->GetMatrixFinal() * local : local;
            finalDirty_ = false;
        }

        return mtxFinal_;
    }

    const Mat4& TransformInheritance::GetParentMatrix() const
    {
        return parent_ != nullptr ? parent_->GetMatrixFinal() : Identity;
    }

    void TransformInheritance::SetParentMatrix(TransformInheritance* parent)
    {
        parent_ = parent;
        finalDirty_ = true;
    }

    bool TransformInheritance::IsDirty() const
    {
        return finalDirty_ || static_cast<bool>(dirtyFlags_);
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
