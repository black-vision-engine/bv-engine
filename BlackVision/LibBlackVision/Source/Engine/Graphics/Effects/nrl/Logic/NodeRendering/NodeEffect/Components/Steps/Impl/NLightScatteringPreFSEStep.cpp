#include "stdafx.h"

#include "NLightScatteringPreFSEStep.h"

#include "Engine/Graphics/Effects/nrl/Logic/Components/NRenderContext.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NNodeRenderLogic.h"

#include "Engine/Graphics/Effects/nrl/Logic/State/NRenderComponentState.h"

#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"
#include "Engine/Graphics/Resources/RenderTarget.h"

#include "Engine/Types/Values/ValuesFactory.h"


namespace bv { namespace nrl {

// **************************
//
NLightScatteringPreFSEStep::NLightScatteringPreFSEStep          ()
    : Parent( nullptr )
    , m_renderResult( 1 )
{
    auto state = std::make_shared< NRenderComponentState >();

    Parent::SetState( state );
}

// **************************
//
unsigned int            NLightScatteringPreFSEStep::GetNumOutputs               () const
{
    return 1;
}

// **************************
//
void                    NLightScatteringPreFSEStep::ReadInputState              ()
{}

// **************************
//
void                    NLightScatteringPreFSEStep::AllocateRenderTargets       ( NRenderContext * ctx )
{
    auto rt0 = allocator( ctx )->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );
 
    m_renderResult.SetLastRenderTargetIdx( allocator( ctx )->GetTopIndex() );
    m_renderResult.SetEntry( 0, rt0 );
}

// **************************
//
const NRenderedData *   NLightScatteringPreFSEStep::ApplyImpl                   ( SceneNodeRepr * nodeRepr, NRenderContext * ctx )
{       
    assert( ctx->GetBoundRenderTarget() != nullptr );

    auto mainRT = disableBoundRT( ctx );

    // Render output
	NNodeRenderLogic::Render( nodeRepr, m_renderResult.GetEntry( 0 ), ctx );

    enable( ctx, mainRT );

    return &m_renderResult;
}

// **************************
//
bool    NLightScatteringPreFSEStep::IsIdle                    ( SceneNodeRepr * ) const
{
	return false;
}

// **************************
// 
bool    NLightScatteringPreFSEStep::IsFinal                   ( SceneNodeRepr * ) const
{
	return false;

}

} // nrl
} // bv
