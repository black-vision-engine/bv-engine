#pragma once

#include <gl/glew.h>
#include <gl/wglew.h>
#include <gl/gl.h>

namespace bv
{

class VertexDescriptor;
class PdrVertexBuffer;
class Renderer;

class PdrVertexDescriptor
{
private:

    VertexDescriptor *  m_vertexDescriptor;

public:

                PdrVertexDescriptor     ( Renderer * renderer, VertexDescriptor * vd );
                ~PdrVertexDescriptor    ();

        void    VertexAttribPointer     ( GLuint index, PdrVertexBuffer * vb );
};

}