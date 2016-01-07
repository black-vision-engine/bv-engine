#include "XML/XMLSerializer.h"
#include "XML/XMLDeserializer.h"

#include "SerializationHelper.h"
#include "BVDeserializeContext.h"

#include <sstream>

namespace bv {

namespace CloneViaSerialization {

template< typename T >
T*                              Clone( const T* obj, std::string name, model::OffsetTimeEvaluatorPtr sceneTimeline = nullptr )
{
    XMLSerializer ser;
    
    obj->Serialize( ser );

    std::stringstream ss;
    ser.Save( ss );

    //ser.Save( "clone.xml" ); // for debugging only

    XMLDeserializer deser( ss, ss.tellp() );

	dynamic_cast< BVDeserializeContext* >( deser.GetDeserializeContext() )->m_sceneTimeline = sceneTimeline;

    return SerializationHelper::DeserializeObject< T >( deser, name );
}

template< typename T >
std::shared_ptr< T >                              ClonePtr( const T* obj, std::string name, model::OffsetTimeEvaluatorPtr sceneTimeline = nullptr )
{
    XMLSerializer ser;
    
    obj->Serialize( ser );

    std::stringstream ss;
    ser.Save( ss );

    //ser.Save( "clone.xml" ); // for debugging only

    XMLDeserializer deser( ss, ss.tellp() );

	dynamic_cast< BVDeserializeContext* >( deser.GetDeserializeContext() )->m_sceneTimeline = sceneTimeline;

    auto sucess = deser.EnterChild( name );
    assert( sucess ); { sucess; } // FIXME error handling
    auto clone = T::Create( deser );
    deser.ExitChild();
    return std::static_pointer_cast< T >( clone );
}

} }
