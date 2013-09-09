#include "PdrVertexDescriptor.h"
#include "PdrVertexBuffer.h"

#include "Engine/Graphics/Resources/VertexDescriptor.h"

//FIXME: implement
namespace bv {

// **************************
//
PdrVertexDescriptor::PdrVertexDescriptor            ( VertexDescriptor * vd )
    : m_vertexDescriptor( vd )
{
}

// **************************
//
PdrVertexDescriptor::~PdrVertexDescriptor           ()
{
}

// **************************
//
void    PdrVertexDescriptor::VertexAttribPointer     ( GLuint index, PdrVertexBuffer * vb )
{
    vb->Bind();
    //FIXME: implement the right way - with multiple channels and multiple vb objects and different Descriptor signatures
    glVertexAttribPointer( (GLuint)index, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );
    //FIXME: right now it is assumed that vertex descriptor is as above and vertex buffer represents it the same way
}

}
