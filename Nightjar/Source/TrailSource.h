//------------------------------------------------------------------------------
//
// File Name:	TrailSource.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class for managing a visual trail.
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

#include "GraphicsSystem.h"

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
	class TrailSource : public Object
	{
		// Public Constants, Enums, and Structs:
	public:

		// Constructors/Destructors:
	public:
		TrailSource(void);

		TrailSource(const TrailSource& other);

		TrailSource(TrailSource&& other) noexcept;

		TrailSource& operator=(const TrailSource& other);

		~TrailSource(void);

		// Public Static Functions:
	public:

		// Public Functions:
	public:
        //
        void AddSegment(void);

        //
        const Mesh* GetShell(void) const;

        //
        void SetShell(string_view_arg shellName);

        //
        void SetShell(const Mesh* shell);

        //
        const Material* GetMaterial(void) const;

        //
        void SetMaterial(string_view_arg materialName);

        //
        void SetMaterial(const Material* material);

        //
        bool GetSlerp(void) const;

        //
        void SetSlerp(bool slerp);

        //
        void Render(const Mat4& transform) const;

		//
		void DisplayIMGUI(void);

		// Public Event Handlers
	public:

		// Private Constants, Enums, and Structs:
	private:
        //
        struct Segment : public TransformComponents
        {
            // Quaternion rotation, uniform scale.
        };

		// Private Static Functions:
	private:

		// Private Functions:
	private:

		// Private Static Variables:
	private:

		// Private Variables:
	private:
        //
        vector_t<Segment> segments_;
        //
        const Mesh* shell_;
        //
        Mesh* lines_;
        //
        Material* material_;
        // 
        unsigned int subdivisionCount_;
        //
        bool slerp_;
	};

}	// namespace
