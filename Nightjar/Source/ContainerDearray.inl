//------------------------------------------------------------------------------
//
// File Name:	ContainerDearray.inl
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

#include "ContainerDearray.h"

//------------------------------------------------------------------------------
// Preprocessor Directives:
//------------------------------------------------------------------------------

#pragma warning(push)
#pragma warning(disable : 4514)

// MSVC has a known bug with incorrectly parsing out-of-line template
// definitions containing a constrained non-type template parameter, so it is
// split into "auto N" and a requires clause.
#define TMP_DEARRAY_TMP template<typename T, auto N, bool preserve> requires (array_size_t<decltype(N)>)
#define TMP_DEARRAY_ARGS T, N, preserve

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

    TMP_DEARRAY_TMP
    dearray_t<TMP_DEARRAY_ARGS>::dearray_t(void)
        : front_(0)
        , back_(0)
    {
    }

    //--------------------------------------------------------------------------

    TMP_DEARRAY_TMP
    dearray_t<TMP_DEARRAY_ARGS>::~dearray_t(void)
    {
    }

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

    TMP_DEARRAY_TMP
    const T& dearray_t<TMP_DEARRAY_ARGS>::operator[](size_t index) const
    {
        size_t indexRaw = Traverse(front_, static_cast<int>(index));
        return base_t::operator[][indexRaw];
    }

    TMP_DEARRAY_TMP
    T& dearray_t<TMP_DEARRAY_ARGS>::operator[](size_t index)
    {
        size_t indexRaw = Traverse(front_, static_cast<int>(index));
        return base_t::operator[][indexRaw];
    }

    TMP_DEARRAY_TMP
    size_t dearray_t<TMP_DEARRAY_ARGS>::size(void) const
    {
        return empty() ? 0 : (back_ + 1 + (back_ < front_ ? capacity() : 0) - front_);
    }

    TMP_DEARRAY_TMP
    size_t dearray_t<TMP_DEARRAY_ARGS>::capacity(void) const
    {
        return N;
    }

    TMP_DEARRAY_TMP
    bool dearray_t<TMP_DEARRAY_ARGS>::empty(void) const
    {
        return (int)back_ == -1;
    }

    TMP_DEARRAY_TMP
    bool dearray_t<TMP_DEARRAY_ARGS>::full(void) const
    {
        return Traverse(back_, 1) == front_;
    }

    TMP_DEARRAY_TMP
    bool dearray_t<TMP_DEARRAY_ARGS>::push_back(bool overwrite)
    {
        const bool isFull = full();
        if (isFull)
        {
            if (!overwrite)
            {
                return false;
            }
            front_ = Traverse(front_, 1);
        }
        back_ = end();
        return true;
    }

    TMP_DEARRAY_TMP
    bool dearray_t<TMP_DEARRAY_ARGS>::pop_front(void)
    {
        const bool isNotEmpty = !empty();
        if (isNotEmpty)
        {
            front_ = Traverse(front_, 1);
        }
        return isNotEmpty;
    }

    TMP_DEARRAY_TMP
    const T& dearray_t<T, N, preserve>::front(void) const
    {
        return base_t::operator[](front_);
    }

    TMP_DEARRAY_TMP
    T& dearray_t<TMP_DEARRAY_ARGS>::front(void)
    {
        return base_t::operator[](front_);
    }

    TMP_DEARRAY_TMP
    const T& dearray_t<TMP_DEARRAY_ARGS>::back(void) const
    {
        return base_t::operator[](back_);
    }

    TMP_DEARRAY_TMP
    T& dearray_t<TMP_DEARRAY_ARGS>::back(void)
    {
        return base_t::operator[](back_);
    }

#pragma endregion Public Functions

    //--------------------------------------------------------------------------
    // Private Static Functions:
    //--------------------------------------------------------------------------

#pragma region Private Static Functions

    TMP_DEARRAY_TMP
    unsigned dearray_t<TMP_DEARRAY_ARGS>::Traverse(unsigned index, int offset)
    {
        return (index + static_cast<unsigned>(offset)) % static_cast<unsigned>(N);
    }

#pragma endregion Private Static Functions

    //--------------------------------------------------------------------------
    // Private Functions:
    //--------------------------------------------------------------------------

#pragma region Private Functions

    TMP_DEARRAY_TMP
    unsigned dearray_t<TMP_DEARRAY_ARGS>::end(void) const
    {
        return empty() ? front_ : Traverse(back_, 1);
    }

#pragma endregion Private Functions

}	// namespace

#pragma warning(pop)

#undef TMP_DEARRAY_ARGS
#undef TMP_DEARRAY_TMP
