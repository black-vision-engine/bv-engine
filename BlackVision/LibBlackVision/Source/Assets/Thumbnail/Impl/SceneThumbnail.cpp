#include "stdafx.h"

#include "SceneThumbnail.h"

#include "tools/Base64.h"



#include "Memory/MemoryLeaks.h"



namespace bv
{

// ******************************
//
SceneThumbnail::SceneThumbnail    ( const MemoryChunkConstPtr & data )
    : m_data( data )
    , m_dataBase64( "" )
{}

// ******************************
//
SceneThumbnail::SceneThumbnail   ( const std::string & dataBase64 )
    : m_data( nullptr )
    , m_dataBase64( dataBase64 )
{}

// ******************************
//
SceneThumbnail::~SceneThumbnail   ()
{}

// ******************************
//
SceneThumbnail::SceneThumbnail    ( IDeserializer & deser )
{
    deser.EnterChild( "thumbnail" );
    
    assert( deser.GetAttribute( "type" ) == "SCENE_THUMBNAIL" );

    m_dataBase64 = deser.GetAttribute( "data" );

    deser.ExitChild(); // thumbnail
}

// ******************************
//
SceneThumbnailConstPtr       SceneThumbnail::Create      ( const MemoryChunkConstPtr & data )
{
    return SceneThumbnailConstPtr( new SceneThumbnail( data ) );
}

// ******************************
//
MemoryChunkConstPtr                  SceneThumbnail::Data        () const
{
    if( !m_data )
    {
        m_data = DecodeBase64( m_dataBase64 );
    }
    return m_data;
}

// ******************************
//
const std::string &                  SceneThumbnail::DataBase64  () const
{
    if( m_dataBase64.empty() )
    {
        m_dataBase64 = m_data ? EncodeBase64( m_data ) : "";
    }

    return m_dataBase64;
}


// ******************************
//
void                                SceneThumbnail::Serialize   ( ISerializer & ser ) const
{
    ser.EnterChild( "thumbnail" );
    ser.SetAttribute( "type", "SCENE_THUMBNAIL" );

    ser.SetAttribute( "data", DataBase64() );

    ser.ExitChild(); // thumbnail
}

// ******************************
//
SceneThumbnailConstPtr    SceneThumbnail::Create          ( IDeserializer & deser )
{
    return SceneThumbnailConstPtr( new SceneThumbnail( deser ) );
}
}	// bv