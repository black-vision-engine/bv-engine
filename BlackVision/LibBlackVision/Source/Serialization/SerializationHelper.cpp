#include "SerializationHelper.h"


#include <vector>
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

glm::vec4               String2Vec4( std::string s )
{
    auto vals = SerializationHelper::_split( s, ',' );
    assert( vals.size() == 4 );
    return glm::vec4( std::stof( vals[0] ), 
        std::stof( vals[1] ), 
        std::stof( vals[2] ), 
        std::stof( vals[3] ) );
}

glm::vec3               String2Vec3( std::string s )
{
    auto vals = SerializationHelper::_split( s, ',' );
    assert( vals.size() == 3 );
    return glm::vec3( std::stof( vals[0] ), 
        std::stof( vals[1] ), 
        std::stof( vals[2] ) );
}

glm::vec2               String2Vec2( std::string s )
{
    auto vals = SerializationHelper::_split( s, ',' );
    assert( vals.size() == 2 );
    return glm::vec2( std::stof( vals[0] ), 
        std::stof( vals[1] ) );
}

template<> float String2T( std::string s ) { if( s == "" ) return 0.f; else return std::stof( s ); }
template<> bool String2T( std::string s ) { if( s == "true" ) return true; else if( s == "false" ) return false; assert( false ); return false; } // FIXME error handling
template<> int String2T( std::string s ) { return std::stoi( s ); }
template<> glm::vec2 String2T( std::string s ) { return String2Vec2( s ); }
template<> glm::vec3 String2T( std::string s ) { return String2Vec3( s ); }
template<> glm::vec4 String2T( std::string s ) { return String2Vec4( s ); }

} }

namespace std
{
    string to_string( const glm::vec2 & v ) { return to_string( v[0] ) + ", " + to_string( v[1] ); }
    string to_string( const glm::vec3 & v ) { return to_string( v[0] ) + ", " + to_string( v[1] ) + ", " + to_string( v[2] ); }
    string to_string( const glm::vec4 & v ) { return to_string( v[0] ) + ", " + to_string( v[1] ) + ", " + to_string( v[2] ) + ", " + to_string( v[3] ); }
    string to_string( const std::string & val ) { return val; }
    string to_string( const std::wstring & val ) { return std::string( val.begin(), val.end() ); }
}
