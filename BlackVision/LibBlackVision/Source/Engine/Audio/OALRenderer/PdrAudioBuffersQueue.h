#pragma once

#include "BVAL.h"

#include "Engine/Audio/Resources/AudioBuffer.h"
#include "Engine/Audio/OALRenderer/PdrSource.h"
#include "DataTypes/Queue.h"


namespace bv { namespace audio {


class PdrAudioBuffersQueue
{
private:

    static const Int32              QUEUE_SIZE;

private:

    ALuint *                        m_bufferHandles;
    Queue< ALuint >                 m_unqueuedBufferHandles;
    Queue< AudioBufferConstPtr >    m_buffers;

public:

            PdrAudioBuffersQueue    ();
            ~PdrAudioBuffersQueue   ();

    void    PushData                ( const std::vector< AudioBufferConstPtr > & buffers );
    
    bool    BufferData              ( const PdrSource * source );

};

} // audio
} // bv