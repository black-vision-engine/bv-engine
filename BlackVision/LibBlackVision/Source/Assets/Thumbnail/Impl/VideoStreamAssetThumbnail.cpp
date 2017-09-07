#include "stdafx.h"
#include "VideoStreamAssetThumbnail.h"

#include "tools/Base64.h"



#include "Memory/MemoryLeaks.h"



namespace bv
{

// ******************************
//
VideoStreamAssetThumbnail::VideoStreamAssetThumbnail    ( const MemoryChunkConstPtr & data, const Hash & h )
    : m_data( data )
    , m_dataBase64( "" )
    , m_origDataHash( h )
{}

// ******************************
//
VideoStreamAssetThumbnail::VideoStreamAssetThumbnail    ( const std::string & dataBase64, const Hash & h )
    : m_data( nullptr )
    , m_dataBase64( dataBase64 )
    , m_origDataHash( h )
{}


// ***********************
//
VideoStreamAssetThumbnail::~VideoStreamAssetThumbnail()
{}

// ******************************
//
VideoStreamAssetThumbnail::VideoStreamAssetThumbnail    ( IDeserializer & deser )
    : m_origDataHash( "" )
{
    deser.EnterChild( "thumbnail" );
    
    assert( deser.GetAttribute( "type" ) == "VIDEO_STREAM_ASSET_THUMBNAIL" );

    m_origDataHash = Hash( deser.GetAttribute( "hash" ) );
    m_dataBase64 = deser.GetAttribute( "data" );

    deser.ExitChild(); // thumbnail
}

// ******************************
//
VideoStreamAssetThumbnailConstPtr       VideoStreamAssetThumbnail::Create      ( const MemoryChunkConstPtr & data, const Hash & h )
{
    return VideoStreamAssetThumbnailConstPtr( new VideoStreamAssetThumbnail( data, h ) );
}

// ******************************
//
MemoryChunkConstPtr                  VideoStreamAssetThumbnail::Data        () const
{
    if( !m_data )
    {
        m_data = DecodeBase64( m_dataBase64 );
    }
    return m_data;
}

// ******************************
//
const std::string &                  VideoStreamAssetThumbnail::DataBase64  () const
{
    if( m_dataBase64.empty() )
    {
        m_dataBase64 = m_data ? EncodeBase64( m_data ) : "";
    }

    return m_dataBase64;
}

// ******************************
//
const Hash &                        VideoStreamAssetThumbnail::GetHash     () const
{
    return m_origDataHash;
}

// ******************************
//
void                                VideoStreamAssetThumbnail::Serialize   ( ISerializer & ser ) const
{
    ser.EnterChild( "thumbnail" );
    ser.SetAttribute( "type", "VIDEO_STREAM_ASSET_THUMBNAIL" );
    ser.SetAttribute( "hash", m_origDataHash.Get() );

    ser.SetAttribute( "data", DataBase64() );

    ser.ExitChild(); // thumbnail
}

// ******************************
//
VideoStreamAssetThumbnailConstPtr    VideoStreamAssetThumbnail::Create          ( IDeserializer & deser )
{
    return VideoStreamAssetThumbnailConstPtr( new VideoStreamAssetThumbnail( deser ) );
}


}   // bv
