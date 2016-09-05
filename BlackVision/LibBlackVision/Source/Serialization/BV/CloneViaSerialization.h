#include "XML/BVXMLSerializer.h"
#include "XML/BVXMLDeserializer.h"

#include "Serialization/SerializationHelper.h"
#include "BVDeserializeContext.h"

#include <sstream>

namespace bv {

namespace CloneViaSerialization {

template< typename T >
std::shared_ptr< T >                              Clone( const T* obj, std::string name, AssetDescsWithUIDsPtr assets, model::OffsetTimeEvaluatorPtr sceneTimeline )
{
    BVXMLSerializer ser( assets );
    
    obj->Serialize( ser );

    std::stringstream ss;
    ser.Save( ss );

    //ser.Save( "clone.xml" ); // for debugging only

    BVXMLDeserializer deser( ss, ss.tellp(), sceneTimeline, assets );

    return SerializationHelper::DeserializeObject< T >( deser, name );
}

template< typename T >
std::shared_ptr< T >                              ClonePtr( const T* obj, std::string name, AssetDescsWithUIDsPtr assets, model::OffsetTimeEvaluatorPtr sceneTimeline )
{
    BVXMLSerializer ser;
    
    obj->Serialize( ser );

    std::stringstream ss;
    ser.Save( ss );

    //ser.Save( "clone.xml" ); // for debugging only

    BVXMLDeserializer deser( ss, ss.tellp(), sceneTimeline, assets );
    
    auto sucess = deser.EnterChild( name );
    assert( sucess ); { sucess; } // FIXME error handling
    auto clone = T::Create( deser );
    deser.ExitChild();
    return std::static_pointer_cast< T >( clone );
}

} }
