#include "stdafx.h"

#include "NBlurPreFSEStep.h"

#include "Engine/Graphics/Effects/nrl/Logic/Components/NRenderContext.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NNodeRenderLogic.h"

#include "Engine/Graphics/Effects/nrl/Logic/State/NRenderComponentState.h"

#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"
#include "Engine/Graphics/Resources/RenderTarget.h"

#include "Engine/Types/Values/ValuesFactory.h"


namespace bv { namespace nrl {

// **************************
//
NBlurPreFSEStep::NBlurPreFSEStep          ( IValuePtr blurSize )
    : Parent( nullptr )
    , m_renderResult( 1 )
{
    auto state = std::make_shared< NRenderComponentState >();

	state->AppendValue( blurSize );

    Parent::SetState( state );
}

// **************************
//
unsigned int            NBlurPreFSEStep::GetNumOutputs               () const
{
    return 1;
}

// **************************
//
void                    NBlurPreFSEStep::ReadInputState              ()
{}

// **************************
//
void                    NBlurPreFSEStep::AllocateRenderTargets       ( NRenderContext * ctx )
{
	if( m_renderResult.GetNumEntries() == 0 || m_renderResult.GetEntry( 0 ) == nullptr )
	{
		auto rt0 = allocator( ctx )->CreateCustomRenderTarget( 5, RenderTarget::RTSemantic::S_DRAW_ONLY );

		m_renderResult.SetLastRenderTargetIdx( 1 );
		m_renderResult.SetEntry( 0, rt0 );
	}
}

// **************************
//
const NRenderedData *   NBlurPreFSEStep::ApplyImpl                   ( SceneNodeRepr * nodeRepr, NRenderContext * ctx )
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
Float32 NBlurPreFSEStep::GetBlurSize				() const
{
	auto blurSize = GetState()->GetValueAt( 0 );

	return QueryTypedValue< ValueFloatPtr >( blurSize )->GetValue();
}

// **************************
//
bool    NBlurPreFSEStep::IsIdle                    ( SceneNodeRepr * ) const
{
	return GetBlurSize() == 0.f;
}

// **************************
// 
bool    NBlurPreFSEStep::IsFinal                   ( SceneNodeRepr * ) const
{
	return false;

}

} // nrl
} // bv
