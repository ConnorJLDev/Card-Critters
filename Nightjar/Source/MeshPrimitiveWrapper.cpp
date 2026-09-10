//------------------------------------------------------------------------------
//
// File Name:	MeshPrimitiveWrapper.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Static wrapper class for mesh primitives.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "MeshPrimitiveWrapper.h"

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

    Mesh::PrimitiveWrapper::PrimitiveWrapper()
        : data()
    {
    }

    Mesh::PrimitiveWrapper::PrimitiveWrapper(DrawType drawType)
        : data(drawType)
    {
    }

    Mesh::PrimitiveWrapper::PrimitiveWrapper(DrawType drawType, const vector_t<VertexIndex_t>& indices)
        : data(drawType, indices)
    {
    }

	//--------------------------------------------------------------------------

    Mesh::PrimitiveWrapper::~PrimitiveWrapper(void)
	{
	}

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

    Mesh::PrimitiveWrapper::operator Mesh::Primitive& ()
    {
        return data;
    }

    Mesh::PrimitiveWrapper::operator const Mesh::Primitive&() const
    {
        return data;
    }

    void Mesh::PrimitiveWrapper::Clear(void)
    {
        // TODO: Properly clear the primitive (but handle reinitialization)
    }

    Mesh::Primitive& Mesh::PrimitiveWrapper::Create()
    {
        return ConstContainerBase::Create<PrimitiveWrapper>();
    }

    Mesh::Primitive& Mesh::PrimitiveWrapper::Create(DrawType drawType)
    {
        return ConstContainerBase::Create<PrimitiveWrapper>(drawType);
    }

    Mesh::Primitive& Mesh::PrimitiveWrapper::Create(DrawType drawType, const vector_t<VertexIndex_t>& indices)
    {
        return ConstContainerBase::Create<PrimitiveWrapper>(drawType, indices);
    }

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
