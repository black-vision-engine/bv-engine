#include "RenderableEntity.h"

#include "Engine\Graphics\Resources\RenderableArrayData.h"
#include "Engine\Graphics\Shaders\RenderableEffect.h"

namespace bv {

// *********************************
//
RenderableEntity::RenderableEntity          ( RenderableType type, RenderableArrayData * rad, RenderableEffect * effect )
    : m_type( type )
    , m_renderableArrayData( rad )
    , m_effect( effect )
{
}

// *********************************
//
RenderableEntity::~RenderableEntity         ()
{
}

// *********************************
//
RenderableEntity::RenderableType RenderableEntity::GetType                      () const
{
    return m_type;
}

// *********************************
//
const RenderableArrayData *     RenderableEntity::GetRenderableArrayData        () const
{
    return m_renderableArrayData;
}

// *********************************
//
RenderableEffect *              RenderableEntity::GetRenderableEffect           ()
{
    return m_effect;
}

// *********************************
//
int                             RenderableEntity::GetNumconnectedComponents     () const
{
    return m_renderableArrayData->GetNumConnectedComponents();
}

// *********************************
//
const RenderableArrayData *     RenderableEntity::RAD                         () const
{
    return m_renderableArrayData;
}

}
