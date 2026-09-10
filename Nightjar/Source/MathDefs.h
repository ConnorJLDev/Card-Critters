//------------------------------------------------------------------------------
//
// File Name:	MathDefs.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Provides typedefs and wrappers around vectors and matrices.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

//#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_decompose.hpp>

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    // Typedefs:
    typedef glm::vec1 Vec1;
    typedef glm::vec2 Vec2;
    typedef glm::vec3 Vec3;
    typedef glm::vec4 Vec4;
    typedef glm::dvec1 DVec1;
    typedef glm::dvec2 DVec2;
    typedef glm::dvec3 DVec3;
    typedef glm::dvec4 DVec4;
    typedef glm::i8vec1 I8Vec1;
    typedef glm::i8vec2 I8Vec2;
    typedef glm::i8vec3 I8Vec3;
    typedef glm::i8vec4 I8Vec4;
    typedef glm::u8vec1 U8Vec1;
    typedef glm::u8vec2 U8Vec2;
    typedef glm::u8vec3 U8Vec3;
    typedef glm::u8vec4 U8Vec4;
    typedef glm::i16vec1 I16Vec1;
    typedef glm::i16vec2 I16Vec2;
    typedef glm::i16vec3 I16Vec3;
    typedef glm::i16vec4 I16Vec4;
    typedef glm::u16vec1 U16Vec1;
    typedef glm::u16vec2 U16Vec2;
    typedef glm::u16vec3 U16Vec3;
    typedef glm::u16vec4 U16Vec4;
    typedef glm::i32vec1 I32Vec1;
    typedef glm::i32vec2 I32Vec2;
    typedef glm::i32vec3 I32Vec3;
    typedef glm::i32vec4 I32Vec4;
    typedef glm::u32vec1 U32Vec1;
    typedef glm::u32vec2 U32Vec2;
    typedef glm::u32vec3 U32Vec3;
    typedef glm::u32vec4 U32Vec4;
    typedef glm::i64vec1 I64Vec1;
    typedef glm::i64vec2 I64Vec2;
    typedef glm::i64vec3 I64Vec3;
    typedef glm::i64vec4 I64Vec4;
    typedef glm::u64vec1 U64Vec1;
    typedef glm::u64vec2 U64Vec2;
    typedef glm::u64vec3 U64Vec3;
    typedef glm::u64vec4 U64Vec4;
    typedef I32Vec1 IVec1;
    typedef I32Vec2 IVec2;
    typedef I32Vec3 IVec3;
    typedef I32Vec4 IVec4;
    typedef U32Vec1 UVec1;
    typedef U32Vec2 UVec2;
    typedef U32Vec3 UVec3;
    typedef U32Vec4 UVec4;
    typedef glm::bvec1 BVec1;
    typedef glm::bvec2 BVec2;
    typedef glm::bvec3 BVec3;
    typedef glm::bvec4 BVec4;
    typedef glm::mat2 Mat2;
    typedef Mat2 Mat2x2;
    typedef glm::mat2x3 Mat2x3;
    typedef glm::mat2x4 Mat2x4;
    typedef glm::mat3x2 Mat3x2;
    typedef glm::mat3 Mat3;
    typedef Mat3 Mat3x3;
    typedef glm::mat3x4 Mat3x4;
    typedef glm::mat4x2 Mat4x2;
    typedef glm::mat4x3 Mat4x3;
    typedef glm::mat4 Mat4;
    typedef Mat4 Mat4x4;
    typedef glm::dmat2 DMat2;
    typedef DMat2 DMat2x2;
    typedef glm::dmat2x3 DMat2x3;
    typedef glm::dmat2x4 DMat2x4;
    typedef glm::dmat3x2 DMat3x2;
    typedef glm::dmat3 DMat3;
    typedef DMat3 DMat3x3;
    typedef glm::dmat3x4 DMat3x4;
    typedef glm::dmat4x2 DMat4x2;
    typedef glm::dmat4x3 DMat4x3;
    typedef glm::dmat4 DMat4;
    typedef DMat4 DMat4x4;
    typedef glm::quat Quat;

}	// namespace
