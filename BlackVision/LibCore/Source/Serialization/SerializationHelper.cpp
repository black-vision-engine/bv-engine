#include "stdafxCore.h"

#include "SerializationHelper.h"

#include "Tools/StringHeplers.h"

#include "Mathematics/glm_inc.h"

#include <sstream>



namespace std
{
    string to_string( const glm::vec2 & v )
    {
        return to_string( v[ 0 ] ) + ", " + to_string( v[ 1 ] );
    }
    string to_string( const glm::vec3 & v )
    {
        return to_string( v[ 0 ] ) + ", " + to_string( v[ 1 ] ) + ", " + to_string( v[ 2 ] );
    }
    string to_string( const glm::vec4 & v )
    {
        return to_string( v[ 0 ] ) + ", " + to_string( v[ 1 ] ) + ", " + to_string( v[ 2 ] ) + ", " + to_string( v[ 3 ] );
    }
    string to_string( const std::string & val )
    {
        return val;
    }
    string to_string( const std::wstring & val )
    {
        return std::string( val.begin(), val.end() );
    }
}


namespace bv {

namespace SerializationHelper {

namespace {

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

} // anonymous

// *************************************
// String2T-s
// *************************************

// *************************************
//
template<> Expected< Float32 >      String2T( const std::string & s ) 
{ 
    char * end = nullptr;
    auto ret = strtod( s.c_str(), &end );

    if( !*end && end != s.c_str() )
    {
        if ( ret < std::numeric_limits< Float32 >::lowest() )
            return std::numeric_limits< Float32 >::lowest();

        if ( ret > std::numeric_limits< Float32 >::max() ) 
            return std::numeric_limits< Float32 >::max();

        return ( Float32 )ret;
    }

    return Expected< Float32 >();
}

// *************************************
//
template<> Expected< Float64 >      String2T( const std::string & s ) 
{ 
    char * end = nullptr;
    auto ret = strtod( s.c_str(), &end );

    if( !*end && end != s.c_str() )
    {
        return ret;
    }

    return Expected< Float64 >();
}

// *************************************
//
template<> Expected< bool >         String2T( const std::string & s ) 
{ 
    if( s == "true" ) 
        return true; 
    else if( s == "false" ) 
        return false; 
    return Expected< bool >(); 
}

// *************************************
//
template<> Expected< UInt32 >       String2T( const std::string & s ) 
{ 
    char * end = nullptr;
    auto ret = strtoul( s.c_str(), &end, 10 );

    if( !*end && end != s.c_str() )
    {
        if ( ret < std::numeric_limits< UInt32 >::lowest() )
            return std::numeric_limits< UInt32 >::lowest();

        if ( ret > std::numeric_limits< UInt32 >::max() ) 
            return std::numeric_limits< UInt32 >::max();

        return ( UInt32 )ret;
    }
    
    return Expected< UInt32 >();
}

// *************************************
//
template<> Expected< Int32 >        String2T( const std::string & s ) 
{ 
    char * end = nullptr;
    auto ret = strtol( s.c_str(), &end, 10 );

    if( !*end && end != s.c_str() )
    {
        if ( ret < std::numeric_limits< Int32 >::lowest() )
            return std::numeric_limits< Int32 >::lowest();

        if ( ret > std::numeric_limits< Int32 >::max() ) 
            return std::numeric_limits< Int32 >::max();

        return ( Int32 )ret;
    }

    return Expected< Int32 >();
}

// *************************************
//
template<> Expected< UInt64 >       String2T( const std::string & s ) 
{ 
    char * end = nullptr;
    auto ret = strtoul( s.c_str(), &end, 10 );

    if( !*end && end != s.c_str() )
    {
        return ret;
    }
    
    return Expected< UInt64 >();
}

// *************************************
//
template<> Expected< Int64 >        String2T( const std::string & s ) 
{ 
    char * end = nullptr;
    auto ret = strtol( s.c_str(), &end, 10 );

    if( !*end && end != s.c_str() )
    {
        return ret;
    }

    return Expected< Int64 >();
}

// *************************************
//
template< typename T > 
Expected< std::vector< T > > Strings2T( std::vector< std::string > strings )
{
    std::vector< T > ret;

    for( auto s : strings )
    {
        auto t = String2T< T >( s );
        if( t.isValid )
            ret.push_back( t );
        else
            return Expected< std::vector< T > >();
    }

    return ret;
}

template<> Expected< glm::vec2 >    String2T( const std::string & s ) 
{ 
    auto strings = SerializationHelper::_split( s, ',' );

    if( strings.size() == 2 )
    {
        auto vals = Strings2T< Float32 >( strings );
        if( vals.isValid )
            return glm::vec2( vals.ham[ 0 ], vals.ham[ 1 ] );
        else
            return Expected< glm::vec2 >();
    }
    else
        return Expected< glm::vec2 >();
}

// *************************************
//
template<> Expected< glm::vec3 >    String2T( const std::string & s )
{ 
    auto strings = SerializationHelper::_split( s, ',' );

    if( strings.size() == 3 )
    {
        auto vals = Strings2T< Float32 >( strings );
        if( vals.isValid )
            return glm::vec3( vals.ham[ 0 ], vals.ham[ 1 ], vals.ham[ 2 ] );
        else
            return Expected< glm::vec3 >();
    }
    else
        return Expected< glm::vec3 >();
}


// *************************************
//
template<> Expected< glm::vec4 >    String2T( const std::string & s )
{ 
    auto strings = SerializationHelper::_split( s, ',' );

    if( strings.size() == 4 )
    {
        auto vals = Strings2T< Float32 >( strings );
        if( vals.isValid )
            return glm::vec4( vals.ham[ 0 ], vals.ham[ 1 ], vals.ham[ 2 ], vals.ham[ 3 ] );
        else
            return Expected< glm::vec4 >();
    }
    else
        return Expected< glm::vec4 >();
}


// *************************************
//
template<> Expected< std::string >  String2T( const std::string & s ) 
{ 
    return s; 
}

// *************************************
//
template<> Expected< std::wstring > String2T( const std::string & s ) 
{ 
    return StringToWString( s ); 
}

// *************************************
//
template< typename T > 
T       String2T        ( const std::string & s, const T & defaultVal ) 
{ 
    auto ret = String2T< T >( s );
    if( ret.isValid )
        return ret.ham;
    else
        return defaultVal;
}

template bool           String2T        ( const std::string & s, const bool & defaultVal );
template Float32        String2T        ( const std::string & s, const Float32 & defaultVal );
template UInt32         String2T        ( const std::string & s, const UInt32 & defaultVal );
template Int32          String2T        ( const std::string & s, const Int32 & defaultVal );
//template UInt16         String2T        ( const std::string & s, const UInt16 & defaultVal );
template glm::vec2      String2T        ( const std::string & s, const glm::vec2 & defaultVal );
template glm::vec3      String2T        ( const std::string & s, const glm::vec3 & defaultVal );
template glm::vec4      String2T        ( const std::string & s, const glm::vec4 & defaultVal );

// *************************************
// T2String-s
// *************************************

template<>
std::string T2String( const bool& b )
{
    if( b )
        return "true";
    else
        return "false";
}

template<>
std::string T2String( const std::wstring& wstr )
{
    return WStringToString( wstr );
}


template< typename T >
std::string T2String( const T& t )
{
    return std::to_string( t );
}


template std::string T2String( const std::string& v );
template std::string T2String( const glm::vec2& v );
template std::string T2String( const glm::vec3& v );
template std::string T2String( const glm::vec4& v );
template std::string T2String( const float& v );
template std::string T2String( const double& v );
template std::string T2String( const int& v );
template std::string T2String( const unsigned int& v );
template std::string T2String( const UInt64& v );
template std::string T2String( const Int64& v );

} 
}