//------------------------------------------------------------------------------
//
// File Name:	TraceSource.h
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

#include "ConstexprString.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    // Struct Definition:
    struct /*alignas(32)*/ TraceSource
    {
        // Public Constants, Enums, and Structs:
    public:

        // Private Constants, Enums, and Structs:
    private:
        // Used for instance-based template construction.
        struct X
        {
        };

        // Constructors/Destructors:
    public:
        constexpr TraceSource(string_view_t source);
        constexpr TraceSource(X, string_view_t source);
    public:
        TraceSource(void) = delete;
        TraceSource(const TraceSource& other) = delete;
        TraceSource& operator=(const TraceSource& other) = delete;

        // Public Static Functions:
    public:
        /// <summary>
        /// Gets a string representing the specified location, for CSV usage.
        /// </summary>
        /// <param name="loc">The source location.</param>
        /// <returns>A string formatted for CSV usage that specifies the source location.</returns>
        static constexpr std::string GetSourceLocation(std::source_location loc = std::source_location::current());

        /// <summary>
        /// Helper function to ensure a TraceSource struct is locally created.
        /// </summary>
        /// <param name="source">The source to use when constructing the TraceSource.</param>
        /// <returns>The local TraceSource struct.</returns>
        static consteval TraceSource MakeLocal(string_view_t source);

        // Public Functions:
    public:
        // Implicit string_view converter. Gives the whole source string.
        constexpr operator string_view_t(void) const;

        // Gets the entire source as a string_view formatted for insertion into a CSV table.
        constexpr string_view_t GetSourceCSV(void) const;

        // Gets the source file as a string_view.
        constexpr string_view_t GetSourceFile(void) const;

        // Gets the source file line number as a string_view.
        constexpr string_view_t GetSourceLineStr(void) const;

        // Gets the source function as a string_view.
        constexpr string_view_t GetSourceFunction(void) const;

        // Public Variables:
    public:
        // The variables are somewhat chaotically organized to optimize space.

        // The source this message was traced from.
        const char* source;
        // The source file line number this message was traced from, as a string.
        uint_fast8_t sourceLineStr;
        // Length of the source file line number string.
        uint_fast8_t sourceLineStrLen;
        // The source function this message was traced from.
        uint_fast8_t sourceFunction;
        // Length of the source file string.
        uint_fast16_t sourceFileLen;
        // Length of the source string.
        uint_fast16_t sourceLen;
        // The source file this message was traced from.
        uint_fast16_t sourceFile;
        // Length of the source function string.
        uint_fast16_t sourceFunctionLen;
        // The source file line number this message was traced from.
        uint_fast16_t sourceLine;
    };

    namespace TraceSourceFuncs
    {
        // Generates a reference to the template TraceSource.
        template<TraceSource T>
        consteval const TraceSource& Generate(void) noexcept;
    }

}	// namespace

#include "TraceSource.inl"

// Constructs a TraceSource struct at the location this is called at.
#define BuildTraceSource() (Nightjar::TraceSourceFuncs::Generate<Nightjar::TraceSource::MakeLocal(BuildStr(Nightjar::TraceSource::GetSourceLocation, std::source_location::current()))>())
