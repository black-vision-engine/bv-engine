#include "stdafx.h"

#include "DefaultAudioChannel.h"


namespace bv { namespace model {

// ******************************
//
DefaultAudioChannel::DefaultAudioChannel  ( Int32 frequency, AudioFormat format )
    : m_frequency( frequency )
    , m_format( format )
{
}

// ******************************
//
DefaultAudioChannel::~DefaultAudioChannel ()
{
}

// ******************************
//
DefaultAudioChannelPtr  DefaultAudioChannel::Create         ( Int32 frequency, AudioFormat format )
{
    return std::make_shared< DefaultAudioChannel >( frequency, format );
}

// ******************************
//
void                    DefaultAudioChannel::PushPacket     ( MemoryChunkPtr packets )
{
    m_packets.Push( packets );
}

// ******************************
//
MemoryChunkPtr          DefaultAudioChannel::PopPacket      ()
{
    if( !m_packets.IsEmpty() )
    {
        auto packets = m_packets.Front();
        m_packets.Pop();
        return packets;
    }

    return nullptr;
}

// ******************************
//
Int32                   DefaultAudioChannel::GetFrequency   () const 
{
    return m_frequency;
}

// ******************************
//
AudioFormat             DefaultAudioChannel::GetFormat      () const 
{
    return m_format;
}

} //model
} //bv
