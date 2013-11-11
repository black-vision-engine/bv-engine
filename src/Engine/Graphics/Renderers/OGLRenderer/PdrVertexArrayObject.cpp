#include "PdrVertexArrayObject.h"

#include <cassert>

#include "Engine/Graphics/Resources/VertexArray.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrVertexBuffer.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrVertexDescriptor.h"
#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Graphics/Renderers/OGLRenderer/PdrVertexBuffer.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrVertexDescriptor.h"


namespace bv
{

// *******************************
//
PdrVertexArrayEntry::PdrVertexArrayEntry( PdrVertexBuffer * vb, PdrVertexDescriptor * vd )
    : vertexBuffer( vb )
    , vertexDescriptor( vd )
{
}

// *******************************
//
PdrVertexArrayObject::PdrVertexArrayObject    ( Renderer * renderer, const VertexArray * vao )
{
    assert( vao );

    //glGenVertexArrays         ( 1, &m_VaoHandle );
    //glBindVertexArray         ( m_VaoHandle );
    //glBindBuffer              (GL_ARRAY_BUFFER, handle[0]);
    //glVertexAttribPointer     ( (GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );

    glGenVertexArrays( 1, &m_vaoHandle );

    GLuint index = 0;

    Bind();

    for( GLuint index = 0; index < (GLuint) vao->NumEntries(); ++index )
    {
        this->EnableVertexAttribArray( index );
    }

    for( GLuint index = 0; index < (GLuint) vao->NumEntries(); ++index )
    {
        VertexArrayEntry ve = vao->Entry( index );

        PdrVertexBuffer *       pvb = renderer->GetPdrVertexBuffer( ve.vertexBuffer );
        PdrVertexDescriptor *   pvd = renderer->GetPdrVertexDescriptor( ve.vertexDescriptor );

        pvd->VertexAttribPointer( index, pvb );

        m_vaoEntries.push_back( PdrVertexArrayEntry( pvb, pvd ) );
    }
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
