#include "XML/XMLSerializer.h"
#include "XML/XMLDeserializer.h"

#include "SerializationHelper.h"

namespace bv {

namespace CloneViaSerialization {

template< typename T >
std::shared_ptr< T >            Clone( T* obj, std::string name )
{
    XMLSerializer ser;
    
    obj->Serialize( ser );

    std::stringstream ss;
    ser.Save( ss );

    XMLDeserializer deser( ss, ss.tellp() );

    return SerializationHelper::DeserializeObjectLoadImpl< T >( deser, name );
}

} }
