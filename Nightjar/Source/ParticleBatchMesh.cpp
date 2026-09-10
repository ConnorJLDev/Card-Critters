//------------------------------------------------------------------------------
//
// File Name:	ParticleBatchMesh.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class that manages a mesh representing particles.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ParticleBatchMesh.h"

#include "GraphicsSystem.h"

#include GRAPHICS_INCLUDE

#include "Mesh.inl"
#include "Shader.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
	//--------------------------------------------------------------------------
	// Public Constants:
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// Public Static Variables:
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// Public Variables:
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// Private Constants:
	//--------------------------------------------------------------------------

    struct ParticleOffset
    {
        enum _ParticleOffset : unsigned int
        {
            Position,
            Rotation,
            Scale_Drag,
            Velocity_DragRotational,
            Acceleration_LifeTime,
            RotationalVelocity,
            NUM_ENTRIES,
        };
    };

	//--------------------------------------------------------------------------
	// Private Static Variables:
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// Private Variables:
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// Constructors/Destructors:
	//--------------------------------------------------------------------------

#pragma region Constructors

    ParticleBatchMesh::ParticleBatchMesh(void)
		: Mesh()
        , mainQuery_(ResourceUnbind)
        , swapQuery_(ResourceUnbind)
        , swapVertArray_(ResourceUnbind)
        , swapVertDataLast_(ResourceUnbind)
        , mainVertDataLast_(ResourceUnbind)
	{
        attributes_.clear();
        attributes_.reserve(6);
        attributes_.emplace_back(DataType::Vec4);   // Position
        attributes_.emplace_back(DataType::Vec4);   // Rotation
        attributes_.emplace_back(DataType::Vec4);   // Scale_Drag
        attributes_.emplace_back(DataType::Vec4);   // Velocity_DragRotational
        attributes_.emplace_back(DataType::Vec4);   // Acceleration_LifeTime
        attributes_.emplace_back(DataType::Vec4);   // RotationalVelocity

        immediateDrawType_ = DrawType::Points;

        CalculateAttributes();

        CreateVertexDescriptor();
        CreateVertexBuffer();

        glGenVertexArrays(1, &swapVertArray_);
        OpenGLCheckError();

        glGenQueries(1, &mainQuery_);
        OpenGLCheckError();

        glGenQueries(1, &swapQuery_);
        OpenGLCheckError();
	}

	//--------------------------------------------------------------------------

    ParticleBatchMesh::~ParticleBatchMesh(void)
	{
        if (mainQuery_ != ResourceUnbind)
        {
            glDeleteQueries(1, &mainQuery_);
            OpenGLCheckError();
            mainQuery_ = ResourceUnbind;
        }
        if (swapQuery_ != ResourceUnbind)
        {
            glDeleteQueries(1, &swapQuery_);
            OpenGLCheckError();
            swapQuery_ = ResourceUnbind;
        }
        if (swapVertArray_ != ResourceUnbind)
        {
            glDeleteVertexArrays(1, &swapVertArray_);
            OpenGLCheckError();
            swapVertArray_ = ResourceUnbind;
        }
	}

#pragma endregion Constructors

	//--------------------------------------------------------------------------
	// Public Static Functions:
	//--------------------------------------------------------------------------

