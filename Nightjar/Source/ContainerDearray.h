//------------------------------------------------------------------------------
//
// File Name:	ContainerDearray.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Double-ended queue using an array.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "STLTypes.h"

//------------------------------------------------------------------------------
// Preprocessor Directives:
//------------------------------------------------------------------------------

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
    template<typename T, auto N, bool preserve = false>
        requires (array_size_t<decltype(N)>)
    class dearray_t : public array_t<T, N>
    {
        // Public Constants, Enums, and Structs:
    public:
        //
        using base_t = array_t<T, N>;

        // Constructors/Destructors:
    public:
        dearray_t(void);

        ~dearray_t(void);

        // Public Static Functions:
    public:

        // Public Functions:
    public:
        //
        const T& operator[](size_t index) const;

        //
        T& operator[](size_t index);

        //
        size_t size(void) const;

        //
        size_t capacity(void) const;

        //
        bool empty(void) const;

        //
        bool full(void) const;

        //
        bool push_back(bool overwrite);

        //
        bool push_front(bool overwrite);

        //
        bool pop_back(void);

        //
        bool pop_front(void);

        //
        unsigned end(void) const;

        //
        const T& front(void) const;

        //
        T& front(void);

        //
        const T& back(void) const;

        //
        T& back(void);

        // Private Constants, Enums, and Structs:
    private:

        // Private Static Functions:
    private:
        //
        static unsigned Traverse(unsigned index, int offset);

        // Private Functions:
    private:

        // Private Static Variables:
    private:

        // Private Variables:
    private:
        unsigned front_;
        unsigned back_;
    };

}	// namespace

#include "ContainerDearray.inl"
