#pragma once

#include "TransformableEntity.h"

namespace bv {

class RenderableArrayDataSingleVertexBuffer;
class RenderableEffect;

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

    RenderableType                          m_type;

    RenderableArrayDataSingleVertexBuffer * m_renderableArrayData;
    RenderableEffect *                      m_effect;


public:
                    //FIXME: effect should be registered via some method so that it can be changed during to some other effect during runtime (RenderableEffect or RenderableEffectInstance if needed)
                    RenderableEntity        ( RenderableType type, RenderableArrayDataSingleVertexBuffer * rad, RenderableEffect * effect );
    virtual         ~RenderableEntity       ();

public:

    RenderableType                                  GetType                     () const;
    const RenderableArrayDataSingleVertexBuffer *   GetRenderableArrayData      () const;
    RenderableEffect *                              GetRenderableEffect         ();

    int                                             GetNumconnectedComponents   () const;

protected:

    const RenderableArrayDataSingleVertexBuffer *   RAD                         () const;

};

}