#pragma region Public Static Functions

    void ParticleBatchMesh::ApplyMeshToMaterial(const Mesh& mesh, Material& material)
    {
        // Combine the indices
        {
            vector_t<VertexIndex_t> indices{};
            for (const auto& primitive : mesh.primitives_)
            {
                switch (primitive.type)
                {
                case DrawType::Triangles:
                {
                    static constexpr unsigned TriStep = 3;
                    size_t sizeOld = indices.size();
                    size_t newIndicesCount = (size_t)primitive.vertexCount + (primitive.vertexCount / TriStep);
                    indices.reserve(sizeOld + newIndicesCount);
                    for (unsigned i = 0; i < (unsigned)primitive.vertexCount; i += TriStep)
                    {
                        indices.emplace_back(Primitive::ResetIndex);
                        auto insert = primitive.indices.begin() + i;
                        indices.insert(indices.cend(), insert, insert + TriStep);
                    }
                }
                break;
                case DrawType::TriangleStrip:
                {
                    size_t sizeOld = indices.size();
                    size_t newIndicesCount = (size_t)primitive.vertexCount + 1;
                    indices.reserve(sizeOld + newIndicesCount);
                    indices.emplace_back(Primitive::ResetIndex);
                    indices.insert(indices.cend(), primitive.indices.cbegin(), primitive.indices.cend());
                }
                break;
                default:
                    TraceWarning("Unable to add primitive of type {}", ENUM_WRITE_VALUE(DrawType, primitive.type));
                    break;
                }
            }
            // The indices always start with the reset index, so we offset by one.
            unsigned indicesCount = (unsigned)indices.size() - 1;
            material.SetUniform("instVertCount", indicesCount);
            material.SetUniform("instIndices[0]", indices.data() + 1, indicesCount);
        }
        // Set the attribute values
        {
            string_t uniformNameBuffer{};
            static constexpr unsigned reserveSize = _countof("insts[999].xxx");
            uniformNameBuffer.reserve(reserveSize);
            for (const auto& attribute : mesh.attributes_)
            {
#define APPLY_ATTRIBUTE(attrName, attrType)\
                for (unsigned i = 0; i < mesh.vertexCount_; ++i)\
                {\
                    uniformNameBuffer.clear();\
                    std::format_to(std::back_inserter(uniformNameBuffer), "insts[{}]." #attrName , i);\
                    material.SetUniform(uniformNameBuffer, reinterpret_cast<const attrType*>(attribute.data)[i]);\
                }\

                switch (attribute.attribute)
                {
                case Attributes::Position:
                    APPLY_ATTRIBUTE(pos, Vec3);
                    break;
                case Attributes::Normal:
                    APPLY_ATTRIBUTE(nrm, Vec3);
                    break;
                case Attributes::TexCoord0:
                    APPLY_ATTRIBUTE(tex, Vec2);
                    break;
                case Attributes::Color0:
                case Attributes::Color1:
                    APPLY_ATTRIBUTE(col, Vec4);
                    break;
                default:
                    break;
                }
            }
        }
    }

#pragma endregion Public Static Functions

	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

    typedef array_t<Vec4, ParticleOffset::NUM_ENTRIES> ParticleCompact_t;

    void ParticleBatchMesh::SetParticle(const Particle& particle, unsigned index)
    {
        const ParticleCompact_t props{{
            { particle.GetTranslation3D(), 1.0f },
            { particle.GetRotation3D(), 1.0f }, // Quat?
            { particle.GetScale3D(), particle.GetDrag() },
            { particle.GetVelocity3D(), particle.GetRotationalDrag() },
            { particle.GetAcceleration3D(), particle.GetLifeTime() },
            { particle.GetRotationalVelocity3D(), 1.0f }, // Quat?
        }};
        BindVertexBuffer();
        SetDataDirect(attributes_[0], &props, sizeof(ParticleCompact_t), index);
    }

    void ParticleBatchMesh::SetParticles(const size_t* particleIndices, unsigned count, const Particle* particles, unsigned index)
    {
        static vector_t<ParticleCompact_t>& buffer(StaticManagement::VectorWrapper<ParticleCompact_t>::Create());
        if (count > buffer.size())
        {
            buffer.resize(std::bit_ceil(count));
        }
        for (unsigned i = 0; i < count; ++i)
        {
            const Particle& particle = particles[particleIndices[i]];
            buffer[i] = {{
                { particle.GetTranslation3D(), 1.0f },
                { particle.GetRotation3D(), 1.0f }, // Quat?
                { particle.GetScale3D(), particle.GetDrag() },
                { particle.GetVelocity3D(), particle.GetRotationalDrag() },
                { particle.GetAcceleration3D(), particle.GetLifeTime() },
                { particle.GetRotationalVelocity3D(), 1.0f }, // Quat?
            }};
        }
        BindVertexBuffer();
        SetDataDirect(attributes_[0], buffer.data(), count * sizeof(ParticleCompact_t), index);
    }

    void ParticleBatchMesh::GetParticle(Particle& particle, unsigned index)
    {
        assert(index < vertexCount_);
        ParticleCompact_t props;
        static constexpr size_t propsSize = sizeof(ParticleCompact_t);
        glBindBuffer(GL_ARRAY_BUFFER, idVertexBuffer_);
        OpenGLCheckError();
        assert(vertexSize_ == propsSize);
        glGetBufferSubData(GL_ARRAY_BUFFER, static_cast<GLintptr>(index * propsSize), propsSize, props.data());
        OpenGLCheckError();
        particle.SetTranslation3D(Vec3(props[ParticleOffset::Position]));
        particle.SetRotation3D(Vec3(props[ParticleOffset::Rotation])); // Quat?
        particle.SetScale3D(Vec3(props[ParticleOffset::Scale_Drag]));
        particle.SetDrag(props[ParticleOffset::Scale_Drag].w);
        particle.SetVelocity3D(Vec3(props[ParticleOffset::Velocity_DragRotational]));
        particle.SetRotationalDrag(props[ParticleOffset::Velocity_DragRotational].w);
        particle.SetAcceleration3D(Vec3(props[ParticleOffset::Acceleration_LifeTime]));
        particle.SetLifeTime(props[ParticleOffset::Acceleration_LifeTime].w);
        particle.SetRotationalVelocity3D(Vec3(props[ParticleOffset::RotationalVelocity])); // Quat?
    }

    void ParticleBatchMesh::Resize(size_t size)
    {
        // Only resize if we're increasing.
        if (vertexCount_ >= size)
        {
            return;
        }
        Mesh::Resize(size, false);
        mainVertDataLast_ = idVertexBuffer_;
    }

    void ParticleBatchMesh::PreUpdate(void)
    {
        if (glIsQuery(swapQuery_))
        {
            GLuint primitivesWritten;
            glGetQueryObjectuiv(swapQuery_, GL_QUERY_RESULT, &primitivesWritten);
            OpenGLCheckError();

            immediateDrawCount_ = static_cast<decltype(immediateDrawCount_)>(primitivesWritten);
        }
    }

    void ParticleBatchMesh::Update(Material& batchUpdater)
    {
        // If we're using swapped data buffers, check if they're the same as before.
        if (mainVertDataLast_ != idVertexBuffer_)
        {
            glBindBuffer(GL_ARRAY_BUFFER, idVertexBuffer_);
            OpenGLCheckError();

            RefreshVertexDescriptor();

            glBindBuffer(GL_ARRAY_BUFFER, ResourceUnbind);
            OpenGLCheckError();

            mainVertDataLast_ = idVertexBuffer_;
        }

        // Update the particles.
        UpdatePre();
        GraphicsSystem::DrawCall(TransformComponents::Identity, *this, batchUpdater);
        UpdatePost();

        // Unbind the relevant buffers in case of stale binding usage.
        glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, ResourceUnbind);
        OpenGLCheckError();
        glBindBuffer(GL_ARRAY_BUFFER, ResourceUnbind);
        OpenGLCheckError();

        // Swap the buffers.
        std::swap(idVertexBuffer_, batchUpdater.GetOutputBufferEdit());
        std::swap(idVertexDescriptor_, swapVertArray_);
        std::swap(mainVertDataLast_, swapVertDataLast_);
        std::swap(mainQuery_, swapQuery_);
    }

    void ParticleBatchMesh::Render(const Mat4& transform, Material& batchRenderer)
    {
        GraphicsSystem::DrawCall(transform, *this, batchRenderer);
    }

    unsigned int ParticleBatchMesh::GetCountParticlesActive(void)
    {
        return immediateDrawCount_;
    }

    void ParticleBatchMesh::SetCountParticlesActive(unsigned int count)
    {
        immediateDrawCount_ = static_cast<decltype(immediateDrawCount_)>(count);
    }

#pragma endregion Public Functions

	//--------------------------------------------------------------------------
	// Private Static Functions:
	//--------------------------------------------------------------------------

#pragma region Private Static Functions

#pragma endregion Private Static Functions

	//--------------------------------------------------------------------------
	// Private Functions:
	//--------------------------------------------------------------------------

#pragma region Private Functions

    void ParticleBatchMesh::UpdatePre(void)
    {
        glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, mainQuery_);
        OpenGLCheckError();
        assert(glIsQuery(mainQuery_) == GL_TRUE);
    }

    void ParticleBatchMesh::UpdatePost(void)
    {
        glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
        OpenGLCheckError();
    }

#pragma endregion Private Functions

}	// namespace
