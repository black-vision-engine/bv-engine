#include "stdafx.h"

#include "NBlurFSEStep.h"

#include "Engine/Graphics/Effects/nrl/Logic/NRenderContext.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NNodeRenderLogic.h"
#include "Engine/Graphics/Resources/RenderTarget.h"

#include "Engine/Types/Values/TypedValues.h"

#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"

#include "Engine/Graphics/Effects/nrl/Logic/FullscreenRendering/NFullscreenEffectFactory.h"


namespace bv { namespace nrl {

// **************************
//
NBlurFSEStep::NBlurFSEStep          ()
    : Parent( nullptr )
{
    // FIXME: maybe one class is enough as we only use NFullscreenEffectType type here and always set FSE state as current state
    m_blurEffect = CreateFullscreenEffect( NFullscreenEffectType::NFET_BLUR );
    SetState( m_blurEffect->GetState() );
}

// **************************
//
NBlurFSEStep::~NBlurFSEStep           ()
{
    delete m_blurEffect;
}

// **************************
//
unsigned int            NBlurFSEStep::GetNumRequiredInputs         () const
{
    return 1;
}

// **************************
//
void                    NBlurFSEStep::ReadInputState               ()
{}

// **************************
//
void                    NBlurFSEStep::ApplyImpl                    ( NRenderContext * ctx, const NRenderedData * input )
{
    assert( input );

	auto textureSize = GetState()->GetValueAt( 0 );
	auto vertical = GetState()->GetValueAt( 3 );

	auto wrt = input->GetEntry( 0 )->Width();
	auto hrt = input->GetEntry( 0 )->Height();

	// Set textureSize param value needed by blur shader.
	QueryTypedValue< ValueVec2Ptr >( textureSize )->SetValue( glm::vec2( wrt, hrt ) );

	// Allocate new render target for vertical blur pass
	auto rt0 = allocator( ctx )->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );
	NRenderedData rd( 1 );
	rd.SetEntry( 0, rt0 );

	// Run vertical blur pass
	QueryTypedValue< ValueBoolPtr >( vertical )->SetValue( true );
	m_blurEffect->Render( ctx, rt0, *input );

	// Run horizontal blur pass
	QueryTypedValue< ValueBoolPtr >( vertical )->SetValue( false );
	m_blurEffect->Render( ctx, rd );
}

// **************************
//
void                    NBlurFSEStep::FreeRenderTargets            ( NRenderContext * ctx, const NRenderedData * input )
{
	input;
    assert( input->GetNumEntries() == GetNumRequiredInputs() );
    assert( allocator( ctx )->GetTopIndex() == input->GetLastRenderTargetIdx() );

    allocator( ctx )->Free( GetNumRequiredInputs() + 1 ); // +1 allocated in NBlurFSEStep::ApplyImpl
}

// **************************
//
Float32					NBlurFSEStep::GetBlurSize				() const
{
	auto blurSize = GetState()->GetValueAt( 1 );

	return QueryTypedValue< ValueFloatPtr >( blurSize )->GetValue();
}

// **************************
//
bool                    NBlurFSEStep::IsIdle                       ( SceneNodeRepr * ) const
{
	// Nothing to do if blur size is 0.
	return GetBlurSize() == 0;
}

// **************************
// If 
bool                    NBlurFSEStep::IsFinal                      ( SceneNodeRepr * ) const
{
	// if we've just rendered blured image fo not run next default finalize pass.
	// else run it.
	return GetBlurSize() != 0;
}

} // nrl
} // bv
