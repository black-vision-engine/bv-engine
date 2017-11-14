#pragma once

#include "Engine/Graphics/InputSlots/InputSlots.h"
#include "Engine/Graphics/InputSlots/SlotsLogic/AVFrame/AVFrameInputSlots.h"


namespace bv
{


// ***********************
/// @ingroup EngineInputs
class VideoInputSlots
{
private:

    AVFrameInputSlots       m_avInputSlots;

public:

    explicit        VideoInputSlots ( InputSlotsPtr slots );
    virtual         ~VideoInputSlots();
};



}	// bv


