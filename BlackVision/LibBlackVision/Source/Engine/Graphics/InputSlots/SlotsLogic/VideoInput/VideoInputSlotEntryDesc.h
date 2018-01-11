#pragma once


#include "CoreDEF.h"

#include "VideoInput/VideoInputChannelDesc.h"
#include "Engine/Graphics/InputSlots/InputSlots.h"

#include <string>



namespace bv
{




/**@brief Descriptor maps video input channel to input slot.

@ingroup VideoCards
@ingroup EngineInputs*/
class VideoInputSlotEntryDesc
{
private:

    videocards::VideoInputChannelDesc       m_videoChannel;
    SlotIndex                               m_slotIdx;

public:

    explicit    VideoInputSlotEntryDesc     ( const videocards::VideoInputChannelDesc & channelDesc, SlotIndex slotIdx );
  
public:

    SlotIndex                   GetSlotIdx      () const { return m_slotIdx; }
    videocards::VideoInputID    GetVideoInputID () const { return m_videoChannel.GetInputID(); }


    const videocards::VideoInputChannelDesc &   GetVideoChannelDesc     () const { return m_videoChannel; }
};




// ========================================================================= //
// Implementation
// ========================================================================= //

// ***********************
//
inline VideoInputSlotEntryDesc::VideoInputSlotEntryDesc    ( const videocards::VideoInputChannelDesc & channelDesc, SlotIndex slotIdx )
    : m_videoChannel( channelDesc )
    , m_slotIdx( slotIdx )
{}



}   // bv

