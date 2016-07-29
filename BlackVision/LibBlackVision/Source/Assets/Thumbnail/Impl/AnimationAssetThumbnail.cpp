#include "stdafx.h"

#include "AnimationAssetThumbnail.h"

#include "Tools/Base64.h"

namespace {

static std::string EMPTY_STRING = "";

}



#include "Memory/MemoryLeaks.h"



namespace bv
{
// ******************************
//
AnimationAssetThumbnail::AnimationAssetThumbnail    ( const MemoryChunkVector & data, const Hash & h )
    : m_data( data )
    , m_origDataHash( h )
{
}

// ******************************
//
AnimationAssetThumbnail::~AnimationAssetThumbnail   ()
{
}

// ******************************
//
AnimationAssetThumbnail::AnimationAssetThumbnail    ( IDeserializer & deser )
    : m_origDataHash( "" )
{
    deser.EnterChild( "thumbnail" );
    
    assert( deser.GetAttribute( "type" ) == "ANIMATION_ASSET_THUMBNAIL" );

    m_origDataHash = Hash( deser.GetAttribute( "hash" ) );

    deser.EnterChild( "frames" );
    
    if( deser.EnterChild( "data" ) )
    {
        do
        {
            m_data.push_back( DecodeBase64( deser.GetAttribute( "data" ) ) );
        }
        while( deser.NextChild() );

        deser.ExitChild();
    }

    deser.ExitChild(); // "frames"

    deser.ExitChild(); // thumbnail
}

// ******************************
//
AnimationAssetThumbnailConstPtr       AnimationAssetThumbnail::Create      ( const MemoryChunkVector & data, const Hash & h )
{
    return AnimationAssetThumbnailConstPtr( new AnimationAssetThumbnail( data, h ) );
}

// ******************************
//
MemoryChunkConstPtr                 AnimationAssetThumbnail::Data        () const
{
    return nullptr;
}

// ******************************
//
const std::string &                 AnimationAssetThumbnail::DataBase64  () const
{
    return EMPTY_STRING;
}

// ******************************
//
const Hash &                        AnimationAssetThumbnail::GetHash     () const
{
    return m_origDataHash;
}

// ******************************
//
void                                AnimationAssetThumbnail::Serialize   ( ISerializer & ser ) const
{
    ser.EnterChild( "thumbnail" );
    ser.SetAttribute( "type", "ANIMATION_ASSET_THUMBNAIL" );
    ser.SetAttribute( "hash", m_origDataHash.Get() );

    ser.EnterArray( "frames" );

    for( auto d : m_data )
    {
        ser.EnterChild( "data" );
        ser.SetAttribute( "data", EncodeBase64( d ) );
        ser.ExitChild();
    }

    ser.ExitChild(); // frames

    ser.ExitChild(); // thumbnail
}

// ******************************
//
AnimationAssetThumbnailConstPtr    AnimationAssetThumbnail::Create          ( IDeserializer & deser )
{
    return AnimationAssetThumbnailConstPtr( new AnimationAssetThumbnail( deser ) );
}

} // bv