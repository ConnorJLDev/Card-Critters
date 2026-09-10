//------------------------------------------------------------------------------
//
// File Name:	Mesh.inl
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Mesh encapsulator.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//#define VERTEX_DATA_BATCHED

#ifndef VERTEX_DATA_BATCHED
#define VERTEX_DATA_INTERLEAVED
#endif

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Mesh.h"

#include GRAPHICS_INCLUDE

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

	//--------------------------------------------------------------------------
	// Public Static Variables:
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// Public Variables:
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// Private Static Constants:
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

#pragma endregion Constructors

	//--------------------------------------------------------------------------
	// Public Static Functions:
	//--------------------------------------------------------------------------

#pragma region Public Static Functions


#pragma endregion Public Static Functions

	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

#pragma endregion Public Functions

	//--------------------------------------------------------------------------
	// Private Functions:
	//--------------------------------------------------------------------------

#pragma region Private Functions

    template<typename T>
    void Mesh::SetData(const Attribute& attr, const T* data)
    {
#ifdef VERTEX_DATA_INTERLEAVED
        for (unsigned i = 0; i < vertexCount_; ++i)
        {
            SetDataInternal(attr, &data[i], sizeof(T), i);
        }
#endif
#ifdef VERTEX_DATA_BATCHED
#error Batched vertex data not implemented!
		//glBufferSubData
#endif
    }

    template<typename T>
    void Mesh::SetData(const Attribute& attr, const T& data, unsigned vertIndex)
    {
        SetDataInternal(attr, &data, sizeof(T), vertIndex);
    }

#pragma endregion Private Functions

}	// namespace
