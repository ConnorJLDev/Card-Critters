//------------------------------------------------------------------------------
//
// File Name:	ImGuiCommon.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Provides convenience functions that directly interface with the
//              ImGui implementation.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ImGuiCommon.h"

#include "Texture.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace ImGui
{
    constexpr ImU32 ValueColor(IM_COL32(255, 200, 0, 255));

    void TextUnformatted(string_view_arg text)
    {
        TextUnformatted(text.data(), text.data() + text.size());
    }

    // The following region of code is based on imgui/misc/cpp/imgui_stdlib.cpp, modified to use templatization.
#pragma region Input Text STL

    // 
    template <typename TChar, typename TTraits, typename TAllocator>
    struct InputTextCallback_UserData
    {
        std::basic_string<TChar, TTraits, TAllocator>* Str;
        ImGuiInputTextCallback  ChainCallback;
        void* ChainCallbackUserData;
    };

    // 
    template <typename TChar, typename TTraits, typename TAllocator>
    int InputTextCallback(ImGuiInputTextCallbackData* data)
    {
        InputTextCallback_UserData<TChar, TTraits, TAllocator>* user_data = (InputTextCallback_UserData<TChar, TTraits, TAllocator>*)data->UserData;
        if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
        {
            // Resize string callback
            // If for some reason we refuse the new length (BufTextLen) and/or capacity (BufSize) we need to set them back to what we want.
            std::basic_string<TChar, TTraits, TAllocator>* str = user_data->Str;
            IM_ASSERT(data->Buf == str->c_str());
            str->resize((size_t)data->BufTextLen);
            data->Buf = (char*)str->c_str();
        }
        else if (user_data->ChainCallback)
        {
            // Forward to user callback, if any
            data->UserData = user_data->ChainCallbackUserData;
            return user_data->ChainCallback(data);
        }
        return 0;
    }

    template <typename TChar, typename TTraits, typename TAllocator>
    bool InputText(const char* label, std::basic_string<TChar, TTraits, TAllocator>* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
    {
        IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
        flags |= ImGuiInputTextFlags_CallbackResize;

        InputTextCallback_UserData<TChar, TTraits, TAllocator> cb_user_data;
        cb_user_data.Str = str;
        cb_user_data.ChainCallback = callback;
        cb_user_data.ChainCallbackUserData = user_data;
        return ImGui::InputText(label, (TChar*)str->c_str(), str->capacity() + 1, flags, InputTextCallback<TChar, TTraits, TAllocator>, &cb_user_data);
    }

    template <typename TChar, typename TTraits, typename TAllocator>
    bool InputTextMultiline(const char* label, std::basic_string<TChar, TTraits, TAllocator>* str, const ImVec2& size, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
    {
        IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
        flags |= ImGuiInputTextFlags_CallbackResize;

        InputTextCallback_UserData<TChar, TTraits, TAllocator> cb_user_data;
        cb_user_data.Str = str;
        cb_user_data.ChainCallback = callback;
        cb_user_data.ChainCallbackUserData = user_data;
        return ImGui::InputTextMultiline(label, (TChar*)str->c_str(), str->capacity() + 1, size, flags, InputTextCallback<TChar, TTraits, TAllocator>, &cb_user_data);
    }

    template <typename TChar, typename TTraits, typename TAllocator>
    bool InputTextWithHint(const char* label, const char* hint, std::basic_string<TChar, TTraits, TAllocator>* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
    {
        IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
        flags |= ImGuiInputTextFlags_CallbackResize;

        InputTextCallback_UserData<TChar, TTraits, TAllocator> cb_user_data;
        cb_user_data.Str = str;
        cb_user_data.ChainCallback = callback;
        cb_user_data.ChainCallbackUserData = user_data;
        return InputTextWithHint(label, hint, (TChar*)str->c_str(), str->capacity() + 1, flags, InputTextCallback<TChar, TTraits, TAllocator>, &cb_user_data);
    }

#define TEMPLATIZE_TEXT_IMPL(TChar, TTraits, TAllocator)\
    template InputTextCallback_UserData<TChar, TTraits, TAllocator>;\
    template int InputTextCallback<TChar, TTraits, TAllocator>(ImGuiInputTextCallbackData* data);\
    template bool InputText<TChar, TTraits, TAllocator>(const char* label, std::basic_string<TChar, TTraits, TAllocator>* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data);\
    template bool InputTextMultiline<TChar, TTraits, TAllocator>(const char* label, std::basic_string<TChar, TTraits, TAllocator>* str, const ImVec2& size, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data);\
    template bool InputTextWithHint<TChar, TTraits, TAllocator>(const char* label, const char* hint, std::basic_string<TChar, TTraits, TAllocator>* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data);\

#define TEMPLATIZE_TEXT_STR(TString) TEMPLATIZE_TEXT_IMPL(TString::value_type, TString::traits_type, TString::allocator_type)

    TEMPLATIZE_TEXT_STR(std::string);
    TEMPLATIZE_TEXT_STR(Nightjar::string_t);

#pragma endregion Input Text STL

    void Image(const Nightjar::Texture* texture, const Nightjar::Vec2& size)
    {
        ImGui::Image((ImTextureID)(intptr_t)texture->id_, size);
    }

    // Based on imgui_demo.cpp
    bool CreateHoverTooltip(string_view_arg description)
    {
        ImGui::SameLine();
        ImGui::TextDisabled("(?)");
        bool hovered = ImGui::IsItemHovered();
        if (hovered)
        {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(description.data(), description.data() + description.size());
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
        return hovered;
    }

}	// namespace
