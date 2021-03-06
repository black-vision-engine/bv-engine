#pragma once

#include "BVGL.h"

#include "Engine/Graphics/Renderers/OGLRenderer/PdrConstants.h"
#include "Engine/Graphics/Resources/UniformBuffer.h"


namespace bv {

class Renderer;
class UniformBuffer;

class PdrUniformBufferObject
{
private:

    GLuint                  m_bufferHandle;
    GLuint                  m_blockBindingIdx;
    std::string             m_blockName;

public:

            PdrUniformBufferObject     ( Renderer * renderer, const UniformBuffer * ub, UInt32 bindingBlockIdx );
            ~PdrUniformBufferObject    ();

    void    Enable              ( Renderer * renderer );
    void    Disable             ( Renderer * renderer );

    void *  Lock                ( MemoryLockingType mlt );
    void    Unlock              ();

    void    Update              ( const UniformBuffer * ub );
    void    Recreate            ( const UniformBuffer * ub );

    void    Bind                ();
    void    Unbind              ();

    void    UniformBlockBinding ( UInt32 program ) const;

    static UniformBlockLayout * GetUniformBlockLayout  ( UInt32 program, const std::string & blockName );

private:

    void    BufferData                          ( const UniformBuffer * ub ) const;
    void    CreateBuffer                        ( const UniformBuffer * ub );

};

}