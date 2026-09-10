//------------------------------------------------------------------------------
//
// File Name:	ImGuiCommon.inl
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Provides convenience functions that directly interface with the
//              ImGui implementation.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiCommon.h"

//------------------------------------------------------------------------------
// Preprocessor Directives:
//------------------------------------------------------------------------------

#pragma warning(push)
#pragma warning(disable : 4514)

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace ImGui
{

    template<typename T, typename TContainer>
    bool CreateEnumSelector(string_view_arg name, T& enumVal, const TContainer& enumValNames)
    {
        bool changed = false;
        const char* preview;
        // get the preview name depending on the type of TContainer.
        if constexpr (is_map_like_v<TContainer>)
        {
            // if map, use at() function.
            const auto& cur = enumValNames.at(enumVal);
            if constexpr (std::is_same_v<std::decay_t<decltype(cur)>, string_view_t>)
            {
                preview = cur.data();
            }
            else // assume std::string
            {
                preview = cur.c_str();
            }
        }
        else
        {
            // if not map, assume it's anything that supports operator[].
            const auto& cur = enumValNames[(unsigned int)enumVal];
            if constexpr (std::is_same_v<std::decay_t<decltype(cur)>, string_view_t>)
            {
                preview = cur.data();
            }
            else // assume std::string
            {
                preview = cur.c_str();
            }
        }
        if (ImGui::BeginCombo(name.data(), preview))
        {
            for (auto i = T(); i < T::NUM_ENTRIES; i = (T)((unsigned int)i + 1))
            {
                ImGuiSelectableFlags flags{};
                if (i == enumVal)
                {
                    flags |= ImGuiSelectableFlags_Disabled;
                }
                // constexpr if checks for determining function calls are difficult, so this is copy-pasted from above.
                const char* valueName;
                if constexpr (is_map_like_v<TContainer>)
                {
                    // if map, use at() function.
                    const auto& cur = enumValNames.at(i);
                    if constexpr (std::is_same_v<std::decay_t<decltype(cur)>, string_view_t>)
                    {
                        valueName = cur.data();
                    }
                    else // assume std::string
                    {
                        valueName = cur.c_str();
                    }
                }
                else
                {
                    // if not map, assume it's anything that supports operator[].
                    const auto& cur = enumValNames[(unsigned int)i];
                    if constexpr (std::is_same_v<std::decay_t<decltype(cur)>, string_view_t>)
                    {
                        valueName = cur.data();
                    }
                    else // assume std::string
                    {
                        valueName = cur.c_str();
                    }
                }
                if (ImGui::Selectable(valueName, false, flags))
                {
                    enumVal = i;
                    changed = true;
                }
            }
            ImGui::EndCombo();
        }
        return changed;
    }

    template<typename T, typename TContainer>
        requires(is_map_like_v<TContainer>)
    bool CreateEnumFlagSelector(string_view_arg name, T& enumVal, const TContainer& enumValNames)
    {
        bool changed = false;
        Nightjar::string_t preview;
        // Build the preview name.
        for (const auto& [val, ename] : enumValNames)
        {
            if (static_cast<bool>(enumVal & val))
            {
                if (!preview.empty())
                {
                    preview += ',';
                }
                preview += ename;
            }
        }
        const char* const previewLabel = preview.empty() ? "None" : preview.c_str();
        if (ImGui::BeginCombo(name.data(), previewLabel))
        {
            for (const auto& [val, ename] : enumValNames)
            {
                bool set = static_cast<bool>(enumVal & val);

                const char* valueName;
                if constexpr (std::is_same_v<std::decay_t<decltype(ename)>, string_view_t>)
                {
                    valueName = ename.data();
                }
                else // assume std::string
                {
                    valueName = ename.c_str();
                }
                if (ImGui::Checkbox(valueName, &set))
                {
                    if (set)
                    {
                        enumVal |= val;
                    }
                    else
                    {
                        enumVal &= ~val;
                    }
                    changed = true;
                }
            }
            ImGui::EndCombo();
        }
        return changed;
    }

    template<typename T>
    bool CreateDragRotation(const char* label, T& value, float v_speed, float v_min, float v_max, const char* format, int flags)
    {
        static_assert(std::is_same_v<T, float> || std::is_same_v<T, Nightjar::Vec3>, "Invalid type!");
        bool changed = false;
        T degrees = glm::degrees(value);
        if constexpr (std::is_same_v<T, float>)
        {
            changed = ImGui::DragFloat(label, &degrees, v_speed, v_min, v_max, format, flags);
        }
        else if constexpr (std::is_same_v<T, Nightjar::Vec3>)
        {
            changed = ImGui::DragFloat3(label, &degrees.x, v_speed, v_min, v_max, format, flags);
        }
        else
        {
            static_assert(false);
        }
        if (changed)
        {
            value = glm::radians(degrees);
        }
        return changed;
    }

}	// namespace

#pragma warning(pop)
