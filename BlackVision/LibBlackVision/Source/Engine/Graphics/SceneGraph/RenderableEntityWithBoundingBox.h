#pragma once

#include "RenderableEntity.h"

namespace bv {

namespace mathematics { struct Box; }

class RenderableEntityWithBoundingBox : public RenderableEntity
{
    const mathematics::Box *    m_boundingBox;
public:
                                RenderableEntityWithBoundingBox         ( RenderableType type, RenderableArrayDataSingleVertexBuffer * rad, const mathematics::Box * boundingBox, RenderableEffectPtr effect );

    const mathematics::Box *    GetBoundingBox                          () const;
};

}
