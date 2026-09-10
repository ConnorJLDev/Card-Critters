//------------------------------------------------------------------------------
//
// File Name:	ShaderMaterialBuffer.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Shader encapsulator.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Shader.h"

#include GRAPHICS_INCLUDE

#undef TRACE_CATEGORY
#define TRACE_CATEGORY SHADER

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

	//--------------------------------------------------------------------------
	// Private Variables:
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// Constructors/Destructors:
	//--------------------------------------------------------------------------

#pragma region Constructors

    Shader::ShaderParser::ShaderParser(vector_t<const char*>& sources_, vector_t<int>& sourceLengths_, vector_t<string_t>& outputs_, vector_t<string_t>& depths_, StageType stage_)
        : sources(sources_)
        , sourceLengths(sourceLengths_)
        , outputs(outputs_)
        , depths(depths_)
        , versionNum()
        , versionType()
        , version()
        , definitions()
        , stage(stage_)
    {
    }

	//--------------------------------------------------------------------------

#pragma endregion Constructors

	//--------------------------------------------------------------------------
	// Public Static Functions:
	//--------------------------------------------------------------------------

#pragma region Public Static Functions

#pragma endregion Public Static Functions

	//--------------------------------------------------------------------------
	// Public Functions:
	//--------------------------------------------------------------------------

#pragma region Public Functions

#pragma endregion Public Functions

	//--------------------------------------------------------------------------
	// Private Static Functions:
	//--------------------------------------------------------------------------

#pragma region Private Static Functions

    string_t Shader::ReadShaderStream(ShaderParser& parser, std::istream& shaderFile)
    {
        string_t shaderString, line;

        static constexpr string_view_t parseFormat = "/*PARSED:{}*/\n";
#define MARK_PARSED std::format_to(std::back_inserter(shaderString), parseFormat, line); continue

        static const std::regex& regexVersion(StaticManagement::RegexWrapper::Create(std::regex(R"(^\s*#version\s+(\d+)\s+([a-zA-Z]+)\s*$)"))); // Matches #version NUM ALPHA
        static const std::regex& includeRegex(StaticManagement::RegexWrapper::Create(std::regex(R"del(^\s*#include\s+"([^"]+)"\s*$)del"))); // Matches #include "filename.glsl"
        static const std::regex& pragmaRegex(StaticManagement::RegexWrapper::Create(std::regex(R"(^\s*#pragma\s+(\w+.*$))"))); // Matches "#pragma" with start of word(s) after.
        static const std::regex& pragmaUsageRegex(StaticManagement::RegexWrapper::Create(std::regex(R"(UBO_USAGE\s+([^\s]+)\s+([^\s]+)\s*$)"))); // Matches #pragma UBO_USAGE xxx yyy
        static const std::regex& pragmaOutputRegex(StaticManagement::RegexWrapper::Create(std::regex(R"(UBO_OUTPUT\s+([^\s]+)\s*$)"))); // Matches #pragma UBO_OUTPUT xxx
        static const std::regex& inputRegex(StaticManagement::RegexWrapper::Create(std::regex(R"(^\s*layout\s+\(\s*location\s*\=\s*(\d+)\s*\)\s+in\s+([^\s]+)\s+([^\s]+)\s*;\s*$)"))); // Matches layout(location=?) in TYPE NAME

        const bool canReadInputVertAttributes = parser.stage == StageType::Vertex;
        const bool canSetTransformFeedbackOutputs = parser.stage == StageType::Vertex || parser.stage == StageType::Geometry;

#define ADD_SOURCE(var) parser.sources.emplace_back(var.data()); parser.sourceLengths.emplace_back(static_cast<int>(var.size()));
        while (std::getline(shaderFile, line))
        {
            smatch_t matches;
            if (std::regex_match(line, matches, includeRegex) && matches.size() == 2)
            {
                std::string match1 = matches[1].str();
                string_t includeFileName(CONVERT_STR(match1));
                const string_t& includeData = GetCommon(includeFileName);
                std::istringstream includeStream{ includeData };
                parser.depths.emplace_back(includeFileName);
                ReadShaderStream(parser, includeStream);
                ADD_SOURCE(includeData);
                MARK_PARSED;
            }
            else if (canReadInputVertAttributes && std::regex_match(line, matches, inputRegex) && matches.size() == 4)
            {
                int location = std::stoi(matches[1].str());
                std::string match2 = matches[2].str();
                string_t attrTypeName(CONVERT_STR(match2));
                std::string match3 = matches[3].str();
                string_t attrName(CONVERT_STR(match3));
                //glEnableVertexAttribArray((GLuint)location);
                OpenGLCheckError();
                TraceDebug("Vert attr {} @ {}: \"{}\"", attrTypeName, location, attrName);
                shaderString += line;
            }
            else if (std::regex_match(line, matches, pragmaRegex) && matches.size() == 2)
            {
                std::string matchsub = matches[1].str();
                string_t substr(CONVERT_STR(matchsub));
                if (std::regex_match(substr, matches, pragmaUsageRegex) && matches.size() == 3)
                {
                    std::string match1 = matches[1].str();
                    std::string match2 = matches[2].str();
                    auto iter = ENUM_READ(BufferUsage).find(match2);
                    if (iter == ENUM_READ(BufferUsage).end())
                    {
                        TraceWarning("Unrecognized UBO_USAGE usage \"{}\": \"{}\"", match2, line);
                    }
                    else
                    {
                        string_t bufferName(CONVERT_STR(match1));
                        BufferUsages()[bufferName] = iter->second;
                    }
                    MARK_PARSED;
                }
                else if (canSetTransformFeedbackOutputs && std::regex_match(substr, matches, pragmaOutputRegex) && matches.size() == 2)
                {
                    std::string match1 = matches[1].str();
                    parser.outputs.push_back(CONVERT_STR_INL(match1));
                    MARK_PARSED;
                }
                else
                {
                    TraceWarning("Unrecognized glsl pragma: \"{}\" ({})", line, substr);
                }
            }
            else if (std::regex_match(line, matches, regexVersion) && matches.size() == 3)
            {
                assert(parser.versionNum.empty() == parser.versionType.empty());
                std::string match1 = matches[1].str();
                std::string match2 = matches[2].str();
                if (parser.versionNum.empty() && parser.versionType.empty())
                {
                    parser.versionNum = match1;
                    parser.versionType = match2;
                    parser.version = line;
                    ADD_SOURCE(parser.version);
                    // Add macros defined in engine
                    parser.definitions = "";
                    for (const auto& macro : GlobalDefinitions())
                    {
                        parser.definitions += "\n" + macro;
                    }
                    ADD_SOURCE(parser.definitions);
                }
                else if (parser.versionNum != CONVERT_STR_INL(match1) || parser.versionType != CONVERT_STR_INL(match2))
                {
                    assert(false);
                }
                MARK_PARSED;
            }
            else
            {
                shaderString += line;
            }
            shaderString += "\n";
        }

        parser.depths.pop_back();

        return shaderString;
    }

#pragma endregion Private Static Functions

    //--------------------------------------------------------------------------
    // Private Functions:
    //--------------------------------------------------------------------------

#pragma region Private Functions

#pragma endregion Private Functions

}	// namespace
