#include "stdafx.h"

#include "AudioBuffer.h"


namespace bv { namespace audio {


// *********************************
//
AudioBuffer::AudioBuffer	( MemoryChunkConstPtr data, Int32 frequency, AudioFormat format, bool eof )
	: m_data( data )
	, m_frequency( frequency )
    , m_format( format )
    , m_eof( eof )
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
AudioFormat         AudioBuffer::GetFormat          () const
{
	return m_format;
}

// *********************************
//
bool                AudioBuffer::IsEOF              () const
{
	return m_eof;
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
AudioBufferPtr      AudioBuffer::Create             ( MemoryChunkConstPtr data, Int32 frequency, AudioFormat format, bool eof )
{
    return std::make_shared< AudioBuffer >( data, frequency, format, eof ); 
}

// ***********************
//
void                AudioBuffer::Update             ( MemoryChunkConstPtr data )
{
    m_data = data;
}

// ***********************
//
void                AudioBuffer::Clear              ()
{
    m_data = MemoryChunk::Create( m_data->Size() );
}

} // audio
} //bv