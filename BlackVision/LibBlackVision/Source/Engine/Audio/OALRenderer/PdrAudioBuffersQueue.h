#pragma once

#include "BVAL.h"

#include "Engine/Audio/Resources/AudioBuffer.h"
#include "Engine/Audio/OALRenderer/PdrSource.h"
#include "DataTypes/Queue.h"


namespace bv { namespace audio {


class PdrAudioBuffersQueue
{
private:

    static const UInt32             QUEUE_SIZE;

private:

    ALuint *                        m_bufferHandles;
    Queue< ALuint >                 m_unqueuedBufferHandles;
    Queue< AudioBufferConstPtr >    m_buffers;

    ALuint                          m_sourceHandle;

    Int32                           m_frequency;
    ALenum                          m_format;

public:

            PdrAudioBuffersQueue    ( ALuint sourceHandle, Int32 frequency, AudioFormat format );
            ~PdrAudioBuffersQueue   ();

    void    Reinitialize            ( Int32 frequency, AudioFormat format );
    
    void    PushData                ( const std::vector< AudioBufferConstPtr > & buffers );
    
    bool    BufferData              ();

private:

    void    InitBuffers             ( Int32 frequency, AudioFormat format );
    void    ClearBuffers            ();

};

} // audio
} // bv