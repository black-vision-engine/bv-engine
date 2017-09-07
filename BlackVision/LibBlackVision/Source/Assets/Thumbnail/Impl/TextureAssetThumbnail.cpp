#include "stdafx.h"

#include "TextureAssetThumbnail.h"

#include "tools/Base64.h"



#include "Memory/MemoryLeaks.h"



namespace bv
{

// ******************************
//
TextureAssetThumbnail::TextureAssetThumbnail    ( const MemoryChunkConstPtr & data, const Hash & h )
    : m_data( data )
    , m_dataBase64( "" )
    , m_origDataHash( h )
{}

// ******************************
//
TextureAssetThumbnail::TextureAssetThumbnail   ( const std::string & dataBase64, const Hash & h )
    : m_data( nullptr )
    , m_dataBase64( dataBase64 )
    , m_origDataHash( h )
{}

// ******************************
//
TextureAssetThumbnail::~TextureAssetThumbnail   ()
{}

// ******************************
//
TextureAssetThumbnail::TextureAssetThumbnail    ( IDeserializer & deser )
    : m_origDataHash( "" )
{
    deser.EnterChild( "thumbnail" );
    
    assert( deser.GetAttribute( "type" ) == "TEXTURE_ASSET_THUMBNAIL" );

    m_origDataHash = Hash( deser.GetAttribute( "hash" ) );
    m_dataBase64 = deser.GetAttribute( "data" );

    deser.ExitChild(); // thumbnail
}

// ******************************
//
TextureAssetThumbnailConstPtr       TextureAssetThumbnail::Create      ( const MemoryChunkConstPtr & data, const Hash & h )
{
    return TextureAssetThumbnailConstPtr( new TextureAssetThumbnail( data, h ) );
}

// ******************************
//
MemoryChunkConstPtr                  TextureAssetThumbnail::Data        () const
{
    if( !m_data )
    {
        m_data = DecodeBase64( m_dataBase64 );
    }
    return m_data;
}

// ******************************
//
const std::string &                  TextureAssetThumbnail::DataBase64  () const
{
    if( m_dataBase64.empty() )
    {
        m_dataBase64 = m_data ? EncodeBase64( m_data ) : "";
    }

    return m_dataBase64;
}

// ******************************
//
const Hash &                        TextureAssetThumbnail::GetHash     () const
{
    return m_origDataHash;
}

// ******************************
//
void                                TextureAssetThumbnail::Serialize   ( ISerializer & ser ) const
{
    ser.EnterChild( "thumbnail" );
    ser.SetAttribute( "type", "TEXTURE_ASSET_THUMBNAIL" );
    ser.SetAttribute( "hash", m_origDataHash.Get() );

    ser.SetAttribute( "data", DataBase64() );

    ser.ExitChild(); // thumbnail
}

// ******************************
//
TextureAssetThumbnailConstPtr    TextureAssetThumbnail::Create          ( IDeserializer & deser )
{
    return TextureAssetThumbnailConstPtr( new TextureAssetThumbnail( deser ) );
}

} // bv