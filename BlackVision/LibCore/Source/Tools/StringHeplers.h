#pragma once

// #pragma directive added due to a warning produced by boost::split:
// 'std::_Copy_impl': Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'
#pragma warning ( push )
#pragma warning ( disable : 4996 )

#include <string>
#include <vector>

typedef std::vector< std::string > StrVec;


namespace bv {

std::wstring    StringToWString ( const std::string & data );
std::string     WStringToString ( const std::wstring & data );

StrVec          Split           ( const std::string & str, const std::string & sep );
std::string     Join            ( const StrVec & strVec, const std::string & sep );
std::string     Trim            ( const std::string & str, const std::string & mark );

} //bv

#pragma warning ( pop )
