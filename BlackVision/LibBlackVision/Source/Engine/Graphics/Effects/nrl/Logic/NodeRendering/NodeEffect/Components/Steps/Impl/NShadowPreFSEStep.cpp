#include "stdafx.h"

#include "NShadowPreFSEStep.h"

#include "Engine/Graphics/Effects/nrl/Logic/NRenderContext.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NNodeRenderLogic.h"

#include "Engine/Graphics/Effects/nrl/Logic/State/NRenderComponentState.h"

#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"
#include "Engine/Graphics/Resources/RenderTarget.h"

#include "Engine/Types/Values/ValuesFactory.h"


namespace bv { namespace nrl {

// **************************
//
NShadowPreFSEStep::NShadowPreFSEStep          ( IValuePtr blurSize )
    : Parent( nullptr )
    , m_renderResult( 1 )
{
    auto state = std::make_shared< NRenderComponentState >();

	state->AppendValue( blurSize );

    Parent::SetState( state );
}

// **************************
//
unsigned int            NShadowPreFSEStep::GetNumOutputs               () const
{
    return 1;
}

// **************************
//
void                    NShadowPreFSEStep::ReadInputState              ()
{}

// **************************
//
void                    NShadowPreFSEStep::AllocateRenderTargets       ( NRenderContext * ctx )
{
    auto rt0 = allocator( ctx )->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );
 
    m_renderResult.SetLastRenderTargetIdx( allocator( ctx )->GetTopIndex() );
    m_renderResult.SetEntry( 0, rt0 );
}

// **************************
//
const NRenderedData *   NShadowPreFSEStep::ApplyImpl                   ( SceneNodeRepr * nodeRepr, NRenderContext * ctx )
{       
    assert( ctx->GetBoundRenderTarget() != nullptr );

    NNodeRenderLogic::RenderRoot( nodeRepr, ctx );

    auto mainRT = disableBoundRT( ctx );

    // Render output
	NNodeRenderLogic::Render( nodeRepr, m_renderResult.GetEntry( 0 ), ctx );

    enable( ctx, mainRT );

    return &m_renderResult;
}

// **************************
//
Float32 NShadowPreFSEStep::GetBlurSize				() const
{
	auto blurSize = GetState()->GetValueAt( 0 );

	return QueryTypedValue< ValueFloatPtr >( blurSize )->GetValue();
}

// **************************
//
bool    NShadowPreFSEStep::IsIdle                    ( SceneNodeRepr * ) const
{
	return GetBlurSize() == 0.f;
}

// **************************
// 
bool    NShadowPreFSEStep::IsFinal                   ( SceneNodeRepr * ) const
{
	return false;

}

} // nrl
} // bv
