#pragma once

#include "DataTypes/Queue.h"
#include "Engine/Audio/Resources/AudioBuffer.h"
#include "Memory/MemoryChunk.h"


namespace bv {


class AudioEntity
{
protected:

    Queue< audio::AudioBufferConstPtr > m_audioData;

    Int32                               m_frequency;
    AudioFormat                         m_format;

public:

                                AudioEntity     ( Int32 frequency, AudioFormat format );
    virtual                     ~AudioEntity    ();

public:

    void                                        PushData        ( MemoryChunkConstPtr data );
    void                                        PushData        ( const std::vector< MemoryChunkConstPtr > & data );

    std::vector< audio::AudioBufferConstPtr >   PopData         ();

};

}
