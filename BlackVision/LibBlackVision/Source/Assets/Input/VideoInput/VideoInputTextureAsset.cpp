#include "stdafx.h"
#include "Assets/Input/TextureInputAsset.h"

#include "Application/ApplicationContext.h"
#include "VideoInputTextureAsset.h"


namespace bv
{

const std::string       VideoInputTextureAsset::uid = "VIDEO_INPUT_TEXTURE_ASSET";




// ***********************
//
VideoInputTextureAsset::VideoInputTextureAsset        ( VideoInputSlotsPtr slots, videocards::VideoInputID videoInputIdx, videocards::VideoType type )
    : TextureInputAsset( slots->GetInputSlots(), InputSlotBinding( 0 ) )
    , m_videoInputIdx( videoInputIdx )
    , m_videoType( type )
{}

// ***********************
//
void                        VideoInputTextureAsset::EvaluateSlot   ()
{}

// ***********************
//
VideoInputTextureAssetPtr          VideoInputTextureAsset::Create         ( VideoInputSlotsPtr slots, videocards::VideoInputID videoInputIdx, videocards::VideoType type )
{
    return VideoInputTextureAssetPtr( new VideoInputTextureAsset( slots, videoInputIdx, type ) );
}

// ***********************
//
const std::string &         VideoInputTextureAsset::GetUID         () const
{
    return UID();
}

// ***********************
//
const std::string &         VideoInputTextureAsset::UID            ()
{
    return VideoInputTextureAsset::uid;
}

}   // bv

