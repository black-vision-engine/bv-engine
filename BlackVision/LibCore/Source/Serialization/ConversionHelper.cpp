#include "stdafxCore.h"

#include "ConversionHelper.h"

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
namespace Convert
{


namespace impl
{


// ***********************
//
inline std::vector< std::string > &     Split      ( const std::string & s, char delim, std::vector< std::string > & elems ) 
{
    std::stringstream ss( s );
    std::string item;
    while( std::getline( ss, item, delim ) )
    {
        elems.push_back( item );
    }
    return elems;
}


// ***********************
//
inline std::vector< std::string >       Split      ( const std::string & s, char delim )
{
    std::vector< std::string > elems;
    Split( s, delim, elems );
    return elems;
}

} // anonymous



// *************************************
// String2T-s
// *************************************


// ========================================================================= //
// Floating points String2T
// ========================================================================= //


// *************************************
//
template<> Expected< Float32 >      String2T        ( const std::string & s ) 
{ 
    char * end = nullptr;
    auto ret = strtof( s.c_str(), &end );

    if( !*end && end != s.c_str() )
    {
        return ( Float32 )ret;
    }

    return Expected< Float32 >();
}

// *************************************
//
template<> Expected< Float64 >      String2T        ( const std::string & s ) 
{ 
    char * end = nullptr;
    auto ret = strtod( s.c_str(), &end );

    if( !*end && end != s.c_str() )
    {
        return ret;
    }

    return Expected< Float64 >();
}

// ========================================================================= //
// Bool String2T
// ========================================================================= //



// *************************************
//
template<> Expected< bool >         String2T        ( const std::string & s ) 
{ 
    if( s == "true" ) 
        return true; 
    else if( s == "false" ) 
        return false; 
    return Expected< bool >(); 
}


// ========================================================================= //
// Integer String2T
// ========================================================================= //

// ***********************
//
template< typename IntType, typename std::enable_if< std::is_signed< IntType >::value, bool >::type = true >
Int64                               CharToInt64     ( const char * str, char ** end )
{
    return strtoll( str, end, 10 );
}

// ***********************
//
template< typename IntType, typename std::enable_if< std::is_unsigned< IntType >::value, bool >::type = true >
UInt64                              CharToInt64     ( const char * str, char ** end )
{
    return strtoull( str, end, 10 );
}

// ***********************
//
template< typename IntType, typename std::enable_if< std::is_signed< IntType >::value, bool >::type = true >
bool                                 IsOverflow      ( Int64 value, const char * )
{
    auto max = std::numeric_limits< Int64 >::max();
    auto min = std::numeric_limits< Int64 >::min();
    return ( max == value ) || ( min == value );
}

// ***********************
//
template< typename IntType, typename std::enable_if< std::is_unsigned< IntType >::value, bool >::type = true >
bool                                 IsOverflow      ( UInt64 value, const char * str )
{
    // Max value tells us about overflow.
    if( std::numeric_limits< UInt64 >::max() == value )
        return true;

    // Underflows aren't reported !! :(
    // We must find -sign. Ommit spaces and check first character if it isn't minus.
    SizeType strIter = 0;
    while( str[ strIter ] != '\0' )
    {
        if( !isspace( str[ strIter ] ) )
            break;

        strIter++;
    }

    return str[ strIter ] == '-';
}

// ***********************
//
template< typename IntType>
Expected< IntType >                 String2IntType  ( const std::string & s )
{
    char * end = nullptr;
    const char * str = s.c_str();
    auto ret = CharToInt64< IntType >( str, &end );

    if( !*end && end != str )
    {
        // This means over- or underflow.
        // Note that in case of Int64 and UInt64 we can't represent max value.
        // I wanted to implement this with errno, but in spite of what is written in docs,
        // conversion function don't set errno :(
        if( IsOverflow< IntType >( ret, str ) )
            return Expected< IntType >();

        if( ret < std::numeric_limits< IntType >::lowest() )
            return Expected< IntType >();

        if( ret > std::numeric_limits< IntType >::max() )
            return Expected< IntType >();

        return static_cast< IntType >( ret );
    }

    return Expected< IntType >();
}

// *************************************
//
template<> Expected< UInt64 >       String2T        ( const std::string & s )
{
    return String2IntType< UInt64 >( s );
}

// *************************************
//
template<> Expected< Int64 >        String2T        ( const std::string & s )
{
    return String2IntType< Int64 >( s );
}

// *************************************
//
template<> Expected< UInt32 >       String2T        ( const std::string & s )
{
    return String2IntType< UInt32 >( s );
}

// *************************************
//
template<> Expected< Int32 >        String2T        ( const std::string & s )
{
    return String2IntType< Int32 >( s );
}

// *************************************
//
template<> Expected< UInt16 >       String2T        ( const std::string & s )
{
    return String2IntType< UInt16 >( s );
}

// *************************************
//
template<> Expected< Int16 >        String2T        ( const std::string & s )
{
    return String2IntType< Int16 >( s );
}

// *************************************
//
template<> Expected< UInt8 >       String2T        ( const std::string & s )
{
    return String2IntType< UInt8 >( s );
}

// *************************************
//
template<> Expected< Int8 >        String2T        ( const std::string & s )
{
    return String2IntType< Int8 >( s );
}

// ========================================================================= //
// Complex types
// ========================================================================= //


// *************************************
//
template< typename T > 
Expected< std::vector< T > >        Strings2T       ( std::vector< std::string > strings )
{
    std::vector< T > ret;

    for( auto s : strings )
    {
        auto t = String2T< T >( s );
        if( t.IsValid() )
            ret.push_back( t );
        else
            return Expected< std::vector< T > >();
    }

    return ret;
}

// ***********************
//
template<> Expected< glm::vec2 >    String2T        ( const std::string & s ) 
{ 
    auto strings = Convert::impl::Split( s, ',' );

    if( strings.size() == 2 )
    {
        auto vals = Strings2T< Float32 >( strings );
        if( vals.IsValid() )
        {
            return glm::vec2( vals.GetVal()[ 0 ], vals.GetVal()[ 1 ] );
        }
        else
            return Expected< glm::vec2 >();
    }
    else
        return Expected< glm::vec2 >();
}

// *************************************
//
template<> Expected< glm::vec3 >    String2T        ( const std::string & s )
{ 
    auto strings = Convert::impl::Split( s, ',' );

    if( strings.size() == 3 )
    {
        auto vals = Strings2T< Float32 >( strings );
        if( vals.IsValid() )
            return glm::vec3( vals.GetVal()[ 0 ], vals.GetVal()[ 1 ], vals.GetVal()[ 2 ] );
        else
            return Expected< glm::vec3 >();
    }
    else
        return Expected< glm::vec3 >();
}


// *************************************
//
template<> Expected< glm::vec4 >    String2T        ( const std::string & s )
{ 
    auto strings = Convert::impl::Split( s, ',' );

    if( strings.size() == 4 )
    {
        auto vals = Strings2T< Float32 >( strings );
        if( vals.IsValid() )
            return glm::vec4( vals.GetVal()[ 0 ], vals.GetVal()[ 1 ], vals.GetVal()[ 2 ], vals.GetVal()[ 3 ] );
        else
            return Expected< glm::vec4 >();
    }
    else
        return Expected< glm::vec4 >();
}


// *************************************
//
template<> Expected< std::string >  String2T        ( const std::string & s ) 
{ 
    return s; 
}

// *************************************
//
template<> Expected< std::wstring > String2T        ( const std::string & s ) 
{ 
    return StringToWString( s ); 
}

// *************************************
//
template< typename T > 
T       String2T        ( const std::string & s, const T & defaultVal ) 
{ 
    auto ret = String2T< T >( s );
    if( ret.IsValid() )
        return ret;
    else
        return defaultVal;
}

template bool           String2T        ( const std::string & s, const bool & defaultVal );
template Float32        String2T        ( const std::string & s, const Float32 & defaultVal );
template Float64        String2T        ( const std::string & s, const Float64 & defaultVal );
template UInt64         String2T        ( const std::string & s, const UInt64 & defaultVal );
template Int64          String2T        ( const std::string & s, const Int64 & defaultVal );
template UInt32         String2T        ( const std::string & s, const UInt32 & defaultVal );
template Int32          String2T        ( const std::string & s, const Int32 & defaultVal );
template UInt16         String2T        ( const std::string & s, const UInt16 & defaultVal );
template Int16          String2T        ( const std::string & s, const Int16 & defaultVal );
template UInt8          String2T        ( const std::string & s, const UInt8 & defaultVal );
template Int8           String2T        ( const std::string & s, const Int8 & defaultVal );
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
    return WStringToString( wstr ).GetVal();
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
template std::string T2String( const Int16& v );
template std::string T2String( const UInt16& v );

}   // Convert
}   // bv