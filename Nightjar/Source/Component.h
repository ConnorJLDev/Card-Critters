//------------------------------------------------------------------------------
//
// File Name:	Component.h
// Author(s):	Jason Nguyen (jason.nguyen)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Component class for entities.
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
// Preprocessor Directives:
//------------------------------------------------------------------------------

#define RENDERTIMER_COMPONENTS

// Internal definition for common component declarations for use with other
// compiler switches.
#define INTERNAL_COMPONENT_COMMON(componentName)\
    DECLARE_READ_KEY(componentName);\
    string_view_t GetReadKey(void) override { return ReadKey; }\
    componentName& operator=(const componentName& other) = delete;

#ifdef _EDITOR

// Definition for common declarations that are required across all component
// declarations.
#define COMPONENT_COMMON(componentName)\
    INTERNAL_COMPONENT_COMMON(componentName)\
    void DisplayIMGUI(void) override;

#else

// Definition for common declarations that are required across all component
// declarations.
#define COMPONENT_COMMON(componentName)\
    INTERNAL_COMPONENT_COMMON(componentName)

#endif

// Definition for consistent "final" implementation across all component
// declarations.
#define COMPONENT_FINAL final

// Convenience macro for getting another component on the same entity as this
// component.
#define EntityHas(type) _EntityHas<type>(Component::TypeEnum::type)

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    // Forward Declarations:
    class Entity;
    class Scene;

    // Typedefs:

    // Class Definition:
    class Component : public Object
    {
        friend class Entity;

        // Public Constants, Enums, and Structs:
    public:
        // Bit offset for major component category indices.
        static constexpr int TypeMajorOffset = 10;

        // Bit offset for minor component category indices.
        static constexpr int TypeMinorOffset = 6;

        // Represents the type of a given component.
        enum class TypeEnum : uint16_t
        {
            SIMULATION			=  (1 << TypeMajorOffset),
            Transform,
            Physics,
            Collider            =  (1 << TypeMajorOffset) + (1 << TypeMinorOffset),
#define REGISTER_COLLIDER(colliderName) colliderName,
#include "Colliders.def"
#undef REGISTER_COLLIDER
            GRAPHICS			=  (2 << TypeMajorOffset),
            Sprite,
            SpriteText,
            Trail,
            Model,
            Camera,
            Light,
            ParticleEmitter,
            AudioEmitter		=  (3 << TypeMajorOffset),
            Animation			=  (4 << TypeMajorOffset),
            BEHAVIORS			=  (5 << TypeMajorOffset),
            Behavior,
#define REGISTER_BEHAVIOR(behaviorName) behaviorName,
#include "Behaviors.def"
#undef REGISTER_BEHAVIOR
        };

        // Constructors/Destructors:
    protected:
        Component(void) = delete;

        /// <summary>
        /// Constructs the component.
        /// </summary>
        /// <param name="type">The type of the component.</param>
        Component(TypeEnum type);

        Component(const Component& other);

        Component(Component&& other) noexcept;

        Component& operator=(const Component& other) = delete;

        Component& operator=(Component&& other) noexcept;

    public:
        virtual ~Component(void) = 0;

        // Public Static Functions:
    public:

        // Public Functions:
    public:
        /// <summary>
        /// Clones the component.
        /// </summary>
        /// <returns>A pointer to the newly cloned component</returns>
        virtual Component* Clone(void) const = 0;

        /// <summary>
        /// Reads the stream data into the component
        /// </summary>
        /// <param name="stream">The stream to read from.</param>
        virtual void Read(StreamIn& stream);

        /// <summary>
        /// Writes the component data into the stream.
        /// </summary>
        /// <param name="stream">The stream to write into.</param>
        virtual void Write(StreamOut& stream);

#ifdef _EDITOR
        /// <summary>
        /// Displays this component's data in an IMGUI context.
        /// </summary>
        virtual void DisplayIMGUI(void) = 0;
#endif

        /// <summary>
        /// Gets the string key representing this type of component.
        /// </summary>
        /// <returns>This component's type as a string_view.</returns>
        virtual string_view_t GetReadKey(void) = 0;

        /// <summary>
        /// Updates the component.
        /// </summary>
        /// <param name="dt">Delta time, the amount of time the last frame took to finish.</param>
        virtual void Update(float dt);

        /// <summary>
        /// Renders the component.
        /// </summary>
        virtual void Render(void) const;

        /// <summary>
        /// Gets the type of the component.
        /// </summary>
        /// <returns>The type of the component.</returns>
        TypeEnum GetType(void) const;

        /// <summary>
        /// Gets the parent entity of the component.
        /// </summary>
        /// <returns>The parent entity of the component.</returns>
        Entity* GetParent(void) const;

        /// <summary>
        /// Sets the parent entity of the component.
        /// </summary>
        /// <param name="entity">The entity to set as this component's parent.</param>
        void SetEntity(Entity* entity);

        /// <summary>
        /// Convenience function for getting the name of the parent entity.
        /// </summary>
        /// <returns>The name of the entity.</returns>
        const string_t& EntityName(void) const;

        /// <summary>
        /// Convenience function for checking the name of the parent entity.
        /// </summary>
        /// <returns>True if the entity's name matches, false otherwise.</returns>
        bool EntityNamed(string_view_arg name) const;

        /// <summary>
        /// Convenience function for getting another component on the same entity as this component.
        /// </summary>
        /// <typeparam name="T">The component type.</typeparam>
        /// <param name="typeId">The ID of the component type.</param>
        /// <returns>A pointer to the component if found, or nullptr if not found.</returns>
        template <typename T>
        T* _EntityHas(TypeEnum typeId) const;

        /// <summary>
        /// Gets the scene containing the component's entity.
        /// </summary>
        /// <returns>A pointer to the scene containing the entity, or nullptr if not found.</returns>
        Scene* GetScene(void) const;

        // Public Event Handlers
    public:

        // Private Constants, Enums, and Structs:
    private:

        // Private Static Functions:
    private:

        // Protected Functions:
    protected:

        // Private Static Variables:
    private:

        // Private Variables:
    private:
        // The derived component type that this component is.
        const TypeEnum type_;
        // The parent entity of this component.
        Entity* parent_;
    };

}	// namespace
