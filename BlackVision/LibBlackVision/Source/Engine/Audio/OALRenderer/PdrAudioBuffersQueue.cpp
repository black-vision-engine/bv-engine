#include "stdafx.h"

#include "PdrAudioBuffersQueue.h"
#include "Engine/Audio/OALRenderer/PdrOALConstants.h"
#include "Util/Audio/AudioUtils.h"


namespace bv { namespace audio {


const UInt32 PdrAudioBuffersQueue::QUEUE_SIZE    = 15;

// *******************************
//
PdrAudioBuffersQueue::PdrAudioBuffersQueue          ( ALuint sourceHandle, Int32 frequency, AudioFormat format )
    : m_sourceHandle( sourceHandle )
{
    InitBuffers( frequency, format );
}

// *******************************
//
PdrAudioBuffersQueue::~PdrAudioBuffersQueue         ()
{
    ClearBuffers();
}

// *******************************
//
void    PdrAudioBuffersQueue::Reinitialize  ( Int32 frequency, AudioFormat format )
{
    ClearBuffers();
    InitBuffers( frequency, format );
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
bool    PdrAudioBuffersQueue::BufferData      ()
{
    auto success = false;

    UInt32 bufferId = 0;
    Int32 processed = 0;
    BVAL::bvalGetSourcei( m_sourceHandle, AL_BUFFERS_PROCESSED, &processed );

	// Get indexes of already played buffers.
    while( processed )
    {
        BVAL::bvalSourceUnqueueBuffers( m_sourceHandle, 1, &bufferId );
        m_unqueuedBufferHandles.Push( bufferId );
        processed--;
    }
	
	// If there is no space to play the newest frames clear everything.
	if( m_unqueuedBufferHandles.IsEmpty() && !m_buffers.IsEmpty() )
	{
		BVAL::bvalSourceStop( m_sourceHandle );

		BVAL::bvalGetSourcei( m_sourceHandle, AL_BUFFERS_PROCESSED, &processed );

		while( processed )
		{
			BVAL::bvalSourceUnqueueBuffers( m_sourceHandle, 1, &bufferId );
			m_unqueuedBufferHandles.Push( bufferId );
			processed--;
		}
	}

	// Fill bufffers with new frames to play.
    while( !m_buffers.IsEmpty() && !m_unqueuedBufferHandles.IsEmpty() )
    {
        auto buffer = m_buffers.Front();
        bufferId = m_unqueuedBufferHandles.Front();

        BVAL::bvalBufferData( bufferId, m_format, ( const ALvoid * )buffer->GetRawData(), ( Int32 )buffer->GetSize(), m_frequency );
        BVAL::bvalSourceQueueBuffers( m_sourceHandle, 1, &bufferId );

        m_buffers.Pop();
        m_unqueuedBufferHandles.Pop();

        success = true;

        
    }

    //if( success )
    //    LOG_MESSAGE( SeverityLevel::debug ) << "success:  m_unqueuedBufferHandles.size() == " << m_unqueuedBufferHandles.Size();
    //else
    //    LOG_MESSAGE( SeverityLevel::debug ) << "fail: m_unqueuedBufferHandles.size() == " << m_unqueuedBufferHandles.Size();

    //if( m_unqueuedBufferHandles.Size() > 13 )
    //{
    //    success = success;
    //}

	//auto enoughDataBuffers = m_unqueuedBufferHandles.Size() < QUEUE_SIZE - 1; // Start playing only if at least 2 frames are buffered.

	//if( success && !enoughDataBuffers )
		//LOG_MESSAGE( SeverityLevel::debug ) << "Not enought audio data buffered. Starting playing will be postponed.";

    return success;// && enoughDataBuffers;
}

// *******************************
//
void    PdrAudioBuffersQueue::InitBuffers   ( Int32 frequency, AudioFormat format )
{
    m_frequency = frequency;
    m_format = OALConstantsMapper::ALConstant( format );

    m_bufferHandles = new ALuint[ QUEUE_SIZE ];
    BVAL::bvalGenBuffers( QUEUE_SIZE, m_bufferHandles );

    for( UInt32 i = 0; i < QUEUE_SIZE; ++i )
    {
        m_unqueuedBufferHandles.Push( m_bufferHandles[ i ] );
    }
}

// *******************************
//
void    PdrAudioBuffersQueue::ClearBuffers  ()
{
    BVAL::bvalSourcei( m_sourceHandle, AL_BUFFER, 0 );
    BVAL::bvalDeleteBuffers( QUEUE_SIZE, m_bufferHandles );
    delete [] m_bufferHandles;

    m_unqueuedBufferHandles.Clear();
    m_buffers.Clear();
}

// *******************************
//
Int32    PdrAudioBuffersQueue::GetFrequency         () const
{
    return m_frequency;
}

// *******************************
//
bool    PdrAudioBuffersQueue::BufferingDone         () const
{
    return ( m_unqueuedBufferHandles.Size() == QUEUE_SIZE );
}

} // audio
} // bv