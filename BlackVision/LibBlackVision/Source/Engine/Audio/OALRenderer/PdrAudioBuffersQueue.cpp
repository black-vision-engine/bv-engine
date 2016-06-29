#include "stdafx.h"

#include "PdrAudioBuffersQueue.h"
#include "Engine/Audio/OALRenderer/PdrOALConstants.h"


namespace bv { namespace audio {


const UInt32 PdrAudioBuffersQueue::QUEUE_SIZE    = 3;

// *******************************
//
PdrAudioBuffersQueue::PdrAudioBuffersQueue          ( Int32 frequency, AudioFormat format )
    : m_frequency( frequency )
    , m_format( OALConstantsMapper::ALConstant( format ) )
{
    m_bufferHandles = new ALuint[ QUEUE_SIZE ];
    BVAL::bvalGenBuffers( QUEUE_SIZE, m_bufferHandles );

    for( UInt32 i = 0; i < QUEUE_SIZE; ++i )
    {
        m_unqueuedBufferHandles.Push( m_bufferHandles[ i ] );
    }
}

// *******************************
//
PdrAudioBuffersQueue::~PdrAudioBuffersQueue         ()
{
    BVAL::bvalDeleteBuffers( QUEUE_SIZE, m_bufferHandles );
    delete [] m_bufferHandles;
}

// *******************************
//
void    PdrAudioBuffersQueue::PushData     ( const std::vector< AudioBufferConstPtr > & buffers )
{
    for( auto & buffer : buffers )
    {
        if( buffer )
        {
            m_buffers.Push( buffer );
        }
    }
}

// *******************************
//
bool    PdrAudioBuffersQueue::BufferData      ( const PdrSource * source )
{
    auto success = false;

    auto sourceHandle = source->GetHandle();

    UInt32 bufferId = 0;
    Int32 processed = 0;
    BVAL::bvalGetSourcei( sourceHandle, AL_BUFFERS_PROCESSED, &processed );

    while( processed )
    {
        BVAL::bvalSourceUnqueueBuffers( sourceHandle, 1, &bufferId );
        m_unqueuedBufferHandles.Push( bufferId );
        processed--;
    }

    while( !m_buffers.IsEmpty() && !m_unqueuedBufferHandles.IsEmpty() )
    {
        auto buffer = m_buffers.Front();
        bufferId = m_unqueuedBufferHandles.Front();

        BVAL::bvalBufferData( bufferId, m_format, ( const ALvoid * )buffer->Data(), ( Int32 )buffer->GetSize(), m_frequency );
        BVAL::bvalSourceQueueBuffers( sourceHandle, 1, &bufferId );
        m_buffers.Pop();
        m_unqueuedBufferHandles.Pop();

        success = true;
    }

    return success;
}

} // audio
} // bv