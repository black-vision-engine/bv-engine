#pragma once

#include <gl/glew.h>
#include <gl/wglew.h>
#include <gl/gl.h>

#include "PdrConstants.h"

namespace bv {

class Renderer;
class VertexBuffer;

class PdrVertexBuffer
{
private:

    GLuint m_bufferHandle;

public:

            PdrVertexBuffer     ( Renderer * renderer, const VertexBuffer * vb );
            ~PdrVertexBuffer    ();

    void    Enable              ( Renderer * renderer );
    void    Disable             ( Renderer * renderer );

    void *  Lock                ( MemoryLockingType mlt );
    void    Unlock              ();


    void    Bind                ();
    void    Unbind              ();

private:

    void    BufferData          ( const VertexBuffer * vb ) const;

};

}