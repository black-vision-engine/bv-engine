#pragma once

#include "ConversionHelper.h"



namespace bv {
namespace Convert
{


// ***********************
//
template< typename T >
std::string         Enum2String             ( const std::pair< T, const char* > t2s[], const T& t )
{
    for( int i = 0; ; i++ )
        if( t2s[ i ].first == t )
            return t2s[ i ].second;
        else if( strcmp( t2s[ i ].second, "" ) == 0 )
            return "";
}

// ***********************
//
template< typename T >
Expected< T >       String2Enum             ( const std::pair< T, const char* > t2s[], const std::string& s )
{
    int i = 0;
    while( strcmp( t2s[ i ].second, "" ) )
    {
        if( t2s[ i ].second == s )
            return t2s[ i ].first;
        ++i;
    }
    return Expected< T >();
}

// ***********************
//
template< typename T >
T                   String2Enum             ( const std::pair< T, const char* > t2s[], const std::string& s, const T& defaultVal )
{
    Expected< T > result = String2Enum< T >( t2s, s );

    if( result.IsValid() )
        return result.GetVal();
    else
        return defaultVal;
}

}   // Convert
}   // bv
