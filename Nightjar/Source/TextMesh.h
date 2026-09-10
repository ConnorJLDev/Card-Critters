//------------------------------------------------------------------------------
//
// File Name:	TextMesh.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class that manages a mesh representing text.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Mesh.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	// Forward Declarations:
    class Font;

	// Typedefs:

	// Class Definition:
	class TextMesh : public Mesh
	{
		// Public Constants, Enums, and Structs:
	public:
        // The default capacity a text mesh starts with.
        static const unsigned DefaultSize;

		// Constructors/Destructors:
	public:
		TextMesh(void);

		TextMesh(string_view_arg name, string_view_arg text, const Font* font = nullptr);

		~TextMesh(void);

		// Public Static Functions:
	public:

		// Public Functions:
	public:
        //
        void SetText(string_view_arg text, const Font* font = nullptr);

		// Public Event Handlers
	public:

		// Private Constants, Enums, and Structs:
	private:

		// Private Static Functions:
	private:
        //
        static Primitive& CommonPrimitive(void);

        //
        static void ResizeCommonPrimitive(unsigned int size);

		// Private Functions:
	private:
        //
        void SetTextInternal(string_view_arg text);

		// Private Static Variables:
	private:

		// Private Variables:
	private:
        vector_t<float> lineWidths_;
        const Font* font_;
        float alignment_;
        bool dynamic_;
        bool notInit_;
	};

}	// namespace
