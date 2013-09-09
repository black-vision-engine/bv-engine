#pragma once

#include <gl/glew.h>
#include <gl/wglew.h>
#include <gl/gl.h>

namespace bv {

class Renderer;

class PdrVertexBuffer;
class PdrVertexDescriptor;

class PdrVertexArrayObject
{
private:
    
    PdrVertexBuffer *       m_vertexBuffer;
    PdrVertexDescriptor *   m_vertexDescriptor;

    GLuint                  m_vaoHandle;

public:

            PdrVertexArrayObject    ( PdrVertexBuffer * vb, PdrVertexDescriptor * vd );
            ~PdrVertexArrayObject   ();

    void    Enable                  ( Renderer * renderer );
    void    Disable                 ( Renderer * renderer );

    void    Bind                    ();
    void    Unbind                  ();

    void    EnableVertexAttribArray ( GLuint index );

};

}