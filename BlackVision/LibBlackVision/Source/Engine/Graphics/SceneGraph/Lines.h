#pragma once

#include "RenderableEntityWithBoundingBox.h"

namespace bv {

class Lines : public RenderableEntityWithBoundingBox
{
    float       m_width;

public:

    Lines        ( RenderableArrayDataSingleVertexBuffer * rad, const model::BoundingVolume * boundingBox, RenderableEffectPtr effect, float width = 1.f )
        : RenderableEntityWithBoundingBox          ( RenderableType::RT_LINES, rad, boundingBox, effect )
        , m_width( width )
    {
    }

    float           GetWidth()
    {
        return m_width;
    }

    void            SetWidth( float width )
    {
        m_width = width;
    }

};

}
