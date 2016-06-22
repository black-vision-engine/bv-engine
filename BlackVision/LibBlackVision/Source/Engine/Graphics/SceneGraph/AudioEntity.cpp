#include "stdafx.h"

#include "AudioEntity.h"


namespace bv {

// *********************************
//
AudioEntity::AudioEntity            ( Int32 frequency, AudioFormat format )
    : m_frequency( frequency )
    , m_format( format )
{
}

// *********************************
//
AudioEntity::~AudioEntity           ()
{
    m_audioData.Clear();
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
bool                        AudioEntity::IsEmpty        () const
{
    return m_audioData.IsEmpty();
}

// ***********************
//
void                        AudioEntity::PushData       ( const std::vector< MemoryChunkConstPtr > & data )
{
    auto buffer = audio::AudioBuffer::Create( data, m_frequency, m_format );
    m_audioData.Push( buffer );
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

} //bv
