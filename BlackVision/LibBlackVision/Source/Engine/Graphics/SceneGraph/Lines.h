#pragma once

#include "RenderableEntity.h"

namespace bv {

class Lines : public RenderableEntity
{
public:

    Lines        ( RenderableArrayDataSingleVertexBuffer * rad, RenderableEffectPtr effect )
        : RenderableEntity          ( RenderableType::RT_LINES, rad, effect )
    {
    }

};

}
