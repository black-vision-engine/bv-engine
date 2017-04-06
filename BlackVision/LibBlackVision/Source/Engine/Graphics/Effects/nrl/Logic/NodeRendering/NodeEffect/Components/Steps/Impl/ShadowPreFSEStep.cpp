#include "stdafx.h"

#include "ShadowPreFSEStep.h"

#include "Engine/Graphics/Effects/nrl/Logic/Components/RenderContext.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeRenderLogic.h"

#include "Engine/Graphics/Effects/nrl/Logic/State/RenderComponentState.h"

#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"
#include "Engine/Graphics/Resources/RenderTarget.h"

#include "Engine/Types/Values/ValuesFactory.h"


namespace bv { namespace nrl {

// **************************
//
ShadowPreFSEStep::ShadowPreFSEStep          ( IValuePtr blurSize, IValuePtr shift, IValuePtr inner, IValuePtr outer )
    : Parent( nullptr )
    , m_renderResult( 1 )
{
    auto state = std::make_shared< RenderComponentState >();

	state->AppendValue( blurSize );
	state->AppendValue( shift );
	state->AppendValue( inner );
	state->AppendValue( outer );

    Parent::SetState( state );
}

// **************************
//
unsigned int            ShadowPreFSEStep::GetNumOutputs               () const
{
    return 1;
}

// **************************
//
void                    ShadowPreFSEStep::ReadInputState              ()
{}

// **************************
//
void                    ShadowPreFSEStep::AllocateRenderTargets       ( RenderContext * ctx )
{
    auto rt0 = allocator( ctx )->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );
 
    m_renderResult.SetLastRenderTargetIdx( allocator( ctx )->GetTopIndex() );
    m_renderResult.SetEntry( 0, rt0 );
}

// **************************
//
const RenderedData *   ShadowPreFSEStep::ApplyImpl                   ( SceneNodeRepr * nodeRepr, RenderContext * ctx )
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
Float32 ShadowPreFSEStep::GetBlurSize				() const
{
	auto blurSize = GetState()->GetValueAt( 0 );

	return QueryTypedValue< ValueFloatPtr >( blurSize )->GetValue();
}

// **************************
//
glm::vec2 ShadowPreFSEStep::GetShift				() const
{
	auto shift = GetState()->GetValueAt( 1 );

	return QueryTypedValue< ValueVec2Ptr >( shift )->GetValue();
}

// **************************
//
bool ShadowPreFSEStep::GetInner					() const
{
	auto inner = GetState()->GetValueAt( 2 );

	return QueryTypedValue< ValueBoolPtr >( inner )->GetValue();
}

// **************************
//
bool ShadowPreFSEStep::GetOuter					() const
{
	auto outer = GetState()->GetValueAt( 3 );

	return QueryTypedValue< ValueBoolPtr >( outer )->GetValue();
}


// **************************
//
bool    ShadowPreFSEStep::IsIdle                    ( SceneNodeRepr * ) const
{
	return  ( GetShift() == glm::vec2() && GetBlurSize() == 0.f && !GetInner() );
}

// **************************
// 
bool    ShadowPreFSEStep::IsFinal                   ( SceneNodeRepr * ) const
{
	return false;

}

} // nrl
} // bv
