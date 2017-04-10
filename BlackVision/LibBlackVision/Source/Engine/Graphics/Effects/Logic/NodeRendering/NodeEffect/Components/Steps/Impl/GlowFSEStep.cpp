#include "stdafx.h"

#include "GlowFSEStep.h"

#include "Engine/Graphics/Effects/Logic/Components/RenderContext.h"
#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeRenderLogic.h"
#include "Engine/Graphics/Resources/RenderTarget.h"

#include "Engine/Types/Values/TypedValues.h"

#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"

#include "Engine/Graphics/Effects/Logic/FullscreenRendering/FullscreenEffectFactory.h"


namespace bv { 

// **************************
//
GlowFSEStep::GlowFSEStep          ()
    : Parent( nullptr )
{
    // FIXME: maybe one class is enough as we only use FullscreenEffectType type here and always set FSE state as current state
    m_blurEffect = CreateFullscreenEffect( FullscreenEffectType::NFET_BLUR );
	m_glowEffect = CreateFullscreenEffect( FullscreenEffectType::NFET_GLOW );

	auto state = std::make_shared< RenderComponentState >();

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
GlowFSEStep::~GlowFSEStep           ()
{
    delete m_blurEffect;
	delete m_glowEffect;
}

// **************************
//
unsigned int            GlowFSEStep::GetNumRequiredInputs         () const
{
    return 1;
}

// **************************
//
void                    GlowFSEStep::ReadInputState               ()
{}

// **************************
//
void                    GlowFSEStep::ApplyImpl                    ( RenderContext * ctx, const RenderedData * input )
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
	RenderedData rd0( 1 );
	rd0.SetEntry( 0, rt0 );

	// Clear blur output render target.
	enable( ctx, rt0 );
	clearBoundRT( ctx, glm::vec4() );
	disableBoundRT( ctx );

	// Run vertical blur pass
	QueryTypedValue< ValueBoolPtr >( vertical )->SetValue( true );
	m_blurEffect->Render( ctx, rt0, *input );

	auto rt1 = allocator( ctx )->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );
	RenderedData rd1( 1 );
	rd1.SetEntry( 0, rt1 );

	enable( ctx, rt1 );
	clearBoundRT( ctx, glm::vec4() );
	disableBoundRT( ctx );

	// Run horizontal blur pass
	QueryTypedValue< ValueBoolPtr >( vertical )->SetValue( false );
	m_blurEffect->Render( ctx, rt1, rd0 );

	RenderedData rd2( 2 );
	rd2.SetEntry( 0, rt1 );
	rd2.SetEntry( 1, input->GetEntry( 0 ) );

	enable( ctx, mainRT );

	m_glowEffect->Render( ctx, rd2 );

	allocator( ctx )->Free();
	allocator( ctx )->Free();
}

// **************************
//
void                    GlowFSEStep::FreeRenderTargets            ( RenderContext * ctx, const RenderedData * input )
{
	input;
    assert( input->GetNumEntries() == GetNumRequiredInputs() );
    assert( allocator( ctx )->GetTopIndex() == input->GetLastRenderTargetIdx() );

    allocator( ctx )->Free( GetNumRequiredInputs() );
}

// **************************
//
Float32					GlowFSEStep::GetBlurSize		() const
{
	auto blurSize = GetState()->GetValueAt( 0 );

	return QueryTypedValue< ValueFloatPtr >( blurSize )->GetValue();
}
// **************************
//
bool GlowFSEStep::GetInner							() const
{
	auto inner = GetState()->GetValueAt( 5 );

	return QueryTypedValue< ValueBoolPtr >( inner )->GetValue();
}

// **************************
//
bool GlowFSEStep::GetOuter							() const
{
	auto outer = GetState()->GetValueAt( 6 );

	return QueryTypedValue< ValueBoolPtr >( outer )->GetValue();
}


// **************************
//
bool                    GlowFSEStep::IsIdle			( SceneNodeRepr * ) const
{
	return  ( GetBlurSize() == 0.f && !GetInner() );
}

// **************************
// If 
bool                    GlowFSEStep::IsFinal         ( SceneNodeRepr * scnNodeRepr ) const
{
	// if we've just rendered blured image fo not run next default finalize pass.
	// else run it.
	return !IsIdle( scnNodeRepr );
}

// **************************
//
void                GlowFSEStep::AppendRenderPasses_DIRTY_HACK   ( std::set< const RenderablePass * > * passes ) const
{
    m_blurEffect->AppendRenderPasses_DIRTY_HACK( passes );
    m_glowEffect->AppendRenderPasses_DIRTY_HACK( passes );
}


} // bv
