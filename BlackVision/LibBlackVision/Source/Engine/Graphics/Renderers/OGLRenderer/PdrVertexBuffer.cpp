#include "PdrVertexBuffer.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/Resources/VertexBuffer.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrConstants.h"

#include "System/HRTimer.h"


namespace bv {

extern HighResolutionTimer GTimer;

// *******************************
//
PdrVertexBuffer::PdrVertexBuffer     ( Renderer * renderer, const VertexBuffer * vb )
{
    CreateBuffer( vb );
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

    GLvoid * vidMem = glMapBuffer( GL_ARRAY_BUFFER, ConstantsMapper::GLConstant( mlt ) );
    
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
void    PdrVertexBuffer::Update              ( const VertexBuffer * vb )
{
    void * data = Lock( MemoryLockingType::MLT_WRITE_ONLY );
    memcpy( data, vb->Data(), vb->Size() );
    Unlock();
}

// *******************************
//FIXME: it is much better to simply create larger PdrBuffer and use part of it than recreate it all the time (to be implemented - it is not that difficult)
void    PdrVertexBuffer::Recreate            ( const VertexBuffer * vb )
{
    glDeleteBuffers( 1, &m_bufferHandle );
    CreateBuffer( vb );
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
    //FIXME: tutaj w trybie DEBUG (odpalanie przez F5) jest zwiecha co 4096 ramkê, jeœli ta pamiêæ jest co ramkê reuploadowana - czyli co ramkê bufor jest od zera zapisywany na nowo (to jest drastyczny przypadek, ale 
    //testowo takie rzeczy warto sprawdzaæ). Semantic jest ustawione na DYNAMIC_DRAW wiec lepiej sie nie da od tej strony. Problem nie wystepuje, jesli appka jest odpalona przez Ctrl+F5 (czyli Run) i nie jest podpieta
    //do debuggera
    glBufferData( GL_ARRAY_BUFFER, vb->Size(), 0, ConstantsMapper::GLConstant( vb->GetSemantic() ) );

    //GTimer.StartRe();
    //glBufferData( GL_ARRAY_BUFFER, vb->Size(), 0, ConstantsMapper::GLConstant( vb->GetSemantic() ) );
    //GTimer.StopRe();
}

// *******************************
//
void    PdrVertexBuffer::CreateBuffer        ( const VertexBuffer * vb )
{
    //FIXME: Odpalany czesto powoduje przy F5 problemy ze zwiechami co 4096 ramek
    glGenBuffers( 1, &m_bufferHandle );

    Bind();
    BufferData( vb );
    Unbind();

    void * data = Lock( MemoryLockingType::MLT_WRITE_ONLY );
    memcpy( data, vb->Data(), vb->Size() );

    Unlock();
}

}
