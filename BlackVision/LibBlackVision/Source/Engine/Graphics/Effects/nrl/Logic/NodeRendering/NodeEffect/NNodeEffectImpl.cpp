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

} // nrl
} // bv
