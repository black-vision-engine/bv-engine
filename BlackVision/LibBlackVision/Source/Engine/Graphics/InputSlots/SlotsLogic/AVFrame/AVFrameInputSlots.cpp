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
void        AVFrameInputSlots::UpdateSlot       ( SlotIndex slotIdx, AVFrameConstPtr frame )
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
void        AVFrameInputSlots::UpdateTexture    ( Texture2DPtr texture, AVFrameConstPtr frame )
{
    if( texture )
    {
        texture->SetData( frame->m_videoData );
    }
}

// ***********************
//
void        AVFrameInputSlots::UpdateAudio      ( audio::AudioBufferPtr audio, AVFrameConstPtr frame )
{
    if( audio )
    {
        if( frame->m_audioData )
            audio->Update( frame->m_audioData );
        else
            audio->Clear();
    }
}


}	// bv

