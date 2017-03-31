#include "stdafx.h"

#include "NNodeEffectImpl.h"

#include "Engine/Graphics/Effects/nrl/Logic\NodeRendering/NodeEffect/NNodeEffectRenderLogic.h"


namespace bv { namespace nrl {

// *********************************
//
NNodeEffectImpl::NNodeEffectImpl ( NNodeEffectRenderLogic * renderLogic, NNodeEffectType type )
    : NNodeEffect( type )
    , m_nodeEffectRenderLogic( renderLogic )
{
}

// *********************************
//
NNodeEffectImpl::~NNodeEffectImpl()
{
    delete m_nodeEffectRenderLogic;
}

// *********************************
//
void        NNodeEffectImpl::Render          ( SceneNodeRepr * nodeRepr, NRenderContext * ctx )
{
    m_nodeEffectRenderLogic->Render( nodeRepr, ctx );
}

// *********************************
//
IValuePtr   NNodeEffectImpl::GetValue        ( const std::string & name ) const
{
    return m_nodeEffectRenderLogic->GetValue( name );
}

// ***********************
//
void        NNodeEffectImpl::GetRenderPasses_DIRTY_HACK     ( std::set< const RenderablePass * > * passes ) const
{
    m_nodeEffectRenderLogic->GetRenderPasses_DIRTY_HACK( passes );
}

// ***********************
//
bool        NNodeEffectImpl::IsBlendable_DIRTY_DESIGN_HACK  () const
{
    return m_nodeEffectRenderLogic->IsBlendable_DIRTY_DESIGN_HACK();
}

// ***********************
//
bool        NNodeEffectImpl::IsDepthOverriden_DIRTY_DESIGN_HACK() const
{
    return m_nodeEffectRenderLogic->IsDepthOverriden_DIRTY_DESIGN_HACK();
}

// ***********************
//
float       NNodeEffectImpl::GetDepth_DIRTY_DESIGN_HACK() const
{
    return m_nodeEffectRenderLogic->GetDepth_DIRTY_DESIGN_HACK();
}

} // nrl
} // bv
