#pragma once

#include "Engine/Graphics/InputSlots/InputSlots.h"

#include "Memory/AVFrame.h"


namespace bv
{



/**@brief Wrapper for InputSlots adds functionality of updating input source by using AVFrame.

@ingroup EngineInputs*/
class AVFrameInputSlots
{
private:

    InputSlotsPtr       m_inputSlots;

public:

    explicit        AVFrameInputSlots   ( InputSlotsPtr slots );

    void            UpdateSlot          ( SlotIndex slotIdx, AVFramePtr frame );

public:

    InputSlotsPtr   GetInputSlots       () const { return m_inputSlots; }

private:

    void            UpdateTexture       ( Texture2DPtr texture, AVFramePtr frame );
    void            UpdateAudio         ( audio::AudioBufferPtr audio, AVFramePtr frame );
};



}	// bv


