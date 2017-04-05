#include "stdafx.h"

#include "GlowPreFSEStep.h"

#include "Engine/Graphics/Effects/nrl/Logic/Components/RenderContext.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeRenderLogic.h"

#include "Engine/Graphics/Effects/nrl/Logic/State/RenderComponentState.h"

#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"
#include "Engine/Graphics/Resources/RenderTarget.h"

#include "Engine/Types/Values/ValuesFactory.h"


namespace bv { namespace nrl {

// **************************
//
GlowPreFSEStep::GlowPreFSEStep          ( IValuePtr blurSize, IValuePtr inner, IValuePtr outer )
    : Parent( nullptr )
    , m_renderResult( 1 )
{
    auto state = std::make_shared< RenderComponentState >();

	state->AppendValue( blurSize );
	state->AppendValue( inner );
	state->AppendValue( outer );

    Parent::SetState( state );
}

// **************************
//
unsigned int            GlowPreFSEStep::GetNumOutputs               () const
{
    return 1;
}

// **************************
//
void                    GlowPreFSEStep::ReadInputState              ()
{}

// **************************
//
void                    GlowPreFSEStep::AllocateRenderTargets       ( RenderContext * ctx )
{
    auto rt0 = allocator( ctx )->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );
    m_renderResult.SetLastRenderTargetIdx( allocator( ctx )->GetTopIndex() );
    m_renderResult.SetEntry( 0, rt0 );
}

// **************************
//
const RenderedData *   GlowPreFSEStep::ApplyImpl                   ( SceneNodeRepr * nodeRepr, RenderContext * ctx )
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
Float32 GlowPreFSEStep::GetBlurSize				() const
{
	auto blurSize = GetState()->GetValueAt( 0 );

	return QueryTypedValue< ValueFloatPtr >( blurSize )->GetValue();
}

// **************************
//
bool GlowPreFSEStep::GetInner					() const
{
	auto inner = GetState()->GetValueAt( 1 );

	return QueryTypedValue< ValueBoolPtr >( inner )->GetValue();
}

// **************************
//
bool GlowPreFSEStep::GetOuter					() const
{
	auto outer = GetState()->GetValueAt( 2 );

	return QueryTypedValue< ValueBoolPtr >( outer )->GetValue();
}


// **************************
//
bool    GlowPreFSEStep::IsIdle                    ( SceneNodeRepr * ) const
{
	return  ( GetBlurSize() == 0.f && !GetInner() );
}

// **************************
// 
bool    GlowPreFSEStep::IsFinal                   ( SceneNodeRepr * ) const
{
	return false;

}

} // nrl
} // bv
