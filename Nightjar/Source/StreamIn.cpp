//------------------------------------------------------------------------------
//
// File Name:	StreamIn.cpp
// Author(s):	Connor Lariviere (connor.lariviere), Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Class for reading from a file.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "StreamIn.h"

#include <istreamwrapper.h>

//------------------------------------------------------------------------------
// Preprocessor Directives:
//------------------------------------------------------------------------------

#pragma warning(push)
#pragma warning(disable : 4583)

#define RAPIDSTR(str) rapidjson::GenericStringRef<char>(str.data(), static_cast<rapidjson::SizeType>(str.size()))

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

using namespace rapidjson;

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

    StreamIn::StreamIn(string_view_arg filePath)
        : Object()
        , doc_(nullptr)
        , current_(nullptr)
        , stack_()
        , path_(filePath)
        , lastError_(ReadValueError::None)
        , valid_(false)
    {
        std::ifstream inFileStream(path_.c_str());
        valid_ = inFileStream.is_open();

        if (!valid_)
        {
            TraceWarning("Failed to open StreamIn: \"{}\"", path_);
            return;
        }

        rapidjson::IStreamWrapper inStreamWrapper(inFileStream);

        doc_.ParseStream(inStreamWrapper);

        inFileStream.close();

        if (doc_.HasParseError())
        {
            TraceWarning("Parse error 0x{:02X} at offset {} when opening StreamIn: \"{}\"", (int)doc_.GetParseError(), doc_.GetErrorOffset(), path_);
            return;
        }

        // Arbitrarily reserve a stack size of 16 elements to avoid reallocations.
        stack_.reserve(16);

        ResetCurrent();
    }

    StreamIn::StreamIn(StreamIn&& other) noexcept
        : Object(std::move(other))
        , doc_(std::move(other.doc_))
        , current_(std::move(other.current_))
        , stack_(std::move(other.stack_))
        , path_(std::move(other.path_))
        , lastError_(std::move(other.lastError_))
        , valid_(std::move(other.valid_))
    {
    }

    //--------------------------------------------------------------------------

    StreamIn::~StreamIn()
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

    bool StreamIn::IsValid() const
    {
        return valid_;
    }

    const string_t& StreamIn::GetPath() const
    {
        return path_;
    }

    string_t StreamIn::GetHierarchy() const
    {
        string_t hierarchy = GetPath();
        char sep = ':';
        for (const auto& obj : stack_)
        {
            hierarchy += sep;
            sep = '|';
            if (obj.isKey)
            {
                hierarchy += obj.key;
            }
            else
            {
                FORMAT_TO(hierarchy, "{}", obj.index);
            }
        }
        return hierarchy;
    }

    bool StreamIn::InObject(void) const
    {
        assert(current_ != nullptr);
        return current_->IsObject();
    }

    bool StreamIn::InArray(void) const
    {
        assert(current_ != nullptr);
        return current_->IsArray();
    }

    DataType StreamIn::GetElementType(bool checkVector)
    {
        return GetElementType(*current_, checkVector);
    }

    DataType StreamIn::GetElementType(string_view_arg key, bool checkVector)
    {
        if (!InObject())
        {
            lastError_ = ReadValueError::KeyType;
            return DataType::Invalid;
        }
        auto obj = current_->GetObject();
        auto it = obj.FindMember(key.data());
        if (it == obj.MemberEnd())
        {
            lastError_ = ReadValueError::KeyMissing;
            return DataType::Invalid;
        }
        const auto& val = it->value;
        return GetElementType(val, checkVector);
    }

    DataType StreamIn::GetElementType(size_t index, bool checkVector)
    {
        if (!InArray())
        {
            lastError_ = ReadValueError::KeyType;
            return DataType::Invalid;
        }
        auto arr = current_->GetArray();
        if (index >= arr.Size())
        {
            lastError_ = ReadValueError::KeyMissing;
            return DataType::Invalid;
        }
        const auto& val = arr[static_cast<rapidjson::SizeType>(index)];
        return GetElementType(val, checkVector);
    }

    template <> bool StreamIn::TryReadValueInternal<bool>(const StreamValue& streamValue, bool& value)
    {
        if (streamValue.IsBool())
        {
            value = streamValue.GetBool();
            return true;
        }
        return false;
    }

