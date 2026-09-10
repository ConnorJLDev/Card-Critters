//------------------------------------------------------------------------------
//
// File Name:	StreamInValidator.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Validates the structure of a file read by StreamIn.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "StreamCommon.h"
#include <schema.h>

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    // Forward Declarations:

    // Typedefs:
    using SchemaDocument_t = rapidjson::GenericSchemaDocument<rapidjson::Value, StreamAllocator>;
    using SchemaValidator_t = rapidjson::GenericSchemaValidator<SchemaDocument_t, rapidjson::BaseReaderHandler<typename SchemaDocument_t::SchemaType::EncodingType>,StreamAllocator>;

    // Class Definition:
    class StreamIn::Validator : public Object
    {
        // Public Constants, Enums, and Structs:
    public:

        // Constructors/Destructors:
    private:
        Validator(Document_t& document);
    public:
        Validator() = delete;

        Validator(const Validator& other) = delete;

        Validator(Validator&& other) noexcept;

        Validator& operator=(const Validator& other) = delete;

        ~Validator(void);

        // Public Static Functions:
    public:
        //
        static Validator Create(string_view_arg schema);

        // Public Functions:
    public:
        //
        bool Validate(const StreamIn& stream);

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
        SchemaDocument_t schema_;
        SchemaValidator_t validator_;
    };

}	// namespace
