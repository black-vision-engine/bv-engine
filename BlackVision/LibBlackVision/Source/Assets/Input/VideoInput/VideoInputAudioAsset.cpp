#include "stdafx.h"
#include "Assets/Input/TextureInputAsset.h"

#include "Application/ApplicationContext.h"
#include "VideoInputAudioAsset.h"


namespace bv
{

const std::string       VideoInputAudioAsset::uid = "VIDEO_INPUT_AUDIO_ASSET";




// ***********************
//
VideoInputAudioAsset::VideoInputAudioAsset        ( VideoInputSlotsPtr slots, videocards::VideoInputID videoInputIdx )
    : AudioInputAsset( slots->GetInputSlots(), InputSlotBinding( 0 ) )
    , m_videoSlots( slots )
    , m_videoInputIdx( videoInputIdx )
{}

// ***********************
//
void                        VideoInputAudioAsset::EvaluateSlot   ()
{
    auto slotIdx = m_videoSlots->GetSlotIndex( m_videoInputIdx );
    if( slotIdx.IsValid() )
    {
        m_binding = InputSlotBinding( slotIdx.GetVal() );
    }

    AudioInputAsset::EvaluateSlot();
}

// ***********************
//
VideoInputAudioAssetPtr          VideoInputAudioAsset::Create         ( VideoInputSlotsPtr slots, videocards::VideoInputID videoInputIdx )
{
    return VideoInputAudioAssetPtr( new VideoInputAudioAsset( slots, videoInputIdx ) );
}

// ***********************
//
const std::string &         VideoInputAudioAsset::GetUID         () const
{
    return UID();
}

// ***********************
//
const std::string &         VideoInputAudioAsset::UID            ()
{
    return VideoInputAudioAsset::uid;
}

}   // bv

