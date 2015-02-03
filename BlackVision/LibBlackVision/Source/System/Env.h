#pragma once

#include <string>


namespace bv
{

class Env
{
public:

    static bool         GetVar ( const std::string & var, std::string * varVal );
    static std::string  GetVar ( const std::string & var );

};

} // bv
