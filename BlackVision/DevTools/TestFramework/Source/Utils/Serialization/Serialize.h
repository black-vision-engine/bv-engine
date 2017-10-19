#pragma once


#include "Serialization/IDeserializer.h"
#include "Serialization/ISerializer.h"

#include "Serialization/XML/XMLSerializer.h"
#include "Serialization/XML/XMLDeserializer.h"
#include "Serialization/BV/XML/BVXMLSerializer.h"
#include "Serialization/BV/XML/BVXMLDeserializer.h"

#include "System/Path.h"

namespace bv
{

// ***********************
//
template< typename Type >
void                        Serialize       ( const Type & toSerialize, const std::string & path )
{
    BVXMLSerializer ser;
    toSerialize.Serialize( ser );

    ser.Save( path );
}

// ***********************
//
template< typename Type >
std::shared_ptr< Type >     Deserialize     ( const std::string & path, const std::string & enterName )
{
    BVXMLDeserializer deser( path, nullptr, nullptr );

    deser.EnterChild( enterName );
    auto object = Type::Create( deser );
    deser.ExitChild();

    return object;
}

// ***********************
//
inline
std::shared_ptr< CompositeInterpolator< TimeType, std::string > >
                            DeserializeFloatCompositeInterpolator       ( const std::string & path )
{
    return Deserialize< CompositeInterpolator< TimeType, std::string > >( path, "interpolator" );
}

}	// bv
