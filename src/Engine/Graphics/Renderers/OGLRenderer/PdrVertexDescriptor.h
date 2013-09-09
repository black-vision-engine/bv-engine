#pragma once

#include <gl/glew.h>
#include <gl/wglew.h>
#include <gl/gl.h>

namespace bv
{

class VertexDescriptor;
class PdrVertexBuffer;

//FIXME: implement
class PdrVertexDescriptor
{
private:

    VertexDescriptor *  m_vertexDescriptor;

public:

                PdrVertexDescriptor     ( VertexDescriptor * vd );
                ~PdrVertexDescriptor    ();

        void    VertexAttribPointer     ( GLuint index, PdrVertexBuffer * vb );
};

}