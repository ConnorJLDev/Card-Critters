//------------------------------------------------------------------------------
//
// File Name:	MemoryException.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class that represents exceptions relating to memory management.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

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
    
    // Class Definition:
    class MemoryException : public std::exception
    {
        // Public Constants, Enums, and Structs:
    public:
        // Exception codes.
        enum class ErrorCode
        {
            Generic,                    // Generic memory-related exception that is not classified by any other error code.
            PageInitFailure,            // Initialization of a memory page failed.
            NoMemory,                   // System memory allocation failed; there is no memory available.
            InvalidPattern,             // Invalid memory pattern was found when allocating or deallocating memory.
            InvalidHeader,              // Invalid header data.
        };

        // Constructors/Destructors:
    public:
        MemoryException(ErrorCode code, string_view_arg message) noexcept;

        ~MemoryException(void) noexcept;

        // Public Static Functions:
    public:

        // Public Functions:
    public:
        /// <summary>
        /// Gets a message describing the details of the exception.
        /// </summary>
        /// <returns>An explanatory null-terminated string.</returns>
        const char* what(void) const noexcept override;

        /// <summary>
        /// Gets the specific error code of this exception.
        /// </summary>
        /// <returns>The error code represented by this exception.</returns>
        ErrorCode code(void) const noexcept;

        /// <summary>
        /// Gets a string representing the specific error code of this exception.
        /// </summary>
        /// <returns>The error code represented by this exception as a string.</returns>
        const char* code_str(void) const noexcept;

        // Public Event Handlers
    public:

        // Private Constants, Enums, and Structs:
    private:

        // Private Static Functions:
    private:
        // The maximum number of characters that the exception message can contain.
        static constexpr uint8_t MessageCapacity = 127 - sizeof(ErrorCode) - sizeof(std::exception);

        // Private Functions:
    private:

        // Private Static Variables:
    private:

        // Private Variables:
    private:
        // The error code.
        ErrorCode code_;
        // The error message.
        string_fixed_t<MessageCapacity> message_;
    };

}	// namespace
