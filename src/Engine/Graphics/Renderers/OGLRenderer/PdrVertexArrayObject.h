#pragma once

#include <vector>

#include <gl/glew.h>
#include <gl/wglew.h>
#include <gl/gl.h>

namespace bv {

class Renderer;

class VertexArray;

class PdrVertexBuffer;
class PdrVertexDescriptor;

struct PdrVertexArrayEntry
{
    PdrVertexArrayEntry( PdrVertexBuffer * vb, PdrVertexDescriptor * vd );

    PdrVertexBuffer *      vertexBuffer;
    PdrVertexDescriptor *  vertexDescriptor;
};

class PdrVertexArrayObject
{
private:
    
    std::vector< PdrVertexArrayEntry > m_vaoEntries;

    GLuint                  m_vaoHandle;

public:

            PdrVertexArrayObject    ( Renderer * renderer, VertexArray * vao );
            ~PdrVertexArrayObject   ();

    void    Enable                  ( Renderer * renderer );
    void    Disable                 ( Renderer * renderer );

    void    Bind                    ();
    void    Unbind                  ();

    void    EnableVertexAttribArray ( GLuint index );

};

}