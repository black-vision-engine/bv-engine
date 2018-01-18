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
VideoInputAssetDesc::VideoInputAssetDesc( videocards::VideoInputID inputIdx )
    : TextureInputAssetDesc( InputSlotBinding( 0 ) )
    , m_videoInputIdx( inputIdx )
{}


// ***********************
//
VideoInputAssetDescPtr	VideoInputAssetDesc::Create   ( videocards::VideoInputID inputIdx )
{
    return VideoInputAssetDescPtr( new VideoInputAssetDesc( inputIdx ) );
}

// ***********************
//
void                VideoInputAssetDesc::Serialize        ( ISerializer & ser ) const
{
    ser.EnterChild( "asset" );

    ser.SetAttribute( "VideoInputIdx", Convert::T2String( m_videoInputIdx ) );

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
    auto videoInputIdx = Convert::String2T< videocards::VideoInputID >( deser.GetAttribute( "VideoInputIdx" ) );

    if( videoInputIdx.IsValid() )
        return VideoInputAssetDesc::Create( videoInputIdx.GetVal() );
    else
        return nullptr;
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
    return "VideoInputAsset" + Convert::T2String( m_videoInputIdx );
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

