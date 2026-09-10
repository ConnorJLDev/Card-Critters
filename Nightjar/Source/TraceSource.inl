//------------------------------------------------------------------------------
//
// File Name:	TraceSource.inl
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Struct for referencing trace source locations with
//              compile-time strings.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "TraceSource.h"

//------------------------------------------------------------------------------
// Preprocessor Directives:
//------------------------------------------------------------------------------

#pragma warning(push)
#pragma warning(disable : 4514)

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    //--------------------------------------------------------------------------
    // Constructors/Destructors:
    //--------------------------------------------------------------------------

#pragma region Constructors

    constexpr TraceSource::TraceSource(string_view_t source_)
        : source(source_.data())
        , sourceLineStr()
        , sourceLineStrLen()
        , sourceFunction()
        , sourceFileLen()
        , sourceLen(static_cast<decltype(sourceLen)>(source_.size()))
        , sourceFile()
        , sourceFunctionLen()
        , sourceLine()
    {
        ASSERT_CONSTEVAL;
        size_t indexStart, indexEnd;
        string_view_t sourceView(source, sourceLen);
        indexStart = sourceView.find_first_of('\"') + 1;
        indexEnd = sourceView.find_first_of('\"', indexStart);
        sourceFile = static_cast<decltype(sourceFile)>(indexStart);
        sourceFileLen = static_cast<decltype(sourceFileLen)>(indexEnd - indexStart);
        indexStart = sourceView.find_first_of(',', indexEnd + 1) + 1;
        const size_t baseOffset = indexStart;
        sourceFile = static_cast<decltype(sourceFile)>(baseOffset - sourceFile);
        source += baseOffset;
        indexEnd = sourceView.find_first_of(',', indexStart);
        sourceLineStr = static_cast<decltype(sourceLineStr)>(indexStart - baseOffset);
        sourceLineStrLen = static_cast<decltype(sourceLineStrLen)>(indexEnd - indexStart);
        sourceLine = StringToUIntConst(string_view_t(&source[sourceLineStr], sourceLineStrLen));
        indexStart = sourceView.find_first_of('\"', indexEnd + 1) + 1;
        indexEnd = sourceView.find_first_of('\"', indexStart);
        sourceFunction = static_cast<decltype(sourceFunction)>(indexStart - baseOffset);
        sourceFunctionLen = static_cast<decltype(sourceFunctionLen)>(indexEnd - indexStart);
    }

    constexpr TraceSource::TraceSource(X, string_view_t source_)
        : TraceSource(source_)
    {
        ASSERT_CONSTEVAL;
    }

    //--------------------------------------------------------------------------

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

    constexpr std::string TraceSource::GetSourceLocation(std::source_location loc)
    {
        ASSERT_CONSTEVAL;
        std::string fileStr, lineStr, funcStr;
        {
            constexpr string_view_t ProjectPath(_PROJECT_DIR);
            fileStr = loc.file_name();
            if (fileStr.starts_with(ProjectPath))
            {
                fileStr = fileStr.substr(ProjectPath.size());
            }
        }
        {
            lineStr = StringFromIntConst(loc.line());
        }
        {
            funcStr = loc.function_name();
        }
        std::string finalStr = "\"" + fileStr + "\"," + lineStr + ",\"" + funcStr + "\"";
        return finalStr;
    }

    consteval TraceSource TraceSource::MakeLocal(string_view_t source_)
    {
        return TraceSource(X{}, source_);
    }

    template<TraceSource T>
    consteval const TraceSource& TraceSourceFuncs::Generate(void) noexcept
    {
        return T;
    }

#pragma endregion Public Static Functions

    //--------------------------------------------------------------------------
    // Public Functions:
    //--------------------------------------------------------------------------

#pragma region Public Functions

    constexpr TraceSource::operator string_view_t(void) const
    {
        return GetSourceCSV();
    }

    constexpr string_view_t TraceSource::GetSourceCSV(void) const
    {
        return string_view_t(source - sourceFile - 1, sourceLen);
    }

    constexpr string_view_t TraceSource::GetSourceFile(void) const
    {
        return string_view_t(source - sourceFile, sourceFileLen);
    }

    constexpr string_view_t TraceSource::GetSourceLineStr(void) const
    {
        return string_view_t(source + sourceLineStr, sourceLineStrLen);
    }

    constexpr string_view_t TraceSource::GetSourceFunction(void) const
    {
        return string_view_t(source + sourceFunction, sourceFunctionLen);
    }

#pragma endregion Public Functions

}	// namespace

#pragma warning(pop)
