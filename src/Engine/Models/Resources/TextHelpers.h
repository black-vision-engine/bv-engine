#pragma once

#include <string>

namespace bv
{

struct TextHelper
{
    static size_t       GetSizeOfFile           ( const std::wstring& path );
    static std::wstring LoadUtf8FileToString    ( const std::wstring& filename );
};


} // bv