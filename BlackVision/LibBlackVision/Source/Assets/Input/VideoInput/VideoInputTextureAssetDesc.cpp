#include "stdafx.h"

#include "VideoInputTextureAssetDesc.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"
#include "Serialization/BV/BVSerializeContext.h"



#include "Memory/MemoryLeaks.h"



namespace bv
{

const std::string   VideoInputTextureAssetDesc::uid = "VIDEO_INPUT_TEXTURE_ASSET_DESC";



namespace Convert
{


// ***********************
//
std::pair< VideoInputTextureAssetDesc::VideoType, const char* > VideoInputTypeMapping[] =
{
    std::make_pair( bv::VideoInputTextureAssetDesc::VideoType::Fill, "Fill" )
    , std::make_pair( bv::VideoInputTextureAssetDesc::VideoType::Key, "Key" )
    , std::make_pair( bv::VideoInputTextureAssetDesc::VideoType::Total, "" )      // default
};

IMPLEMENT_ENUM_SERIALIZATION( VideoInputTextureAssetDesc::VideoType, VideoInputTypeMapping )

}   // Convert


// ***********************
//
VideoInputTextureAssetDesc::VideoInputTextureAssetDesc( videocards::VideoInputID inputIdx, VideoType type )
    : TextureInputAssetDesc( InputSlotBinding( 0 ) )
    , m_videoInputIdx( inputIdx )
    , m_inputType( type )
{}


// ***********************
//
VideoInputTextureAssetDescPtr	VideoInputTextureAssetDesc::Create   ( videocards::VideoInputID inputIdx, VideoType type )
{
    return VideoInputTextureAssetDescPtr( new VideoInputTextureAssetDesc( inputIdx, type ) );
}

// ***********************
//
void                VideoInputTextureAssetDesc::Serialize        ( ISerializer & ser ) const
{
    ser.EnterChild( "asset" );

    ser.SetAttribute( "type", UID() );

    ser.SetAttribute( "VideoInputIdx", Convert::T2String( m_videoInputIdx ) );
    ser.SetAttribute( "VideoType", Convert::T2String( m_inputType ) );

    ser.ExitChild();
}

// ***********************
//
void                VideoInputTextureAssetDesc::Deserialize      ( const IDeserializer & sob )
{
    auto obj = SerializationHelper::Create< const VideoInputTextureAssetDesc >( sob );

    if( obj == nullptr || obj->GetUID() != uid )
    {
        assert( !"Failed" ); //FIXME: Error handling.
        return;
    }

    *this = *obj; // if you think this is ugly, see why it cannot be done:
}


// ***********************
//
ISerializableConstPtr       VideoInputTextureAssetDesc::Create          ( const IDeserializer & deser )
{
    auto videoInputIdx = Convert::String2T< videocards::VideoInputID >( deser.GetAttribute( "VideoInputIdx" ) );
    auto inputType = Convert::String2T< VideoType >( deser.GetAttribute( "VideoType" ) );

    if( videoInputIdx.IsValid(), inputType.IsValid() )
        return VideoInputTextureAssetDesc::Create( videoInputIdx.GetVal(), inputType.GetVal() );
    else
        return nullptr;
}


// ***********************
//
const std::string &	        VideoInputTextureAssetDesc::GetUID() const
{
    return VideoInputTextureAssetDesc::UID();
}

// ***********************
//
const std::string &         VideoInputTextureAssetDesc::UID()
{
    return VideoInputTextureAssetDesc::uid;
}


// ***********************
//
bool                        VideoInputTextureAssetDesc::IsCacheable() const
{
    return false;
}

// ***********************
//
VoidConstPtr                VideoInputTextureAssetDesc::QueryThis() const
{
    return shared_from_this();
}



// ***********************
//
std::string				VideoInputTextureAssetDesc::GetKey		() const
{
    if( m_key.empty() )
        m_key = ComputeKey();

    return m_key;
}


// ***********************
//
std::string             VideoInputTextureAssetDesc::ComputeKey   () const
{
    return "VideoInputAsset" + Convert::T2String( m_videoInputIdx );
}

// ***********************
//
std::string             VideoInputTextureAssetDesc::GetProposedShortKey () const
{
    return GetKey();
}

// ***********************
//
SizeType                VideoInputTextureAssetDesc::EstimateMemoryUsage () const
{
    SizeType assetSize = 0;

    assert( false );

    return assetSize;
}



} // bv

