#pragma once

#include "RenderableEntity.h"

namespace bv {

namespace model { class BoundingVolume; }

class RenderableEntityWithBoundingBox : public RenderableEntity
{
    RenderableEntityUPtr        m_renderableBoundingBox;
    RenderableEntityUPtr        m_renderableCenterOfMass;

public:
                                RenderableEntityWithBoundingBox         ( RenderableType type, RenderableArrayDataSingleVertexBuffer * rad, const model::BoundingVolume * boundingVolume, RenderableEffectPtr effect );

    RenderableEntity *          GetBoundingBox                          () const;
    RenderableEntity *          GetCenterOfMass                         () const;

    virtual void                SetLocalTransform                       ( const Transform & t );
    virtual void                UpdateTransform                         ( const Transform & transform );
};

}
