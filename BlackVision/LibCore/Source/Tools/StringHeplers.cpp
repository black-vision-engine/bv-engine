#include "stdafxCore.h"

#include "StringHeplers.h"

#include <locale>
#include <codecvt>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>


//@see: http://stackoverflow.com/questions/2573834/c-convert-string-or-char-to-wstring-or-wchar-t
// std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
// std::string narrow = converter.to_bytes(L"foo");
// std::wstring wide = converter.from_bytes("bar");

namespace bv {

// *******************************
//
Expected< std::wstring >        StringToWString     ( const std::string & data )
{
    try
    {
        std::wstring_convert< std::codecvt_utf8_utf16< wchar_t > > converter;
        return converter.from_bytes( data );
    }
    catch( ... )
    {
        return Expected< std::wstring >();
    };
    
}

// *******************************
//
Expected< std::string >         WStringToString     ( const std::wstring & data )
{
    try
    {
        std::wstring_convert< std::codecvt_utf8_utf16< wchar_t > > converter;
        return converter.to_bytes( data );
    }
    catch( ... )
    {
        return Expected< std::string >();
    }
}

// *******************************
//
StrVec          Split           ( const std::string & str, const std::string & sep )
{
    StrVec ret;

    boost::split( ret, str, boost::is_any_of( sep ) );

    return ret;
}

// *******************************
//
std::string     Join            ( const StrVec & strVec, const std::string & sep )
{
    return boost::algorithm::join( strVec, sep );
}

// *******************************
//
std::string     Trim            ( const std::string & str, const std::string & mark )
{
    auto ret = str;
    ret.erase( ret.find_last_not_of( mark ) + 1 );
    ret.erase( 0, ret.find_first_not_of( mark ) );
    return ret;
}

} //bv
