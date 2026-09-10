//------------------------------------------------------------------------------
//
// File Name:	TransformBase.h
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

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    // Forward Declarations:

    // Typedefs:

    // Struct containing all of the fundamental transformation values.
    struct TransformComponents
    {
        enum class OptimizeFlags : unsigned char
        {
            None = 0,
            Translation = 1 << 0,
            Rotation = 1 << 1,
            Scale = 1 << 2,
            RotationScale = Rotation | Scale,
        };

        TransformComponents(void);

        TransformComponents(const TransformComponents& other);

        TransformComponents(TransformComponents&& other) noexcept;

        TransformComponents& operator=(const TransformComponents& other);

        ~TransformComponents(void);

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        const Vec3& GetTranslation3D() const;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="translation"></param>
        void SetTranslation3D(const Vec3& translation);

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        const Vec2& GetTranslation2D() const;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="translation"></param>
        void SetTranslation2D(const Vec2& translation);

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        const Vec3& GetRotation3D() const;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="rotation"></param>
        void SetRotation3D(const Vec3& rotation);

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        const float& GetRotation2D() const;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="rotation"></param>
        void SetRotation2D(const float& rotation);

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        const Quat& GetRotationQuaternion() const;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="rotation"></param>
        void SetRotationQuaternion(const Quat& rotation);

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        const Vec3& GetScale3D() const;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="scale"></param>
        void SetScale3D(const Vec3& scale);

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        const Vec2& GetScale2D() const;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="scale"></param>
        void SetScale2D(const Vec2& scale);

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        Mat4 GetTranslationMatrix() const;

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        Mat4 GetRotationMatrix() const;

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        Mat4 GetScaleMatrix() const;

        //
        void DisplayIMGUI2D();

        //
        void DisplayIMGUI3D();

        typedef unsigned char DirtyFlags_t;

        //
        enum class DirtyFlags : DirtyFlags_t
        {
            None = 0,
            Translation = 1 << 0,
            Rotation = 1 << 1,
            Scale = 1 << 2,
            All = (DirtyFlags_t)-1,
        };

        ENUM_DECLARE_OPS(DirtyFlags);

        //
        static const StreamKey ReadKeyParent, ReadKeyTranslation, ReadKeyRotation, ReadKeyScale;

        static const Mat4 Identity;

        union
        {
            // The transform's 2D translation.
            Vec2 translation2_;
            // The transform's 3D translation.
            Vec3 translation3_;
        };
        union
        {
            struct
            {
                Vec2 _;
                // The transform's 2D rotation.
                float rotation2_;
            };
            // The transform's 3D rotation as XYZ Euler angles.
            Vec3 rotation3_;
            // The transform's 3D rotation as a quaternion.
            Quat rotation4_;
        };
        union
        {
            // The transform's 2D scale.
            Vec2 scale2_;
            // The transform's 3D scale.
            Vec3 scale3_;
        };
        //
        bool useQuaternion_;
        //
        bool use2D_;
        // Flags for if the transform has been modified since the matrix was last calculated.
        DirtyFlags dirtyFlags_;
    };

    // Struct that contains and updates the matrices of a transform.
    struct TransformInheritance : public TransformComponents
    {
        TransformInheritance();

        TransformInheritance(const TransformInheritance& other);

        TransformInheritance(TransformInheritance&& other) noexcept;

        TransformInheritance& operator=(const TransformInheritance& other);

        ~TransformInheritance(void);

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        OptimizeFlags GetOptimizeMode() const;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="flags"></param>
        void SetOptimizeMode(OptimizeFlags flags);

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        const Mat4& GetMatrixLocal();

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        const Mat4& GetMatrixFinal();

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        const Mat4& GetParentMatrix() const;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="parent"></param>
        void SetParentMatrix(TransformInheritance* parent);

    protected:
        /// <summary>
        /// Checks if the bone matrix is dirty.
        /// </summary>
        /// <returns>True if the matrix is dirty, false otherwise.</returns>
        bool IsDirty() const;

        // Parent to inherit transforms from.
        TransformInheritance* parent_;
        // Intermediate matrix for storing infrequently updated values.
        Mat4 mtxIntermediate_;
        // The local transform matrix (ignores parent).
        Mat4 mtxLocal_;
        // The final transform matrix (includes parent).
        Mat4 mtxFinal_;
        // Flag for if the transform's parent has been modified since the matrix was last calculated.
        bool finalDirty_;
        // Flags for what to use mtxIntermediate_ for.
        OptimizeFlags optimizeFlags_;
    };

    // Class that manages transform parenting and child management.
    template<class Base, class Impl>
    class TransformBase : public Base, public TransformInheritance
    {
        // Public Constants, Enums, and Structs:
    public:

        using Children_t = vector_t<Impl*>;
        using ChildIter_t = Children_t::iterator;
        using ChildConstIter_t = Children_t::const_iterator;

        // Constructors/Destructors:
    public:
        template<typename... Args>
            requires ((sizeof...(Args) == 0) || !std::conjunction_v<std::is_base_of<std::decay_t<Args>, Impl>...>)
        TransformBase(Args&&... args);

        TransformBase(const TransformBase& other);

        TransformBase(TransformBase&& other) noexcept;

        TransformBase& operator=(const TransformBase& other);

        TransformBase& operator=(TransformBase&& other) noexcept;

        ~TransformBase(void);

        // Public Static Functions:
    public:

        // Public Functions:
    public:
        //
        void SetParentTransform(Impl* parent);

        //
        Impl* GetParentTransform() const;

        //
        void AddChild(Impl* child);

        //
        void RemoveChild(Impl* child);

        //
        const Children_t& GetChildren() const;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="translation"></param>
        void SetTranslation3D(const Vec3& translation);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="translation"></param>
        void SetTranslation2D(const Vec2& translation);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="rotation"></param>
        void SetRotation3D(const Vec3& rotation);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="rotation"></param>
        void SetRotation2D(const float& rotation);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="rotation"></param>
        void SetRotationQuaternion(const Quat& rotation);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="scale"></param>
        void SetScale3D(const Vec3& scale);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="scale"></param>
        void SetScale2D(const Vec2& scale);

        // Public Event Handlers
    public:

        // Protected Constants, Enums, and Structs:
    protected:

        // Protected Static Functions:
    protected:

        // Protected Functions:
    protected:
        /// <summary>
        /// Retrieves the iterator element of a child to find in the list of children.
        /// </summary>
        /// <param name="child">The child to search for.</param>
        /// <returns>The iterator representing where the child is in the list of children.</returns>
        ChildConstIter_t FindChildIter(const Impl* child) const;
        
        /// <summary>
        /// Retrieves the iterator element of a child to find in the list of children.
        /// </summary>
        /// <param name="child">The child to search for.</param>
        /// <returns>The iterator representing where the child is in the list of children.</returns>
        ChildIter_t FindChildIter(const Impl* child);

        /// <summary>
        /// Marks this transform and its children as having a dirty final matrix.
        /// </summary>
        void SetChildrenFinalDirty(void);

        /// <summary>
        /// Dissolves this transform, removing itself from its parent's children and setting all of its childrens' parent to it's former parent.
        /// </summary>
        void Dissolve(void) noexcept;

        // Private Functions:
    private:
        /// <summary>
        /// Inherits and replaces the hierarchy data from another transform. This is only intended for use within move construction/assignment.
        /// </summary>
        /// <param name="other"></param>
        void Inherit(TransformBase* other) noexcept;

        // Protected Static Variables:
    protected:

        // Protected Variables:
    protected:
        // The transform's parent.
        Impl* parent_;
        // The transform's children.
        Children_t children_;
    };

}	// namespace
