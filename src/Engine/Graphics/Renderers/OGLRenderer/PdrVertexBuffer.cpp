#include "PdrVertexBuffer.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "PdrConstants.h"

namespace bv {

// *******************************
//
PdrVertexBuffer::PdrVertexBuffer     ( Renderer * renderer, const VertexBuffer * vb )
{
    glGenBuffers( 1, &m_bufferHandle);

    Bind();
    BufferData( vb );
    Unbind();

    void * data = Lock( MemoryLockingType::MLT_WRITE_ONLY );
    memcpy( data, vb->Data(), vb->Size() );

    Unlock();
}

// *******************************
//
PdrVertexBuffer::~PdrVertexBuffer    ()
{
    glDeleteBuffers( 1, &m_bufferHandle );
}

// *******************************
//
void PdrVertexBuffer::Enable         ( Renderer * renderer )
{
    Bind();
}

// *******************************
//
void PdrVertexBuffer::Disable        ( Renderer * renderer )
{
    Unbind();
}

// *******************************
//
void * PdrVertexBuffer::Lock         ( MemoryLockingType mlt )
{
    Bind();

    GLvoid * vidMem = glMapBuffer( GL_ARRAY_BUFFER, ConstantsMapper::GlConstant( mlt ) );
    
    Unbind();

    return vidMem;
}

// *******************************
//
void PdrVertexBuffer::Unlock         ()
{
    Bind();

    glUnmapBuffer( GL_ARRAY_BUFFER );

    Unbind();
}

// *******************************
//
void    PdrVertexBuffer::Bind                ()
{
    glBindBuffer( GL_ARRAY_BUFFER, m_bufferHandle );
}

// *******************************
//
void    PdrVertexBuffer::Unbind              ()
{
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

// *******************************
//
void    PdrVertexBuffer::BufferData          ( const VertexBuffer * vb ) const
{
    glBufferData( GL_ARRAY_BUFFER, vb->Size(), 0, ConstantsMapper::GlConstant( vb->GetSemantic() ) );
}

}
