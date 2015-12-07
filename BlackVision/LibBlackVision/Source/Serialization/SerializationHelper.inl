#include "Serialization/SerializationHelper.h"

namespace bv { namespace SerializationHelper {

template< typename T >
void SerializeAttribute( ISerializer& ser, const T& t, std::string name )
{
    ser.SetAttribute( name, T2String( t ) );
}

template< typename T >
std::string Enum2String( const std::pair< T, const char* > t2s[], const T& t )
{
    for( int i = 0; ; i++ ) // FIXME so much
        if( t2s[i].first == t )
            return t2s[i].second;
        else if( strcmp( t2s[i].second, "" ) == 0 )
            return "";
}

template< typename T >
T String2T( const std::pair< T, const char* > t2s[], const std::string& s )
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

std::vector<std::string> split(const std::string &s, char delim);

template< typename T >
T _String2T( std::string s );

template< typename T, typename U >
std::pair< T, U > String2Pair( std::string s )
{
    auto ss = split( s, ',' );
    assert( ss.size() == 2 );
    return std::make_pair< T, U >(
        _String2T< T >( ss[ 0 ] ),
        _String2T< U >( ss[ 1 ] ) );
};

} }
