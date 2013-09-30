#include "VertexArray.h"

#include <cassert>

#include "Engine\Graphics\Resources\VertexDescriptor.h"
#include "Engine\Graphics\Resources\VertexBuffer.h"

namespace bv
{

// *******************************
//
VertexArrayEntry::VertexArrayEntry( VertexBuffer * vb, VertexDescriptor * vd, unsigned int numVert, unsigned int vbOffset )
    : vertexBuffer( vb )
    , vertexDescriptor( vd )
    , numVertices( numVert )
    , vertexBufferOffset( vbOffset )
{
}

// *******************************
// Creates exactly one entry
VertexArray::VertexArray         ( VertexBuffer * vb, VertexDescriptor * vd, unsigned int numVertices, unsigned int vertexBufferOffset )
{
    AddEntry( vb, vd, numVertices, vertexBufferOffset );
}

// *******************************
//
VertexArray::VertexArray         ()
{
}

// *******************************
//
VertexArray::~VertexArray        ()
{
}

// *******************************
//
int                     VertexArray::NumEntries          () const
{
    return m_vaoEntries.size();
}

// *******************************
//
void                    VertexArray::AddEntry            ( VertexBuffer * vb, VertexDescriptor * vd, unsigned int numVertices, unsigned int vertexBufferOffset )
{
    m_vaoEntries.push_back( VertexArrayEntry( vb, vd, numVertices, vertexBufferOffset ) );
}

// *******************************
//
VertexArrayEntry        VertexArray::Entry               ( int i ) const
{
    assert( i >= 0 );
    assert( i < NumEntries() );

    return m_vaoEntries[ i ];
}


// *************************************  VertexArraySingleVertexBuffer *********************************************

// *******************************
//
VertexArraySingleVertexBuffer::VertexArraySingleVertexBuffer   ( VertexBuffer * vb, VertexDescriptor * vd, unsigned int numVertices, unsigned int vertexBufferOffset )
    : m_vertexBuffer( vb )
    , m_vertexDescriptor( vd )
    , m_numVertices( numVertices )
    , m_vertexBuferOffset( vertexBufferOffset )
{
}

// *******************************
//
VertexArraySingleVertexBuffer::~VertexArraySingleVertexBuffer  ()
{
}

// *******************************
//
const VertexBuffer *        VertexArraySingleVertexBuffer::GetVertexBuffer                 () const
{
    return m_vertexBuffer;
}

// *******************************
//
const VertexDescriptor *    VertexArraySingleVertexBuffer::GetVertexDescriptor             () const
{
    return m_vertexDescriptor;
}

// *******************************
//
unsigned int                VertexArraySingleVertexBuffer::GetNumVertices                  () const
{
    return m_numVertices;
}

// *******************************
//
unsigned int                VertexArraySingleVertexBuffer::GetVertexBufferOffset           () const
{
    return m_vertexBuferOffset;
}

}
