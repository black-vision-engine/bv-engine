#include "stdafx.h"

#include "NSoftMaskFSEStep.h"

#include "Engine/Graphics/Effects/nrl/Logic/NRenderContext.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NNodeRenderLogic.h"
#include "Engine/Graphics/Resources/RenderTarget.h"

#include "Engine/Types/Values/TypedValues.h"

#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"

#include "Engine/Graphics/Effects/nrl/Logic/FullscreenRendering/NFullscreenEffectFactory.h"


namespace bv { namespace nrl {

// **************************
//
NSoftMaskFSEStep::NSoftMaskFSEStep          ()
    : Parent( nullptr )
{
    // FIXME: maybe one class is enough as we only use NFullscreenEffectType type here and always set FSE state as current state
	m_softMaskEffect = CreateFullscreenEffect( NFullscreenEffectType::NFET_SOFT_MASK );
    SetState( m_softMaskEffect->GetState() );
}

// **************************
//
NSoftMaskFSEStep::~NSoftMaskFSEStep           ()
{
    delete m_softMaskEffect;
}

// **************************
//
unsigned int            NSoftMaskFSEStep::GetNumRequiredInputs         () const
{
    return 1;
}

// **************************
//
void                    NSoftMaskFSEStep::ReadInputState               ()
{}

// **************************
//
void                    NSoftMaskFSEStep::ApplyImpl                    ( NRenderContext * ctx, const NRenderedData * input )
{
	assert( input );
	ctx;

	//// FIXME: Some general mechanism should be implemented to set values in effect.
	auto aspectRatioVal = GetState()->GetValueAt( 0 );

	auto wrt = input->GetEntry( 0 )->Width();
	auto hrt = input->GetEntry( 0 )->Height();

	//// Set textureSize param value needed by blur shader.
	QueryTypedValue< ValueFloatPtr >( aspectRatioVal )->SetValue( Float32( wrt ) / Float32( hrt ) );

	m_softMaskEffect->Render( ctx, *input );
}

// **************************
//
void                    NSoftMaskFSEStep::FreeRenderTargets            ( NRenderContext * ctx, const NRenderedData * input )
{
	input;
    assert( input->GetNumEntries() == GetNumRequiredInputs() );
    assert( allocator( ctx )->GetTopIndex() == input->GetLastRenderTargetIdx() );

    allocator( ctx )->Free( GetNumRequiredInputs() );
}

// **************************
//
bool                    NSoftMaskFSEStep::IsIdle                       ( SceneNodeRepr * ) const
{
	return false;
}

// **************************
// If 
bool                    NSoftMaskFSEStep::IsFinal                      ( SceneNodeRepr * ) const
{
	return true;
}

} // nrl
} // bv
