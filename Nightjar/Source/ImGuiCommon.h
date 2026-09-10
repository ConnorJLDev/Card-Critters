//------------------------------------------------------------------------------
//
// File Name:	ImGuiCommon.h
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

#include "ImGuiInclude.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    class Texture;
}

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace ImGui
{
    using Nightjar::string_view_t;
    using Nightjar::string_view_arg;

    // Common color used for displaying values in text.
    extern const unsigned int ValueColor;

    /// <summary>
    /// Convenience function for displaying a string_view as unformatted text.
    /// </summary>
    /// <param name="text">The text to display.</param>
    void TextUnformatted(string_view_arg text);

    // The following region of code is based on imgui/misc/cpp/imgui_stdlib.h, modified to use templatization.
#pragma region Input Text STL

    template <typename TChar, typename TTraits, typename TAllocator>
    bool InputText(const char* label, std::basic_string<TChar, TTraits, TAllocator>* str, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = nullptr, void* user_data = nullptr);

    template <typename TChar, typename TTraits, typename TAllocator>
    bool InputTextMultiline(const char* label, std::basic_string<TChar, TTraits, TAllocator>* str, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = nullptr, void* user_data = nullptr);

    template <typename TChar, typename TTraits, typename TAllocator>
    bool InputTextWithHint(const char* label, const char* hint, std::basic_string<TChar, TTraits, TAllocator>* str, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = nullptr, void* user_data = nullptr);

#pragma endregion Input Text STL

    /// <summary>
    /// Renders a texture in ImGui.
    /// </summary>
    /// <param name="texture">The texture to render.</param>
    /// <param name="size">The size to render the texture at.</param>
    void Image(const Nightjar::Texture* texture, const Nightjar::Vec2& size);

    /// <summary>
    /// Appends a (?) after a field, providing a tooltip when hovered.
    /// </summary>
    /// <param name="description">The text to display in the tooltip.</param>
    /// <returns>True if hovered, false otherwise.</returns>
    bool CreateHoverTooltip(string_view_arg description);

    /// <summary>
    /// Creates a selector for an enum value.
    /// </summary>
    /// <typeparam name="T">The enum type.</typeparam>
    /// <typeparam name="TContainer">The container type of the enum names.</typeparam>
    /// <param name="name">The text displayed alongside the value.</param>
    /// <param name="enumVal">The value used for display and modification</param>
    /// <param name="enumValNames">The container of the enum names.</param>
    /// <returns>True if the value was changed, false otherwise.</returns>
    template<typename T, typename TContainer>
    bool CreateEnumSelector(string_view_arg name, T& enumVal, const TContainer& enumValNames);

    /// <summary>
    /// Creates a selector for an enum value used for flags.
    /// </summary>
    /// <typeparam name="T">The enum type.</typeparam>
    /// <typeparam name="TContainer">The container type of the enum names.</typeparam>
    /// <param name="name">The text displayed alongside the value.</param>
    /// <param name="enumVal">The value used for display and modification</param>
    /// <param name="enumValNames">The container of the enum names.</param>
    /// <returns>True if the value was changed, false otherwise.</returns>
    template<typename T, typename TContainer>
        requires(is_map_like_v<TContainer>)
    bool CreateEnumFlagSelector(string_view_arg name, T& enumVal, const TContainer& enumValNames);

    /// <summary>
    /// Creates a special float drag field that displays the values as converted from radians to degrees.
    /// </summary>
    /// <typeparam name="T">The value type.</typeparam>
    /// <param name="label">The text displayed alongside the value.</param>
    /// <param name="value">The value used for display and modification.</param>
    /// <param name="v_speed">How fast the value changes when interacted.</param>
    /// <param name="v_min">The minimum value.</param>
    /// <param name="v_max">The maximum value.</param>
    /// <param name="format">How the value is displayed as text.</param>
    /// <param name="flags">ImGui flags to render with.</param>
    /// <returns>True if the value was changed, false otherwise.</returns>
    template<typename T>
    bool CreateDragRotation(const char* label, T& value, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", int flags = 0);

}	// namespace

#include "ImGuiCommon.inl"
