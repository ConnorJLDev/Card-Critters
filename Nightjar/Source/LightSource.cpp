//------------------------------------------------------------------------------
//
// File Name:	LightSource.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class representing a light source.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "LightSource.h"

#include GRAPHICS_INCLUDE

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

	//--------------------------------------------------------------------------
	// Private Static Variables:
	//--------------------------------------------------------------------------

	ResourceID LightSource::lightingBuffer_ = ResourceUnbind;

	//vector_t<LightSource*> LightSource::globalLights_;

	//--------------------------------------------------------------------------
	// Private Variables:
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// Constructors/Destructors:
	//--------------------------------------------------------------------------

#pragma region Constructors

	LightSource::LightSource(void)
		: Object()
		, color_()
		, pos_()
		, normal_()
		, attenuationDistance_()
		, attenuationAngle_()
		, index_()
	{
	}

	LightSource::LightSource(const LightSource& other)
		: Object(other)
		, color_(other.color_)
		, pos_(other.pos_)
		, normal_(other.normal_)
		, attenuationDistance_(other.attenuationDistance_)
		, attenuationAngle_(other.attenuationAngle_)
		, index_(other.index_)
	{
	}

	//--------------------------------------------------------------------------

	LightSource::~LightSource(void)
	{
	}

#pragma endregion Constructors

	//--------------------------------------------------------------------------
	// Public Static Functions:
	//--------------------------------------------------------------------------

#pragma region Public Static Functions

	void LightSource::SetAmbientLight(const Vec4& color)
	{
		const auto* ubo = GetUBO();
		if (ubo != nullptr)
		{
			BindGlobalBuffer(ubo);
		}
		else
		{
			TraceWarning("Failed to find and set global lighting buffer data for ambient light!");
			return;
		}
		glBufferSubData(GL_UNIFORM_BUFFER, ubo->offsets.at("lightAmbientColor"), sizeof(Vec4), &color);
	}

#pragma endregion Public Static Functions

	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

	void LightSource::UpdateGraphics()
	{
		const auto* ubo = GetUBO();
		if (ubo != nullptr)
		{
			BindGlobalBuffer(ubo);
		}
		else
		{
			TraceWarning("Failed to find and set global lighting buffer data for light {}!", index_);
			return;
		}
        string_t prefix{};
        FORMAT_TO(prefix, "lights[{}].", index_);
		const auto& offsets = ubo->offsets;
		glBufferSubData(GL_UNIFORM_BUFFER, offsets.at(prefix + "color"), sizeof(Vec4), &color_);
		glBufferSubData(GL_UNIFORM_BUFFER, offsets.at(prefix + "position"), sizeof(Vec3), &pos_);
		glBufferSubData(GL_UNIFORM_BUFFER, offsets.at(prefix + "normal"), sizeof(Vec3), &normal_);
		glBufferSubData(GL_UNIFORM_BUFFER, offsets.at(prefix + "attenDistance"), sizeof(Vec3), &attenuationDistance_);
		glBufferSubData(GL_UNIFORM_BUFFER, offsets.at(prefix + "attenAngle"), sizeof(Vec3), &attenuationAngle_);

		glBindBuffer(GL_UNIFORM_BUFFER, ResourceUnbind);
		OpenGLCheckError();
	}

#pragma endregion Public Functions

	//--------------------------------------------------------------------------
	// Private Static Functions:
	//--------------------------------------------------------------------------

#pragma region Private Static Functions

	void LightSource::Init()
	{
		const auto* ubo = GetUBO();
		if (ubo != nullptr)
		{
			BindGlobalBuffer(ubo);

            const auto& offsets = ubo->offsets;
            static constexpr Vec4 ResetColor { 0, 0, 0, 0 };
            string_t strBuffer{};
            strBuffer.reserve(20);
            for (unsigned i = 0; i < 8; ++i)
            {
                strBuffer.clear();
                std::format_to(std::back_inserter(strBuffer), "lights[{}].color", i);
                glBufferSubData(GL_UNIFORM_BUFFER, offsets.at(strBuffer), sizeof(Vec4), &ResetColor);
            }
		}
		else
		{
			TraceWarning("Failed to find and initialize global lighting buffer!");
		}

		glBindBuffer(GL_UNIFORM_BUFFER, ResourceUnbind);
		OpenGLCheckError();
	}

	void LightSource::Exit()
	{
		/*for (LightSource*& light : globalLights_)
		{
			if (light != nullptr)
			{
				delete light;
				light = nullptr;
			}
		}*/
		if (lightingBuffer_ != ResourceUnbind)
		{
			glDeleteBuffers(1, &lightingBuffer_);
			OpenGLCheckError();
		}
	}

	void LightSource::BindGlobalBuffer(const Shader::UniformBlockInfoGlobal* ubo)
	{
		if (lightingBuffer_ == ResourceUnbind)
		{
			GLsizeiptr bufSize = ubo->size;

			glGenBuffers(1, &lightingBuffer_);
			OpenGLCheckError();

			glBindBuffer(GL_UNIFORM_BUFFER, lightingBuffer_);
			OpenGLCheckError();

			glBufferData(GL_UNIFORM_BUFFER, bufSize, nullptr, GL_DYNAMIC_DRAW);
			OpenGLCheckError();

            glBindBufferRange(GL_UNIFORM_BUFFER, static_cast<GLuint>(ubo->binding), lightingBuffer_, 0, bufSize);
			OpenGLCheckError();
		}
		else
		{
			glBindBuffer(GL_UNIFORM_BUFFER, lightingBuffer_);
			OpenGLCheckError();
		}
	}

	const Shader::UniformBlockInfoGlobal* LightSource::GetUBO()
	{
		auto bufSizeIter = Shader::GlobalUBOs().find(string_t(Shader::GlobalBindLightingName));
		if (bufSizeIter == Shader::GlobalUBOs().end())
		{
			return nullptr;
		}
		return &bufSizeIter->second;
	}

#pragma endregion Private Static Functions

	//--------------------------------------------------------------------------
	// Private Functions:
	//--------------------------------------------------------------------------

#pragma region Private Functions

#pragma endregion Private Functions

}	// namespace
