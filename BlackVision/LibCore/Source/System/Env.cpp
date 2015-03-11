#include "Env.h"

#include <cstdlib>


namespace bv {

// ******************************
//
bool         Env::GetVar ( const std::string & var, std::string * varVal )
{
    const char * varValRaw = getenv( var.c_str() );
    auto retVal = false;

    if( varValRaw )
    {
        if( varVal )
        {
            *varVal = std::string( varValRaw );
        }
    
        retVal = true;
    }

    return retVal;
}

// ******************************
//
std::string  Env::GetVar ( const std::string & var )
{
    std::string varVal;

    auto success = Env::GetVar( var, &varVal );
    
    if( !success )
    {
        varVal = "";
    }

    return varVal;
}

} //bv
