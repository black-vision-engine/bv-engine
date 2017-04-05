#include "stdafx.h"

#include "NAlphaMaskFSEStep.h"

#include "Engine/Graphics/Effects/nrl/Logic/Components/NRenderContext.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NNodeRenderLogic.h"
#include "Engine/Graphics/Resources/RenderTarget.h"

#include "Engine/Types/Values/TypedValues.h"

#include "Engine/Graphics/Effects/nrl/Logic/FullscreenRendering/NFullscreenEffectFactory.h"


namespace bv { namespace nrl {

namespace {

const std::string   GAlphaValName = "alpha";

}

// **************************
//
NAlphaMaskFSEStep::NAlphaMaskFSEStep          ( float minAlphaThreshold, float maxAlphaThreshold )
    : Parent( nullptr )
    , m_blitWithAlphaEffect( nullptr )
    , m_minThreshold( minAlphaThreshold )
    , m_maxThreshold( maxAlphaThreshold )
    , m_alpha( 0.f )

{
    // FIXME: maybe one class is enough as we only use NFullscreenEffectType type here and always set FSE state as current state
    m_blitWithAlphaEffect = CreateFullscreenEffect( NFullscreenEffectType::NFET_BLIT_WITH_ALPHA );
    SetState( m_blitWithAlphaEffect->GetState() );
}

// **************************
//
NAlphaMaskFSEStep::~NAlphaMaskFSEStep           ()
{
    delete m_blitWithAlphaEffect;
}

// **************************
//
unsigned int            NAlphaMaskFSEStep::GetNumRequiredInputs         () const
{
    return 1;
}

// **************************
//
void                    NAlphaMaskFSEStep::ReadInputState               ()
{
    m_alpha = GetAlpha();
}

// **************************
//
void                    NAlphaMaskFSEStep::ApplyImpl                    ( NRenderContext * ctx, const NRenderedData * input )
{
    assert( input );

    // FIXME: add sync here on smewhere else, where it fits
    m_blitWithAlphaEffect->Render( ctx, *input );
}

// **************************
//
void                    NAlphaMaskFSEStep::FreeRenderTargets            ( NRenderContext * ctx, const NRenderedData * input )
{
	input;
    assert( input->GetNumEntries() == GetNumRequiredInputs() );
    assert( allocator( ctx )->GetTopIndex() == input->GetLastRenderTargetIdx() );

    allocator( ctx )->Free( GetNumRequiredInputs() );
}

// **************************
//
bool                    NAlphaMaskFSEStep::IsIdle                       ( SceneNodeRepr * nodeRepr ) const
{
    { nodeRepr; }

    return m_alpha > m_maxThreshold || m_alpha < m_minThreshold;
}

// **************************
// If 
bool                    NAlphaMaskFSEStep::IsFinal                      ( SceneNodeRepr * nodeRepr ) const
{
    { nodeRepr; }

    return m_alpha <= m_maxThreshold;
}

// **************************
//
void                    NAlphaMaskFSEStep::AppendRenderPasses_DIRTY_HACK   ( std::set< const RenderablePass * > * passes ) const
{
    m_blitWithAlphaEffect->AppendRenderPasses_DIRTY_HACK( passes );
}

// **************************
//
float                   NAlphaMaskFSEStep::GetAlpha                     () const
{
    auto val = GetState()->GetValueAt( 0 );

    assert( val->GetName() == GAlphaValName );

    return QueryTypedValue< ValueFloatPtr >( val )->GetValue();
}

} // nrl
} // bv
