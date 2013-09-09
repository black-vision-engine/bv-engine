#include "IndexBuffer.h"
#include <cassert>

namespace bv {

// *********************************
//
IndexBuffer::IndexBuffer	( int numIndices, int indexSize, DataBuffer::Semantic semantic )
	: DataBuffer( numIndices, indexSize, semantic )
	, m_offset( 0 )
    , m_curIndex( 0 )
{
}

// *********************************
//
IndexBuffer::~IndexBuffer	()
{
	//FIXME: Unbind all resources associated with this object
//    Renderer::UnbindAll(this);
}

// *********************************
//
char* IndexBuffer::Data		() const
{
    return m_data;
}

// *********************************
//
void IndexBuffer::SetOffset ( int offset )
{
	assert( offset >=0 );
   
	m_offset = offset;
}

// *********************************
//
int IndexBuffer::Offset		() const
{
    return m_offset;
}

}