#define _ReadInt(intType, readType)\
    template <> bool StreamIn::TryReadValueInternal<intType>(const StreamValue& streamValue, intType& value)\
    {\
        if (streamValue.Is##readType())\
        {\
            value = static_cast<intType>(streamValue.Get##readType());\
            return true;\
        }\
        return false;\
    }
#define _ReadIntAll(intTypeBase, readTypeBase)\
    _ReadInt(intTypeBase##64_t, readTypeBase##64);\
    _ReadInt(intTypeBase##32_t, readTypeBase);\
    _ReadInt(intTypeBase##16_t, readTypeBase);\
    _ReadInt(intTypeBase##8_t, readTypeBase);
    _ReadIntAll(int, Int);
    _ReadIntAll(uint, Uint);
    // "char" is not considered a signed or unsigned integer type, so it needs its own template.
    _ReadInt(char, Int);
    _ReadInt(wchar_t, Int);
#undef _ReadIntAll
#undef _ReadInt

    template <> bool StreamIn::TryReadValueInternal<float>(const StreamValue& streamValue, float& value)
    {
        if (streamValue.IsFloat())
        {
            value = streamValue.GetFloat();
            return true;
        }
        else if (streamValue.IsInt())
        {
            value = static_cast<float>(streamValue.GetInt());
            return true;
        }
        return false;
    }

    template <> bool StreamIn::TryReadValueInternal<double>(const StreamValue& streamValue, double& value)
    {
        if (streamValue.IsDouble())
        {
            value = streamValue.GetDouble();
            return true;
        }
        else if (streamValue.IsInt64())
        {
            value = static_cast<double>(streamValue.GetInt64());
            return true;
        }
        return false;
    }

#define _ReadVec(vtype, dim, etype)\
    template <> bool StreamIn::TryReadValueInternal<vtype##dim>(const StreamValue& streamValue, vtype##dim& value)\
    {\
        return ReadVecT<etype, dim>(streamValue, &value[0]);\
    }
#define _ReadVecAll(vtype, etype)\
    _ReadVec(vtype, 2, etype);\
    _ReadVec(vtype, 3, etype);\
    _ReadVec(vtype, 4, etype);
    _ReadVecAll(Vec, float);
    _ReadVecAll(DVec, double);
    _ReadVecAll(I8Vec, int8_t);
    _ReadVecAll(U8Vec, uint8_t);
    _ReadVecAll(I16Vec, int16_t);
    _ReadVecAll(U16Vec, uint16_t);
    _ReadVecAll(I32Vec, int32_t);
    _ReadVecAll(U32Vec, uint32_t);
    _ReadVecAll(I64Vec, int64_t);
    _ReadVecAll(U64Vec, uint64_t);
    _ReadVecAll(BVec, bool);
#undef _ReadVecAll
#undef _ReadVec

    template <> bool StreamIn::TryReadValueInternal<string_t>(const StreamValue& streamValue, string_t& value)
    {
        if (streamValue.IsString())
        {
            value = { streamValue.GetString(), streamValue.GetStringLength() };
            return true;
        }
        return false;
    }

    template <> bool StreamIn::TryReadValueInternal<string_view_t>(const StreamValue& streamValue, string_view_t& value)
    {
        if (streamValue.IsString())
        {
            value = { streamValue.GetString(), streamValue.GetStringLength() };
            return true;
        }
        return false;
    }

    template <> bool StreamIn::TryReadValueInternal<wstring_t>(const StreamValue& streamValue, wstring_t& value)
    {
        if (streamValue.IsString())
        {
            const string_view_t str(streamValue.GetString(), streamValue.GetStringLength());
            value = StringMultiToWide(str);
            return true;
        }
        return false;
    }

    // wstring_view_t is not supported because it is non-owning and json documents are parsed as UTF-8, so any conversions would have to be stored (i.e. wstring_t) or they would be invalidated by falling out of scope.

    void StreamIn::ReadArrayObjects(string_view_arg key, const std::function<void(StreamIn&, int)>& objectRead)
    {
        const auto& arr = (*current_)[&key.front()];
        assert(arr.IsArray());
        stack_.emplace_back(string_t(key), current_);

        int index = 0;
        for (auto& v : arr.GetArray())
        {
            assert(v.IsObject());
            //sets previous node

            stack_.emplace_back(index, current_);
            //sets current node 
            current_ = &v;

            objectRead(*this, index++);
            StepBack();
        }
        StepBack();
    }

    int StreamIn::GetArraySize(string_view_arg key)
    {
        assert(current_ && current_->IsObject());
        auto obj = current_->GetObject();
        auto it = obj.FindMember(&key.front());
        return it != obj.MemberEnd() && it->value.IsArray() ? static_cast<int>(it->value.GetArray().Size()) : -1;
    }

    void StreamIn::ReadObjectKeyValues(string_view_arg key, const std::function<void(string_view_arg, StreamIn&, int)>& objectRead)
    {
        const auto& obj = (*current_)[&key.front()];
        assert(obj.IsObject());
        stack_.emplace_back(string_t(key), current_);

        int index = 0;
        for (auto& v : obj.GetObject())
        {
            assert(v.value.IsObject());
            //sets previous node

            const string_view_t name = v.name.GetString();

            stack_.emplace_back(name, current_);
            //sets current node 
            current_ = &v.value;

            objectRead(name, *this, index++);
            StepBack();
        }
        StepBack();
    }

    int StreamIn::GetObjectSize(string_view_arg key)
    {
        assert(current_ && current_->IsObject());
        auto obj = current_->GetObject();
        auto it = obj.FindMember(&key.front());
        return it != obj.MemberEnd() && it->value.IsObject() ? static_cast<int>(it->value.GetObject().MemberCount()) : -1;
    }

    bool StreamIn::StepBack()
    {
        if (stack_.empty()) return false;
        current_ = stack_.back().value;
        stack_.pop_back();
        return true;
    }

    bool StreamIn::StepInto(string_view_arg key)
    {
        if (!InObject())
        {
            return false;
        }
        auto obj = current_->GetObject();
        auto it = obj.FindMember(&key.front());
        if (it == obj.MemberEnd())
        {
            return false;
        }
        // Push previous node onto stack.
        stack_.emplace_back(key, current_);
        // Enter new node.
        current_ = &it->value;
        return true;
    }

    bool StreamIn::StepInto(size_t index)
    {
        if (!InArray())
        {
            return false;
        }
        auto arr = current_->GetArray();
        if (static_cast<rapidjson::SizeType>(index) >= arr.Size())
        {
            return false;
        }
        // Push previous node onto stack.
        stack_.emplace_back(index, current_);
        // Enter new node.
        current_ = &arr[static_cast<rapidjson::SizeType>(index)];
        return false;
    }

    void StreamIn::ResetCurrent()
    {
        stack_.clear();
        current_ = &doc_;
    }

    bool StreamIn::StreamHas(string_view_arg key) const
    {

        if (!current_ || !current_->IsObject()) return false;
        auto obj = current_->GetObject();
        return obj.HasMember(&key.front());

    }

    StreamIn::ReadValueError StreamIn::GetLastError(void) const
    {
        return lastError_;
    }

    template <> bool StreamIn::ReadArrayElement<bool>()
    {
        assert(current_->IsBool());
        return current_++->GetBool();
    }

#define _ReadArrayInt(intType, readType)\
    template <> intType StreamIn::ReadArrayElement<intType>()\
    {\
        assert(current_->Is##readType());\
        return static_cast<intType>(current_++->Get##readType());\
    }
#define _ReadArrayIntAll(intTypeBase, readTypeBase)\
    _ReadArrayInt(intTypeBase##64_t, readTypeBase##64);\
    _ReadArrayInt(intTypeBase##32_t, readTypeBase);\
    _ReadArrayInt(intTypeBase##16_t, readTypeBase);\
    _ReadArrayInt(intTypeBase##8_t, readTypeBase);
    _ReadArrayIntAll(int, Int);
    _ReadArrayIntAll(uint, Uint);
    // "char" is not considered a signed or unsigned integer type, so it needs its own template.
    _ReadArrayInt(char, Int);
#undef _ReadArrayIntAll
#undef _ReadArrayInt

    template <> float StreamIn::ReadArrayElement<float>()
    {
        assert(current_->IsFloat() || current_->IsInt());
        return current_->IsInt() ? static_cast<float>(current_++->GetInt()) : current_++->GetFloat();
    }

    template <> double StreamIn::ReadArrayElement<double>()
    {
        assert(current_->IsDouble() || current_->IsInt64());
        return current_->IsInt64() ? static_cast<double>(current_++->GetInt64()) : current_++->GetDouble();
    }

    template <> string_t StreamIn::ReadArrayElement<string_t>()
    {
        assert(current_->IsString());
        string_t value{ current_->GetString(), current_->GetStringLength() };
        ++current_;
        return value;
    }

    template <> string_view_t StreamIn::ReadArrayElement<string_view_t>()
    {
        assert(current_->IsString());
        string_view_t value{ current_->GetString(), current_->GetStringLength() };
        ++current_;
        return value;
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

    StreamIn::KeyValue::KeyValue()
        : key()
        , value(nullptr)
        , isKey(true)
    {
    }

    StreamIn::KeyValue::KeyValue(string_view_arg key_, const StreamValue* value_)
        : key(key_)
        , value(value_)
        , isKey(true)
    {
    }

    StreamIn::KeyValue::KeyValue(size_t index_, const StreamValue* value_)
        : index(index_)
        , value(value_)
        , isKey(false)
    {
    }

    StreamIn::KeyValue::KeyValue(KeyValue&& other) noexcept
        : value(std::exchange(other.value, nullptr))
        , isKey(std::move(other.isKey))
    {
        if (isKey)
        {
            ::new (&key) string_t(std::move(other.key));
        }
        else
        {
            ::new (&index) size_t(std::move(other.index));
        }
    }

    StreamIn::KeyValue::~KeyValue(void)
    {
        if (isKey)
        {
            key.~string_t();
        }
    }

    template<modifiable_t T>
    bool StreamIn::TryReadValueInternal([[maybe_unused]] const StreamValue& streamValue, [[maybe_unused]] T& value)
    {
        static_assert(sizeof(T) == 0, "Only specializations allowed!");
        return false;
    }

    template <modifiable_t T, size_t C>
    bool StreamIn::ReadVecT(const StreamValue& streamValue, T* vec)
    {
        if (!streamValue.IsArray())
        {
            return false;
        }
        auto array = streamValue.GetArray();
        unsigned char vecSize;
        auto arrSize = array.Size();
        if (arrSize < C)
        {
            TraceWarning("Array too small ({}/{}) at {}", arrSize, C, GetHierarchy());
            vecSize = static_cast<unsigned char>(arrSize);
        }
        else if (arrSize > C)
        {
            TraceWarning("Array too large ({}/{}) at {}", arrSize, C, GetHierarchy());
            vecSize = static_cast<unsigned char>(C);
        }
        else
        {
            vecSize = static_cast<unsigned char>(C);
        }
        stack_.emplace_back("<ARRAY_INTERNAL>", current_);
        current_ = array.Begin();
        for (unsigned char i{ 0 }; i < vecSize; ++i)
        {
#ifdef _DEBUG
            // Used to make sure that the array index always progresses forward
            const StreamValue* currentStart = current_;
#endif
            vec[i] = ReadArrayElement<T>();
#ifdef _DEBUG
            assert(current_ > currentStart);
#endif
        }
        StepBack();
        return true;
    }

    DataType StreamIn::GetElementType(const StreamValue& value, bool checkVector)
    {
        using rapidjson::Type;
        Type type = value.GetType();
        switch (type)
        {
        case Type::kNullType:
            return DataType::Invalid;
        case Type::kFalseType:
        case Type::kTrueType:
            return DataType::Bool;
        case Type::kObjectType:
            return DataType::Object;
        case Type::kArrayType:
            if (checkVector)
            {
                auto arr = value.GetArray();
                auto arrSize = arr.Size();
                if (arrSize <= 1 || arrSize > 4)
                {
                    return DataType::Array;
                }
                DataType elementType = DataType::Array;
                int64_t i64Min = 0;
                uint64_t u64Max = 0;
                //double doubleMin = 0, doubleMax = 0;
                for (rapidjson::SizeType i = 0; i < arrSize; ++i)
                {
                    const auto& elementVal = arr[i];
                    if (elementVal.GetType() != Type::kNumberType)
                    {
                        return DataType::Array;
                    }
                    if (elementVal.IsInt())
                    {
                        int32_t val = elementVal.GetInt();
                        i64Min = MathMin(i64Min, (int64_t)val);
                        u64Max = MathMax(u64Max, (uint64_t)MathMax(val, 0));
                        //doubleMin = MathMin(doubleMin, (double)val);
                        //doubleMax = MathMax(doubleMax, (double)val);
                    }
                    else if (elementVal.IsUint())
                    {
                        uint32_t val = elementVal.GetUint();
                        u64Max = MathMax(u64Max, (uint64_t)val);
                        //doubleMax = MathMax(doubleMax, (double)val);
                    }
                    else if (elementVal.IsInt64())
                    {
                        int64_t val = elementVal.GetInt64();
                        i64Min = MathMin(i64Min, (int64_t)val);
                        u64Max = MathMax(u64Max, (uint64_t)MathMax(val, 0ll));
                        //doubleMin = MathMin(doubleMin, (double)val);
                        //doubleMax = MathMax(doubleMax, (double)val);
                    }
                    else if (elementVal.IsUint64())
                    {
                        uint64_t val = elementVal.GetUint64();
                        u64Max = MathMax(u64Max, (uint64_t)val);
                        //doubleMax = MathMax(doubleMax, (double)val);
                    }
                    else if (elementVal.IsFloat())
                    {
                        elementType = DataType::Float;
                        //float val = elementVal.GetFloat();
                        //doubleMin = MathMin(doubleMin, (double)val);
                        //doubleMax = MathMax(doubleMax, (double)val);
                    }
                    else if (elementVal.IsDouble())
                    {
                        elementType = DataType::Double;
                        //double val = elementVal.GetDouble();
                        //doubleMin = MathMin(doubleMin, (double)val);
                        //doubleMax = MathMax(doubleMax, (double)val);
                    }
                    else
                    {
                        return DataType::Array;
                    }
                }
                static constexpr int32_t fltMax = 1 << std::numeric_limits<float>::digits;
                static constexpr int64_t dblMax = 1ll << std::numeric_limits<double>::digits;
                switch (elementType)
                {
#define RETURN_VEC_OFFSET(vec) DataType(static_cast<std::underlying_type_t<DataType>>(DataType::vec##2) + (arrSize - 2))
                case DataType::Float:
                {
                    if (i64Min >= -fltMax && u64Max <= (uint64_t)fltMax)
                    {
                        return RETURN_VEC_OFFSET(Vec);
                    }
                    else
                    {
                        if (i64Min < -dblMax || u64Max > (uint64_t)dblMax)
                        {
                            TraceWarning("Unable to accurately represent arithmetic float vector! Path: \"{}\"", GetHierarchy());
                        }
                        return RETURN_VEC_OFFSET(DVec);
                    }
                }
                    break;
                case DataType::Double:
                {
                    if (i64Min < -dblMax || u64Max > (uint64_t)dblMax)
                    {
                        TraceWarning("Unable to accurately represent arithmetic double vector! Path: \"{}\"", GetHierarchy());
                    }
                    return RETURN_VEC_OFFSET(DVec);
                }
                    break;
                case DataType::Array:
                {
                    if (i64Min >= (int64_t)std::numeric_limits<int32_t>::min() && u64Max <= (uint64_t)std::numeric_limits<int32_t>::max())
                    {
                        return RETURN_VEC_OFFSET(I32Vec);
                    }
                    else if (i64Min >= (int64_t)std::numeric_limits<uint32_t>::min() && u64Max <= (uint64_t)std::numeric_limits<uint32_t>::max())
                    {
                        return RETURN_VEC_OFFSET(U32Vec);
                    }
                    else if (i64Min >= (int64_t)std::numeric_limits<int64_t>::min() && u64Max <= (uint64_t)std::numeric_limits<int64_t>::max())
                    {
                        return RETURN_VEC_OFFSET(I64Vec);
                    }
                    else if (i64Min >= (int64_t)std::numeric_limits<uint64_t>::min() && u64Max <= (uint64_t)std::numeric_limits<uint64_t>::max())
                    {
                        return RETURN_VEC_OFFSET(U64Vec);
                    }
                    else
                    {
                        TraceWarning("Unable to accurately represent arithmetic integral vector! Path: \"{}\"", GetHierarchy());
                        return RETURN_VEC_OFFSET(DVec);
                    }
                }
                    break;
                default:
                    break;
                }
            }
            return DataType::Array;
        case Type::kStringType:
            return DataType::String;
        case Type::kNumberType:
        {
            if (value.IsInt())
            {
                return DataType::Int;
            }
            else if (value.IsUint())
            {
                return DataType::UInt;
            }
            else if (value.IsInt64())
            {
                return DataType::Long;
            }
            else if (value.IsUint64())
            {
                return DataType::ULong;
            }
            else if (value.IsFloat())
            {
                return DataType::Float;
            }
            else if (value.IsDouble())
            {
                return DataType::Double;
            }
        }
            break;
        default:
            break;
        }
        return DataType::Invalid;
    }

    bool StreamIn::FindMember(string_view_arg key, const StreamValue*& value)
    {
        assert(current_ && current_->IsObject());
        auto obj = current_->GetObject();
        auto it = obj.FindMember(RAPIDSTR(key));
        bool found = it != obj.MemberEnd();
        if (found)
        {
            value = &it->value;
        }
        return found;
    }

    bool StreamIn::FindMember(size_t index, const StreamValue*& value)
    {
        assert(current_ && current_->IsArray());
        auto arr = current_->GetArray();
        auto indexClamp = static_cast<rapidjson::SizeType>(index);
        bool success = indexClamp < arr.Size();
        if (success)
        {
            value = &arr[indexClamp];
        }
        return success;
    }

#pragma endregion Private Functions

}	// namespace

#pragma warning(pop)
