#include "PdrVertexArrayObjectSingleVB.h"

#include "Engine/Graphics/Resources/VertexArray.h"
#include "Engine/Graphics/Resources/VertexBuffer.h"
#include "Engine/Graphics/Resources/VertexDescriptor.h"

#include "Engine\Graphics\Renderers\Renderer.h"

#include "Renderer.h"

#include "PdrVertexBuffer.h"
#include "PdrVertexDescriptor.h"

#include <cassert>

namespace bv
{

// *******************************
//
PdrVertexArrayObjectSingleVB::PdrVertexArrayObjectSingleVB    ( Renderer * renderer, const VertexArraySingleVertexBuffer * vao )
{
    assert( vao );

    //glGenVertexArrays         ( 1, &m_VaoHandle );
    //glBindVertexArray         ( m_VaoHandle );
    //glBindBuffer              (GL_ARRAY_BUFFER, handle[0]);
    //glVertexAttribPointer     ( (GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );

    glGenVertexArrays( 1, &m_vaoHandle );

    GLuint index = 0;

    Bind();

    const VertexDescriptor * desc = vao->GetVertexDescriptor();
    GLuint numChannels = vao->GetVertexDescriptor()->NumAttrs();

    for( GLuint index = 0; index < numChannels; ++index )
    {
        GLuint location = desc->ChannelLocation( index );
        this->EnableVertexAttribArray( location );
    }

    unsigned int vbOffset       = vao->GetVertexBufferOffset();
    unsigned int numVertices    = vao->GetNumVertices();
    unsigned int stride         = desc->Stride();

    unsigned int locOffset      = 0;

    for( GLuint index = 0; index < numChannels; ++index )
    {
        unsigned int channel        = desc->ChannelLocation( index );
        unsigned int numComponents  = desc->NumComponents( index );
        GLubyte * ptrOffset         = (GLubyte*)(vbOffset + locOffset);

        //FIXME: not general enough: component type and normalized to be implemented
        glVertexAttribPointer( (GLuint) channel, numComponents, GL_FLOAT, GL_FALSE, stride, ptrOffset );

        locOffset += desc->ComponentSize( index ) * numComponents;
    }

    Unbind();
}

// *******************************
//
PdrVertexArrayObjectSingleVB::~PdrVertexArrayObjectSingleVB   ()
{
    glDeleteVertexArrays( 1, &m_vaoHandle );
}

// *******************************
//
void    PdrVertexArrayObjectSingleVB::Enable                  ( Renderer * renderer )
{
    Bind();
}

// *******************************
//
void    PdrVertexArrayObjectSingleVB::Disable                 ( Renderer * renderer )
{
    Unbind();
}

// *******************************
//
void    PdrVertexArrayObjectSingleVB::Bind                    ()
{
    glBindVertexArray( m_vaoHandle );
}

// *******************************
//
void    PdrVertexArrayObjectSingleVB::Unbind                  ()
{
    glBindVertexArray( 0 );
}

// *******************************
//
void    PdrVertexArrayObjectSingleVB::EnableVertexAttribArray ( GLuint index )
{
    glEnableVertexAttribArray( index );
}

} //bv
