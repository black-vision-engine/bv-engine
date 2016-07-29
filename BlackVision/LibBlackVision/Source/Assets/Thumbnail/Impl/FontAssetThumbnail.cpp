#include "stdafx.h"

#include "FontAssetThumbnail.h"

#include "Tools/Base64.h"



#include "Memory/MemoryLeaks.h"



namespace bv
{
// ******************************
//
FontAssetThumbnail::FontAssetThumbnail    ( const MemoryChunkConstPtr & data, const Hash & h )
    : m_data( data )
    , m_origDataHash( h )
    , m_dataBase64( "" )
{
}

// ******************************
//
FontAssetThumbnail::FontAssetThumbnail   ( const std::string & data, const Hash & h )
    : m_data( nullptr )
    , m_origDataHash( h )
    , m_dataBase64( data )
{

}

// ******************************
//
FontAssetThumbnail::~FontAssetThumbnail   ()
{
}

// ******************************
//
FontAssetThumbnail::FontAssetThumbnail    ( IDeserializer & deser )
    : m_origDataHash( "" )
    , m_data( nullptr )
{
    deser.EnterChild( "thumbnail" );
    
    assert( deser.GetAttribute( "type" ) == "FONT_ASSET_THUMBNAIL" );

    m_origDataHash = Hash( deser.GetAttribute( "hash" ) );
    
    m_dataBase64 = deser.GetAttribute( "data" );

    deser.ExitChild(); // thumbnail
}

// ******************************
//
FontAssetThumbnailConstPtr       FontAssetThumbnail::Create      ( const MemoryChunkConstPtr & data, const Hash & h )
{
    return FontAssetThumbnailConstPtr( new FontAssetThumbnail( data, h ) );
}

// ******************************
//
MemoryChunkConstPtr                 FontAssetThumbnail::Data        () const
{
    if( !m_data )
    {
        m_data = DecodeBase64( m_dataBase64 );
    }
    return m_data;
}

// ******************************
//
const std::string &                 FontAssetThumbnail::DataBase64  () const
{
    if( m_dataBase64.empty() )
    {
        m_dataBase64 = m_data ? EncodeBase64( m_data ) : "";
    }

    return m_dataBase64;
}
// ******************************
//
const Hash &                        FontAssetThumbnail::GetHash     () const
{
    return m_origDataHash;
}

// ******************************
//
void                                FontAssetThumbnail::Serialize   ( ISerializer & ser ) const
{
    ser.EnterChild( "thumbnail" );
    ser.SetAttribute( "type", "FONT_ASSET_THUMBNAIL" );
    ser.SetAttribute( "hash", m_origDataHash.Get() );

    ser.SetAttribute( "data", DataBase64() );

    ser.ExitChild(); // thumbnail
}

// ******************************
//
FontAssetThumbnailConstPtr    FontAssetThumbnail::Create          ( IDeserializer & deser )
{
    return FontAssetThumbnailConstPtr( new FontAssetThumbnail( deser ) );
}

} // bv