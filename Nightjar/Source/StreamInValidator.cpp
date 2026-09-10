//------------------------------------------------------------------------------
//
// File Name:	StreamInValidator.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Validates the structure of a file read by StreamIn.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "StreamInValidator.h"

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

    StreamIn::Validator::Validator(Document_t& document)
        : Object()
        , schema_(document)
        , validator_(schema_)
    {
    }

    StreamIn::Validator::Validator(Validator&& other) noexcept
        : Object(std::move(other))
        , schema_(std::move(other.schema_))
        , validator_(schema_)
    {
    }

    //--------------------------------------------------------------------------

    StreamIn::Validator::~Validator(void)
    {
    }

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

    StreamIn::Validator StreamIn::Validator::Create(string_view_arg schema)
    {
        Document_t doc;
        if (doc.Parse(schema.data(), schema.size()).HasParseError())
        {
            TraceWarning("Parse error 0x{:02X} at offset {} when creating schema: {}", (int)doc.GetParseError(), doc.GetErrorOffset(), schema);
            assert(false);
        }
        return Validator(doc);
    }

#pragma endregion Public Static Functions

    //--------------------------------------------------------------------------
    // Public Functions:
    //--------------------------------------------------------------------------

#pragma region Public Functions

    bool StreamIn::Validator::Validate(const StreamIn& stream)
    {
        // Reference: https://rapidjson.org/md_doc_schema.html
        validator_.Reset();
        bool success = stream.doc_.Accept(validator_);
        if (!success)
        {
            // Input JSON is invalid according to the schema
            // Output diagnostic information
            rapidjson::StringBuffer sb;
            validator_.GetInvalidSchemaPointer().StringifyUriFragment(sb);
            string_t invalidSchema = sb.GetString();
            string_t invalidKeyword = validator_.GetInvalidSchemaKeyword();
            sb.Clear();
            validator_.GetInvalidDocumentPointer().StringifyUriFragment(sb);
            string_t invalidDocument = sb.GetString();
            TraceWarning("Invalid document \"{}\":\n\tSchema:{}\n\tKeyword:{}\n\tDocument:{}", stream.GetPath(), invalidSchema, invalidKeyword, invalidDocument);
        }
        return success;
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

#pragma endregion Private Functions

}	// namespace
