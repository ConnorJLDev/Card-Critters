//------------------------------------------------------------------------------
//
// File Name:	TransformBase.inl
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Base transform class for handling parenting and world space.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

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

    template<class Base, class Impl>
    template<typename... Args>
        requires ((sizeof...(Args) == 0) || !std::conjunction_v<std::is_base_of<std::decay_t<Args>, Impl>...>)
    TransformBase<Base, Impl>::TransformBase(Args&&... args)
        : Base(std::forward<Args>(args)...)
        , TransformInheritance()
        , parent_(nullptr)
        , children_()
    {
    }

    template<class Base, class Impl>
    TransformBase<Base, Impl>::TransformBase(const TransformBase& other)
        : Base(other)
        , TransformInheritance(other)
        , parent_(nullptr)
        , children_()
    {
        SetParentTransform(other.parent_);
    }

    template<class Base, class Impl>
    TransformBase<Base, Impl>::TransformBase(TransformBase&& other) noexcept
        : Base(std::move(other))
        , TransformInheritance(std::move(other))
        , parent_(std::exchange(other.parent_, nullptr))
        , children_(std::move(other.children_))
    {
        Inherit(&other);
    }

    template<class Base, class Impl>
    TransformBase<Base, Impl>& TransformBase<Base, Impl>::operator=(const TransformBase<Base, Impl>& other)
    {
        if constexpr (std::is_copy_assignable_v<Base>)
        {
            Base::operator=(other);
        }
        TransformInheritance::operator=(other);
        SetParentTransform(other.parent_);
        return *this;
    }

    template<class Base, class Impl>
    TransformBase<Base, Impl>& TransformBase<Base, Impl>::operator=(TransformBase<Base, Impl>&& other) noexcept
    {
        if (this != &other)
        {
            if constexpr (std::is_move_assignable_v<Base>)
            {
                Base::operator=(std::move(other));
            }
            TransformInheritance::operator=(other);
            Dissolve();
            parent_ = std::exchange(other.parent_, nullptr);
            children_ = std::move(other.children_);
            Inherit(&other);
        }
        return *this;
    }

    //--------------------------------------------------------------------------

    template<class Base, class Impl>
    TransformBase<Base, Impl>::~TransformBase(void)
    {
        Dissolve();
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

    template<class Base, class Impl>
    void TransformBase<Base, Impl>::SetParentTransform(Impl* parent)
    {
        if (parent_ != nullptr)
        {
            parent_->RemoveChild(static_cast<Impl*>(this));
        }
        parent_ = parent;
        TransformInheritance::parent_ = parent;
        if (parent_ != nullptr)
        {
            parent_->AddChild(static_cast<Impl*>(this));
        }
    }

    template<class Base, class Impl>
    Impl* TransformBase<Base, Impl>::GetParentTransform() const
    {
        return parent_;
    }

    template<class Base, class Impl>
    void TransformBase<Base, Impl>::AddChild(Impl* child)
    {
        assert(FindChildIter(child) == children_.end());
        children_.push_back(child);
    }

    template<class Base, class Impl>
    void TransformBase<Base, Impl>::RemoveChild(Impl* child)
    {
        ChildConstIter_t childIter = FindChildIter(child);
        if (childIter != children_.end())
        {
            children_.erase(childIter);
            child->parent_ = parent_;
        }
    }

    template<class Base, class Impl>
    const TransformBase<Base, Impl>::Children_t& TransformBase<Base, Impl>::GetChildren() const
    {
        return children_;
    }

    template<class Base, class Impl>
    void TransformBase<Base, Impl>::SetTranslation3D(const Vec3& translation)
    {
        TransformComponents::SetTranslation3D(translation);
        SetChildrenFinalDirty();
    }

    template<class Base, class Impl>
    void TransformBase<Base, Impl>::SetTranslation2D(const Vec2& translation)
    {
        TransformComponents::SetTranslation2D(translation);
        SetChildrenFinalDirty();
    }

    template<class Base, class Impl>
    void TransformBase<Base, Impl>::SetRotation3D(const Vec3& rotation)
    {
        TransformComponents::SetRotation3D(rotation);
        SetChildrenFinalDirty();
    }

    template<class Base, class Impl>
    void TransformBase<Base, Impl>::SetRotation2D(const float& rotation)
    {
        TransformComponents::SetRotation2D(rotation);
        SetChildrenFinalDirty();
    }

    template<class Base, class Impl>
    void TransformBase<Base, Impl>::SetRotationQuaternion(const Quat& rotation)
    {
        TransformComponents::SetRotationQuaternion(rotation);
        SetChildrenFinalDirty();
    }

    template<class Base, class Impl>
    void TransformBase<Base, Impl>::SetScale3D(const Vec3& scale)
    {
        TransformComponents::SetScale3D(scale);
        SetChildrenFinalDirty();
    }

    template<class Base, class Impl>
    void TransformBase<Base, Impl>::SetScale2D(const Vec2& scale)
    {
        TransformComponents::SetScale2D(scale);
        SetChildrenFinalDirty();
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

    template<class Base, class Impl>
    TransformBase<Base, Impl>::ChildConstIter_t TransformBase<Base, Impl>::FindChildIter(const Impl* child) const
    {
        ChildConstIter_t childIter = std::find(children_.begin(), children_.end(), child);
        return childIter;
    }

    template<class Base, class Impl>
    TransformBase<Base, Impl>::ChildIter_t TransformBase<Base, Impl>::FindChildIter(const Impl* child)
    {
        ChildIter_t childIter = std::find(children_.begin(), children_.end(), child);
        return childIter;
    }

    template<class Base, class Impl>
    void TransformBase<Base, Impl>::SetChildrenFinalDirty(void)
    {
        if (finalDirty_)
        {
            return;
        }
        finalDirty_ = true;
        for (auto* child : children_)
        {
            child->SetChildrenFinalDirty();
        }
    }

    template<class Base, class Impl>
    void TransformBase<Base, Impl>::Dissolve(void) noexcept
    {
        // We could call SetParent on each child, but that would incur
        // repeated lookups when removing the child from ourselves, so the 
        // process is unwrapped directly in each loop here.
        if (parent_ != nullptr)
        {
            parent_->RemoveChild(static_cast<Impl*>(this));
            for (auto* child : children_)
            {
                child->parent_ = parent_;
                parent_->AddChild(static_cast<Impl*>(child));
                child->TransformInheritance::parent_ = parent_;
                child->SetChildrenFinalDirty();
            }
            parent_ = nullptr;
        }
        else
        {
            for (auto* child : children_)
            {
                child->parent_ = nullptr;
                child->TransformInheritance::parent_ = nullptr;
                child->SetChildrenFinalDirty();
            }
        }
        children_.clear();
    }

#pragma endregion Protected Functions

    //--------------------------------------------------------------------------
    // Private Functions:
    //--------------------------------------------------------------------------

#pragma region Private Functions

    template<class Base, class Impl>
    void TransformBase<Base, Impl>::Inherit(TransformBase<Base, Impl>* other) noexcept
    {
        if (parent_ != nullptr)
        {
            ChildIter_t childIter = parent_->FindChildIter(static_cast<Impl*>(other));
            *childIter = static_cast<Impl*>(this);
        }
        for (auto* child : children_)
        {
            child->parent_ = static_cast<Impl*>(this);
            child->TransformInheritance::parent_ = this;
        }
    }

#pragma endregion Private Functions

}	// namespace
