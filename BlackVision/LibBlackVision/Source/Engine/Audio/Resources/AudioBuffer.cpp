#include "stdafx.h"

#include "AudioBuffer.h"
#include "AudioUtils.h"


namespace bv { namespace audio {


// *********************************
//
AudioBuffer::AudioBuffer	( MemoryChunkConstPtr data, Int32 frequency, AudioFormat format )
	: m_data( data )
	, m_frequency( frequency )
    , m_format( format )
{
}

// *********************************
//
AudioBuffer::~AudioBuffer   ()
{
}

// *********************************
//
SizeType            AudioBuffer::GetSize			    () const
{
	return m_data->Size();
}

// *********************************
//
Int32               AudioBuffer::GetFrequency	    () const
{
    return m_frequency;
}

// *********************************
//
AudioFormat         AudioBuffer::GetFormat           () const
{
	return m_format;
}

// *********************************
//
Int32               AudioBuffer::GetChannels         () const
{
	return AudioUtils::ChannelsCount( m_format );
}

// *********************************
//
Int32               AudioBuffer::GetChannelBitDepth  () const
{
	return AudioUtils::ChannelBitDepth( m_format );
}

// *********************************
//
MemoryChunkConstPtr AudioBuffer::GetData			() const
{
    return m_data;
}

// *********************************
//
const char *        AudioBuffer::GetRawData			() const
{
    return m_data->Get();
}

// *********************************
//
AudioBufferPtr      AudioBuffer::Create             ( MemoryChunkConstPtr data, Int32 frequency, AudioFormat format )
{
    return std::make_shared< AudioBuffer >( data, frequency, format ); 
}

} // audio
} //bv