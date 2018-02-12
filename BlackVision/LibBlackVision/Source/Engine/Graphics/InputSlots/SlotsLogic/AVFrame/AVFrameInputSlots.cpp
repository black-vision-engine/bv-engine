#include "stdafx.h"
#include "AVFrameInputSlots.h"






namespace bv
{

// ***********************
//
AVFrameInputSlots::AVFrameInputSlots    ( InputSlotsPtr slots )
    :   m_inputSlots( slots )
{}

// ***********************
//
void        AVFrameInputSlots::UpdateSlot       ( SlotIndex slotIdx, AVFramePtr frame )
{
    auto inputSlot = m_inputSlots->AccessSource( slotIdx );

    if( inputSlot.IsValid() )
    {
        UpdateTexture( inputSlot.GetVal().Texture, frame );
        UpdateAudio( inputSlot.GetVal().Audio, frame );
    }
}

// ***********************
//
void        AVFrameInputSlots::UpdateTexture    ( Texture2DPtr texture, AVFramePtr frame )
{
    if( texture )
    {
        texture->SetData( frame->m_videoData );
    }
}

// ***********************
//
void        AVFrameInputSlots::UpdateAudio      ( audio::AudioBufferPtr audio, AVFramePtr frame )
{
    if( audio )
    {
        audio->Update( frame->m_audioData );
    }
}


}	// bv

