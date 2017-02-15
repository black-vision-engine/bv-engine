#include "stdafx.h"

#include "VertexArray.h"

#include <cassert>

#include "Engine/Graphics/Resources/VertexDescriptor.h"
#include "Engine/Graphics/Resources/VertexBuffer.h"




#include "Memory/MemoryLeaks.h"



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
SizeType                VertexArray::NumEntries          () const
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
VertexArrayEntry        VertexArray::Entry               ( unsigned int i ) const
{
    assert( i >= 0 );
    assert( i < (unsigned int) NumEntries() );

    return m_vaoEntries[ i ];
}


// *************************************  VertexArraySingleVertexBuffer *********************************************

// *******************************
//
VertexArraySingleVertexBuffer::VertexArraySingleVertexBuffer   ( VertexBuffer * vb, VertexDescriptor * vd )
    : m_vertexBuffer( vb )
    , m_vertexDescriptor( vd )
    , m_needsUpdateMemUpload( false )
    , m_needsUpdateRecreation( false )
    , m_numTotalVertices( 0 )
{
}

// *******************************
//
VertexArraySingleVertexBuffer::~VertexArraySingleVertexBuffer  ()
{
    delete m_vertexBuffer;
    delete m_vertexDescriptor;
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
VertexBuffer *              VertexArraySingleVertexBuffer::GetVertexBuffer                 ()
{
    return m_vertexBuffer;
}

// *******************************
//
VertexDescriptor *          VertexArraySingleVertexBuffer::GetVertexDescriptor             ()
{
    return m_vertexDescriptor;
}

// *******************************
//
void         VertexArraySingleVertexBuffer::SetVertexDescriptor             ( VertexDescriptor * desc )
{
    if( m_vertexDescriptor )
    {
        delete m_vertexDescriptor;
    }
    m_vertexDescriptor = desc;
}


// *******************************
//
void                        VertexArraySingleVertexBuffer::AddCCEntry                      ( unsigned int numVertices )
{
    m_ccVertexNum.push_back( numVertices );
    m_numTotalVertices += numVertices;
}

// *******************************
//
SizeType                    VertexArraySingleVertexBuffer::GetNumConnectedComponents       () const
{
    return m_ccVertexNum.size();
}

// *******************************
//
unsigned int                VertexArraySingleVertexBuffer::GetNumVertices                  ( unsigned int ccNum ) const
{
    assert( ccNum < m_ccVertexNum.size() );
    return m_ccVertexNum[ ccNum ];
}

// *******************************
//
unsigned int                VertexArraySingleVertexBuffer::GetNumTotalVertices              () const
{
    return m_numTotalVertices;
}

// *******************************
//
void                        VertexArraySingleVertexBuffer::SetNeedsUpdateMemUpload         ( bool needsUpdateMemUpload ) const
{
    m_needsUpdateMemUpload = needsUpdateMemUpload;
}

// *******************************
//
void                        VertexArraySingleVertexBuffer::SetNeedsUpdateRecreation        ( bool needsUpdateRecreation ) const
{
    m_needsUpdateRecreation = needsUpdateRecreation;
}

// *******************************
//
bool                        VertexArraySingleVertexBuffer::NeedsUpdateMemUpload            () const
{
    return m_needsUpdateMemUpload;
}

// *******************************
//
bool                        VertexArraySingleVertexBuffer::NeedsUpdateRecreation           () const
{
    return m_needsUpdateRecreation;
}

// *******************************
//
void                        VertexArraySingleVertexBuffer::ResetState                      ()
{
    m_numTotalVertices = 0;
    m_needsUpdateMemUpload = false;
    m_needsUpdateRecreation = false;
    m_ccVertexNum.clear();
}

}
