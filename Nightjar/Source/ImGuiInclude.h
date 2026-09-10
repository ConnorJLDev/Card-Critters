//------------------------------------------------------------------------------
//
// File Name:	ImGuiInclude.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Includes commonly used ImGui headers and defines implicit
//              cast/construction functions with the math library in use.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Preprocessor Directives:
//------------------------------------------------------------------------------

#define IM_VEC2_CLASS_EXTRA\
    constexpr ImVec2(const Nightjar::Vec2& vec)\
        : x(vec.x)\
        , y(vec.y)\
        {\
        }\
    operator Nightjar::Vec2() const\
    {\
        return Nightjar::Vec2(x, y);\
    }\

#define IM_VEC3_CLASS_EXTRA\
    constexpr ImVec3(const Nightjar::Vec3& vec)\
        : x(vec.x)\
        , y(vec.y)\
        , z(vec.z)\
        {\
        }\
    operator Nightjar::Vec3() const\
    {\
        return Nightjar::Vec3(x, y, z);\
    }\

#define IM_VEC4_CLASS_EXTRA\
    constexpr ImVec4(const Nightjar::Vec4& vec)\
        : x(vec.x)\
        , y(vec.y)\
        , z(vec.z)\
        , w(vec.w)\
        {\
        }\
    operator Nightjar::Vec4() const\
    {\
        return Nightjar::Vec4(x, y, z, w);\
    }\

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

#include "ImGuiCommon.h"

#include "ImGuiContextData.h"
