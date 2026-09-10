//------------------------------------------------------------------------------
//
// File Name:	IOSystemTemplate.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		System for unifying IO operations and directories.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include <filesystem>

#include "IOSystem.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	//--------------------------------------------------------------------------
	// Public Static Functions:
	//--------------------------------------------------------------------------

#pragma region Public Static Functions

    bool IOSystem::FindWithoutExtension(string_t& name, const std::ranges::range auto& extensions)
    {
        if (std::filesystem::exists(name))
        {
            return true;
        }
        for (const auto& ext : extensions)
        {
            string_t nameExt = name + string_t(ext);
            if (std::filesystem::exists(nameExt))
            {
                name = nameExt;
                return true;
            }
        }
        return false;
    }

    template<typename T>
    StreamIn IOSystem::OpenStreamIn(string_view_arg name, bool& success)
    {
        StreamIn stream{ string_t(T::ReadDir) + "/" + string_t(name) + ".json"};

        if (!stream.IsValid())
        {
            TraceWarning("Failed to open file \"{}\" for reading", stream.GetPath());
            success = false;
        }
        else if (!stream.StepInto(T::ReadKey))
        {
            TraceError("Failed to find ReadKey \"{}\" in file \"{}\"", T::ReadKey, stream.GetPath());
            success = false;
        }
        else
        {
            success = true;
        }

        return stream;
    }

#pragma endregion Public Static Functions

	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

#pragma endregion Public Functions

}	// namespace
