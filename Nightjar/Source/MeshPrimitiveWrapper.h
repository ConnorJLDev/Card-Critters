//------------------------------------------------------------------------------
//
// File Name:	MeshPrimitiveWrapper.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Static wrapper class for mesh primitives.
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

	// Typedefs:

	// Class Definition:
    class Mesh::PrimitiveWrapper : public StaticManagement::ContainerBase
    {
    public:
        PrimitiveWrapper();
        PrimitiveWrapper(DrawType drawType);
        PrimitiveWrapper(DrawType drawType, const vector_t<VertexIndex_t>& indices);
        PrimitiveWrapper(const PrimitiveWrapper& other) = delete;
        PrimitiveWrapper& operator=(const PrimitiveWrapper& other) = delete;
    public:
        ~PrimitiveWrapper();
    public:
        operator Primitive&();
        operator const Primitive&() const;
    public:
        void Clear(void) override;
    public:
        static Primitive& Create();
        static Primitive& Create(DrawType drawType);
        static Primitive& Create(DrawType drawType, const vector_t<VertexIndex_t>& indices);
    private:
        Primitive data;
    };

}	// namespace
