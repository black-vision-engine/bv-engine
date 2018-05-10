#include "stdafx.h"

#include "AlphaMaskFSEStep.h"

#include "Engine/Graphics/Effects/Logic/Components/RenderContext.h"
#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeRenderLogic.h"
#include "Engine/Graphics/Resources/RenderTarget.h"

#include "Engine/Types/Values/TypedValues.h"

#include "Engine/Graphics/Effects/Logic/FullscreenRendering/FullscreenEffectFactory.h"


namespace bv { 

namespace {

const std::string   GAlphaValName = "alpha";

}

// **************************
//
AlphaMaskFSEStep::AlphaMaskFSEStep          ( float minAlphaThreshold, float maxAlphaThreshold )
    : Parent( nullptr )
    , m_blitWithAlphaEffect( nullptr )
    , m_minThreshold( minAlphaThreshold )
    , m_maxThreshold( maxAlphaThreshold )
    , m_alpha( 0.f )

{
    // FIXME: maybe one class is enough as we only use FullscreenEffectType type here and always set FSE state as current state
    m_blitWithAlphaEffect = CreateFullscreenEffect( FullscreenEffectType::NFET_BLIT_WITH_ALPHA );
    SetState( m_blitWithAlphaEffect->GetState() );
}

// **************************
//
AlphaMaskFSEStep::~AlphaMaskFSEStep           ()
{
    delete m_blitWithAlphaEffect;
}

// **************************
//
unsigned int            AlphaMaskFSEStep::GetNumRequiredInputs         () const
{
    return 1;
}

// **************************
//
void                    AlphaMaskFSEStep::ReadInputState               ()
{
    m_alpha = GetAlpha();
}

// **************************
//
void                    AlphaMaskFSEStep::ApplyImpl                    ( RenderContext * ctx, const RenderedData * input )
{
//    assert( input ); // commented out due to FullscreenEffectPass::Apply behaviour, perhaphs one of those should be fixed
    if( input == nullptr )
        return;

    // FIXME: add sync here on smewhere else, where it fits
    m_blitWithAlphaEffect->Render( ctx, *input );
}

// **************************
//
void                    AlphaMaskFSEStep::FreeRenderTargets            ( RenderContext * ctx, const RenderedData * input )
{
	input;
    assert( input->GetNumEntries() == GetNumRequiredInputs() );
    assert( allocator( ctx )->GetTopIndex() == input->GetLastRenderTargetIdx() );

    allocator( ctx )->Free( GetNumRequiredInputs() );
}

// **************************
//
bool                    AlphaMaskFSEStep::IsIdle                       ( SceneNodeRepr * nodeRepr ) const
{
    { nodeRepr; }

    return m_alpha > m_maxThreshold || m_alpha < m_minThreshold;
}

// **************************
// If 
bool                    AlphaMaskFSEStep::IsFinal                      ( SceneNodeRepr * nodeRepr ) const
{
    { nodeRepr; }

    return m_alpha <= m_maxThreshold;
}

// **************************
//
void                    AlphaMaskFSEStep::AppendRenderPasses_DIRTY_HACK   ( std::set< const RenderablePass * > * passes ) const
{
    m_blitWithAlphaEffect->AppendRenderPasses_DIRTY_HACK( passes );
}

// **************************
//
float                   AlphaMaskFSEStep::GetAlpha                     () const
{
    auto val = GetState()->GetValueAt( 0 );

    assert( val->GetName() == GAlphaValName );

    return QueryTypedValue< ValueFloatPtr >( val )->GetValue();
}


} // bv
