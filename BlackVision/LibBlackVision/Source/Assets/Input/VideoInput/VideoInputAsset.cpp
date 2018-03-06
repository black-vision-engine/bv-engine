#include "stdafx.h"
#include "Assets/Input/TextureInputAsset.h"

#include "Application/ApplicationContext.h"
#include "VideoInputAsset.h"


namespace bv
{

const std::string       VideoInputAsset::uid = "VIDEO_INPUT_ASSET";




// ***********************
//
VideoInputAsset::VideoInputAsset        ( VideoInputTextureAssetConstPtr fill, VideoInputTextureAssetConstPtr key, VideoInputAudioAssetConstPtr audio )
    : m_fill( fill )
    , m_key( key )
    , m_audio( audio )
{}

// ***********************
//
VideoInputAssetPtr          VideoInputAsset::Create         ( VideoInputTextureAssetConstPtr fill, VideoInputTextureAssetConstPtr key, VideoInputAudioAssetConstPtr audio )
{
    return VideoInputAssetPtr( new VideoInputAsset( fill, key, audio ) );
}

// ***********************
//
const std::string &         VideoInputAsset::GetUID         () const
{
    return UID();
}

// ***********************
//
const std::string &         VideoInputAsset::UID            ()
{
    return VideoInputAsset::uid;
}

// ***********************
//
VoidConstPtr                VideoInputAsset::QueryThis      () const
{
    return shared_from_this();
}

}   // bv

