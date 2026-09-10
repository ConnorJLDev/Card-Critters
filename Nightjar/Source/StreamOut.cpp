//------------------------------------------------------------------------------
//
// File Name:	StreamOut.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class for writing to a file.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "StreamOut.h"

#include <ostreamwrapper.h>
#include <prettywriter.h>

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

#define RAPIDSTR(str) rapidjson::GenericStringRef<char>(str.data(), static_cast<rapidjson::SizeType>(str.size()))

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

using namespace rapidjson;

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

    StreamOut::StreamOut(string_view_arg filePath)
        : Object()
        , doc_()
        , allocator_(doc_.GetAllocator())
        , path_(filePath)
        , valid_(false)
        , hasWritten_(false)
        , writeStack_()
        , nextKey_()
    {
        // The root document is always an object.
        doc_.SetObject();

        std::ofstream outFile(path_.c_str(), std::ios::out | std::ios::app);

        valid_ = outFile.is_open();

        if (!valid_)
        {
            TraceError("Failed to open StreamOut: \"{}\"", path_);
            return;
        }

        outFile.close();
    }

    StreamOut::StreamOut(StreamOut&& other) noexcept
        : Object(std::move(other))
        , doc_(std::move(other.doc_))
        , allocator_(doc_.GetAllocator())
        , path_(std::move(other.path_))
        , valid_(std::move(other.valid_))
        , hasWritten_(std::move(other.hasWritten_))
        , writeStack_(std::move(other.writeStack_))
    {
    }

    //--------------------------------------------------------------------------

    StreamOut::~StreamOut(void)
    {
        // If this stream could have written and didn't, why did we create it?
        assert(!valid_ || hasWritten_);
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

    bool StreamOut::IsValid(void) const
    {
        return valid_;
    }

    void StreamOut::Write(void)
    {
        assert(valid_);
        // Open file for writing.
        std::ofstream outFileStream(path_.c_str(), std::ios::out | std::ios::trunc);
        if (!outFileStream.is_open())
        {
            TraceError("Failed to open file for writing: \"{}\"", path_);
        }
        else
        {
            // Create rapidjson writer.
            rapidjson::OStreamWrapper outStreamWrapper(outFileStream);
            rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(outStreamWrapper);
            // Write to file.
            doc_.Accept(writer);
            // Clean up.
            outFileStream.close();
            hasWritten_ = true;
        }
    }

    void StreamOut::SetNextKey(string_view_arg key)
    {
        nextKey_ = key;
    }

    void StreamOut::ClearNextKey(void)
    {
        nextKey_.clear();
    }

    void StreamOut::WriteObjectStart(string_view_arg key)
    {
        WriteObjectStart_(key);
    }

    void StreamOut::WriteObjectEnd(void)
    {
        assert(GetCurrent().IsObject());
        WritePop();
    }

    void StreamOut::WriteArrayStart(string_view_arg key)
    {
        WriteArrayStart_(key);
    }

    void StreamOut::WriteArrayEnd(void)
    {
        assert(GetCurrent().IsArray());
        WritePop();
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

    StreamOut::KeyValue::KeyValue(void)
        : key()
        , value()
    {
    }

    StreamOut::KeyValue::KeyValue(KeyValue&& other) noexcept
        : key(std::move(other.key))
        , value(std::move(other.value))
    {
    }

    StreamOut::KeyValue::KeyValue(string_view_arg key_, bool isObject)
        : key(key_)
        , value(isObject ? kObjectType : kArrayType)
    {
    }

    void StreamOut::WriteObjectStart_(string_view_arg key)
    {
        writeStack_.emplace_back(key, true);
    }

    StreamValue StreamOut::WriteObjectEnd_(void)
    {
        assert(GetCurrent().IsObject());
        StreamValue back(std::move(writeStack_.back().value));
        writeStack_.pop_back();
        return back;
    }

    void StreamOut::WriteArrayStart_(string_view_arg key)
    {
        writeStack_.emplace_back(key, false);
    }

    StreamValue StreamOut::WriteArrayEnd_(void)
    {
        assert(GetCurrent().IsArray());
        StreamValue back(std::move(writeStack_.back().value));
        writeStack_.pop_back();
        return back;
    }

#define _WriteValueDefault(intType)\
    template <>  StreamValue StreamOut::WriteValueRaw<intType>(const intType& value) { return StreamValue(value); }

#define _WriteIntAll(intTypeBase)\
    _WriteValueDefault(intTypeBase##64_t);\
    _WriteValueDefault(intTypeBase##32_t);\
    _WriteValueDefault(intTypeBase##16_t);\
    _WriteValueDefault(intTypeBase##8_t);

    _WriteIntAll(int);
    _WriteIntAll(uint);

    _WriteValueDefault(char);
    _WriteValueDefault(wchar_t);
    _WriteValueDefault(bool);
    _WriteValueDefault(float);
    _WriteValueDefault(double);

#define _WriteVec(typeBase,typeComponent,count)\
    template <> StreamValue StreamOut::WriteValueRaw<typeBase##count>(const typeBase##count& value) { return WriteVec<typeBase##count, typeComponent, count>(value); }

#define _WriteVecAll(typeBase,typeComponent)\
    _WriteVec(typeBase,typeComponent,2);\
    _WriteVec(typeBase,typeComponent,3);\
    _WriteVec(typeBase,typeComponent,4);

    _WriteVecAll(Vec, float);
    _WriteVecAll(DVec, double);
    _WriteVecAll(I8Vec, int8_t);
    _WriteVecAll(U8Vec, uint8_t);
    _WriteVecAll(I16Vec, int16_t);
    _WriteVecAll(U16Vec, uint16_t);
    _WriteVecAll(I32Vec, int32_t);
    _WriteVecAll(U32Vec, uint32_t);
    _WriteVecAll(I64Vec, int64_t);
    _WriteVecAll(U64Vec, uint64_t);
    _WriteVecAll(BVec, bool);

    template <> StreamValue StreamOut::WriteValueRaw<string_view_t>(const string_view_t& value)
    {
        return StreamValue(RAPIDSTR(value));
    }

    template <> StreamValue StreamOut::WriteValueRaw<string_t>(const string_t& value)
    {
        return StreamValue(RAPIDSTR(value));
    }

    template <> StreamValue StreamOut::WriteValueRaw<wstring_view_t>(const wstring_view_t& value)
    {
        string_t str = StringWideToMulti(value);
        return StreamValue(str.data(), static_cast<rapidjson::SizeType>(str.size()), allocator_);
    }

    template <> StreamValue StreamOut::WriteValueRaw<wstring_t>(const wstring_t& value)
    {
        string_t str = StringWideToMulti(value);
        return StreamValue(str.data(), static_cast<rapidjson::SizeType>(str.size()), allocator_);
    }

    void StreamOut::WritePop(void)
    {
        KeyValue& current = writeStack_.back();
        StreamValue& currentValue = current.value;
        string_view_arg currentKey(current.key);

        auto& parent = GetParent();
        assert(parent.IsObject() || parent.IsArray());
        if (parent.IsObject())
        {
            assert(!currentKey.empty());
            parent.AddMember(RAPIDSTR(currentKey), currentValue, allocator_);
        }
        else // Array
        {
            parent.PushBack(currentValue, allocator_);
        }
        writeStack_.pop_back();
    }

    StreamValue& StreamOut::GetCurrent(void)
    {
        return writeStack_.size() > 0 ? writeStack_.back().value : doc_;
    }

    StreamValue& StreamOut::GetParent(void)
    {
        return writeStack_.size() > 1 ? writeStack_[writeStack_.size() - 2].value : doc_;
    }

    template<typename T>
    StreamValue StreamOut::WriteValueRaw(const T& value)
    {
        static_assert(sizeof(T) == 0, "Only specializations allowed!");
        UNREFERENCED_PARAMETER(value);
        return StreamValue();
    }

    template<typename T, typename TComponent, size_t C>
    StreamValue StreamOut::WriteVec(const T& vec)
    {
        WriteArrayStart_();
        for (unsigned i = 0; i < C; ++i)
        {
            WriteValue<TComponent>(vec[(int)i]);
        }
        return WriteArrayEnd_();
    }

#pragma endregion Private Functions

}	// namespace
