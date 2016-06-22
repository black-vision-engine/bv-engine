#include "stdafx.h"

#include "AudioBuffer.h"


namespace bv { namespace audio {


// *********************************
//
AudioBuffer::AudioBuffer	( SizeType size, Int32 frequency, AudioFormat format )
	: m_size( size )
	, m_frequency( frequency )
    , m_format( format )
{
	m_data = new char[ m_size ];
}

// *********************************
//
AudioBuffer::~AudioBuffer   ()
{
    delete[] m_data;
}

// *********************************
//
SizeType            AudioBuffer::GetSize			    () const
{
	return m_size;
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
const char *        AudioBuffer::Data				() const
{
    return m_data;
}

// *********************************
//
void                AudioBuffer::WriteToBuffer       ( const char * src, SizeType size )
{
    memcpy( m_data, src, size );
}
    
// *********************************
//
AudioBufferPtr      AudioBuffer::Create             ( const std::vector< MemoryChunkConstPtr > & data, Int32 frequency, AudioFormat format )
{
    SizeType size = 0;
    for( auto & chunk : data )
    {
        size += chunk->Size();
    }

    auto buffer = std::make_shared< AudioBuffer >( size, frequency, format ); 

    SizeType offset = 0;
    for( auto & chunk : data )
    {
        memcpy( buffer->m_data + offset, chunk->Get(), chunk->Size() );
        offset += chunk->Size();
    }

    return buffer;
}

// *********************************
//
AudioBufferPtr      AudioBuffer::Create             ( const char * data, SizeType size, Int32 frequency, AudioFormat format )
{
    auto buffer = std::make_shared< AudioBuffer >( size, frequency, format ); 
    buffer->WriteToBuffer( data, size );
    return buffer;
}


} // audio
} //bv