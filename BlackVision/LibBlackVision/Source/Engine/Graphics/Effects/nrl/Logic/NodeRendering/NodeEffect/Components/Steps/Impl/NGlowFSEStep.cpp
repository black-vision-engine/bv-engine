#include "stdafx.h"

#include "NGlowFSEStep.h"

#include "Engine/Graphics/Effects/nrl/Logic/Components/NRenderContext.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NNodeRenderLogic.h"
#include "Engine/Graphics/Resources/RenderTarget.h"

#include "Engine/Types/Values/TypedValues.h"

#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"

#include "Engine/Graphics/Effects/nrl/Logic/FullscreenRendering/NFullscreenEffectFactory.h"


namespace bv { namespace nrl {

// **************************
//
NGlowFSEStep::NGlowFSEStep          ()
    : Parent( nullptr )
{
    // FIXME: maybe one class is enough as we only use NFullscreenEffectType type here and always set FSE state as current state
    m_blurEffect = CreateFullscreenEffect( NFullscreenEffectType::NFET_BLUR );
	m_glowEffect = CreateFullscreenEffect( NFullscreenEffectType::NFET_GLOW );

	auto state = std::make_shared< NRenderComponentState >();

	state->AppendValue( m_blurEffect->GetState()->GetValue( "blurSize" ) );
	state->AppendValue( m_blurEffect->GetState()->GetValue( "blurKernelType" ) );
	state->AppendValue( m_blurEffect->GetState()->GetValue( "normalize" ) );
	state->AppendValue( m_glowEffect->GetState()->GetValue( "color" ) );
	state->AppendValue( m_glowEffect->GetState()->GetValue( "inner" ) );
	state->AppendValue( m_glowEffect->GetState()->GetValue( "outer" ) );
	state->AppendValue( m_glowEffect->GetState()->GetValue( "glowStrength" ) );

	Parent::SetState( state );
}

// **************************
//
NGlowFSEStep::~NGlowFSEStep           ()
{
    delete m_blurEffect;
	delete m_glowEffect;
}

// **************************
//
unsigned int            NGlowFSEStep::GetNumRequiredInputs         () const
{
    return 1;
}

// **************************
//
void                    NGlowFSEStep::ReadInputState               ()
{}

// **************************
//
void                    NGlowFSEStep::ApplyImpl                    ( NRenderContext * ctx, const NRenderedData * input )
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

	m_glowEffect->Render( ctx, rd2 );

	allocator( ctx )->Free();
	allocator( ctx )->Free();
}

// **************************
//
void                    NGlowFSEStep::FreeRenderTargets            ( NRenderContext * ctx, const NRenderedData * input )
{
	input;
    assert( input->GetNumEntries() == GetNumRequiredInputs() );
    assert( allocator( ctx )->GetTopIndex() == input->GetLastRenderTargetIdx() );

    allocator( ctx )->Free( GetNumRequiredInputs() );
}

// **************************
//
Float32					NGlowFSEStep::GetBlurSize		() const
{
	auto blurSize = GetState()->GetValueAt( 0 );

	return QueryTypedValue< ValueFloatPtr >( blurSize )->GetValue();
}
// **************************
//
bool NGlowFSEStep::GetInner							() const
{
	auto inner = GetState()->GetValueAt( 5 );

	return QueryTypedValue< ValueBoolPtr >( inner )->GetValue();
}

// **************************
//
bool NGlowFSEStep::GetOuter							() const
{
	auto outer = GetState()->GetValueAt( 6 );

	return QueryTypedValue< ValueBoolPtr >( outer )->GetValue();
}


// **************************
//
bool                    NGlowFSEStep::IsIdle			( SceneNodeRepr * ) const
{
	return  ( GetBlurSize() == 0.f && !GetInner() );
}

// **************************
// If 
bool                    NGlowFSEStep::IsFinal         ( SceneNodeRepr * scnNodeRepr ) const
{
	// if we've just rendered blured image fo not run next default finalize pass.
	// else run it.
	return !IsIdle( scnNodeRepr );
}

} // nrl
} // bv
