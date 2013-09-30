#pragma once

#include <vector>

#include <gl/glew.h>
#include <gl/wglew.h>
#include <gl/gl.h>

namespace bv {

class Renderer;

class VertexArraySingleVertexBuffer;

class PdrVertexBuffer;
class PdrVertexDescriptor;

class PdrVertexArrayObjectSingleVB
{
private:
    
    GLuint                                  m_vaoHandle;
    const VertexArraySingleVertexBuffer *   m_vao;

public:

            PdrVertexArrayObjectSingleVB    ( Renderer * renderer, const VertexArraySingleVertexBuffer * vao );
            ~PdrVertexArrayObjectSingleVB   ();

    void    Enable                  ( Renderer * renderer );
    void    Disable                 ( Renderer * renderer );

    void    Bind                    ( Renderer * renderer );
    void    Unbind                  ( Renderer * renderer );

    void    EnableVertexAttribArray ( GLuint index );

};

}
