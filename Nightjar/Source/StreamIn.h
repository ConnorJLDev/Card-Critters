//------------------------------------------------------------------------------
//
// File Name:	StreamIn.h
// Author(s):	Connor Lariviere (connor.lariviere), Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class for reading from a file.
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

#include "DataTypes.h"

//------------------------------------------------------------------------------
// Preprocessor Directives:
//------------------------------------------------------------------------------

#pragma warning(push)
#pragma warning(disable : 4582)

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
    class StreamIn : Object
    {
        // Public Constants, Enums, and Structs:
    public:
        class Validator;

        enum class ReadValueError : unsigned char
        {
            None, // No error, value was read successfully.
            KeyMissing, // Requested key is missing.
            KeyType, // Requested type is different from the deserialized type.
            InvalidValue, // Value was of correct type but incorrect value, such as an empty string or an incorrect number of items in a list.
        };

        // Constructors/Destructors:
    public:
        StreamIn(string_view_arg filePath);

        StreamIn(const StreamIn& other) = delete;

        StreamIn& operator=(const StreamIn& other) = delete;

        StreamIn(StreamIn&& other) noexcept;

        ~StreamIn(void);

        // Public Static Functions:
    public:
        // Declares the name of an object as it should be read from a stream.
#define DECLARE_READ_KEY(name) static constexpr string_view_t ReadKey{#name}

        // Declares the directory that an object is stored under.
#define DECLARE_READ_DIR(name) static constexpr string_view_t ReadDir{name}

        // Public Functions:
    public:
        /// <summary>
        /// Checks if the stream is valid.
        /// </summary>
        /// <returns>True if the stream was opened successfully, false otherwise.</returns>
        bool IsValid(void) const;

        /// <summary>
        /// Gets the filepath that this stream is using.
        /// </summary>
        /// <returns>The filepath this stream is using.</returns>
        const string_t& GetPath(void) const;

        /// <summary>
        /// Gets the current hierarchy into the json file that this stream is reading from.
        /// </summary>
        /// <returns>The current hierarchy.</returns>
        string_t GetHierarchy(void) const;

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        bool InObject(void) const;

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        bool InArray(void) const;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="checkVector"></param>
        /// <returns></returns>
        DataType GetElementType(bool checkVector);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="key"></param>
        /// <param name="checkVector"></param>
        /// <returns></returns>
        DataType GetElementType(string_view_arg key, bool checkVector);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="index"></param>
        /// <param name="checkVector"></param>
        /// <returns></returns>
        DataType GetElementType(size_t index, bool checkVector);

#define ReadBoolean(key) ReadValue_<bool>(key)
#define ReadInt(key) ReadValue_<int>(key)
#define ReadFloat(key) ReadValue_<float>(key)
#define ReadVec2(key) ReadValue_<Vec2>(key)
#define ReadVec3(key) ReadValue_<Vec3>(key)
#define ReadVec4(key) ReadValue_<Vec4>(key)
#define ReadString(key) ReadValue_<string_t>(key)
        
        /// <summary>
        /// 
        /// </summary>
        /// <param name="key"></param>
        /// <param name="objectRead"></param>
        void ReadArrayObjects(string_view_arg key, const std::function<void(StreamIn&, int)>& objectRead);
        
        /// <summary>
        /// 
        /// </summary>
        /// <param name="key"></param>
        /// <param name="objectRead"></param>
        void ReadObjectKeyValues(string_view_arg key, const std::function<void(string_view_arg, StreamIn&, int)>& objectRead);

        /// <summary>
        /// 
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="key"></param>
        /// <param name="value"></param>
        template<modifiable_t T>
        void ReadValue(string_view_arg key, T& value);

        /// <summary>
        /// 
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="key"></param>
        /// <returns></returns>
        template<typename T>
        T ReadValue_(string_view_arg key);

        /// <summary>
        /// 
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="value"></param>
        /// <returns></returns>
        template<modifiable_t T>
        bool TryReadValue(T& value);

        /// <summary>
        /// 
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="key"></param>
        /// <param name="value"></param>
        /// <returns></returns>
        template<modifiable_t T>
        bool TryReadValue(string_view_arg key, T& value);

        /// <summary>
        /// 
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="key"></param>
        /// <param name="keyOld"></param>
        /// <param name="value"></param>
        /// <returns></returns>
        template<modifiable_t T>
        bool TryReadValue(string_view_arg key, string_view_arg keyOld, T& value);

        /// <summary>
        /// 
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="index"></param>
        /// <param name="value"></param>
        /// <returns></returns>
        template<modifiable_t T>
        bool TryReadValue(size_t index, T& value);

        /// <summary>
        /// 
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="key"></param>
        /// <param name="vec"></param>
        template<modifiable_t T, bool clear = true>
        void ReadVector(string_view_arg key, T& vec) requires (is_vector_v<T>);

        /// <summary>
        /// 
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <typeparam name="TMap"></typeparam>
        /// <param name="key"></param>
        /// <param name="map"></param>
        /// <returns></returns>
        template<typename T, typename TMap>
        T ReadEnum(string_view_arg key, const TMap& map);

        /// <summary>
        /// 
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <typeparam name="TMap"></typeparam>
        /// <param name="key"></param>
        /// <param name="map"></param>
        /// <param name="value"></param>
        /// <returns></returns>
        template<modifiable_t T, typename TMap>
        bool TryReadEnum(string_view_arg key, const TMap& map, T& value);

        /// <summary>
        /// 
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <typeparam name="TMap"></typeparam>
        /// <param name="key"></param>
        /// <param name="map"></param>
        /// <param name="value"></param>
        template<modifiable_t T, typename TMap>
        bool TryReadEnumFlags(string_view_arg key, const TMap& map, T& value);

        /// <summary>
        /// 
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <returns></returns>
        template<typename T>
        T ReadArrayElement(void);

        /// <summary>
        /// Gets the size of an array.
        /// </summary>
        /// <param name="key">The key of the array to check.</param>
        /// <returns>The size of the array, or -1 if no array was found.</returns>
        int GetArraySize(string_view_arg key);

        /// <summary>
        /// Gets the size of an object.
        /// </summary>
        /// <param name="key">The key of the object to check.</param>
        /// <returns>The size of the object, or -1 if no object was found.</returns>
        int GetObjectSize(string_view_arg key);

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        bool StepBack(void);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="key"></param>
        /// <returns></returns>
        bool StepInto(string_view_arg key);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="index"></param>
        /// <returns></returns>
        bool StepInto(size_t index);

        /// <summary>
        /// 
        /// </summary>
        void ResetCurrent(void);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="key"></param>
        /// <returns></returns>
        bool StreamHas(string_view_arg key) const;

        /// <summary>
        /// Gets the last error code from reading a value.
        /// </summary>
        /// <returns>The error code from the most recent attempt to read a value.</returns>
        ReadValueError GetLastError(void) const;

        /// <summary>
        /// Attempts to read the streamValue into value.
        /// </summary>
        /// <typeparam name="T">The type attempting to be read.</typeparam>
        /// <param name="streamValue">The raw value read from stream.</param>
        /// <param name="value">The value to write streamValue into.</param>
        /// <returns>True if value was successfully written to, false otherwise.</returns>
        template<modifiable_t T>
        bool TryReadValueInternal(const StreamValue& streamValue, T& value);

        // Public Event Handlers
    public:

        // Private Constants, Enums, and Structs:
    private:
        struct KeyValue
        {
            union
            {
                string_t key;
                size_t index;
            };
            const StreamValue* value;
            bool isKey;

            KeyValue(void);
            KeyValue(string_view_arg key, const StreamValue* value);
            KeyValue(size_t index, const StreamValue* value);
            KeyValue(const KeyValue& other) = delete;
            KeyValue(KeyValue&& other) noexcept;
            KeyValue& operator=(const KeyValue& other) = delete;
            ~KeyValue(void);
        };

        // Private Static Functions:
    private:

        // Private Functions:
    private:
        /// <summary>
        /// 
        /// </summary>
        /// <param name="streamValue"></param>
        /// <param name="checkVector"></param>
        /// <returns></returns>
        DataType GetElementType(const StreamValue& streamValue, bool checkVector);
        
        /// <summary>
        /// Base function for reading a float vector.
        /// </summary>
        /// <param name="key">The key of the vector.</param>
        /// <param name="vec">The address of the first vector element to receive the values.</param>
        /// <param name="vecSize">The number of elements in the vector.</param>
        template <modifiable_t T, size_t C>
        bool ReadVecT(const StreamValue& streamValue, T* vec);

        /// <summary>
        /// Tries to find the member of the current object.
        /// </summary>
        /// <param name="key">The key of the member to find.</param>
        /// <param name="value">A pointer reference that is updated to point to the StreamValue of the member if found. If not found, it is unchanged.</param>
        /// <returns>True if the member was found, false otherwise.</returns>
        bool FindMember(string_view_arg key, const StreamValue*& value);

        /// <summary>
        /// Tries to find the element at the given index of the current array.
        /// </summary>
        /// <param name="index">The index of the element to find.</param>
        /// <param name="value">A pointer reference that is updated to point to the StreamValue of the element if found. If not found, it is unchanged.</param>
        /// <returns>True if the element was found, false otherwise.</returns>
        bool FindMember(size_t index, const StreamValue*& value);

        // Private Static Variables:
    private:

        // Private Variables:
    private:
        Document_t doc_;
        const StreamValue* current_;
        vector_t<KeyValue> stack_;
        string_t path_;
        ReadValueError lastError_;
        bool valid_;
    };

}	// namespace

#pragma warning(pop)

#include "StreamIn.inl"
