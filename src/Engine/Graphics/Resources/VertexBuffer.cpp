#include "VertexBuffer.h"

namespace bv {

// *********************************
//
VertexBuffer::VertexBuffer ( int numVertices, int vertexSize, DataBuffer::Semantic semantic )
    : DataBuffer(numVertices, vertexSize, semantic )
    , m_curVertex(0)
{
}

// *********************************
//
VertexBuffer::~VertexBuffer ()
{
    //FIXME:: unbind all resoutrces from renderer associated with this object
}

// *********************************
//
char *	VertexBuffer::Data	() const
{
    return m_data;
}

// *********************************
//
void    VertexBuffer::WriteToBuffer   ( const char* src, int size )
{
    memcpy( m_data, src, size );
}

}
