#pragma once


#include "Serialization/IDeserializer.h"
#include "Serialization/ISerializer.h"

#include "Serialization/XML/XMLSerializer.h"
#include "Serialization/XML/XMLDeserializer.h"
#include "Serialization/BV/XML/BVXMLSerializer.h"
#include "Serialization/BV/XML/BVXMLDeserializer.h"
#include "Serialization/BV/BVSerializeContext.h"
#include "Serialization/BV/BVDeserializeContext.h"

#include "gtest/gtest.h"

#include "System/Path.h"

namespace bv
{

// ***********************
//
template< typename Type >
void                        Serialize       ( const Type & toSerialize, const std::string & path, BVSerializeContext * context )
{
    BVXMLSerializer ser( context );
    toSerialize.Serialize( ser );

    ser.Save( path );
}

// ***********************
//
template< typename Type >
void                        Serialize       ( const Type & toSerialize, const std::string & path ) 
{
    auto context = BVSerializeContext::CreateContextFromEmptiness();
    Serialize( toSerialize, path, context );
}


// ***********************
//
template< typename Type >
std::shared_ptr< Type >     Deserialize     ( const std::string & path, const std::string & enterName, BVDeserializeContext * context )
{
    BVXMLDeserializer deser( path, context );

    deser.EnterChild( enterName );
    auto object = Type::Create( deser );
    deser.ExitChild();

    return std::static_pointer_cast< Type >( object );
}

// ***********************
//
template< typename Type >
std::shared_ptr< Type >     Deserialize     ( const std::string & path, const std::string & enterName ) 
{
	auto context = BVDeserializeContext::CreateContextFromEmptiness();
	if( Path::Exists( path ) )
		return Deserialize< Type >( path, enterName, context );
	else
	{
		EXPECT_TRUE( false ) << "Deserialized file does not exist: " << path;
		return nullptr;
	}
}

// ***********************
//
template< typename Type >
std::shared_ptr< Type >     DeserializeAsset     ( const std::string & path, const std::string & enterName )
{
    BVXMLDeserializer deser( path, BVDeserializeContext::CreateContextFromEmptiness() );

    deser.EnterChild( enterName );

    auto type = deser.GetAttribute( "type" );
    if( type != Type::UID() )
        return nullptr;

    auto object = Type::Create( deser );
    deser.ExitChild();

    return std::static_pointer_cast< Type >( object );
}

}	// bv
