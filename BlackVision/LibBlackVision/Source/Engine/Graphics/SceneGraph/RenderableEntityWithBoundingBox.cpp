#include "stdafx.h"

#include "RenderableEntityWithBoundingBox.h"

namespace bv {


// ***********************
//
RenderableEntityWithBoundingBox::RenderableEntityWithBoundingBox         ( RenderableType type, RenderableArrayDataSingleVertexBuffer * rad, const mathematics::Box * boundingBox, RenderableEffectPtr effect )
    : RenderableEntity( type, rad, effect )
    , m_boundingBox( boundingBox )
{
}


// ***********************
//
const mathematics::Box *    RenderableEntityWithBoundingBox::GetBoundingBox                          () const
{
    return m_boundingBox;
}


}
