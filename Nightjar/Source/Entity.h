//------------------------------------------------------------------------------
//
// File Name:	 Entity.h
// Author(s):	 Doug Schilling (dschilling), Connor Lariviere (connor.lariviere)
// Course:       GAM200F25
// Project:      Card Critters
// Purpose:      Class representing an Entity.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Component.h"

//------------------------------------------------------------------------------
// Preprocessor Directives:
//------------------------------------------------------------------------------

#define RENDERTIMER_ENTITIES

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

    // Typedefs:
    typedef LibraryStatic<Entity> EntityLibrary;

    // Class Definition:
    class Entity : public NamedObject 
    {
        friend class ImGuiSystem;
        friend class EntityContainer;
        friend class Scene;

        // Public Constants, Enums, and Structs:
    public:
        DECLARE_READ_KEY(Entity);
        DECLARE_READ_DIR(DIR_DATA"Entities");

        // Constructors/Destructors:
    public:
        Entity();
        Entity(string_view_arg name);
        Entity(const Entity& other);
        Entity(Entity&& other) noexcept;
        Entity& operator=(const Entity& other) = delete;

        ~Entity(void);

        // Public Static Functions:
    public:
        /// <summary>
        /// 
        /// </summary>
        /// <param name="stream"></param>
        /// <returns></returns>
        static Entity* TryRead(StreamIn& stream);

        // Public Functions:
    public:
        /// <summary>
        /// 
        /// </summary>
        /// <param name="destroyChildren"></param>
        void Destroy(bool destroyChildren = true);

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        bool IsDestroyed() const;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="other"></param>
        void ComponentAdd(Component* other);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="other"></param>
        void ComponentRemove(Component* other);

#define Has(type) GetComponent<type>(Component::TypeEnum::type)
        /// <summary>
        /// Type safe method for accessing the components.
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="typeId"></param>
        /// <returns></returns>
        template<typename T>
        T* GetComponent(Component::TypeEnum typeId) const;

#define HasSub(type, subtype) GetComponentSub<type, subtype>(Component::TypeEnum::type, type::c##subtype)
        /// <summary>
        /// Type safe method for accessing the components.
        /// </summary>
        /// <typeparam name="type"></typeparam>
        /// <typeparam name="subtype"></typeparam>
        /// <param name="typeId"></param>
        /// <param name="subtypeId"></param>
        /// <returns></returns>
        template<typename type, typename subtype>
        subtype* GetComponentSub(Component::TypeEnum typeId, type::SubTypeEnum subtypeId) const;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="type"></param>
        /// <returns></returns>
        Component* ComponentGet(Component::TypeEnum type) const;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="name"></param>
        /// <returns></returns>
        Entity* FindEntity(string_view_arg name) const;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="stream"></param>
        void Read(StreamIn& stream);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="filepath"></param>
        void Read(string_view_arg filepath);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="stream"></param>
        void Write(StreamOut& stream);

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        Entity* Clone() const;

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        EntityContainer* GetContainer() const;

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        Scene* GetScene() const;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="dt"></param>
        void Update(float dt);

        /// <summary>
        /// 
        /// </summary>
        void Render() const;

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        bool GetWriteable() const;

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        const vector_t<Component*>& GetComponentList() const;

        //
        void DisplayIMGUI();

        // Public Event Handlers
    public:

        // Private Constants, Enums, and Structs:
    private:
        //Used to sort components using their type Id.
        struct ComponentSorter
        {
            bool operator()(Component* left, Component* right) const;
        };

        static const StreamKey ReadKeyArchetype, ReadKeyWritable, ReadKeyComponents;

        // Private Static Functions:
    private:

        // Private Functions:
    private:
        /// <summary>
        /// 
        /// </summary>
        void DestroyComponents();

        /// <summary>
        /// 
        /// </summary>
        /// <param name="archetype"></param>
        void CopyFromArchetype(const Entity* archetype);

        // Private Static Variables:
    private:

        // Private Variables:
    private:
        vector_t<Component*> components_;
        EntityContainer* parentContainer_;
        string_t archetypeName_;
        bool isDestroyed_;
        bool writeable_;
    };

}	// namespace

#include "Entity.inl"
#include "Component.inl"
