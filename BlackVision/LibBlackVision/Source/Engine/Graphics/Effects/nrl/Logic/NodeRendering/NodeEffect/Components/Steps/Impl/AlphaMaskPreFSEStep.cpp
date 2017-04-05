#include "stdafx.h"

#include "AlphaMaskPreFSEStep.h"

#include "Engine/Graphics/Effects/nrl/Logic/Components/RenderContext.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeRenderLogic.h"
#include "Engine/Graphics/Resources/RenderTarget.h"

#include "Engine/Types/Values/TypedValues.h"


namespace bv { namespace nrl {

// **************************
//
AlphaMaskPreFSEStep::AlphaMaskPreFSEStep          ( RenderComponentStatePtr initialState, float minAlphaThreshold, float maxAlphaThreshold )
    : Parent( initialState )
    , m_renderResult( 1 )
    , m_minThreshold( minAlphaThreshold )
    , m_maxThreshold( maxAlphaThreshold )
    , m_alpha( 0.f )
{
}

// **************************
//
unsigned int            AlphaMaskPreFSEStep::GetNumOutputs               () const
{
    return 1;
}

// **************************
//
void                    AlphaMaskPreFSEStep::ReadInputState              ()
{
    m_alpha = GetAlpha();
}

// **************************
//
void                    AlphaMaskPreFSEStep::AllocateRenderTargets       ( RenderContext * ctx )
{
    auto rt = allocator( ctx )->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );

    m_renderResult.SetLastRenderTargetIdx( allocator( ctx )->GetTopIndex() );
    m_renderResult.SetEntry( 0, rt );
}

// **************************
//
const RenderedData *   AlphaMaskPreFSEStep::ApplyImpl                   ( SceneNodeRepr * nodeRepr, RenderContext * ctx )
{
    // Should be called only if these two conditions are met
    assert( m_alpha >= m_minThreshold && m_alpha <= m_maxThreshold );

    auto mainRT = disableBoundRT( ctx );
    auto outRT = m_renderResult.GetEntry( 0 );

    NodeRenderLogic::Render( nodeRepr, outRT, ctx );

    enable( ctx, mainRT );

    return &m_renderResult;
}

// **************************
//
bool    AlphaMaskPreFSEStep::IsIdle                    ( SceneNodeRepr * nodeRepr ) const
{
    { nodeRepr; }

    return m_alpha > m_maxThreshold || m_alpha < m_minThreshold;
}

// **************************
// If 
bool    AlphaMaskPreFSEStep::IsFinal                   ( SceneNodeRepr * nodeRepr ) const
{
    { nodeRepr; }

    return m_alpha < m_minThreshold;
}

// **************************
//
float   AlphaMaskPreFSEStep::GetAlpha                  () const
{
    auto val = GetState()->GetValueAt( 0 );

    assert( val->GetName() == "alpha" );

    return QueryTypedValue< ValueFloatPtr >( val )->GetValue();
}

} // nrl
} // bv
