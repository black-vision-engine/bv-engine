#include "stdafx.h"

#include "PresetThumbnail.h"

#include "tools/Base64.h"

namespace bv
{

// ******************************
//
PresetThumbnail::PresetThumbnail    ( const MemoryChunkConstPtr & data, const Hash & h )
    : m_data( data )
    , m_dataBase64( "" )
    , m_origDataHash( h )
{}

// ******************************
//
PresetThumbnail::PresetThumbnail   ( const std::string & dataBase64, const Hash & h )
    : m_data( nullptr )
    , m_dataBase64( dataBase64 )
    , m_origDataHash( h )
{}

// ******************************
//
PresetThumbnail::~PresetThumbnail   ()
{}

// ******************************
//
PresetThumbnail::PresetThumbnail    ( IDeserializer & deser )
    : m_origDataHash( "" )
{
    deser.EnterChild( "thumbnail" );
    
    assert( deser.GetAttribute( "type" ) == "PRESET_THUMBNAIL" );

    m_origDataHash = Hash( deser.GetAttribute( "hash" ) );

    m_data = DecodeBase64( deser.GetAttribute( "data" ) );

    auto dataEnc = deser.GetAttribute( "data" );

    deser.ExitChild(); // thumbnail
}

// ******************************
//
PresetThumbnailConstPtr       PresetThumbnail::Create      ( const MemoryChunkConstPtr & data, const Hash & h )
{
    return PresetThumbnailConstPtr( new PresetThumbnail( data, h ) );
}

// ******************************
//
MemoryChunkConstPtr                  PresetThumbnail::Data        () const
{
    if( !m_data )
    {
        m_data = DecodeBase64( m_dataBase64 );
    }
    return m_data;
}

// ******************************
//
const std::string &                  PresetThumbnail::DataBase64  () const
{
    if( m_dataBase64.empty() )
    {
        m_dataBase64 = m_data ? EncodeBase64( m_data ) : "";
    }

    return m_dataBase64;
}

// ******************************
//
const Hash &                        PresetThumbnail::GetHash     () const
{
    return m_origDataHash;
}

// ******************************
//
void                                PresetThumbnail::Serialize   ( ISerializer & ser ) const
{
    ser.EnterChild( "thumbnail" );
    ser.SetAttribute( "type", "PRESET_THUMBNAIL" );
    ser.SetAttribute( "hash", m_origDataHash.Get() );

    ser.SetAttribute( "data", DataBase64() );

    ser.ExitChild(); // thumbnail
}

// ******************************
//
PresetThumbnailConstPtr    PresetThumbnail::Create          ( IDeserializer & deser )
{
    return PresetThumbnailConstPtr( new PresetThumbnail( deser ) );
}
}	// bv