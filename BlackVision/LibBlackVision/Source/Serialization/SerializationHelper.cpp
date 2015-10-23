#include "SerializationHelper.h"


#include <vector>
#include <sstream>

namespace bv {

//void ISerializable::Deserialize( const IDeserializer& ) { assert( false ); } //FIXME taki ch*j

namespace SerializationHelper {

inline std::vector<std::string> &_split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

inline std::vector<std::string> _split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    _split(s, delim, elems);
    return elems;
}

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

} }
