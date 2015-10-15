#pragma once

#include "ISerializable.h"
#include "SerializationObjects.h"

#include <vector>
#include <sstream>

namespace bv {

namespace SerializationHelper {

template< typename T >
void SerializeObjectImpl( const T& o, ISerializer& sob );

//template< typename T >
//T* DeserializeObjectImpl( ISerializer& dob );
//
//template< typename T >
//class SerializedObject : public ISerializable
//{
//    T o;
//
//public:
//    SerializedObject( const T& o_ ) : o( o_ ) { }
//
//    virtual void                Serialize       ( ISerializer& sob ) const { SerializeObjectImpl< T >( o, sob ); }
//
//    static ISerializablePtr     Create          ( ISerializer& dob ) { return ISerializablePtr( DeserializeObjectImpl< T >( dob ); }
//};

// glm stuff

inline std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

inline std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

glm::vec4               String2Vec4( std::string s )
{
    auto vals = SerializationHelper::split( s, ',' );
    assert( vals.size() == 4 );
    return glm::vec4( std::stof( vals[0] ), 
        std::stof( vals[1] ), 
        std::stof( vals[2] ), 
        std::stof( vals[3] ) );
}

glm::vec3               String2Vec3( std::string s )
{
    auto vals = SerializationHelper::split( s, ',' );
    assert( vals.size() == 3 );
    return glm::vec3( std::stof( vals[0] ), 
        std::stof( vals[1] ), 
        std::stof( vals[2] ) );
}

glm::vec2               String2Vec2( std::string s )
{
    auto vals = SerializationHelper::split( s, ',' );
    assert( vals.size() == 2 );
    return glm::vec2( std::stof( vals[0] ), 
        std::stof( vals[1] ) );
}

}

}