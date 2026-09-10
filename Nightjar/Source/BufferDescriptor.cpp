//------------------------------------------------------------------------------
//
// File Name:	BufferDescriptor.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class that describes the memory structure of a buffer that is
//              used with external interfaces.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "BufferDescriptor.h"

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
	// Private Constants:
	//--------------------------------------------------------------------------

    constexpr array_t<ResourceEnum, static_cast<size_t>(BufferDescriptor::BufferBehavior::NUM_ENTRIES)> BufferDescriptor::BehaviorArray
    {
        GL_STREAM_DRAW,
        GL_STREAM_READ,
        GL_STREAM_COPY,
        GL_STATIC_DRAW,
        GL_STATIC_READ,
        GL_STATIC_COPY,
        GL_DYNAMIC_DRAW,
        GL_DYNAMIC_READ,
        GL_DYNAMIC_COPY,
    };

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

    BufferDescriptor::BufferDescriptor(string_view_arg name, ResourceEnum bind, size_t size, BufferUsage usage, BufferBehavior behavior, unsigned short alignment)
        : NamedObject(name)
        , elements_()
        , bind_(bind)
        , usage_(usage)
        , behavior_(behavior)
        , alignment_(alignment)
        , size_(size)
    {
    }

    BufferDescriptor::BufferDescriptor(ResourceEnum bind, size_t size, BufferUsage usage, BufferBehavior behavior, unsigned short alignment)
        : NamedObject()
        , elements_()
        , bind_(bind)
        , usage_(usage)
        , behavior_(behavior)
        , alignment_(alignment)
        , size_(size)
    {
    }

    BufferDescriptor::BufferDescriptor(const BufferDescriptor& other)
        : NamedObject(other)
        , elements_(other.elements_)
        , bind_(other.bind_)
        , usage_(other.usage_)
        , behavior_(other.behavior_)
        , alignment_(other.alignment_)
        , size_(other.size_)
    {
    }

    BufferDescriptor::BufferDescriptor(BufferDescriptor&& other) noexcept
        : NamedObject(std::move(other))
        , elements_(std::move(other.elements_))
        , bind_(std::move(other.bind_))
        , usage_(std::move(other.usage_))
        , behavior_(std::move(other.behavior_))
        , alignment_(std::move(other.alignment_))
        , size_(std::move(other.size_))
    {
    }

    BufferDescriptor& BufferDescriptor::operator=(const BufferDescriptor& other)
    {
        NamedObject::operator=(other);
        elements_ = other.elements_;
        bind_ = other.bind_;
        usage_ = other.usage_;
        behavior_ = other.behavior_;
        alignment_ = other.alignment_;
        size_ = other.size_;
        return *this;
    }

	//--------------------------------------------------------------------------

	BufferDescriptor::~BufferDescriptor(void)
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

    bool BufferDescriptor::GetEntry(string_view_arg name, const Element*& element) const
    {
        const auto iter = elements_.find(name);
        bool success = iter != elements_.end();
        if (success)
        {
            element = &iter->second;
        }
        else
        {
            TraceWarning("Failed to find element \"{}\" in buffer \"{}\"", name, GetName());
        }
        return success;
    }

    bool BufferDescriptor::GetOffset(string_view_arg name, size_t& offset) const
    {
        const Element* element;
        bool success = GetEntry(name, element);
        if (success)
        {
            offset += element->offset;
        }
        return success;
    }

    bool BufferDescriptor::GetOffset(string_view_arg name, size_t index, size_t& offset) const
    {
        const Element* element;
        bool success = GetEntry(name, element);
        if (success)
        {
            assert(index < element->arraySize && "Index out of bounds!");
            offset += static_cast<size_t>(element->offset)
                + index * DataTypeInfos[(unsigned)element->type].size;
        }
        return success;
    }

    size_t BufferDescriptor::ElementCount(void) const
    {
        return elements_.size();
    }

#pragma endregion Public Functions

	//--------------------------------------------------------------------------
	// Protected Static Functions:
	//--------------------------------------------------------------------------

#pragma region Protected Static Functions

#pragma endregion Protected Static Functions

	//--------------------------------------------------------------------------
	// Protected Functions:
	//--------------------------------------------------------------------------

#pragma region Protected Functions

    void BufferDescriptor::SetSize(size_t size)
    {
        assert(alignment_ > 0 && "Alignment must be greater than zero!");
        size_t padOffset = static_cast<size_t>(alignment_ - 1);
        size_t padding = padOffset - ((size + padOffset) % alignment_);
        size_ = static_cast<decltype(size_)>(size + padding);
    }

    void BufferDescriptor::SetAlignment(unsigned short alignment)
    {
        alignment_ = MathMax((decltype(alignment))1, alignment);
    }

#pragma endregion Protected Functions

}	// namespace
