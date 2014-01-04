#pragma once

#include <string>
#include <iostream>


namespace bv
{

bool FileExists ( const std::string & fileName );
bool ReadFile   ( std::ostream & out, const std::string & fileName );

} //bv
