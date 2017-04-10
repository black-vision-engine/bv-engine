#include "stdafx.h"

#include "BlurPreFSEStep.h"

#include "Engine/Graphics/Effects/Logic/Components/RenderContext.h"
#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeRenderLogic.h"

#include "Engine/Graphics/Effects/Logic/State/RenderComponentState.h"

#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"
#include "Engine/Graphics/Resources/RenderTarget.h"

#include "Engine/Types/Values/ValuesFactory.h"


namespace bv { namespace nrl {

// **************************
//
BlurPreFSEStep::BlurPreFSEStep          ( IValuePtr blurSize )
    : Parent( nullptr )
    , m_renderResult( 1 )
{
    auto state = std::make_shared< RenderComponentState >();

	state->AppendValue( blurSize );

    Parent::SetState( state );
}

// **************************
//
unsigned int            BlurPreFSEStep::GetNumOutputs               () const
{
    return 1;
}

// **************************
//
void                    BlurPreFSEStep::ReadInputState              ()
{}

// **************************
//
void                    BlurPreFSEStep::AllocateRenderTargets       ( RenderContext * ctx )
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
const RenderedData *   BlurPreFSEStep::ApplyImpl                   ( SceneNodeRepr * nodeRepr, RenderContext * ctx )
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
Float32 BlurPreFSEStep::GetBlurSize				() const
{
	auto blurSize = GetState()->GetValueAt( 0 );

	return QueryTypedValue< ValueFloatPtr >( blurSize )->GetValue();
}

// **************************
//
bool    BlurPreFSEStep::IsIdle                    ( SceneNodeRepr * ) const
{
	return GetBlurSize() == 0.f;
}

// **************************
// 
bool    BlurPreFSEStep::IsFinal                   ( SceneNodeRepr * ) const
{
	return false;

}

} // nrl
} // bv
