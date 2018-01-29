#include "stdafx.h"
#include "Assets/Input/TextureInputAsset.h"

#include "Application/ApplicationContext.h"
#include "VideoInputAsset.h"


namespace bv
{

const std::string       VideoInputAsset::uid = "VIDEO_INPUT_ASSET";




// ***********************
//
VideoInputAsset::VideoInputAsset        ( VideoInputSlotsPtr slots, videocards::VideoInputID videoInputIdx )
    : TextureInputAsset( slots->GetInputSlots(), InputSlotBinding( 0 ) )
    , m_videoInputIdx( videoInputIdx )
{}

// ***********************
//
void                        VideoInputAsset::EvaluateSlot   ()
{}

// ***********************
//
VideoInputAssetPtr          VideoInputAsset::Create         ( VideoInputSlotsPtr slots, videocards::VideoInputID videoInputIdx )
{
    return VideoInputAssetPtr( new VideoInputAsset( slots, videoInputIdx ) );
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

}   // bv

