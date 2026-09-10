//------------------------------------------------------------------------------
//
// File Name:	TextMesh.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class that manages a mesh representing text.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "TextMesh.h"

#include "Mesh.inl"
#include "MeshPrimitiveWrapper.h"

#include "Font.h"

#define USE_GEOMETRY_SHADER

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

    constexpr unsigned TextMesh::DefaultSize(15u);

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

	TextMesh::TextMesh(void)
		: TextMesh("", "", nullptr)
	{
	}

    TextMesh::TextMesh(string_view_arg name, string_view_arg text, const Font* font)
        : Mesh(name)
        , lineWidths_()
        , font_(font)
        , alignment_(0.0f)
        , dynamic_(true)
        , notInit_(text.empty())
    {
        //vertexCount_ = vertexCapacity_ = MathMax((unsigned)(std::bit_ceil(text.size() + 2) - 1), DefaultSize);
        vertexCount_ = immediateDrawCount_ = (unsigned)text.size();
#ifdef USE_GEOMETRY_SHADER
        immediateDrawType_ = DrawType::Points;
#else
        immediateDrawType_ = DrawType::Invalid;
        primitives_.reserve(1);
        Primitive& primitive = primitives_.emplace_back(DrawType::TriangleStrip);
        primitive.indexID = CommonPrimitive().indexID;
#endif
        attributes_.reserve(2);
        attributes_.emplace_back(DataType::Vec3);
        attributes_.emplace_back(DataType::Int);
        CalculateAttributes();
        Mesh::Resize(vertexCount_);
        CreateVertexBuffer();
        CreateVertexDescriptor();
        SetTextInternal(text);
        if (!notInit_)
        {
            BindVertexBuffer();
            InitializeVertexBuffer();
            BindVertexDescriptor();
            RefreshVertexDescriptor();
            UnbindVertexDescriptor();
            UnbindVertexBuffer();
        }
    }

	//--------------------------------------------------------------------------

	TextMesh::~TextMesh(void)
	{
#ifndef USE_GEOMETRY_SHADER
        Primitive& primitive = primitives_[0];
        primitive.indexID = ResourceUnbind;
#endif
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

    void TextMesh::SetText(string_view_arg text, const Font* font)
    {
        if (font != nullptr)
        {
            font_ = font;
        }

        VertexIndex_t textSize = static_cast<VertexIndex_t>(text.size());

        if (textSize > vertexCount_) [[unlikely]]
        {
            Resize(textSize);
            if (notInit_)
            {
                notInit_ = false;
                SetTextInternal(text);
                BindVertexBuffer();
                InitializeVertexBuffer();
                BindVertexDescriptor();
                RefreshVertexDescriptor();
                UnbindVertexDescriptor();
                UnbindVertexBuffer();

#ifdef USE_GEOMETRY_SHADER
                immediateDrawCount_ = static_cast<decltype(immediateDrawCount_)>(textSize);
#else
                Primitive& primitive = primitives_[0];
                primitive.vertexCount = static_cast<decltype(primitive.vertexCount)>(MathMax(((long long)textSize * 5) - 1), 0);
#endif
                return;
            }
#ifndef USE_GEOMETRY_SHADER
            if ((textSize - 1) * 5 >= CommonPrimitive().vertexCount)
            {
                ResizeCommonPrimitive(textSize);
            }
#endif
        }

        BindVertexBuffer();

        SetTextInternal(text);

        ApplyVertexBuffer();

        UnbindVertexBuffer();

#ifdef USE_GEOMETRY_SHADER
        immediateDrawCount_ = static_cast<decltype(immediateDrawCount_)>(textSize);
#else
        Primitive& primitive = primitives_[0];
        primitive.vertexCount = static_cast<decltype(primitive.vertexCount)>(MathMax(((long long)textSize * 5) - 1), 0);
#endif
    }

#pragma endregion Public Functions

	//--------------------------------------------------------------------------
	// Private Static Functions:
	//--------------------------------------------------------------------------

#pragma region Private Static Functions

    Mesh::Primitive& TextMesh::CommonPrimitive(void)
    {
        static Primitive& CommonPrimitive_(
            Mesh::PrimitiveWrapper::Create(DrawType::TriangleStrip, vector_t<VertexIndex_t>({ 0, 0, 0, 0, }))
        );
        return CommonPrimitive_;
    }

    void TextMesh::ResizeCommonPrimitive(unsigned int size)
    {
#ifdef USE_GEOMETRY_SHADER
        UNREFERENCED_PARAMETER(size);
#else
        size = static_cast<decltype(size)>(std::bit_ceil(static_cast<size_t>(size) + 2) - 1);
        Primitive& primitive = CommonPrimitive();
        primitive.DeleteBuffer();

        vector_t<VertexIndex_t>& indices(primitive.indices);
        VertexIndex_t oldCount = *indices.back();
        indices.reserve(((size_t)size * 5) - 1);
        for (VertexIndex_t i = MathMax(oldCount, 1u); i < size; ++i)
        {
            indices.insert(indices.cend(), { Primitive::ResetIndex, i, i, i, i, });
        }
        primitive.GenerateBuffer();
#endif
    }

#pragma endregion Private Static Functions

	//--------------------------------------------------------------------------
	// Private Functions:
	//--------------------------------------------------------------------------

#pragma region Private Functions

    void TextMesh::SetTextInternal(string_view_arg text)
    {
        // TODO: Handle variable spacing (like kerning and non-monospaced fonts).
        Vec3 pos(0, 0, 0);
        static constexpr Vec2 increment(0.1f, -0.1f);

        for (unsigned i = 0; i < text.size(); ++i)
        {
            SetData(attributes_[0], pos, i);
            pos.x += increment.x;
            const int strChr(text[i]);
            SetData(attributes_[1], strChr, i);
            if (strChr == '\n')
            {
                pos.x = 0;
                pos.y += increment.y;
            }
        }

    }

#pragma endregion Private Functions

}	// namespace
