#include "stdafx.h"

#include "NAlphaMaskPreFSEStep.h"

#include "Engine/Graphics/Effects/nrl/Logic/Components/NRenderContext.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NNodeRenderLogic.h"
#include "Engine/Graphics/Resources/RenderTarget.h"

#include "Engine/Types/Values/TypedValues.h"


namespace bv { namespace nrl {

// **************************
//
NAlphaMaskPreFSEStep::NAlphaMaskPreFSEStep          ( NRenderComponentStatePtr initialState, float minAlphaThreshold, float maxAlphaThreshold )
    : Parent( initialState )
    , m_renderResult( 1 )
    , m_minThreshold( minAlphaThreshold )
    , m_maxThreshold( maxAlphaThreshold )
    , m_alpha( 0.f )
{
}

// **************************
//
unsigned int            NAlphaMaskPreFSEStep::GetNumOutputs               () const
{
    return 1;
}

// **************************
//
void                    NAlphaMaskPreFSEStep::ReadInputState              ()
{
    m_alpha = GetAlpha();
}

// **************************
//
void                    NAlphaMaskPreFSEStep::AllocateRenderTargets       ( NRenderContext * ctx )
{
    auto rt = allocator( ctx )->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );

    m_renderResult.SetLastRenderTargetIdx( allocator( ctx )->GetTopIndex() );
    m_renderResult.SetEntry( 0, rt );
}

// **************************
//
const NRenderedData *   NAlphaMaskPreFSEStep::ApplyImpl                   ( SceneNodeRepr * nodeRepr, NRenderContext * ctx )
{
    // Should be called only if these two conditions are met
    assert( m_alpha >= m_minThreshold && m_alpha <= m_maxThreshold );

    auto mainRT = disableBoundRT( ctx );
    auto outRT = m_renderResult.GetEntry( 0 );

    NNodeRenderLogic::Render( nodeRepr, outRT, ctx );

    enable( ctx, mainRT );

    return &m_renderResult;
}

// **************************
//
bool    NAlphaMaskPreFSEStep::IsIdle                    ( SceneNodeRepr * nodeRepr ) const
{
    { nodeRepr; }

    return m_alpha > m_maxThreshold || m_alpha < m_minThreshold;
}

// **************************
// If 
bool    NAlphaMaskPreFSEStep::IsFinal                   ( SceneNodeRepr * nodeRepr ) const
{
    { nodeRepr; }

    return m_alpha < m_minThreshold;
}

// **************************
//
float   NAlphaMaskPreFSEStep::GetAlpha                  () const
{
    auto val = GetState()->GetValueAt( 0 );

    assert( val->GetName() == "alpha" );

    return QueryTypedValue< ValueFloatPtr >( val )->GetValue();
}

} // nrl
} // bv
