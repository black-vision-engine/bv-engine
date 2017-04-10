#include "stdafx.h"

#include "NodeMaskFSEStep.h"

#include "Engine/Graphics/Effects/Logic/Components/RenderContext.h"
#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeRenderLogic.h"
#include "Engine/Graphics/Resources/RenderTarget.h"

#include "Engine/Types/Values/TypedValues.h"

#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"

#include "Engine/Graphics/Effects/Logic/FullscreenRendering/FullscreenEffectFactory.h"


namespace bv { namespace nrl {

// **************************
//
NodeMaskFSEStep::NodeMaskFSEStep          ( float minAlphaThreshold )
    : Parent( nullptr )
    , m_blitAlphaMaskEffect( nullptr )
    , m_minThreshold( minAlphaThreshold )
    , m_alpha( 0.f )

{
    // FIXME: maybe one class is enough as we only use FullscreenEffectType type here and always set FSE state as current state
    m_blitAlphaMaskEffect = CreateFullscreenEffect( FullscreenEffectType::NFET_BLIT_WITH_ALPHA_MASK );
    SetState( m_blitAlphaMaskEffect->GetState() );
}

// **************************
//
NodeMaskFSEStep::~NodeMaskFSEStep           ()
{
    delete m_blitAlphaMaskEffect;
}

// **************************
//
unsigned int            NodeMaskFSEStep::GetNumRequiredInputs         () const
{
    return 2;
}

// **************************
//
void                    NodeMaskFSEStep::ReadInputState               ()
{
    m_alpha = GetAlpha();
}

// **************************
//
void                    NodeMaskFSEStep::ApplyImpl                    ( RenderContext * ctx, const RenderedData * input )
{
    assert( input );

    // FIXME: add sync here on smewhere else, where it fits
    m_blitAlphaMaskEffect->Render( ctx, *input );
}

// **************************
//
void                    NodeMaskFSEStep::FreeRenderTargets            ( RenderContext * ctx, const RenderedData * input )
{
	input;
    assert( input->GetNumEntries() == GetNumRequiredInputs() );
    assert( allocator( ctx )->GetTopIndex() == input->GetLastRenderTargetIdx() );

    allocator( ctx )->Free( GetNumRequiredInputs() );
}

// **************************
//
bool                    NodeMaskFSEStep::IsIdle                       ( SceneNodeRepr * nodeRepr ) const
{
    return nodeRepr->NumChildNodes() < 2 || m_alpha < m_minThreshold;
}

// **************************
// If 
bool                    NodeMaskFSEStep::IsFinal                      ( SceneNodeRepr * nodeRepr ) const
{
    return nodeRepr->NumChildNodes() <= 2 || m_alpha < m_minThreshold;
}

// **************************
//
void                NodeMaskFSEStep::AppendRenderPasses_DIRTY_HACK   ( std::set< const RenderablePass * > * passes ) const
{
    m_blitAlphaMaskEffect->AppendRenderPasses_DIRTY_HACK( passes );
}

// **************************
//
float                   NodeMaskFSEStep::GetAlpha                     () const
{
    auto val = GetState()->GetValueAt( 1 );

    assert( val->GetName() == "alpha" );

    return QueryTypedValue< ValueFloatPtr >( val )->GetValue();
}

} // nrl
} // bv
