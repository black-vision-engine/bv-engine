#include "stdafx.h"

#include "NNodeMaskFSEStep.h"

#include "Engine/Graphics/Effects/nrl/Logic/Components/NRenderContext.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NNodeRenderLogic.h"
#include "Engine/Graphics/Resources/RenderTarget.h"

#include "Engine/Types/Values/TypedValues.h"

#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"

#include "Engine/Graphics/Effects/nrl/Logic/FullscreenRendering/NFullscreenEffectFactory.h"


namespace bv { namespace nrl {

// **************************
//
NNodeMaskFSEStep::NNodeMaskFSEStep          ( float minAlphaThreshold )
    : Parent( nullptr )
    , m_blitAlphaMaskEffect( nullptr )
    , m_minThreshold( minAlphaThreshold )
    , m_alpha( 0.f )

{
    // FIXME: maybe one class is enough as we only use NFullscreenEffectType type here and always set FSE state as current state
    m_blitAlphaMaskEffect = CreateFullscreenEffect( NFullscreenEffectType::NFET_BLIT_WITH_ALPHA_MASK );
    SetState( m_blitAlphaMaskEffect->GetState() );
}

// **************************
//
NNodeMaskFSEStep::~NNodeMaskFSEStep           ()
{
    delete m_blitAlphaMaskEffect;
}

// **************************
//
unsigned int            NNodeMaskFSEStep::GetNumRequiredInputs         () const
{
    return 2;
}

// **************************
//
void                    NNodeMaskFSEStep::ReadInputState               ()
{
    m_alpha = GetAlpha();
}

// **************************
//
void                    NNodeMaskFSEStep::ApplyImpl                    ( NRenderContext * ctx, const NRenderedData * input )
{
    assert( input );

    // FIXME: add sync here on smewhere else, where it fits
    m_blitAlphaMaskEffect->Render( ctx, *input );
}

// **************************
//
void                    NNodeMaskFSEStep::FreeRenderTargets            ( NRenderContext * ctx, const NRenderedData * input )
{
	input;
    assert( input->GetNumEntries() == GetNumRequiredInputs() );
    assert( allocator( ctx )->GetTopIndex() == input->GetLastRenderTargetIdx() );

    allocator( ctx )->Free( GetNumRequiredInputs() );
}

// **************************
//
bool                    NNodeMaskFSEStep::IsIdle                       ( SceneNodeRepr * nodeRepr ) const
{
    return nodeRepr->NumChildNodes() < 2 || m_alpha < m_minThreshold;
}

// **************************
// If 
bool                    NNodeMaskFSEStep::IsFinal                      ( SceneNodeRepr * nodeRepr ) const
{
    return nodeRepr->NumChildNodes() <= 2 || m_alpha < m_minThreshold;
}

// **************************
//
float                   NNodeMaskFSEStep::GetAlpha                     () const
{
    auto val = GetState()->GetValueAt( 1 );

    assert( val->GetName() == "alpha" );

    return QueryTypedValue< ValueFloatPtr >( val )->GetValue();
}

} // nrl
} // bv
