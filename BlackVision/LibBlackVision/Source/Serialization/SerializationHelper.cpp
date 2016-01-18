#include "SerializationHelper.h"

#include <sstream>

namespace bv {

namespace SerializationHelper {

inline std::vector<std::string> &_split(const std::string &s, char delim, std::vector<std::string> &elems) 
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

inline std::vector<std::string> _split(const std::string &s, char delim) 
{
    std::vector<std::string> elems;
    _split(s, delim, elems);
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim ) { return _split( s, delim ); }

glm::vec4               String2Vec4( const std::string & s )
{
    auto vals = SerializationHelper::_split( s, ',' );
    assert( vals.size() == 4 );
    return glm::vec4( std::stof( vals[0] ), 
        std::stof( vals[1] ), 
        std::stof( vals[2] ), 
        std::stof( vals[3] ) );
}

glm::vec3               String2Vec3( const std::string & s )
{
    auto vals = SerializationHelper::_split( s, ',' );
    assert( vals.size() == 3 );
    return glm::vec3( std::stof( vals[0] ), 
        std::stof( vals[1] ), 
        std::stof( vals[2] ) );
}

glm::vec2               String2Vec2( const std::string & s )
{
    auto vals = SerializationHelper::_split( s, ',' );
    assert( vals.size() == 2 );
    return glm::vec2( std::stof( vals[0] ), 
        std::stof( vals[1] ) );
}

template<> Float32 _String2T( const std::string & s ) { return std::stof( s ); }
template<> bool _String2T( const std::string & s ) { if( s == "true" ) return true; else if( s == "false" ) return false; assert( false ); return false; } // FIXME error handling
template<> UInt32 _String2T( const std::string & s ) { return std::stoul( s ); }
template<> Int32 _String2T( const std::string & s ) { return std::stoi( s ); }
template<> UInt64 _String2T( const std::string & s ) { return std::stoull( s ); }
template<> Int64 _String2T( const std::string & s ) { return std::stoll( s ); }
template<> glm::vec2 _String2T( const std::string & s ) { return String2Vec2( s ); }
template<> glm::vec3 _String2T( const std::string & s ) { return String2Vec3( s ); }
template<> glm::vec4 _String2T( const std::string & s ) { return String2Vec4( s ); }


// FIXME: are those three needed?
template<> double _String2T( const std::string & s ) { return std::stod( s ); }
template<> std::string _String2T( const std::string & s ) { return s; }
template<> std::wstring _String2T( const std::string & s ) { return std::wstring( s.begin(), s.end() ); }


// *************************************
//
template<> 
UInt32       String2T        ( const std::string & s, const UInt32 & defaultVal ) 
{ 
    char * end = nullptr;
    auto ret = strtoul( s.c_str(), &end, 10 );

    if( !*end && end != s.c_str() )
    {
        if ( ret < std::numeric_limits< UInt32 >::min() )
            return std::numeric_limits< UInt32 >::min();

        if ( ret > std::numeric_limits< UInt32 >::max() ) 
            return std::numeric_limits< UInt32 >::max();

        return ( UInt32 )ret;
    }

    return defaultVal;
}

// *************************************
//
template<> 
Int32       String2T        ( const std::string & s, const Int32 & defaultVal ) 
{ 
    char * end = nullptr;
    auto ret = strtol( s.c_str(), &end, 10 );

    if( !*end && end != s.c_str() )
    {
        if ( ret < std::numeric_limits< Int32 >::min() )
            return std::numeric_limits< Int32 >::min();

        if ( ret > std::numeric_limits< Int32 >::max() ) 
            return std::numeric_limits< Int32 >::max();

        return ( Int32 )ret;
    }

    return defaultVal;
}

// *************************************
//
template<> 
Float32       String2T        ( const std::string & s, const Float32 & defaultVal ) 
{ 
    char * end = nullptr;
    auto ret = strtod( s.c_str(), &end );

    if( !*end && end != s.c_str() )
    {
        if ( ret < std::numeric_limits< Float32 >::min() )
            return std::numeric_limits< Float32 >::min();

        if ( ret > std::numeric_limits< Float32 >::max() ) 
            return std::numeric_limits< Float32 >::max();

        return ( Float32 )ret;
    }

    return defaultVal;
}

// *************************************
//
template<> 
bool        String2T    ( const std::string & s, const bool & defaultVal ) 
{ 
    if( s == "true" || s == "1" )
        return true;
    else if( s == "false" || s == "0" )
        return false;
    else
        return defaultVal;
}

// *************************************
//
template<> 
glm::vec2   String2T    ( const std::string & s, const glm::vec2 & defaultVal )
{
    auto vals = SerializationHelper::_split( s, ',' );
    if( vals.size() == 2 )
    {
        return glm::vec2( String2T< Float32 >( vals[ 0 ], defaultVal.x ), 
                          String2T< Float32 >( vals[ 1 ], defaultVal.y ) );
    }
    return defaultVal;
}

// *************************************
//
template<> 
glm::vec3   String2T    ( const std::string & s, const glm::vec3 & defaultVal )
{
    auto vals = SerializationHelper::_split( s, ',' );
    if( vals.size() == 3 )
    {
        return glm::vec3( String2T< Float32 >( vals[ 0 ], defaultVal.x ), 
                          String2T< Float32 >( vals[ 1 ], defaultVal.y ),
                          String2T< Float32 >( vals[ 2 ], defaultVal.z ) );
    }
    return defaultVal;
}

// *************************************
//
template<> 
glm::vec4   String2T    ( const std::string & s, const glm::vec4 & defaultVal )
{
    auto vals = SerializationHelper::_split( s, ',' );
    if( vals.size() == 4 )
    {
        return glm::vec4( String2T< Float32 >( vals[ 0 ], defaultVal.x ), 
                          String2T< Float32 >( vals[ 1 ], defaultVal.y ),
                          String2T< Float32 >( vals[ 2 ], defaultVal.z ),
                          String2T< Float32 >( vals[ 3 ], defaultVal.w ) );
    }
    return defaultVal;
}

// *************************************
//
template< typename T >
std::string T2String( const T& t )
{
    return std::to_string( t );
}

template<>
std::string T2String( const bool& b )
{
    if( b )
        return "true";
    else
        return "false";
}

template std::string T2String( const glm::vec2& v );
template std::string T2String( const glm::vec3& v );
template std::string T2String( const glm::vec4& v );
template std::string T2String( const float& v );
template std::string T2String( const int& v );
template std::string T2String( const unsigned int& v );
template std::string T2String( const UInt64& v );
template std::string T2String( const Int64& v );

} }

namespace std
{
    string to_string( const glm::vec2 & v ) { return to_string( v[0] ) + ", " + to_string( v[1] ); }
    string to_string( const glm::vec3 & v ) { return to_string( v[0] ) + ", " + to_string( v[1] ) + ", " + to_string( v[2] ); }
    string to_string( const glm::vec4 & v ) { return to_string( v[0] ) + ", " + to_string( v[1] ) + ", " + to_string( v[2] ) + ", " + to_string( v[3] ); }
    string to_string( const std::string & val ) { return val; }
    string to_string( const std::wstring & val ) { return std::string( val.begin(), val.end() ); }
}
