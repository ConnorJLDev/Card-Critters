//------------------------------------------------------------------------------
//
// File Name:	GraphicsSystem.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		System for handling graphics assets and interfaces.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#define GRAPHICS_INCLUDE <glad/glad.h>

#define GraphicsDebugCallback

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    // Forward Declarations:
    class BaseSystem;
    class Material;
    class Texture;
    class Sampler;
    class Mesh;
    class Shader;
    class Armature;
    class CameraSource;
    
    // Typedefs:
    typedef unsigned int ResourceID;
    typedef unsigned int ResourceEnum;

    constexpr const ResourceID ResourceUnbind{ 0 };

#ifndef GraphicsDebugCallback
#define OpenGLCheckError() GraphicsSystem::CheckError(GetFilePathRelative())
#else
#define OpenGLCheckError()
#endif

    // Class Definition:
    class GraphicsSystem
    {
        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    private:
        GraphicsSystem(void) = delete;

        // Public Static Functions:
    public:
        /// <summary>
        /// Gets the instance of the GraphicsSystem.
        /// </summary>
        /// <returns>A pointer to the system.</returns>
        static BaseSystem* GetInstance(void);

        /// <summary>
        /// Gets the armature from the library with the requested name.
        /// </summary>
        /// <param name="name">The name of the armature.</param>
        /// <returns>A pointer to the requested armature, or nullptr if not found.</returns>
        static const Armature* GetArmature(string_view_arg name);

        /// <summary>
        /// Constructs a modifiable armature from the library with the requested name.
        /// The new armature is stored in a container for asset tracking.
        /// </summary>
        /// <param name="name">The name of the armature to build from.</param>
        /// <returns>A pointer to the newly built armature, or nullptr on failure.</returns>
        static Armature* BuildArmature(string_view_arg name);

        /// <summary>
        /// Constructs a modifiable armature from the given other armature.
        /// The new armature is stored in a container for asset tracking.
        /// </summary>
        /// <param name="other">The armature to build from.</param>
        /// <returns>A pointer to the newly built armature, or nullptr on failure.</returns>
        static Armature* BuildArmature(const Armature* other);

        /// <summary>
        /// Destroys an armature and removes it from the container used for asset tracking.
        /// </summary>
        /// <param name="armature">The armature to destroy.</param>
        /// <returns>True if the armature was successfully destroyed, false otherwise.</returns>
        static bool DestroyArmature(Armature*& armature);

        /// <summary>
        /// Gets the material from the library with the requested name.
        /// </summary>
        /// <param name="name">The name of the material.</param>
        /// <returns>A pointer to the requested material, or nullptr if not found.</returns>
        static const Material* GetMaterial(string_view_arg name);

        /// <summary>
        /// Constructs a modifiable material from the library with the requested name.
        /// The new material is stored in a container for asset tracking.
        /// </summary>
        /// <param name="name">The name of the material to build from.</param>
        /// <returns>A pointer to the newly built material, or nullptr on failure.</returns>
        static Material* BuildMaterial(string_view_arg name);

        /// <summary>
        /// Constructs a modifiable material from the given other material.
        /// The new material is stored in a container for asset tracking.
        /// </summary>
        /// <param name="other">The material to build from.</param>
        /// <returns>A pointer to the newly built material, or nullptr on failure.</returns>
        static Material* BuildMaterial(const Material* other);

        /// <summary>
        /// Destroys an material and removes it from the container used for asset tracking.
        /// </summary>
        /// <param name="material">The material to destroy.</param>
        /// <returns>True if the material was successfully destroyed, false otherwise.</returns>
        static bool DestroyMaterial(Material*& material);

        /// <summary>
        /// Gets the mesh from the library with the requested name.
        /// </summary>
        /// <param name="name">The name of the mesh.</param>
        /// <returns>A pointer to the requested mesh, or nullptr if not found.</returns>
        static const Mesh* GetMesh(string_view_arg name);

        /// <summary>
        /// Constructs a modifiable mesh from the library with the requested name.
        /// The new mesh is stored in a container for asset tracking.
        /// </summary>
        /// <param name="name">The name of the mesh to build from.</param>
        /// <returns>A pointer to the newly built mesh, or nullptr on failure.</returns>
        static Mesh* BuildMesh(string_view_arg name);

        /// <summary>
        /// Constructs a modifiable mesh from the given other mesh.
        /// The new mesh is stored in a container for asset tracking.
        /// </summary>
        /// <param name="other">The mesh to build from.</param>
        /// <returns>A pointer to the newly built mesh, or nullptr on failure.</returns>
        static Mesh* BuildMesh(const Mesh* other);

        /// <summary>
        /// Destroys an mesh and removes it from the container used for asset tracking.
        /// </summary>
        /// <param name="mesh">The mesh to destroy.</param>
        /// <returns>True if the mesh was successfully destroyed, false otherwise.</returns>
        static bool DestroyMesh(Mesh*& mesh);

        /// <summary>
        /// Gets the shader from the library with the requested name.
        /// </summary>
        /// <param name="name">The name of the shader.</param>
        /// <returns>A pointer to the requested shader, or nullptr if not found.</returns>
        static const Shader* GetShader(string_view_arg name);

        /// <summary>
        /// Constructs a modifiable shader from the library with the requested name.
        /// The new shader is stored in a container for asset tracking.
        /// </summary>
        /// <param name="name">The name of the shader to build from.</param>
        /// <returns>A pointer to the newly built shader, or nullptr on failure.</returns>
        static Shader* BuildShader(string_view_arg name);

        /// <summary>
        /// Constructs a modifiable shader from the given other shader.
        /// The new shader is stored in a container for asset tracking.
        /// </summary>
        /// <param name="other">The shader to build from.</param>
        /// <returns>A pointer to the newly built shader, or nullptr on failure.</returns>
        static Shader* BuildShader(const Shader* other);

        /// <summary>
        /// Destroys an shader and removes it from the container used for asset tracking.
        /// </summary>
        /// <param name="shader">The shader to destroy.</param>
        /// <returns>True if the shader was successfully destroyed, false otherwise.</returns>
        static bool DestroyShader(Shader*& shader);

        /// <summary>
        /// Gets the texture from the library with the requested name.
        /// </summary>
        /// <param name="name">The name of the texture.</param>
        /// <returns>A pointer to the requested texture, or nullptr if not found.</returns>
        static const Texture* GetTexture(string_view_arg name);

        /// <summary>
        /// Constructs a modifiable texture from the library with the requested name.
        /// The new texture is stored in a container for asset tracking.
        /// </summary>
        /// <param name="name">The name of the texture to build from.</param>
        /// <returns>A pointer to the newly built texture, or nullptr on failure.</returns>
        static Texture* BuildTexture(string_view_arg name);

        /// <summary>
        /// Constructs a modifiable texture from the given other texture.
        /// The new texture is stored in a container for asset tracking.
        /// </summary>
        /// <param name="other">The texture to build from.</param>
        /// <returns>A pointer to the newly built texture, or nullptr on failure.</returns>
        static Texture* BuildTexture(const Texture* other);

        /// <summary>
        /// Destroys an texture and removes it from the container used for asset tracking.
        /// </summary>
        /// <param name="texture">The texture to destroy.</param>
        /// <returns>True if the texture was successfully destroyed, false otherwise.</returns>
        static bool DestroyTexture(Texture*& texture);

        /// <summary>
        /// Gets the sampler from the library with the requested name.
        /// </summary>
        /// <param name="name">The name of the sampler.</param>
        /// <returns>A pointer to the requested sampler, or nullptr if not found.</returns>
        static const Sampler* GetSampler(string_view_arg name);

        /// <summary>
        /// Constructs a modifiable sampler from the library with the requested name.
        /// The new sampler is stored in a container for asset tracking.
        /// </summary>
        /// <param name="name">The name of the sampler to build from.</param>
        /// <returns>A pointer to the newly built sampler, or nullptr on failure.</returns>
        static Sampler* BuildSampler(string_view_arg name);

        /// <summary>
        /// Constructs a modifiable sampler from the given other sampler.
        /// The new sampler is stored in a container for asset tracking.
        /// </summary>
        /// <param name="other">The sampler to build from.</param>
        /// <returns>A pointer to the newly built sampler, or nullptr on failure.</returns>
        static Sampler* BuildSampler(const Sampler* other);

        /// <summary>
        /// Destroys an sampler and removes it from the container used for asset tracking.
        /// </summary>
        /// <param name="sampler">The sampler to destroy.</param>
        /// <returns>True if the sampler was successfully destroyed, false otherwise.</returns>
        static bool DestroySampler(Sampler*& texture);

        /// <summary>
        /// Clears the render buffer and fills it with ClearColor.
        /// </summary>
        static void ClearBackground(void);

        /// <summary>
        /// Get the color used when clearing the background.
        /// </summary>
        /// <returns>The color used for clearing the background.</returns>
        static const Vec4& GetClearColor(void);

        /// <summary>
        /// Set the color to use when clearing the background.
        /// </summary>
        /// <param name="color">The new color to clear the background with.</param>
        static void SetClearColor(const Vec4& color);

        /// <summary>
        /// Updates the rendering data to use the given camera.
        /// </summary>
        /// <param name="camera">The camera to use.</param>
        static void SetActiveCamera(CameraSource* camera);

        /// <summary>
        /// Draws a mesh with a given material.
        /// </summary>
        /// <param name="transformMatrix">The transform to use when drawing.</param>
        /// <param name="mesh">The mesh to draw with.</param>
        /// <param name="material">The material to render onto the mesh.</param>
        static void DrawCall(const Mat4& transformMatrix, const Mesh& mesh, const Material& material);

        /// <summary>
        /// Draws a bordered rectangle.
        /// </summary>
        /// <param name="transformMatrix">The transform to use when drawing.</param>
        /// <param name="size">The width and height of the rectangle.</param>
        /// <param name="colorFill">The fill (inside) color of the rectangle.</param>
        /// <param name="colorBorder">The border (outside) color of the rectangle.</param>
        /// <param name="borderSize">The fractional amount the border fills into the rectangle. 0.0 is no border, 0.5 is 50% border, and 1.0 is all border.</param>
        static void DrawRect(const Mat4& transformMatrix, const Vec2& size, const Vec4& colorFill = Vec4(1, 0, 1, 1), const Vec4& colorBorder = Vec4(0, 1, 0, 1), float borderSize = 0.2f);

        /// <summary>
        /// Draws a bordered oval.
        /// </summary>
        /// <param name="transformMatrix">The transform to use when drawing.</param>
        /// <param name="radii">The radii of the oval.</param>
        /// <param name="colorFill">The fill (inside) color of the oval.</param>
        /// <param name="colorBorder">The border (outside) color of the oval.</param>
        /// <param name="borderSize">The fractional amount the border fills into the oval. 0.0 is no border, 0.5 is 50% border, and 1.0 is all border.</param>
        static void DrawOval(const Mat4& transformMatrix, const Vec2& radii, const Vec4& colorFill = Vec4(1, 0, 1, 1), const Vec4& colorBorder = Vec4(0, 1, 0, 1), float borderSize = 0.2f);

        /// <summary>
        /// Draws a bordered circle.
        /// </summary>
        /// <param name="transformMatrix">The transform to use when drawing.</param>
        /// <param name="radius">The radius of the circle.</param>
        /// <param name="colorFill">The fill (inside) color of the circle.</param>
        /// <param name="colorBorder">The border (outside) color of the circle.</param>
        /// <param name="borderSize">The fractional amount the border fills into the circle. 0.0 is no border, 0.5 is 50% border, and 1.0 is all border.</param>
        static void DrawCircle(const Mat4& transformMatrix, float radius, const Vec4& colorFill = Vec4(1, 0, 1, 1), const Vec4& colorBorder = Vec4(0, 1, 0, 1), float borderSize = 0.2f);

        /// <summary>
        /// Checks if an error flag is set in the underlying graphics library.
        /// </summary>
        /// <param name="file">The file location of where this is called, for improved tracing.</param>
        static void CheckError(string_view_arg file);

    public:

        // Public Event Handlers
    public:

        // Private Constants, Enums, and Structs:
    private:

        // Private Static Functions:
    private:

        // Private Functions:
    private:

        // Private Static Variables:
    private:

        // Private Variables:
    private:
    };

}	// namespace
