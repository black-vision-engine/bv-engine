#pragma once

#include "TransformableEntity.h"

namespace bv {

class VertexDescriptor;
class VertexBuffer;
class IndexBuffer;
class RenderableEffect;
class VertexArray;

class RenderableEntity : public TransformableEntity
{
public:

    enum class RenderableType : int
    {
        RT_TRIANGLES = 0,
        RT_TRIANGLE_STRIP,
        RT_TRIANGLE_MESH,

        RT_TOTAL
    };

protected:

    RenderableType      m_type;

    VertexDescriptor *  m_vDesc;
    VertexBuffer *      m_vBuf;
    IndexBuffer *       m_iBuf;
    VertexArray *       m_vao;

    RenderableEffect *  m_effect;

public:
                    //FIXME: effect should be registered via some method so that it can be changed during to some other effect during runtime (RenderableEffect or RenderableEffectInstance if needed)
                    RenderableEntity        ( RenderableType type, VertexDescriptor * vd, VertexBuffer * vb, IndexBuffer * ib, RenderableEffect * effect );
    virtual         ~RenderableEntity       ();

public:

    RenderableType          GetType                    () const;
    VertexDescriptor *      GetVertexDescriptor        () { return m_vDesc; }
    VertexBuffer *          GetVertexBuffer            () { return m_vBuf; }
    IndexBuffer *           GetIndexBuffer             () { return m_iBuf; }
    RenderableEffect *      GetRenderableEffect        () { return m_effect; }
    VertexArray *           GetVertexArray             () { return m_vao; }
};

}
