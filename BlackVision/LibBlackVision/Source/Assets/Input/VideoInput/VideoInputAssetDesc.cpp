#include "stdafx.h"

#include "VideoInputAssetDesc.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"
#include "Serialization/BV/BVSerializeContext.h"



#include "Memory/MemoryLeaks.h"



namespace bv
{

const std::string   VideoInputAssetDesc::uid = "VIDEO_INPUT_ASSET_DESC";



// ***********************
//
VideoInputAssetDesc::VideoInputAssetDesc( videocards::VideoInputID fillIdx, videocards::VideoInputID keyIdx )
    : m_videoFillIdx( fillIdx )
    , m_videoKeyIdx( keyIdx )
{}


// ***********************
//
VideoInputAssetDescPtr	        VideoInputAssetDesc::Create             ( videocards::VideoInputID fillIdx, videocards::VideoInputID keyIdx )
{
    return VideoInputAssetDescPtr( new VideoInputAssetDesc( fillIdx, keyIdx ) );
}

// ***********************
//
VideoInputTextureAssetDescPtr   VideoInputAssetDesc::CreateTextureDesc  ( videocards::VideoType type ) const
{
    if( type == videocards::VideoType::Fill )
        return VideoInputTextureAssetDesc::Create( m_videoFillIdx, type );
    else
        return VideoInputTextureAssetDesc::Create( m_videoKeyIdx, type );
}

// ***********************
//
VideoInputAudioAssetDescPtr     VideoInputAssetDesc::CreateAudioDesc    () const
{
    return VideoInputAudioAssetDesc::Create( m_videoFillIdx );
}

// ***********************
//
void                VideoInputAssetDesc::Serialize        ( ISerializer & ser ) const
{
    ser.EnterChild( "asset" );

    ser.SetAttribute( "type", UID() );
    ser.SetAttribute( "VideoFillIdx", Convert::T2String( m_videoFillIdx ) );
    ser.SetAttribute( "VideoKeyIdx", Convert::T2String( m_videoKeyIdx ) );

    ser.ExitChild();
}

// ***********************
//
void                VideoInputAssetDesc::Deserialize      ( const IDeserializer & sob )
{
    auto obj = SerializationHelper::Create< const VideoInputAssetDesc >( sob );

    if( obj == nullptr || obj->GetUID() != uid )
    {
        assert( !"Failed" ); //FIXME: Error handling.
        return;
    }

    *this = *obj; // if you think this is ugly, see why it cannot be done:
}


// ***********************
//
ISerializableConstPtr       VideoInputAssetDesc::Create          ( const IDeserializer & deser )
{
    auto videoInputFillIdx = Convert::String2T< videocards::VideoInputID >( deser.GetAttribute( "VideoFillIdx" ) );
    auto videoInputKeyIdx = Convert::String2T< videocards::VideoInputID >( deser.GetAttribute( "VideoKeyIdx" ) );

    if( videoInputFillIdx.IsValid() && videoInputKeyIdx.IsValid() )
        return VideoInputAssetDesc::Create( videoInputFillIdx.GetVal(), videoInputKeyIdx.GetVal() );
    else
    {
        if( !videoInputFillIdx.IsValid() )
            Warn< SerializationException >( deser, "VideoFillIdx is invalid or doesn't exist." );

        if( !videoInputKeyIdx.IsValid() )
            Warn< SerializationException >( deser, "VideoKeyIdx is invalid or doesn't exist." );

        return nullptr;
    }
}


// ***********************
//
const std::string &	        VideoInputAssetDesc::GetUID() const
{
    return VideoInputAssetDesc::UID();
}

// ***********************
//
const std::string &         VideoInputAssetDesc::UID()
{
    return VideoInputAssetDesc::uid;
}


// ***********************
//
bool                        VideoInputAssetDesc::IsCacheable() const
{
    return false;
}

// ***********************
//
VoidConstPtr                VideoInputAssetDesc::QueryThis() const
{
    return shared_from_this();
}



// ***********************
//
std::string				VideoInputAssetDesc::GetKey		() const
{
    if( m_key.empty() )
        m_key = ComputeKey();

    return m_key;
}


// ***********************
//
std::string             VideoInputAssetDesc::ComputeKey   () const
{
    return "VideoInputAsset" + Convert::T2String( m_videoFillIdx ) + Convert::T2String( m_videoKeyIdx );
}

// ***********************
//
std::string             VideoInputAssetDesc::GetProposedShortKey () const
{
    return GetKey();
}

// ***********************
//
SizeType                VideoInputAssetDesc::EstimateMemoryUsage () const
{
    SizeType assetSize = 0;

    assert( false );

    return assetSize;
}



} // bv

