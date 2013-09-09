#include "PdrVertexArrayObject.h"

#include "Renderer.h"

#include "PdrVertexBuffer.h"
#include "PdrVertexDescriptor.h"

#include <cassert>

namespace bv
{

// *******************************
//
PdrVertexArrayObject::PdrVertexArrayObject    ( PdrVertexBuffer * vb, PdrVertexDescriptor * vd )
    : m_vertexBuffer( vb )
    , m_vertexDescriptor( vd )
{
    assert( vb );
    assert( vd );

    //glGenVertexArrays         ( 1, &m_VaoHandle );
    //glBindVertexArray         ( m_VaoHandle );
    //glBindBuffer              (GL_ARRAY_BUFFER, handle[0]);
    //glVertexAttribPointer     ( (GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );

    glGenVertexArrays( 1, &m_vaoHandle );

    GLuint index = 0;

    this->Bind();
    this->EnableVertexAttribArray( index );
    vd->VertexAttribPointer( index, vb );
}

// *******************************
//
PdrVertexArrayObject::~PdrVertexArrayObject   ()
{
    glDeleteVertexArrays( 1, &m_vaoHandle );
}

// *******************************
//
void    PdrVertexArrayObject::Enable                  ( Renderer * renderer )
{
    Bind();
}

// *******************************
//
void    PdrVertexArrayObject::Disable                 ( Renderer * renderer )
{
    Unbind();
}

// *******************************
//
void    PdrVertexArrayObject::Bind                    ()
{
    glBindVertexArray( m_vaoHandle );
}

// *******************************
//
void    PdrVertexArrayObject::Unbind                  ()
{
    glBindVertexArray( 0 );
}

// *******************************
//
void    PdrVertexArrayObject::EnableVertexAttribArray ( GLuint index )
{
    glEnableVertexAttribArray( index );
}

} //bv
