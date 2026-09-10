//------------------------------------------------------------------------------
//
// File Name:	Mesh.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Mesh encapsulator.
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

#include "DataTypes.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    // Forward Declarations:
    class Mesh;

    // Typedefs:
    typedef LibraryStatic<Mesh> MeshLibrary;

    // Class Definition:
    class Mesh : public NamedObject
    {
        friend class GraphicsSystem_;
        friend class ModelData;
        friend class ParticleBatchMesh;

        // Public Constants, Enums, and Structs:
    public:
        //
        DECLARE_READ_KEY(Mesh);

        //
        DECLARE_READ_DIR(DIR_DATA"Meshes");

        //
        typedef uint32_t VertexIndex_t;
        typedef uint32_t InstanceIndex_t;

        // Constructors/Destructors:
    public:
        Mesh(void);
        
        Mesh(string_view_arg name);

        Mesh(const Mesh& other);

        Mesh(Mesh&& other) noexcept;

        Mesh& operator=(const Mesh& other);

        ~Mesh(void);

        // Public Static Functions:
    public:

        // Public Functions:
    public:
        //
        void Read(StreamIn& stream);

        //
        void Draw() const;

        //
        void Draw(InstanceIndex_t instances) const;

        //
        void DisplayIMGUI();

        // Public Event Handlers
    public:

        // Private Constants, Enums, and Structs:
    protected:

        //
        enum class DrawType : unsigned char
        {
            Invalid = (unsigned char)-1,
            Points = 0,
            Lines,
            LineStrip,
            Triangles,
            TriangleStrip,
            TriangleFan,
            NUM_ENTRIES,
        };

        //
        enum class Attributes : unsigned short
        {
            None = 0,
            Position = 1 << 0,
            Normal = 1 << 1,
            TexCoord0 = 1 << 2,
            TexCoord1 = 1 << 3,
            TexCoord2 = 1 << 4,
            TexCoord3 = 1 << 5,
            Color0 = 1 << 6,
            Color1 = 1 << 7,
            DeformIndex = 1 << 8,
            DeformWeight = 1 << 9,
        };

        //
        struct Attribute
        {
            DataType dataType;
            unsigned char dataCount;
            unsigned char dataOffset;
            unsigned char index;
            bool normalized;
            Attributes attribute;
            void* data;
            static const array_t<ResourceEnum, DataType::NUM_ENTRIES_ATOMIC> DataTypesInternal;
            Attribute();
            Attribute(const Attribute& other) = delete;
            Attribute& operator=(const Attribute& other) = delete;
            Attribute(Attribute&& other) noexcept;
            Attribute(DataType dataType, unsigned dataCount = 1);
            ~Attribute();
            void Read(StreamIn& stream, Attributes& meshAttributes);
            void SetData(Mesh& mesh);
            unsigned char Size() const;
            bool DisplayIMGUIElement(size_t index);
        private:
            static const StreamKey ReadKeyType, ReadKeyCount, ReadKeyData, ReadKeyNormalized;
        };

        //
        struct Primitive
        {
            DrawType type;
            ResourceEnum typeRaw;
            ResourceEnum indexType;
            VertexIndex_t vertexCount;
            ResourceID indexID;
            vector_t<VertexIndex_t> indices;
            static const VertexIndex_t ResetIndex;
            static const array_t<ResourceEnum, DrawType::NUM_ENTRIES> PrimitiveTypes;
            //
            static const StreamKey ReadKeyType, ReadKeyIndices;
            Primitive();
            Primitive(DrawType drawType);
            Primitive(DrawType drawType, const vector_t<VertexIndex_t>& indices);
            Primitive(const Primitive& other) = delete;
            Primitive& operator=(const Primitive& other) = delete;
            Primitive(Primitive&& other) noexcept;
            ~Primitive();
            void GenerateBuffer();
            void BindBuffer() const;
            bool DeleteBuffer();
            void SetDrawType(DrawType drawType);
            void SetIndices(const vector_t<VertexIndex_t>& indices);
            bool Merge(Primitive& other);
            void Draw() const;
            void Draw(InstanceIndex_t instances) const;
            void Read(StreamIn& stream);
        };

        //
        static const StreamKey ReadKeyVertCount, ReadKeyDrawType, ReadKeyImmediateDrawCount, ReadKeyStoreMain, ReadKeyAttributes, ReadKeyPrimitives;

        ENUM_MAPS_CLASS_SEQUENTIAL_DECLARE(DrawType);

        ENUM_MAPS_CLASS_DECLARE(Attributes);

        class PrimitiveWrapper;

        // Private Static Functions:
    protected:
        ENUM_DECLARE_OPS(Attributes);

        //
        static void UnbindVertexDescriptor(void);

        //
        static void UnbindVertexBuffer(void);

        // Private Functions:
    protected:
        //
        void CreateVertexDescriptor(void);
        
        //
        void CreateVertexBuffer(void);
        
        //
        void InitializeVertexBuffer(void);

        //
        void RefreshVertexDescriptor(void);

        //
        void ApplyVertexBuffer(void);

        //
        void RetrieveVertexBuffer(void);

        //
        bool DestroyVertexDescriptor(void);
        
        //
        bool DestroyVertexBuffer(void);
        
        //
        void BindVertexDescriptor(void) const;
        
        //
        void BindVertexBuffer(void) const;
        
        //
        void CalculateAttributes(void);

        //
        void SetVertexData(void);

        //
        void Resize(size_t size, bool resizeLocal = true);

        //
        template<typename T>
        void SetData(const Attribute& attr, const T* data);

        //
        template<typename T>
        void SetData(const Attribute& attr, const T& data, unsigned vertIndex);

        //
        void SetDataInternal(const Attribute& attr, const void* data, size_t size, unsigned vertIndex);

        //
        void SetDataDirect(const Attribute& attr, const void* data, size_t size, unsigned vertIndex);

        // Private Static Variables:
    protected:

        // Private Variables:
    protected:
        // The ID for the vertex buffer associated with the mesh
        ResourceID idVertexBuffer_;
        //
        ResourceID idVertexDescriptor_;
        // The number of vertices in the mesh
        VertexIndex_t vertexCount_;
        //
        VertexIndex_t immediateDrawCount_;
        //
        unsigned char vertexSize_;
        //
        DrawType immediateDrawType_;
        //
        Attributes attributeFlags_;
        //
        void* vertexBuffer_;
        //
        vector_t<Primitive> primitives_;
        //
        vector_t<Attribute> attributes_;
    };

}	// namespace
