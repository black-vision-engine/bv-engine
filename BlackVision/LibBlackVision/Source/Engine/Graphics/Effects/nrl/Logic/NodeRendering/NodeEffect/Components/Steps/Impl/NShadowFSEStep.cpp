#include "stdafx.h"

#include "NShadowFSEStep.h"

#include "Engine/Graphics/Effects/nrl/Logic/Components/NRenderContext.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NNodeRenderLogic.h"
#include "Engine/Graphics/Resources/RenderTarget.h"

#include "Engine/Types/Values/TypedValues.h"

#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"

#include "Engine/Graphics/Effects/nrl/Logic/FullscreenRendering/NFullscreenEffectFactory.h"


namespace bv { namespace nrl {

// **************************
//
NShadowFSEStep::NShadowFSEStep          ()
    : Parent( nullptr )
{
    // FIXME: maybe one class is enough as we only use NFullscreenEffectType type here and always set FSE state as current state
    m_blurEffect = CreateFullscreenEffect( NFullscreenEffectType::NFET_BLUR );
	m_shadowEffect = CreateFullscreenEffect( NFullscreenEffectType::NFET_SHADOW );

	auto state = std::make_shared< NRenderComponentState >();

	state->AppendValue( m_blurEffect->GetState()->GetValue( "blurSize" ) );
	state->AppendValue( m_blurEffect->GetState()->GetValue( "blurKernelType" ) );
	state->AppendValue( m_blurEffect->GetState()->GetValue( "normalize" ) );
	state->AppendValue( m_shadowEffect->GetState()->GetValue( "color" ) );
	state->AppendValue( m_shadowEffect->GetState()->GetValue( "shift" ) );
	state->AppendValue( m_shadowEffect->GetState()->GetValue( "inner" ) );
	state->AppendValue( m_shadowEffect->GetState()->GetValue( "outer" ) );

	Parent::SetState( state );
}

// **************************
//
NShadowFSEStep::~NShadowFSEStep           ()
{
    delete m_blurEffect;
	delete m_shadowEffect;
}

// **************************
//
unsigned int            NShadowFSEStep::GetNumRequiredInputs         () const
{
    return 1;
}

// **************************
//
void                    NShadowFSEStep::ReadInputState               ()
{}

// **************************
//
void                    NShadowFSEStep::ApplyImpl                    ( NRenderContext * ctx, const NRenderedData * input )
{
    assert( input );

	// FIXME: Some general mechanism should be implemented to set values in effect.
	auto textureSize = m_blurEffect->GetState()->GetValueAt( 0 );
	auto vertical = m_blurEffect->GetState()->GetValueAt( 3 );

	auto wrt = input->GetEntry( 0 )->Width();
	auto hrt = input->GetEntry( 0 )->Height();

	// Set textureSize param value needed by blur shader.
	QueryTypedValue< ValueVec2Ptr >( textureSize )->SetValue( glm::vec2( wrt, hrt ) );

	auto mainRT = disableBoundRT( ctx );

	// Allocate new render target for vertical blur pass
	auto rt0 = allocator( ctx )->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );
	NRenderedData rd0( 1 );
	rd0.SetEntry( 0, rt0 );

	// Clear blur output render target.
	enable( ctx, rt0 );
	clearBoundRT( ctx, glm::vec4() );
	disableBoundRT( ctx );

	// Run vertical blur pass
	QueryTypedValue< ValueBoolPtr >( vertical )->SetValue( true );
	m_blurEffect->Render( ctx, rt0, *input );

	auto rt1 = allocator( ctx )->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );
	NRenderedData rd1( 1 );
	rd1.SetEntry( 0, rt1 );

	// Clear blur output render target.
	enable( ctx, rt1 );
	clearBoundRT( ctx, glm::vec4() );
	disableBoundRT( ctx );

	// Run horizontal blur pass
	QueryTypedValue< ValueBoolPtr >( vertical )->SetValue( false );
	m_blurEffect->Render( ctx, rt1, rd0 );

	NRenderedData rd2( 2 );
	rd2.SetEntry( 0, rt1 );
	rd2.SetEntry( 1, input->GetEntry( 0 ) );

	enable( ctx, mainRT );

	m_shadowEffect->Render( ctx, rd2 );

	allocator( ctx )->Free();
	allocator( ctx )->Free();
}

// **************************
//
void                    NShadowFSEStep::FreeRenderTargets            ( NRenderContext * ctx, const NRenderedData * input )
{
	input;
    assert( input->GetNumEntries() == GetNumRequiredInputs() );
    assert( allocator( ctx )->GetTopIndex() == input->GetLastRenderTargetIdx() );

    allocator( ctx )->Free( GetNumRequiredInputs() );
}

// **************************
//
Float32					NShadowFSEStep::GetBlurSize		() const
{
	auto blurSize = GetState()->GetValueAt( 0 );

	return QueryTypedValue< ValueFloatPtr >( blurSize )->GetValue();
}

// **************************
//
glm::vec2 NShadowFSEStep::GetShift						() const
{
	auto shift = GetState()->GetValueAt( 4 );

	return QueryTypedValue< ValueVec2Ptr >( shift )->GetValue();
}

// **************************
//
bool NShadowFSEStep::GetInner							() const
{
	auto inner = GetState()->GetValueAt( 5 );

	return QueryTypedValue< ValueBoolPtr >( inner )->GetValue();
}

// **************************
//
bool NShadowFSEStep::GetOuter							() const
{
	auto outer = GetState()->GetValueAt( 6 );

	return QueryTypedValue< ValueBoolPtr >( outer )->GetValue();
}


// **************************
//
bool                    NShadowFSEStep::IsIdle			( SceneNodeRepr * ) const
{
	return  ( GetShift() == glm::vec2() && GetBlurSize() == 0.f && !GetInner() );
}

// **************************
// If 
bool                    NShadowFSEStep::IsFinal         ( SceneNodeRepr * scnNodeRepr ) const
{
	// if we've just rendered blured image fo not run next default finalize pass.
	// else run it.
	return !IsIdle( scnNodeRepr );
}

} // nrl
} // bv
