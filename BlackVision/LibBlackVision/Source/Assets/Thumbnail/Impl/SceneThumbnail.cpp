#include "stdafx.h"

#include "SceneThumbnail.h"

#include "tools/Base64.h"

namespace bv
{

// ******************************
//
SceneThumbnail::SceneThumbnail    ( const MemoryChunkConstPtr & data, const Hash & h )
    : m_data( data )
    , m_dataBase64( "" )
    , m_origDataHash( h )
{}

// ******************************
//
SceneThumbnail::SceneThumbnail   ( const std::string & dataBase64, const Hash & h )
    : m_data( nullptr )
    , m_dataBase64( dataBase64 )
    , m_origDataHash( h )
{}

// ******************************
//
SceneThumbnail::~SceneThumbnail   ()
{}

// ******************************
//
SceneThumbnail::SceneThumbnail    ( IDeserializer & deser )
    : m_origDataHash( "" )
{
    deser.EnterChild( "thumbnail" );
    
    assert( deser.GetAttribute( "type" ) == "SCENE_THUMBNAIL" );

    m_origDataHash = Hash( deser.GetAttribute( "hash" ) );

    m_data = DecodeBase64( deser.GetAttribute( "data" ) );

    auto dataEnc = deser.GetAttribute( "data" );

    deser.ExitChild(); // thumbnail
}

// ******************************
//
SceneThumbnailConstPtr       SceneThumbnail::Create      ( const MemoryChunkConstPtr & data, const Hash & h )
{
    return SceneThumbnailConstPtr( new SceneThumbnail( data, h ) );
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
const Hash &                        SceneThumbnail::GetHash     () const
{
    return m_origDataHash;
}

// ******************************
//
void                                SceneThumbnail::Serialize   ( ISerializer & ser ) const
{
    ser.EnterChild( "thumbnail" );
    ser.SetAttribute( "type", "SCENE_THUMBNAIL" );
    ser.SetAttribute( "hash", m_origDataHash.Get() );

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