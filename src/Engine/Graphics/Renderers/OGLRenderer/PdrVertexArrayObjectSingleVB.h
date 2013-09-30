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
    
    GLuint                  m_vaoHandle;

public:

            PdrVertexArrayObjectSingleVB    ( Renderer * renderer, const VertexArraySingleVertexBuffer * vao );
            ~PdrVertexArrayObjectSingleVB   ();

    void    Enable                  ( Renderer * renderer );
    void    Disable                 ( Renderer * renderer );

    void    Bind                    ();
    void    Unbind                  ();

    void    EnableVertexAttribArray ( GLuint index );

};

}
