#include "stdafx.h"

#include "AudioEntity.h"
#include "Resources/AudioUtils.h"


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
    if( data->Size() > 0 )
    {
        m_audioData.Push( audio::AudioBuffer::Create( data, m_frequency, m_format ) );
    }
}

// ***********************
//
AudioEntity::AudioBufferVec AudioEntity::PopData        ()
{
    AudioBufferVec buffers;

    while( !m_audioData.IsEmpty() )
    {
        auto data = m_audioData.Front();
        buffers.push_back( data );
        m_audioData.Pop();
    }

    return buffers;
}

// *******************************
//
UInt32                      AudioEntity::GetUpdateID        () const
{
    return m_updateID;
}


// *********************************
//
Int32                       AudioEntity::GetChannels        () const
{
	return AudioUtils::ChannelsCount( m_format );
}

// *********************************
//
Int32                       AudioEntity::GetChannelDepth () const
{
	return AudioUtils::ChannelDepth( m_format );
}

} //audio
} //bv
