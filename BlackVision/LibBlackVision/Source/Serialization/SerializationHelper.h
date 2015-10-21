#pragma once

#include "ISerializable.h"
#include "SerializationObjects.h"

#include "Mathematics/glm_inc.h"

namespace bv {

namespace SerializationHelper {

template< typename T >
void SerializeObjectImpl( const T& o, ISerializer& sob );

template< typename T >
std::shared_ptr< T > Create( IDeserializer& sob )
{
    auto obj = T::Create( sob );
    return std::static_pointer_cast< T >( obj );
}

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

glm::vec4               String2Vec4( std::string s );
glm::vec3               String2Vec3( std::string s );
glm::vec2               String2Vec2( std::string s );

}

}