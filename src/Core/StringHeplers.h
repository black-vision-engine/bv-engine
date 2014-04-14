#pragma once

#include <string>


namespace bv {

std::wstring StringToWString( const std::string & data );
std::string WStringToString( const std::wstring & data );

} //bv
