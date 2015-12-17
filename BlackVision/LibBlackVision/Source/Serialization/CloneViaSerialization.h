#include "XML/XMLSerializer.h"
#include "XML/XMLDeserializer.h"

#include "SerializationHelper.h"
#include "BVDeserializeContext.h"

namespace bv {

namespace CloneViaSerialization {

template< typename T >
std::shared_ptr< T >            Clone( const T* obj, std::string name )
{
    XMLSerializer ser;
    
    obj->Serialize( ser );

    std::stringstream ss;
    ser.Save( ss );

    //ser.Save( "clone.xml" ); // for debugging only

    XMLDeserializer deser( ss, ss.tellp() );

    return SerializationHelper::DeserializeObjectLoadImpl< T >( deser, name );
}

} }
