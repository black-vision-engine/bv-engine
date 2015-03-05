#pragma once

#include "BVGL.h"


namespace bv
{

class VertexDescriptor;
class PdrVertexBuffer;
class Renderer;

class PdrVertexDescriptor
{
private:

    const VertexDescriptor *  m_vertexDescriptor;

public:

                PdrVertexDescriptor     ( Renderer * renderer, const VertexDescriptor * vd );
                ~PdrVertexDescriptor    ();

        void    VertexAttribPointer     ( GLuint index, PdrVertexBuffer * vb );
};

}