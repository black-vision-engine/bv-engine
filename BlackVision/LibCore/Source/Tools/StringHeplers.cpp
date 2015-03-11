#include "StringHeplers.h"

#include <locale>
#include <codecvt>


//@see: http://stackoverflow.com/questions/2573834/c-convert-string-or-char-to-wstring-or-wchar-t
// std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
// std::string narrow = converter.to_bytes(L"foo");
// std::wstring wide = converter.from_bytes("bar");

namespace bv {

// *******************************
//
std::wstring StringToWString( const std::string & data )
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    
    return converter.from_bytes( data );
}

// *******************************
//
std::string WStringToString( const std::wstring & data )
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

    return converter.to_bytes( data );
}

} //bv
