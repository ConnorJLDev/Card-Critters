//------------------------------------------------------------------------------
//
// File Name:	ShaderGlobalBuffers.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class that handles storage and access of global shader buffers.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ShaderGlobalBuffers.h"

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
	// Private Constants:
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// Private Static Variables:
	//--------------------------------------------------------------------------

    ShaderGlobalBuffers::GlobalBuffer::GlobalBuffer(ShaderBufferDescriptor&& descriptor_)
        : descriptor(std::move(descriptor_))
        , buffer(descriptor)
    {
    }

    ShaderGlobalBuffers::GlobalBuffer::GlobalBuffer(GlobalBuffer&& other) noexcept
        : descriptor(std::move(other.descriptor))
        , buffer(std::move(other.buffer), descriptor)
    {
    }

    ShaderGlobalBuffers::GlobalBuffer::~GlobalBuffer(void)
    {
    }

	//--------------------------------------------------------------------------
	// Public Static Functions:
	//--------------------------------------------------------------------------

#pragma region Public Static Functions

    GraphicsBuffer* ShaderGlobalBuffers::GetBuffer(string_view_arg name)
    {
        static auto& globalBuffers(GlobalBuffers());
        auto iter = globalBuffers.find(name);
        return iter == globalBuffers.end() ? nullptr : &iter->second.buffer;
    }

    void ShaderGlobalBuffers::AddDescriptor(ShaderBufferDescriptor&& descriptor)
    {
        string_view_t name(descriptor.GetName());
        GlobalBuffers().emplace(std::piecewise_construct,
            std::forward_as_tuple(name),
            std::forward_as_tuple(std::move(descriptor))
            );
    }

#pragma endregion Public Static Functions

	//--------------------------------------------------------------------------
	// Private Static Functions:
	//--------------------------------------------------------------------------

#pragma region Private Static Functions

    STATIC_UNORDERED_MAP_CLASS((string_t, ShaderGlobalBuffers::GlobalBuffer, StringHash, std::equal_to<>), ShaderGlobalBuffers, GlobalBuffers)

#pragma endregion Private Static Functions

}	// namespace
