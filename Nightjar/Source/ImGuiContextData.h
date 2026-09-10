//------------------------------------------------------------------------------
//
// File Name:	ImGuiContextData.h
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
    class Entity;

	// Typedefs:

	// Class Definition:
	class ImGuiContextData : public Object
	{
        friend class ImGuiSystem_;

		// Public Constants, Enums, and Structs:
	public:

		// Constructors/Destructors:
	private:
        ImGuiContextData(void);
    public:
		~ImGuiContextData(void);

		// Public Static Functions:
	public:
        //
        static ImGuiContextData& GetContext(void);

		// Public Functions:
	public:

		// Public Event Handlers
	public:

        // Public Variables:
    public:
        Entity* selectedEntity_;
        set_t<Entity*> selectedEntities_;

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
