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
void                        AudioEntity::PushData       ( MemoryChunkConstPtr data )
{
    auto buffer = audio::AudioBuffer::Create( data->Get(), data->Size(), m_frequency, m_format );
    m_audioData.Push( buffer );
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
std::vector< audio::AudioBufferConstPtr >  AudioEntity::PopData        ()
{
    std::vector< audio::AudioBufferConstPtr > buffers;

    while( !m_audioData.IsEmpty() )
    {
        auto data = m_audioData.Front();
        buffers.push_back( data );
        m_audioData.Pop();
    }

    return buffers;
}

} //bv
