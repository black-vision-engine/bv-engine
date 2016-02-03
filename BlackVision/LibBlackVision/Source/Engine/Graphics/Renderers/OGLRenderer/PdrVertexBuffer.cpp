#include "stdafx.h"

#include "PdrVertexBuffer.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/Resources/VertexBuffer.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrConstants.h"

#include "Tools/HRTimer.h"


namespace bv {

extern HighResolutionTimer GTimer;

// *******************************
//
PdrVertexBuffer::PdrVertexBuffer     ( Renderer * renderer, const VertexBuffer * vb )
{
    { renderer; } // FIXME: suppress unused
    CreateBuffer( vb );
}

// *******************************
//
PdrVertexBuffer::~PdrVertexBuffer    ()
{
    BVGL::bvglDeleteBuffers( 1, &m_bufferHandle );
}

// *******************************
//
void PdrVertexBuffer::Enable         ( Renderer * renderer )
{
    { renderer; } // FIXME: suppress unused
    Bind();
}

// *******************************
//
void PdrVertexBuffer::Disable        ( Renderer * renderer )
{
    { renderer; } // FIXME: suppress unused
    Unbind();
}

// *******************************
//
void * PdrVertexBuffer::Lock         ( MemoryLockingType mlt )
{
    Bind();

    GLvoid * vidMem = BVGL::bvglMapBuffer( GL_ARRAY_BUFFER, ConstantsMapper::GLConstant( mlt ) );
    
    Unbind();

    return vidMem;
}

// *******************************
//
void PdrVertexBuffer::Unlock         ()
{
    Bind();

    BVGL::bvglUnmapBuffer( GL_ARRAY_BUFFER );

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
    BVGL::bvglDeleteBuffers( 1, &m_bufferHandle );
    CreateBuffer( vb );
}

// *******************************
//
void    PdrVertexBuffer::Bind                ()
{
    BVGL::bvglBindBuffer( GL_ARRAY_BUFFER, m_bufferHandle );
}

// *******************************
//
void    PdrVertexBuffer::Unbind              ()
{
    BVGL::bvglBindBuffer( GL_ARRAY_BUFFER, 0 );
}

// *******************************
//
void    PdrVertexBuffer::BufferData          ( const VertexBuffer * vb ) const
{
    //FIXME: tutaj w trybie DEBUG (odpalanie przez F5) jest zwiecha co 4096 ramkê, jeœli ta pamiêæ jest co ramkê reuploadowana - czyli co ramkê bufor jest od zera zapisywany na nowo (to jest drastyczny przypadek, ale 
    //testowo takie rzeczy warto sprawdzaæ). Semantic jest ustawione na DYNAMIC_DRAW wiec lepiej sie nie da od tej strony. Problem nie wystepuje, jesli appka jest odpalona przez Ctrl+F5 (czyli Run) i nie jest podpieta
    //do debuggera
    BVGL::bvglBufferData( GL_ARRAY_BUFFER, vb->Size(), 0, ConstantsMapper::GLConstant( vb->GetSemantic() ) );

    //GTimer.StartRe();
    //glBufferData( GL_ARRAY_BUFFER, vb->Size(), 0, ConstantsMapper::GLConstant( vb->GetSemantic() ) );
    //GTimer.StopRe();
}

// *******************************
//
void    PdrVertexBuffer::CreateBuffer        ( const VertexBuffer * vb )
{
    //FIXME: Odpalany czesto powoduje przy F5 problemy ze zwiechami co 4096 ramek
    BVGL::bvglGenBuffers( 1, &m_bufferHandle );

    Bind();
    BufferData( vb );
    Unbind();

    void * data = Lock( MemoryLockingType::MLT_WRITE_ONLY );
    memcpy( data, vb->Data(), vb->Size() );

    Unlock();
}

}
