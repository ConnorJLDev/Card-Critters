//------------------------------------------------------------------------------
//
// File Name:	ComponentRegistry.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class for containing all of the different types of components.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ComponentRegistry.h"
#include "Component.h"

#include "Transform.h"
#include "Physics.h"
#include "Animation.h"

#include "Sprite.h"
#include "SpriteText.h"
#include "Trail.h"
#include "Camera.h"
#include "Light.h"
#include "Model.h"
#include "ParticleEmitter.h"
#include "AudioEmitter.h"

#include "Collider.h"
#include "Colliders.h"

#include "Behavior.h"
#include "Behaviors.h"

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

    constexpr ComponentRegistry::ComponentEntry::ComponentEntry(string_view_arg typeName_, Component::TypeEnum typeId_, int subtypeId_, Component* (*generator_)(void))
        : typeName(typeName_)
        , typeId(typeId_)
        , subtypeId(subtypeId_)
        , generator(generator_)
    {
    }

	constexpr array_t<ComponentRegistry::ComponentEntry, ComponentRegistry::ComponentCount> ComponentRegistry::Entries
	{ {
#define BASE_COMPONENT(comp) ComponentEntry(comp::ReadKey, Component::TypeEnum::comp, 0, []()->Component* { return new comp(); })
#define INVALID_COMPONENT(comp) ComponentEntry(comp::ReadKey, Component::TypeEnum::comp, -1, []()->Component* { throw std::runtime_error(#comp " is a base component type and must be derived from!"); })
#define DERIVED_COMPONENT(compBase, comp) ComponentEntry(comp::ReadKey, Component::TypeEnum::compBase, compBase::c##comp, []()->Component* { return new comp(); })
		BASE_COMPONENT(Transform),
		BASE_COMPONENT(Physics),
        INVALID_COMPONENT(Collider),
#define REGISTER_COLLIDER(colliderName) DERIVED_COMPONENT(Collider, ##colliderName),
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
        INVALID_COMPONENT(Behavior),
#define REGISTER_BEHAVIOR(behaviorName) DERIVED_COMPONENT(Behavior, ##behaviorName),
#include "Behaviors.def"
#undef REGISTER_BEHAVIOR
#undef BASE_COMPONENT
#undef INVALID_COMPONENT
#undef DERIVED_COMPONENT
	} };

	//--------------------------------------------------------------------------
	// Public Static Variables:
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// Public Variables:
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// Private Constants:
	//--------------------------------------------------------------------------

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
    
	//--------------------------------------------------------------------------

#pragma endregion Constructors

	//--------------------------------------------------------------------------
	// Public Static Functions:
	//--------------------------------------------------------------------------

#pragma region Public Static Functions

    const unordered_map_t<Component::TypeEnum, size_t >& ComponentRegistry::MapEnum(void)
    {
        static unordered_map_t<Component::TypeEnum, size_t >& MapEnum_(StaticManagement::UnorderedMapWrapper<Component::TypeEnum, size_t > ::Create(
            {
#define BASE_COMPONENT(comp) { Component::TypeEnum::comp, (size_t)(ComponentIndex::comp) }
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
            }
        ));
        return MapEnum_;
    };

    const unordered_map_t<string_t, size_t, Utility::StringHash, std::equal_to<> >& ComponentRegistry::MapKey(void)
    {
        static unordered_map_t<string_t, size_t, Utility::StringHash, std::equal_to<> >& MapKey_(StaticManagement::UnorderedMapWrapper<string_t, size_t, Utility::StringHash, std::equal_to<> > ::Create(
            {
#define BASE_COMPONENT(comp) { comp::ReadKey.data(), (size_t)(ComponentIndex::comp) }
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
            }
        ));
        return MapKey_;
    };

#pragma endregion Public Static Functions

	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

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

#pragma endregion Private Functions

}	// namespace
