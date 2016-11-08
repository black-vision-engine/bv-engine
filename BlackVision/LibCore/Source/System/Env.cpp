#include "stdafx.h"

#include "Env.h"

#include <cstdlib>


namespace bv {

// ******************************
// @ see: https://msdn.microsoft.com/fr-fr/library/ms175774%28v=vs.90%29.aspx
bool         Env::GetVar ( const std::string & var, std::string * varVal )
{
    char    *pVal;
    size_t  len;
    _dupenv_s( &pVal, &len, var.c_str() );

    auto retVal = false;

    if( pVal )
    {
        if( varVal )
        {
            *varVal = std::string( pVal );
        }
    
        retVal = true;
    }

    free( pVal );

    return retVal;
}

// ******************************
//
std::string  Env::GetVar ( const std::string & var )
{
//    {var;}
//    return "GLOBAL_EFFECT_05";
    std::string varVal;

    auto success = Env::GetVar( var, &varVal );
    
    if( !success )
    {
        varVal = "";
    }

    return varVal;
}

} //bv
