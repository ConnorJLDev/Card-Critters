//------------------------------------------------------------------------------
//
// File Name:	UtilityString.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		Utility functions for strings.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "UtilityString.h"

#include "Windows.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar::Utility
{
    size_t StringHash::operator()(string_view_arg str) const
    {
        return std::hash<string_view_t>{}(str);
    }

    size_t StringHash::operator()(const std::string& str) const
    {
        return std::hash<std::string>{}(str);
    }

    size_t StringHash::operator()(const string_t& str) const
    {
        return std::hash<string_t>{}(str);
    }

    size_t StringHash::operator()(const char* str) const
    {
        return std::hash<string_view_t>{}(str);
    }
    
    size_t StringHash::operator()(wstring_view_arg str) const
    {
        return std::hash<wstring_view_t>{}(str);
    }

    size_t StringHash::operator()(const std::wstring& str) const
    {
        return std::hash<std::wstring>{}(str);
    }

    size_t StringHash::operator()(const wstring_t& str) const
    {
        return std::hash<wstring_t>{}(str);
    }

    size_t StringHash::operator()(const wchar_t* str) const
    {
        return std::hash<wstring_view_t>{}(str);
    }

    string_t StringWideToMulti(wstring_view_arg wstr)
    {
        constexpr UINT encoding = CP_UTF8;
        string_t str{};
        const int wstrSize = (int)wstr.size();
        int strSize = WideCharToMultiByte(encoding, 0, wstr.data(), wstrSize, nullptr, 0, nullptr, nullptr);
        assert(strSize >= 0);
        if (strSize == 0)
        {
            assert(wstrSize == 0);
        }
        else
        {
            str.resize((size_t)strSize);
            WideCharToMultiByte(encoding, 0, wstr.data(), wstrSize, &str[0], strSize, nullptr, nullptr);
        }
        return str;
    }

    wstring_t StringMultiToWide(string_view_arg str)
    {
        constexpr UINT encoding = CP_UTF8;
        wstring_t wstr{};
        const int strSize = (int)str.size();
        int wstrSize = MultiByteToWideChar(encoding, 0, str.data(), strSize, nullptr, 0);
        assert(wstrSize >= 0);
        if (wstrSize == 0)
        {
            assert(strSize == 0);
        }
        else
        {
            wstr.resize((size_t)wstrSize);
            MultiByteToWideChar(encoding, 0, str.data(), strSize, &wstr[0], wstrSize);
        }
        return wstr;
    }

    // Source - https://stackoverflow.com/a/3418285
    // Posted by Michael Mrozek, modified by community. See post 'Timeline' for change history
    // Retrieved 2025-11-08, License - CC BY-SA 3.0
    string_t& StringReplaceAll(string_t& str, string_view_arg find, string_view_arg replace)
    {
        if (find.empty())
        {
            return str;
        }
        size_t start_pos = 0;
        while ((start_pos = str.find(find, start_pos)) != string_t::npos)
        {
            str.replace(start_pos, find.length(), replace);
            start_pos += replace.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
        }
        return str;
    }

}	// namespace
