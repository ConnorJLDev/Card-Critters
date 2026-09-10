//------------------------------------------------------------------------------
//
// File Name:	BaseSystem.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Base class for all systems.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "EngineLoop.h"

//------------------------------------------------------------------------------
// Preprocessor Directives:
//------------------------------------------------------------------------------

// Definition for common declarations that are required across all system
// declarations.
#define SYSTEM_COMMON(system)\
    system(const system& other) = delete;\
    system(system&& other) noexcept = delete;\
    system& operator=(const system& other) = delete;\
    system& operator=(system&& other) noexcept = delete;\

// Definition for consistent "final" implementation across all system
// declarations.
#define SYSTEM_FINAL final

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

	// Class Definition:
	class BaseSystem : public NamedObject
	{
		friend class Engine;
		friend class Engine_;
		friend class PerformanceSystem_;

		// Public Constants, Enums, and Structs:
	public:

        // Protected Constants, Enums, and Structs:
    protected:
        // Flags for indicating what engine loops a system takes part in.
        enum class LoopFlags : unsigned int
        {
            None = 0,
            HasUpdate = 1 << 0,
            HasRender = 1 << 1,
            Default = static_cast<std::underlying_type_t<LoopFlags>>(-1),
        };

		// Constructors/Destructors:
	protected:
		BaseSystem(void) = delete;
		BaseSystem(string_view_arg name, LoopFlags flags = LoopFlags::Default);
        BaseSystem(const BaseSystem& other) = delete;
        BaseSystem(BaseSystem&& other) noexcept = delete;
        BaseSystem& operator=(const BaseSystem& other) = delete;
        BaseSystem& operator=(BaseSystem&& other) noexcept = delete;

		// All systems need a virtual destructor to have their destructor called 
		virtual ~BaseSystem(void) = 0;

		// Public Static Functions:
	public:

		// Public Functions:
	public:

		// Public Event Handlers
	public:

		// Private Static Functions:
	private:
        ENUM_DECLARE_OPS(LoopFlags);

		// Private Functions:
	private:
		/// <summary>
		/// Initialize the system.
		/// </summary>
		/// <returns>True if initialization successful, otherwise false.</returns>
		virtual bool Init(void) = 0;

		/// <summary>
		/// Update the system each frame.
		/// </summary>
		/// <param name="dt">Delta time (in seconds) of the last frame.</param>
		virtual void Update(void);

		/// <summary>
		/// Render the system each frame.
		/// </summary>
		virtual void Render(void);

		/// <summary>
		/// Exit the system.
		/// </summary>
		virtual void Exit(void) = 0;

		/// <summary>
		/// </summary>
        
		/// <summary>
        /// Checks if the system participates in a given engine loop.
		/// </summary>
		/// <param name="loop">The engine loop to check.</param>
		/// <returns>True if the system runs in the given engine loop, false otherwise.</returns>
		bool LoopActive(EngineLoop loop) const;

		/// <summary>
        /// Gets the priority of the system for a given engine loop. Higher = earlier, lower = later.
		/// </summary>
		/// <param name="loop">The engine loop to check.</param>
		/// <returns>An int indicating how early (relative to other systems) to run this system in the given loop.</returns>
		virtual int LoopPriority(EngineLoop loop) const;

		/// <summary>
		/// Moves SetName to private scope.
		/// </summary>
		using NamedObject::SetName;

		// Protected Static Variables:
	protected:

		// Protected Variables:
    protected:
        // Flags for indicating what engine loops this system takes part in.
        const LoopFlags flags_;
	};

}	// namespace
