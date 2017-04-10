#include "stdafx.h"

#include "LightScatteringPreFSEStep.h"

#include "Engine/Graphics/Effects/Logic/Components/RenderContext.h"
#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeRenderLogic.h"

#include "Engine/Graphics/Effects/Logic/State/RenderComponentState.h"

#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"
#include "Engine/Graphics/Resources/RenderTarget.h"

#include "Engine/Types/Values/ValuesFactory.h"


namespace bv { namespace nrl {

// **************************
//
LightScatteringPreFSEStep::LightScatteringPreFSEStep          ()
    : Parent( nullptr )
    , m_renderResult( 1 )
{
    auto state = std::make_shared< RenderComponentState >();

    Parent::SetState( state );
}

// **************************
//
unsigned int            LightScatteringPreFSEStep::GetNumOutputs               () const
{
    return 1;
}

// **************************
//
void                    LightScatteringPreFSEStep::ReadInputState              ()
{}

// **************************
//
void                    LightScatteringPreFSEStep::AllocateRenderTargets       ( RenderContext * ctx )
{
    auto rt0 = allocator( ctx )->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );
 
    m_renderResult.SetLastRenderTargetIdx( allocator( ctx )->GetTopIndex() );
    m_renderResult.SetEntry( 0, rt0 );
}

// **************************
//
const RenderedData *   LightScatteringPreFSEStep::ApplyImpl                   ( SceneNodeRepr * nodeRepr, RenderContext * ctx )
{       
    assert( ctx->GetBoundRenderTarget() != nullptr );

    auto mainRT = disableBoundRT( ctx );

    // Render output
	NodeRenderLogic::Render( nodeRepr, m_renderResult.GetEntry( 0 ), ctx );

    enable( ctx, mainRT );

    return &m_renderResult;
}

// **************************
//
bool    LightScatteringPreFSEStep::IsIdle                    ( SceneNodeRepr * ) const
{
	return false;
}

// **************************
// 
bool    LightScatteringPreFSEStep::IsFinal                   ( SceneNodeRepr * ) const
{
	return false;

}

} // nrl
} // bv
