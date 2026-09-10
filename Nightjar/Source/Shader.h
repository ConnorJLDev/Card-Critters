//------------------------------------------------------------------------------
//
// File Name:	Shader.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Shader encapsulator.
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
	class Shader;

	// Typedefs:
	typedef LibraryStatic<Shader> ShaderLibrary;

	// Class Definition:
	class Shader : public NamedObject
	{
		friend class Material;
		friend class LightSource;
		friend class GraphicsSystem_;
		friend class ModelData;
		friend class ShaderBufferDescriptor;

		// Public Constants, Enums, and Structs:
	public:
		//
		DECLARE_READ_KEY(Shader);

		//
		DECLARE_READ_DIR(DIR_DATA"Shaders");

        //
        enum class BufferUsage : unsigned char
        {
            Material,   // Data per material, changed frequently.
            Asset,      // Data per asset, usually only set once (by the asset).
            Global,     // Data used globally, usually only changed per frame.
            Output,     // Data that is written by the shader.
            NUM_ENTRIES,
        };

        ENUM_MAPS_CLASS_SEQUENTIAL_DECLARE(BufferUsage);

        // Properties of a given attribute in a shader.
        struct AttributeInfo
        {
            string_t name; // Name of the attribute.
            int index; // Index of the attribute in the shader's attribute list.
            int location; // Bind location of the attribute.
            int arraySize; // Array size.
            DataType type; // Type of data attribute expects.
            AttributeInfo(void) = delete;
            AttributeInfo(const Shader& shader, unsigned attributeIndex);
            AttributeInfo(const AttributeInfo& other) = delete;
            AttributeInfo(AttributeInfo&& other) noexcept;
            AttributeInfo& operator=(const AttributeInfo& other) = delete;
            string_t Print(void) const;
        };

        // Properties of a given uniform in a shader.
        struct UniformInfo
        {
            string_t name; // Name of the uniform.
            int indexGiven; // Requested index of the uniform in the shader's uniform list.
            int index; // Returned index of the uniform in the shader's uniform list.
            int location; // 
            int block; // Index of its uniform block (or -1 if not in a block)
            int offset; // Offset into its uniform block (or -1 if not in a block)
            int arraySize; // 
            DataType type; // Type of data in the uniform.
            UniformInfo(void) = delete;
            UniformInfo(const Shader& shader, unsigned uniformIndex);
            UniformInfo(const UniformInfo& other) = delete;
            UniformInfo(UniformInfo&& other) noexcept;
            UniformInfo& operator=(const UniformInfo& other) = delete;
            string_t Print(void) const;
        };

        // Properties of a given uniform block (or uniform buffer object) in a shader.
        struct UniformBlockInfo
        {
            string_t name;
            int binding;
            int size;
            int index;
            BufferUsage usage;
            UniformBlockInfo(const Shader& shader, unsigned uniformBlockIndex, const string_t& name, BufferUsage usage);
            string_t Print(void) const;
        };

        //
        struct UniformBlockInfoGlobal : public UniformBlockInfo
        {
            // Global uniform blocks need a map of their contained uniforms in order to be accessed independently of a shader.
            // Normal uniform blocks can depend on their shader to provide relevant information.
            unordered_map_t<string_t, int> offsets;
            UniformBlockInfoGlobal(const Shader& shader, unsigned uniformBlockIndex, const string_t& name);
        };

        //
        struct UniformBlockInfoOutput : public UniformBlockInfo
        {
            enum class Mode : unsigned char
            {
                Separate,
                Interleaved,
            };
            struct Element
            {
                string_t name;
                DataType type;
                int index;
                int size;
            };
            vector_t<Element> elements;
            unordered_map_t<string_t, unsigned> elementsMap;
            unsigned int count; // Number of variables captured in output.
            Mode mode; // Mode the variables are captured in.
            UniformBlockInfoOutput(const Shader& shader, unsigned uniformBlockIndex);
            string_t Print(void) const;
        };

        //
        const UniformInfo* GetUniform(const string_t& name) const;

		// Constructors/Destructors:
	public:
		Shader(void);

		Shader(const Shader& other);

		~Shader(void);

		// Public Static Functions:
	public:

		// Public Functions:
	public:
		//
		void Read(StreamIn& stream);

        //
        bool HasOutput(void) const;

		// Public Event Handlers
	public:

		// Private Constants, Enums, and Structs:
	private:
        //
        enum class StageType : unsigned char
        {
            Vertex,
            Geometry,
            Fragment,
            NUM_ENTRIES,
        };

		//
		struct MaterialBuffers
		{
			vector_t<ResourceID> uniforms_;
			vector_t<bool> uniformsAvailable_;
			unsigned uniformsUsed_;
			const Shader& shader_;
			MaterialBuffers() = delete;
			MaterialBuffers(const Shader& shader);
            MaterialBuffers& operator=(const MaterialBuffers& other) = delete;
			~MaterialBuffers();
			//
			int GetMaterialBuffers();

			//
			void RemoveMaterialBuffers(int buffersIndex);

            //
            void SetOutputSize(int buffersIndex, size_t size);
		};

        //
        struct ShaderParser
        {
            vector_t<const char*>& sources;
            vector_t<int>& sourceLengths;
            vector_t<string_t>& outputs;
            vector_t<string_t>& depths;
            string_t versionNum, versionType, version, definitions;
            StageType stage;
            ShaderParser(vector_t<const char*>& sources, vector_t<int>& sourceLengths, vector_t<string_t>& outputs, vector_t<string_t>& depths, StageType stage);
            ShaderParser(const ShaderParser& other) = delete;
            ShaderParser& operator=(const ShaderParser& other) = delete;
        };

		// The starting index of bindings that aren't global.
		static const ResourceID InstanceBindStart;

		// The name of the global binding for viewport data.
		static const StreamKey GlobalBindViewportName, GlobalBindLightingName;

		//
		static const StreamKey StageVertex, StageGeometry, StageFragment;
        
		// Private Static Functions:
	private:

        //
        static const map_t<const ResourceID, const DataType>& TypeIdMap(void);

		static const string_t& GetCommon(const string_t& commonName);

		static string_t ReadCommon(const string_t& commonName);

        static string_t ReadShaderStream(ShaderParser& parser, std::istream& shaderFile);

		static void DeleteAllStages();

		static void PrintShaderError(ResourceID shaderID, const string_t& action, const string_t& name);

		// Private Functions:
	private:
		//
		void ReadRaw(const string_t& vertex, const string_t& fragment, const string_t& geometry = "");

        ResourceID GetStage(StageType stageType, const string_t& stageName);

        ResourceID CompileStage(StageType stageType, const string_t& stageName);

		//
		void ParseShader();

		//
		void ParseAttributes();

		//
		void ParseUniforms();

		//
		void ParseUniformBufferObjects();

		//
		void ParseOutputBuffers();

		// Private Static Variables:
	private:
		static array_t<map_t<string_t, ResourceID>*, StageType::NUM_ENTRIES>& CompiledStages();
		static map_t<string_t, UniformBlockInfoGlobal>& GlobalUBOs();
		static map_t<string_t, BufferUsage>& BufferUsages();
		static vector_t<string_t>& GlobalDefinitions();

		// Common shader files used via "#include"
		static map_t<string_t, string_t>& Commons();

		// Private Variables:
	private:
        vector_t<AttributeInfo> inputAttrs_;
        vector_t<UniformInfo> uniforms_;
		vector_t<UniformBlockInfo> uniformBlocks_;
        map_t<string_t, unsigned> uniformsMap_;
        vector_t<string_t> outputNames_; 
        vector_t<UniformBlockInfoOutput> outputs_;
		MaterialBuffers* materialBuffers_;
		ResourceID id_;
        unsigned int bufferCount_;
	};

}	// namespace
