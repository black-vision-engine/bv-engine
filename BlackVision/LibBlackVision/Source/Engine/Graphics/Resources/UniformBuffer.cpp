#include "stdafx.h"

#include "UniformBuffer.h"


namespace bv {

// *********************************
//
UniformBuffer::UniformBuffer ( SizeType size, DataBuffer::Semantic semantic )
    : m_size( size )
    , m_semantic( semantic )
{
    assert( size > 0 );

	m_data = new char[ m_size ];
}

// *********************************
//
UniformBuffer::~UniformBuffer ()
{
    delete[] m_data;
}

// *********************************
//
DataBuffer::Semantic	UniformBuffer::GetSemantic	() const
{
    return m_semantic;
}

// *********************************
//
SizeType    UniformBuffer::Size	() const
{
    return m_size;
}

// *********************************
//
char *	    UniformBuffer::Data	() const
{
    return m_data;
}

// *********************************
//
void        UniformBuffer::WriteToBuffer   ( const char * src, SizeType size )
{
    memcpy( m_data, src, size );
}

}
