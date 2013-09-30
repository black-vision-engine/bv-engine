#include "VertexArray.h"
#include <cassert>

namespace bv
{

// *******************************
//
VertexArrayEntry::VertexArrayEntry( VertexBuffer * vb, VertexDescriptor * vd, unsigned int vboffset )
    : vertexBuffer( vb )
    , vertexDescriptor( vd )
    , vertexBufferOffset( vboffset )
{
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
void                    VertexArray::AddEntry            ( VertexBuffer * vb, VertexDescriptor * vd, unsigned int vertexBufferOffset )
{
    m_vaoEntries.push_back( VertexArrayEntry( vb, vd ,vertexBufferOffset ) );
}

// *******************************
//
VertexArrayEntry        VertexArray::Entry               ( int i ) const
{
    assert( i >= 0 );
    assert( i < NumEntries() );

    return m_vaoEntries[ i ];
}

}
