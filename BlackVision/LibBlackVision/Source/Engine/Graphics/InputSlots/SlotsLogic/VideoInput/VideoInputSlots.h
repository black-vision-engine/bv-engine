#pragma once

#include "Engine/Graphics/InputSlots/InputSlots.h"
#include "Engine/Graphics/InputSlots/SlotsLogic/AVFrame/AVFrameInputSlots.h"

#include "VideoInputSlotEntryDesc.h"



namespace bv
{


/**@brief Wrapper for inputs slots from video cards.

@ingroup EngineInputs
@ingroup VideoCards*/
class VideoInputSlots
{
private:

    AVFrameInputSlots       m_avInputSlots;

    std::vector< VideoInputSlotEntryDesc >      m_entries;

public:

    explicit        VideoInputSlots ( InputSlotsPtr slots );
    virtual         ~VideoInputSlots();


    bool            RegisterVideoInputChannel       ( const videocards::VideoInputChannelDesc & vidInputDesc );

public:

    bool            Exists          ( videocards::VideoInputID id ) const;

};



}	// bv


