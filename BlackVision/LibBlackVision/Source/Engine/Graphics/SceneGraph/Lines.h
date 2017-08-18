#pragma once

#include "RenderableEntity.h"

namespace bv {

class Lines : public RenderableEntity
{
    float       m_width;

public:

    Lines        ( RenderableArrayDataSingleVertexBuffer * rad, RenderableEffectPtr effect, float width = 1.f )
        : RenderableEntity          ( RenderableType::RT_LINES, rad, effect )
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
