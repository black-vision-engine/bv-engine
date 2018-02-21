#include "stdafx.h"

#include "VideoInputAudioAssetDesc.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"
#include "Serialization/BV/BVSerializeContext.h"



#include "Memory/MemoryLeaks.h"



namespace bv
{

const std::string   VideoInputAudioAssetDesc::uid = "VIDEO_INPUT_AUDIO_ASSET_DESC";





// ***********************
//
VideoInputAudioAssetDesc::VideoInputAudioAssetDesc( videocards::VideoInputID inputIdx )
    : AudioInputAssetDesc( InputSlotBinding( 0 ) )
    , m_videoInputIdx( inputIdx )
{}


// ***********************
//
VideoInputAudioAssetDescPtr	VideoInputAudioAssetDesc::Create   ( videocards::VideoInputID inputIdx )
{
    return VideoInputAudioAssetDescPtr( new VideoInputAudioAssetDesc( inputIdx ) );
}

// ***********************
//
void                VideoInputAudioAssetDesc::Serialize        ( ISerializer & ser ) const
{
    ser.EnterChild( "asset" );

    ser.SetAttribute( "type", UID() );

    ser.SetAttribute( "VideoInputIdx", Convert::T2String( m_videoInputIdx ) );

    ser.ExitChild();
}

// ***********************
//
void                VideoInputAudioAssetDesc::Deserialize      ( const IDeserializer & sob )
{
    auto obj = SerializationHelper::Create< const VideoInputAudioAssetDesc >( sob );

    if( obj == nullptr || obj->GetUID() != uid )
    {
        assert( !"Failed" ); //FIXME: Error handling.
        return;
    }

    *this = *obj; // if you think this is ugly, see why it cannot be done:
}


// ***********************
//
ISerializableConstPtr       VideoInputAudioAssetDesc::Create          ( const IDeserializer & deser )
{
    auto videoInputIdx = Convert::String2T< videocards::VideoInputID >( deser.GetAttribute( "VideoInputIdx" ) );

    if( videoInputIdx.IsValid() )
        return VideoInputAudioAssetDesc::Create( videoInputIdx.GetVal() );
    else
    {
        if( !videoInputIdx.IsValid() )
            Warn< SerializationException >( deser, "VideoInputIdx is invalid or doesn't exist." );

        return nullptr;
    }
}


// ***********************
//
const std::string &	        VideoInputAudioAssetDesc::GetUID() const
{
    return VideoInputAudioAssetDesc::UID();
}

// ***********************
//
const std::string &         VideoInputAudioAssetDesc::UID()
{
    return VideoInputAudioAssetDesc::uid;
}


// ***********************
//
bool                        VideoInputAudioAssetDesc::IsCacheable() const
{
    return false;
}

// ***********************
//
VoidConstPtr                VideoInputAudioAssetDesc::QueryThis() const
{
    return shared_from_this();
}



// ***********************
//
std::string				VideoInputAudioAssetDesc::GetKey		() const
{
    if( m_key.empty() )
        m_key = ComputeKey();

    return m_key;
}


// ***********************
//
std::string             VideoInputAudioAssetDesc::ComputeKey   () const
{
    return "VideoInputAudioAsset" + Convert::T2String( m_videoInputIdx );
}

// ***********************
//
std::string             VideoInputAudioAssetDesc::GetProposedShortKey () const
{
    return GetKey();
}

// ***********************
//
SizeType                VideoInputAudioAssetDesc::EstimateMemoryUsage () const
{
    SizeType assetSize = 0;

    assert( false );

    return assetSize;
}



} // bv

