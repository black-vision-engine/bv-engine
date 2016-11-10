#pragma once

#include "SerializationHelper.h"

namespace bv { namespace SerializationHelper {


// ***********************
//
template< typename T >
void SerializeAttribute( ISerializer& ser, const T& t, std::string name )
{
    ser.SetAttribute( name, T2String( t ) );
}

// ***********************
//
template< typename T >
std::string Enum2String( const std::pair< T, const char* > t2s[], const T& t )
{
    for( int i = 0; ; i++ )
        if( t2s[i].first == t )
            return t2s[i].second;
        else if( strcmp( t2s[i].second, "" ) == 0 )
            return "";
}


// ***********************
//
template< typename T >
T String2Enum( const std::pair< T, const char* > t2s[], const std::string& s )
{
    int i = 0;
    while( strcmp( t2s[i].second, "" ) )
    {
        if( t2s[i].second == s )
            return t2s[i].first;
        ++i;
    }
    return t2s[i].first;
}


// ***********************
//
template< typename T >
T String2Enum( const std::pair< T, const char* > t2s[], const std::string& s, const T& defaultVal )
{
    int i = 0;
    while( strcmp( t2s[i].second, "" ) )
    {
        if( t2s[i].second == s )
            return t2s[i].first;
        ++i;
    }
    return defaultVal;
}

} }
