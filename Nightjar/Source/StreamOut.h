//------------------------------------------------------------------------------
//
// File Name:	StreamOut.h
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class for writing to a file.
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
    class StreamOut : public Object
    {
        // Public Constants, Enums, and Structs:
    public:
#define DECLARE_FRIEND_STREAMOUT(item) friend Nightjar::StreamValue Nightjar::StreamOut::WriteValueRaw<item>(const item&)
#define IMPLEMENT_FRIEND_STREAMOUT(item, itemName) template <> Nightjar::StreamValue Nightjar::StreamOut::WriteValueRaw<item>(const item& itemName)

        // Constructors/Destructors:
    public:
        StreamOut(string_view_arg filePath);

        StreamOut(const StreamOut& other) = delete;

        StreamOut& operator=(const StreamOut& other) = delete;

        StreamOut(StreamOut&& other) noexcept;

        ~StreamOut(void);

        // Public Static Functions:
    public:

        // Public Functions:
    public:
        /// <summary>
        /// Checks if the stream is valid.
        /// </summary>
        /// <returns>True if the stream can be opened successfully, false otherwise.</returns>
        bool IsValid(void) const;

        /// <summary>
        /// Writes the StreamOut to file.
        /// </summary>
        void Write(void);

        /// <summary>
        /// Sets the key to write the next value with.
        /// </summary>
        /// <param name="key">The key of the next value to be written.</param>
        void SetNextKey(string_view_arg key);

        /// <summary>
        /// Clears the buffer containing the key for the next value written.
        /// </summary>
        void ClearNextKey(void);

        /// <summary>
        /// Write a value into the current array.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="value">The value to be written.</param>
        template<typename T>
        void WriteValue(const T& value);

        /// <summary>
        /// Write a value into the current object.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="key">The key for this value.</param>
        /// <param name="value">The value to be written.</param>
        template<typename T>
        void WriteValue(string_view_arg key, const T& value);

        /// <summary>
        /// 
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <typeparam name="MapT"></typeparam>
        /// <param name="map"></param>
        /// <param name="value"></param>
        /// <returns></returns>
        template<typename T, typename TContainer>
        void WriteEnum(const TContainer& map, T& value);

        /// <summary>
        /// 
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <typeparam name="MapT"></typeparam>
        /// <param name="key"></param>
        /// <param name="map"></param>
        /// <param name="value"></param>
        /// <returns></returns>
        template<typename T, typename TContainer>
        void WriteEnum(string_view_arg key, const TContainer& map, T& value);

        /// <summary>
        /// Creates a new child object to write into.
        /// </summary>
        /// <param name="key">The key of this new object in the parent object, if applicable.</param>
        void WriteObjectStart(string_view_arg key = EmptyKey);

        /// <summary>
        /// Ends the current object being written to, and writes it into its parent object/array.
        /// </summary>
        void WriteObjectEnd(void);

        /// <summary>
        /// Creates a new child array to write into.
        /// </summary>
        /// <param name="key">The key of this new object in the parent object, if applicable.</param>
        void WriteArrayStart(string_view_arg key = EmptyKey);

        /// <summary>
        /// Ends the current array being written to, and writes it into its parent object/array.
        /// </summary>
        void WriteArrayEnd(void);

        /// <summary>
        /// Converts a given value into a json serializable value.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="value">The value to write.</param>
        /// <returns>The json serializable value.</returns>
        template<typename T>
        StreamValue WriteValueRaw(const T& value);

        // Public Event Handlers
    public:

        // Private Constants, Enums, and Structs:
    private:
        struct KeyValue
        {
            string_view_t key;
            StreamValue value;
            
            KeyValue(void);
            KeyValue(const KeyValue& other) = delete;
            KeyValue& operator=(const KeyValue& other) = delete;
            KeyValue(KeyValue&& other) noexcept;
            KeyValue(string_view_arg key, bool isObject);
        };

        // Private Static Functions:
    private:

        // Private Functions:
    private:
        /// <summary>
        /// Internal function that creates a new child object to write to.
        /// Should only be used in implementations of WriteValueRaw.
        /// </summary>
        /// <param name="key">The key of this object for its parent object, if applicable. Ignored if this object is returned from WriteValueRaw.</param>
        void WriteObjectStart_(string_view_arg key = EmptyKey);

        /// <summary>
        /// Internal function that returns the current object without writing it to its parent.
        /// Should only be used in implementations of WriteValueRaw.
        /// </summary>
        /// <returns>The current object that was written to.</returns>
        StreamValue WriteObjectEnd_(void);
        
        /// <summary>
        /// Internal function that creates a new child array to write to.
        /// Should only be used in implementations of WriteValueRaw.
        /// </summary>
        /// <param name="key">The key of this array for its parent object, if applicable. Ignored if this array is returned from WriteValueRaw.</param>
        void WriteArrayStart_(string_view_arg key = EmptyKey);

        /// <summary>
        /// Internal function that returns the current array without writing it to its parent.
        /// Should only be used in implementations of WriteValueRaw.
        /// </summary>
        /// <returns>The current array that was written to.</returns>
        StreamValue WriteArrayEnd_(void);

        /// <summary>
        /// Common function for writing vectors.
        /// </summary>
        /// <typeparam name="T">The vector type.</typeparam>
        /// <typeparam name="TComponent">The vector component type.</typeparam>
        /// <typeparam name="C">The vector size.</typeparam>
        /// <param name="vec">The vector to write.</param>
        template <typename T, typename TComponent, size_t C>
        StreamValue WriteVec(const T& vec);

        /// <summary>
        /// Writes the current object/array into its parent.
        /// </summary>
        void WritePop(void);

        /// <summary>
        /// Gets the current object/array being written to.
        /// </summary>
        /// <returns></returns>
        StreamValue& GetCurrent(void);

        /// <summary>
        /// Gets the parent of the current object/array being written to.
        /// </summary>
        /// <returns></returns>
        StreamValue& GetParent(void);

        // Private Static Variables:
    private:
        // Empty key used for default string_view references.
        static constexpr string_view_t EmptyKey = "";

        // Private Variables:
    private:
        // The document structure being written to.
        Document_t doc_;
        // The document's allocator, used for writing.
        Document_t::AllocatorType& allocator_;
        // The path being written to.
        string_t path_;
        // Is the path able to be written to?
        bool valid_;
        // Have we written to the file?
        bool hasWritten_;
        // The stack of objects/arrays that are being written into.
        vector_t<KeyValue> writeStack_;
        // The key of the next value to be written.
        string_t nextKey_;
    };

}	// namespace

#include "StreamOut.inl"
