//------------------------------------------------------------------------------
//
// File Name:	MemoryAllocatorInfo.inl
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

#include "MemoryAllocatorInfo.h"

//------------------------------------------------------------------------------
// Preprocessor Directives:
//------------------------------------------------------------------------------

#pragma warning(push)
#pragma warning(disable : 4514)

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar::Memory
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
    // Static Local Functions:
    //--------------------------------------------------------------------------

#pragma region Static Local Functions

    static constexpr std::string GetAllocatorName(std::string funcName)
    {
        size_t angleStart = funcName.find('<') + 1;
        size_t angleEnd = angleStart;
        size_t bracketDepth = 1;
        for (; angleEnd < funcName.size() && bracketDepth > 0; ++angleEnd)
        {
            char c = funcName[angleEnd];
            if (c == '<')
            {
                ++bracketDepth;
            }
            else if (c == '>')
            {
                --bracketDepth;
            }
        }
        angleStart = funcName.find('<', angleEnd) + 1;
        angleEnd = angleStart;
        bracketDepth = 1;
        for (; angleEnd < funcName.size() && bracketDepth > 0; ++angleEnd)
        {
            char c = funcName[angleEnd];
            if (c == '<')
            {
                ++bracketDepth;
            }
            else if (c == '>')
            {
                --bracketDepth;
            }
        }
        if (bracketDepth <= 0 && angleStart < funcName.size() && angleEnd < funcName.size() && angleStart < angleEnd)
        {
            funcName = funcName.substr(angleStart, angleEnd - angleStart - 1);
            size_t lastNonSpace = funcName.find_last_not_of(" \t\n\r\f\v") + 1;
            return funcName.substr(0, lastNonSpace);
        }
        return funcName;
    }

#pragma endregion Static Local Functions

    //--------------------------------------------------------------------------
    // Constructors/Destructors:
    //--------------------------------------------------------------------------

#pragma region Constructors

    template <typename T>
    consteval AllocatorInfo<T>::AllocatorInfo(void)
        : name()
        , nameLen()
        , id()
    {
        string_view_t nameStr = BuildStr(GetAllocatorName, std::source_location::current().function_name());
        name = nameStr.data();
        nameLen = nameStr.size();
        id = fnv1a_hash(nameStr);
    }

    template <typename T>
    template <typename TOther>
    consteval AllocatorInfo<T>::AllocatorInfo(const AllocatorInfo<TOther>& other)
        : name(other.name)
        , nameLen(other.nameLen)
        , id(other.id)
    {
    }

    //--------------------------------------------------------------------------

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

    template <typename T>
    constexpr AllocatorInfo<T>::operator string_view_t(void) const
    {
        return string_view_t(name, nameLen);
    }

    template<typename T>
    constexpr AllocatorInfo<T>::operator MemId_t(void) const
    {
        return id;
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

#pragma warning(pop)
