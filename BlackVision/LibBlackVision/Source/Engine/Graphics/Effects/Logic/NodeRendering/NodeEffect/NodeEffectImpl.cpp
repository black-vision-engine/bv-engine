#include "stdafx.h"

#include "NodeEffectImpl.h"

#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/NodeEffectRenderLogic.h"


namespace bv { 

// *********************************
//
NodeEffectImpl::NodeEffectImpl ( NodeEffectRenderLogic * renderLogic, NodeEffectType type )
    : NodeEffect( type )
    , m_nodeEffectRenderLogic( renderLogic )
{
}

// *********************************
//
NodeEffectImpl::~NodeEffectImpl()
{
    delete m_nodeEffectRenderLogic;
}

// *********************************
//
void        NodeEffectImpl::Render          ( SceneNodeRepr * nodeRepr, RenderContext * ctx )
{
    m_nodeEffectRenderLogic->Render( nodeRepr, ctx );
}

// ***********************
//
void        NodeEffectImpl::RenderDepth     ( SceneNodeRepr * node, RenderContext * ctx )
{
    m_nodeEffectRenderLogic->RenderDepth( node, ctx );
}

// *********************************
//
IValuePtr   NodeEffectImpl::GetValue        ( const std::string & name ) const
{
    return m_nodeEffectRenderLogic->GetValue( name );
}

// ***********************
//
void        NodeEffectImpl::GetRenderPasses_DIRTY_HACK     ( std::set< const RenderablePass * > * passes ) const
{
    m_nodeEffectRenderLogic->GetRenderPasses_DIRTY_HACK( passes );
}

// ***********************
//
bool        NodeEffectImpl::IsBlendable_DIRTY_DESIGN_HACK  () const
{
    return m_nodeEffectRenderLogic->IsBlendable_DIRTY_DESIGN_HACK();
}

// ***********************
//
bool        NodeEffectImpl::IsDepthOverriden_DIRTY_DESIGN_HACK() const
{
    return m_nodeEffectRenderLogic->IsDepthOverriden_DIRTY_DESIGN_HACK();
}

// ***********************
//
float       NodeEffectImpl::GetDepth_DIRTY_DESIGN_HACK() const
{
    return m_nodeEffectRenderLogic->GetDepth_DIRTY_DESIGN_HACK();
}


} // bv
