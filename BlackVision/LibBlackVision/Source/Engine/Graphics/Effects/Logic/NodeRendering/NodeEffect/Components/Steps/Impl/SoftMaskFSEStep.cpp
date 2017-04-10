#include "stdafx.h"

#include "SoftMaskFSEStep.h"

#include "Engine/Graphics/Effects/Logic/Components/RenderContext.h"
#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeRenderLogic.h"
#include "Engine/Graphics/Resources/RenderTarget.h"

#include "Engine/Types/Values/TypedValues.h"

#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"

#include "Engine/Graphics/Effects/Logic/FullscreenRendering/FullscreenEffectFactory.h"


namespace bv { 

// **************************
//
SoftMaskFSEStep::SoftMaskFSEStep          ()
    : Parent( nullptr )
{
    // FIXME: maybe one class is enough as we only use FullscreenEffectType type here and always set FSE state as current state
	m_softMaskEffect = CreateFullscreenEffect( FullscreenEffectType::NFET_SOFT_MASK );
    SetState( m_softMaskEffect->GetState() );
}

// **************************
//
SoftMaskFSEStep::~SoftMaskFSEStep           ()
{
    delete m_softMaskEffect;
}

// **************************
//
unsigned int            SoftMaskFSEStep::GetNumRequiredInputs         () const
{
    return 1;
}

// **************************
//
void                    SoftMaskFSEStep::ReadInputState               ()
{}

// **************************
//
void                    SoftMaskFSEStep::ApplyImpl                    ( RenderContext * ctx, const RenderedData * input )
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
void                    SoftMaskFSEStep::FreeRenderTargets            ( RenderContext * ctx, const RenderedData * input )
{
	input;
    assert( input->GetNumEntries() == GetNumRequiredInputs() );
    assert( allocator( ctx )->GetTopIndex() == input->GetLastRenderTargetIdx() );

    allocator( ctx )->Free( GetNumRequiredInputs() );
}

// **************************
//
bool                    SoftMaskFSEStep::IsIdle                       ( SceneNodeRepr * ) const
{
	return false;
}

// **************************
// If - FIXME: "If" what? 
bool                    SoftMaskFSEStep::IsFinal                      ( SceneNodeRepr * ) const
{
	return true;
}

// **************************
//
void                SoftMaskFSEStep::AppendRenderPasses_DIRTY_HACK   ( std::set< const RenderablePass * > * passes ) const
{
    m_softMaskEffect->AppendRenderPasses_DIRTY_HACK( passes );
}


} // bv
