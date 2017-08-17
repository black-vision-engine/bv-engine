#include "stdafx.h"

#include "AudioEntity.h"
#include "Util/Audio/AudioUtils.h"


namespace bv { namespace audio {


// *********************************
//
AudioEntity::AudioEntity            ( Int32 frequency, AudioFormat format )
    : m_frequency( frequency )
    , m_format( format )
    , m_updateID( 0 )
{
}

// *********************************
//
AudioEntity::~AudioEntity           ()
{
    m_audioData.Clear();
}

// *********************************
//
void                        AudioEntity::Reinitialize   ( Int32 frequency, AudioFormat format )
{
    m_audioData.Clear();

    m_frequency = frequency;
    m_format = format;

	m_updateID++;
}

// ***********************
//
Int32                       AudioEntity::GetFrequency   () const
{
    return m_frequency;
}

// ***********************
//
AudioFormat                 AudioEntity::GetFormat      () const
{
    return m_format;
}

// ***********************
//
void                        AudioEntity::Clear          ()
{
    while( !m_audioData.IsEmpty() )
    {
        m_audioData.Pop();
    }
}

// ***********************
//
bool                        AudioEntity::IsEmpty        () const
{
    return m_audioData.IsEmpty();
}

// ***********************
//
void                        AudioEntity::PushData       ( MemoryChunkConstPtr data )
{
    if( !data )
    {
        m_audioData.Push( audio::AudioBuffer::Create( data, m_frequency, m_format, true ) );
    }
    else if( data->Size() > 0 )
    {
        m_audioData.Push( audio::AudioBuffer::Create( data, m_frequency, m_format, false ) );
    }
}

// ***********************
//
AudioBufferVec AudioEntity::PopData        ()
{
    AudioBufferVec buffers;

    while( !m_audioData.IsEmpty() )
    {
        auto data = m_audioData.Front();
        if( !data->IsEOF() )
        {
            buffers.push_back( data );
            m_audioData.Pop();
        }
        else
        {
			m_audioData.Pop();
            break;
        }
    }

    return buffers;
}

// *******************************
//
bool                        AudioEntity::IsEOF              () const
{
    if( !m_audioData.IsEmpty() )
    {
        return m_audioData.Front()->IsEOF();
    }
    return false;
}

// *******************************
//
UInt32                      AudioEntity::GetUpdateID        () const
{
    return m_updateID;
}

// *******************************
//
bool                        AudioEntity::IsPlaying          () const
{
    return m_playing;
}

// *******************************
//
void                        AudioEntity::Play               ()
{
    m_playing = true;
}

// *******************************
//
void                        AudioEntity::Stop               ()
{
    m_playing = false;
}

} //audio
} //bv
