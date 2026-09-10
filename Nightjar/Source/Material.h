//------------------------------------------------------------------------------
//
// File Name:	Material.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Material encapsulator.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "GraphicsSystem.h"

#include "GraphicsStopwatch.h"
#include "MaterialRenderSettings.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    // Forward Declarations:
    class Texture;
    class Sampler;
    class Shader;
    class Material;

    // Typedefs:
    typedef LibraryStatic<Material> MaterialLibrary;

    // Class Definition:
    class Material : public NamedObject, public MaterialRenderSettings
    {
        friend class ModelData;
        friend class Mesh;
        friend class ParticleBatchMesh;

        // Public Constants, Enums, and Structs:
    public:
        //
        DECLARE_READ_KEY(Material);

        //
        DECLARE_READ_DIR(DIR_DATA"Materials");

        static const char MatrixWorld[], MatrixWorldNormal[];

        // Constructors/Destructors:
    public:
        Material(void);

        Material(const Material& other);

        Material(Material&& other) noexcept;

        Material& operator=(const Material& other);

        ~Material(void);

        // Public Static Functions:
    public:

        // Public Functions:
    public:
        /// <summary>
        /// 
        /// </summary>
        /// <param name="stream"></param>
        void Read(StreamIn& stream);

        /// <summary>
        /// 
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="name"></param>
        /// <param name="value"></param>
        template<typename T>
        void SetUniform(const string_t& name, const T& value) requires (!std::is_pointer_v<T>);

        /// <summary>
        /// 
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="name">The name of the uniform.</param>
        /// <param name="value">The address of the value(s) to write.</param>
        /// <param name="count">How many elements to write.</param>
        /// <param name="startIndex">The starting index of the destination array to write to.</param>
        template<typename T>
        void SetUniform(const string_t& name, const T* value, size_t count = 1, size_t startIndex = 0);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="name"></param>
        /// <param name="value"></param>
        void SetTexture(const string_t& name, const Texture* value);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="name"></param>
        /// <param name="value"></param>
        void SetSampler(const string_t& name, const Sampler* value);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="name"></param>
        /// <param name="value"></param>
        void SetTextureSampler(const string_t& name, const Texture* texture, const Sampler* sampler);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="shader"></param>
        void SetShader(const Shader* shader);

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        const Shader* GetShader() const;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="worldMtx"></param>
        void Activate(const Mat4& worldMtx) const;

        /// <summary>
        /// 
        /// </summary>
        void Deactivate(void) const;

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        bool HasOutput(void) const;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="size"></param>
        void SetOutputSize(size_t size);

        /// <summary>
        /// 
        /// </summary>
        /// <param name=""></param>
        /// <returns></returns>
        nsec_t GetLastRenderTime(void) const;

        /// <summary>
        /// 
        /// </summary>
        void DisplayIMGUI(void);

        // Public Event Handlers
    public:

        // Private Constants, Enums, and Structs:
    private:

        struct TexSampler
        {
            const Texture* texture;
            const Sampler* sampler;
        };

        // Private Static Functions:
    private:

        // Private Functions:
    private:
        /// <summary>
        /// 
        /// </summary>
        /// <param name="worldMtx"></param>
        void SetUniforms(const Mat4& worldMtx) const;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="name">The name of the uniform.</param>
        /// <param name="value">The address of the value(s) to write.</param>
        /// <param name="offset">The offset into the uniform to start writing.</param>
        /// <param name="size">The amount of data to write, in bytes.</param>
        void SetUniformInternal(const string_t& name, const void* value, long long offset, long long size);

        //
        const ResourceID& GetOutputBuffer(void) const;

        //
        ResourceID& GetOutputBufferEdit(void) const;

        // Private Static Variables:
    private:

        // Private Variables:
    private:
        const Shader* shader_;
        map_t<string_t, TexSampler> uniformTextures_;
        int uniformsIndex_;
        unsigned outputSize_;
        mutable GraphicsStopwatch timer_;
    };

}	// namespace

#include "Material.inl"
