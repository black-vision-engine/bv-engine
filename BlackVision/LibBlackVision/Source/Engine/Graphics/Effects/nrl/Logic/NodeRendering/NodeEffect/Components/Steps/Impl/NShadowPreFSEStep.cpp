#include "stdafx.h"

#include "NShadowPreFSEStep.h"

#include "Engine/Graphics/Effects/nrl/Logic/Components/NRenderContext.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NNodeRenderLogic.h"

#include "Engine/Graphics/Effects/nrl/Logic/State/NRenderComponentState.h"

#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"
#include "Engine/Graphics/Resources/RenderTarget.h"

#include "Engine/Types/Values/ValuesFactory.h"


namespace bv { namespace nrl {

// **************************
//
NShadowPreFSEStep::NShadowPreFSEStep          ( IValuePtr blurSize, IValuePtr shift, IValuePtr inner, IValuePtr outer )
    : Parent( nullptr )
    , m_renderResult( 1 )
{
    auto state = std::make_shared< NRenderComponentState >();

	state->AppendValue( blurSize );
	state->AppendValue( shift );
	state->AppendValue( inner );
	state->AppendValue( outer );

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
glm::vec2 NShadowPreFSEStep::GetShift				() const
{
	auto shift = GetState()->GetValueAt( 1 );

	return QueryTypedValue< ValueVec2Ptr >( shift )->GetValue();
}

// **************************
//
bool NShadowPreFSEStep::GetInner					() const
{
	auto inner = GetState()->GetValueAt( 2 );

	return QueryTypedValue< ValueBoolPtr >( inner )->GetValue();
}

// **************************
//
bool NShadowPreFSEStep::GetOuter					() const
{
	auto outer = GetState()->GetValueAt( 3 );

	return QueryTypedValue< ValueBoolPtr >( outer )->GetValue();
}


// **************************
//
bool    NShadowPreFSEStep::IsIdle                    ( SceneNodeRepr * ) const
{
	return  ( GetShift() == glm::vec2() && GetBlurSize() == 0.f && !GetInner() );
}

// **************************
// 
bool    NShadowPreFSEStep::IsFinal                   ( SceneNodeRepr * ) const
{
	return false;

}

} // nrl
} // bv
