#pragma once

#include "RenderableEntity.h"

namespace bv {

namespace model { class BoundingVolume; }

class RenderableEntityWithBoundingBox : public RenderableEntity
{
    RenderableEntity *          m_renderableBoundingBox;

public:
                                RenderableEntityWithBoundingBox         ( RenderableType type, RenderableArrayDataSingleVertexBuffer * rad, const model::BoundingVolume * boundingVolume, RenderableEffectPtr effect );

    RenderableEntity *          GetBoundingBox                          () const;
};

}
