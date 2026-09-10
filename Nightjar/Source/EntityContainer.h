//------------------------------------------------------------------------------
//
// File Name:	EntityContainer.h
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 4
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "Container.h"

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
    class EntityContainer : public Container<Entity>
    {
        friend class Scene;

        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    public:
        EntityContainer();

        // Public Static Functions:
    public:

        // Public Functions:
    public:
        /// <summary>
        /// Updates all Entities in the EntityContainer.
        /// Entities flagged as destroyed are removed.
        /// </summary>
        /// <param name="dt">Change in time (in seconds) since the last game loop.</param>
        void UpdateAll(float dt);

        /// <summary>
        /// Renders all Entities in the EntityContainer.
        /// </summary>
        void RenderAll();

        bool Add(Entity* entity) override;

        bool Add(Entity& entity) override;

        /// <summary>
        /// Check for collisions between all Entities in the EntityContainer.
        /// </summary>
        void CheckCollisions();

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        Scene* GetScene() const;

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
        Scene* scene_;
    };

}	// namespace
