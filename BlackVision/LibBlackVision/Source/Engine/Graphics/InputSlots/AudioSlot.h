#pragma once

#include "Engine/Audio/Resources/AudioBuffer.h"
#include "Application/ApplicationContext.h"


namespace bv
{


// ***********************
//
class AudioSlot
{
public:

    audio::AudioBufferPtr   AudioBuffer;
    UInt64                  UpdateID;

public:

    explicit        AudioSlot   ( audio::AudioBufferPtr audio )
        : AudioBuffer( audio )
        , UpdateID( 0 )
    {}


    void            Update      ( MemoryChunkConstPtr data );
};

DEFINE_PTR_TYPE( AudioSlot )


// ***********************
//
inline void         AudioSlot::Update       ( MemoryChunkConstPtr data )
{
    AudioBuffer->Update( data );
    UpdateID = ApplicationContext::Instance().GetUpdateCounter() + 1;
}


}	// bv



