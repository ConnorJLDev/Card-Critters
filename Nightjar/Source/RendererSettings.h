//------------------------------------------------------------------------------
//
// File Name:	RendererSettings.h
// Author(s):	JSand
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Template class for a new object.
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

	// Class Definition:
	class RendererSettings
	{
		// Public Constants, Enums, and Structs:
	public:

		// Constructors/Destructors:
	public:
		RendererSettings(void);

		~RendererSettings(void);

		// Public Static Functions:
	public:

		// Public Functions:
	public:
        //
        void Activate();

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
        IVec2 viewportOffset_;
        UVec2 viewportSize_;
        IVec2 scissorOffset_;
        UVec2 scissorSize_;
        bool scissorEnable_;
	};

}	// namespace
