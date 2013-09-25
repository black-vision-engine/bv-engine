#include "PdrVertexDescriptor.h"
#include "PdrVertexBuffer.h"

#include "Engine/Graphics/Resources/VertexDescriptor.h"
#include "Engine/Graphics/Renderers/Renderer.h"

//FIXME: implement
namespace bv {

// **************************
//
PdrVertexDescriptor::PdrVertexDescriptor            ( Renderer * renderer, VertexDescriptor * vd )
    : m_vertexDescriptor( vd )
{
}

// **************************
//
PdrVertexDescriptor::~PdrVertexDescriptor           ()
{
}

// **************************
//FIXME: reimplement
void    PdrVertexDescriptor::VertexAttribPointer     ( GLuint index, PdrVertexBuffer * vb )
{
    vb->Bind();

    glVertexAttribPointer( (GLuint)index, m_vertexDescriptor->NumComponents( 0 ), GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );

    //FIXME: implement the right way - with multiple channels and multiple vb objects and different Descriptor signatures
    
    //FIXME: right now it is assumed that vertex descriptor is as above and vertex buffer represents it the same way
}

}
