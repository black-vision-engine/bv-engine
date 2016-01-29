#include "stdafx.h"

#include "RenderableEntity.h"

#include "Engine/Graphics/Resources/RenderableArrayData.h"
#include "Engine/Graphics/Shaders/RenderableEffect.h"


namespace bv {

// *********************************
//
RenderableEntity::RenderableEntity          ( RenderableType type, RenderableArrayDataSingleVertexBuffer * rad, RenderableEffectPtr effect )
    : m_type( type )
    , m_renderableArrayData( rad )
    , m_effect( effect )
{
}

// *********************************
//
RenderableEntity::~RenderableEntity                                                         ()
{
    delete m_renderableArrayData;
}

// *********************************
//
RenderableEntity::RenderableType RenderableEntity::GetType                                  () const
{
    return m_type;
}

// *********************************
//
const RenderableArrayDataSingleVertexBuffer *  RenderableEntity::GetRenderableArrayData     () const
{
    return m_renderableArrayData;
}

// *********************************
//
RenderableArrayDataSingleVertexBuffer *        RenderableEntity::GetRenderableArrayData     ()
{
    return m_renderableArrayData;
}

// *********************************
//
RenderableEffectPtr             RenderableEntity::GetRenderableEffect                       ()
{
    return m_effect;
}

// *********************************
//
void                            RenderableEntity::SetRenderableEffect                       ( RenderableEffectPtr effect )
{
    m_effect = effect;
}

// *********************************
//
SizeType                        RenderableEntity::GetNumconnectedComponents                 () const
{
    return m_renderableArrayData->GetNumConnectedComponents();
}

// *********************************
//
const RenderableArrayDataSingleVertexBuffer *     RenderableEntity::RAD                     () const
{
    return m_renderableArrayData;
}

} //bv
