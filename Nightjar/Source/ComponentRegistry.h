//------------------------------------------------------------------------------
//
// File Name:	ComponentRegistry.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class for containing all of the different types of components.
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
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	// Forward Declarations:
	class Component;

	// Typedefs:

	// Class Definition:
	class ComponentRegistry
	{
		// Public Constants, Enums, and Structs:
	public:
		struct ComponentEntry
		{
			string_view_t typeName;
			Component::TypeEnum typeId;
			int subtypeId;
			Component*(*generator)(void);
            constexpr ComponentEntry(string_view_arg typeName, Component::TypeEnum typeId, int subtypeId, Component*(*generator)(void));
		};

        enum class ComponentIndex : size_t
        {
#define BASE_COMPONENT(comp) comp
            BASE_COMPONENT(Transform),
            BASE_COMPONENT(Physics),
            BASE_COMPONENT(Collider),
#define REGISTER_COLLIDER(colliderName) BASE_COMPONENT(colliderName),
#include "Colliders.def"
#undef REGISTER_COLLIDER
            BASE_COMPONENT(Sprite),
            BASE_COMPONENT(SpriteText),
            BASE_COMPONENT(Trail),
            BASE_COMPONENT(Model),
            BASE_COMPONENT(Camera),
            BASE_COMPONENT(Light),
            BASE_COMPONENT(ParticleEmitter),
            BASE_COMPONENT(AudioEmitter),
            BASE_COMPONENT(Animation),
            BASE_COMPONENT(Behavior),
#define REGISTER_BEHAVIOR(behaviorName) BASE_COMPONENT(behaviorName),
#include "Behaviors.def"
#undef REGISTER_BEHAVIOR
#undef BASE_COMPONENT
            NUM_ENTRIES,
        };

		static constexpr unsigned int ComponentCount = (unsigned int)ComponentIndex::NUM_ENTRIES;

		static const array_t<ComponentEntry, ComponentCount> Entries;

		// Constructors/Destructors:
	public:
		ComponentRegistry(void) = delete;

		// Public Static Functions:
	public:
        //
        static const unordered_map_t<Component::TypeEnum, size_t>& MapEnum(void);
        
        //
        static const unordered_map_string_t<size_t>& MapKey(void);

		// Public Functions:
	public:

		// Public Event Handlers
	public:

		// Private Constants, Enums, and Structs:
	private:

		// Private Static Functions:
	private:

		// Private Functions:
	private:

		// Private Static Variables:
	private:

		// Private Variables:
	private:
	};

}	// namespace
