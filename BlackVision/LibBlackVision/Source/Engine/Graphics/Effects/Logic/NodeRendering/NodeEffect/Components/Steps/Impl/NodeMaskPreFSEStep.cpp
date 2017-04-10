#include "stdafx.h"

#include "NodeMaskPreFSEStep.h"

#include "Engine/Graphics/Effects/Logic/Components/RenderContext.h"
#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeRenderLogic.h"

#include "Engine/Graphics/Effects/Logic/State/RenderComponentState.h"

#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"
#include "Engine/Graphics/Resources/RenderTarget.h"

#include "Engine/Types/Values/ValuesFactory.h"


namespace bv { namespace nrl {

namespace {

const std::string   GAlphaValName   = "alpha";
const std::string   GMaskIdxValName = "maskIdx";
const std::string   GFgIdxValName   = "fgIdx";

const int           GDefaultMaskIdx = 0;
const int           GDefaultFgIdx   = 1;

}

// **************************
//
NodeMaskPreFSEStep::NodeMaskPreFSEStep          ( IValuePtr alphaVal, float minAlphaThreshold )
    : Parent( nullptr )
    , m_renderResult( 2 )
    , m_minThreshold( minAlphaThreshold )
    , m_alpha( 0.f )
    , m_maskIdx( 0 )
    , m_fgIdx( 1 )
{
    assert( alphaVal != nullptr );
    assert( alphaVal->GetType() == ParamType::PT_FLOAT1 );
    assert( alphaVal->GetName() == GAlphaValName );

    auto state = std::make_shared< RenderComponentState >();

    state->AppendValue( alphaVal );
    state->AppendValue( ValuesFactory::CreateValueInt( GMaskIdxValName, GDefaultMaskIdx ) );
    state->AppendValue( ValuesFactory::CreateValueInt( GFgIdxValName, GDefaultFgIdx ) );

    Parent::SetState( state );
}

// **************************
//
unsigned int            NodeMaskPreFSEStep::GetNumOutputs               () const
{
    return 2;
}

// **************************
//
void                    NodeMaskPreFSEStep::ReadInputState              ()
{
    m_alpha     = GetAlpha();
    m_maskIdx   = GetMaskIdx();
    m_fgIdx     = GetFgIdx();

    assert( m_maskIdx == 0 || m_maskIdx == 1 );
    assert( m_fgIdx == 0 || m_fgIdx == 1 );
    assert( m_maskIdx != m_fgIdx );
}

// **************************
//
void                    NodeMaskPreFSEStep::AllocateRenderTargets       ( RenderContext * ctx )
{
    auto rt0 = allocator( ctx )->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );
    auto rt1 = allocator( ctx )->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );

    m_renderResult.SetLastRenderTargetIdx( allocator( ctx )->GetTopIndex() );
    m_renderResult.SetEntry( 0, rt0 );
    m_renderResult.SetEntry( 1, rt1 );
}

// **************************
//
const RenderedData *   NodeMaskPreFSEStep::ApplyImpl                   ( SceneNodeRepr * nodeRepr, RenderContext * ctx )
{       
    const RenderedData * res = nullptr;

    if( nodeRepr->NumChildNodes() < 2 )
    {
        NodeRenderLogic::Render( nodeRepr, ctx );
    }
    else
    {
        if( m_alpha < m_minThreshold )
        {
            NodeRenderLogic::RenderRoot( nodeRepr, ctx );
            NodeRenderLogic::RenderChildren( nodeRepr, ctx, 2 );
        }
        else
        {
            assert( ctx->GetBoundRenderTarget() != nullptr );

            NodeRenderLogic::RenderRoot( nodeRepr, ctx );

            auto mainRT = disableBoundRT( ctx );

            // Render outputs
            NodeRenderLogic::Render( nodeRepr->GetChild( m_fgIdx ), m_renderResult.GetEntry( 0 ), ctx );
            NodeRenderLogic::Render( nodeRepr->GetChild( m_maskIdx ), m_renderResult.GetEntry( 1 ), ctx  );

            enable( ctx, mainRT );

            res = &m_renderResult;
        }
    }

    return res;
}

// **************************
//
bool    NodeMaskPreFSEStep::IsIdle                    ( SceneNodeRepr * nodeRepr ) const
{
    return nodeRepr->NumChildNodes() == 0;
}

// **************************
// If 
bool    NodeMaskPreFSEStep::IsFinal                   ( SceneNodeRepr * nodeRepr ) const
{
    return nodeRepr->NumChildNodes() < 2 || m_alpha < m_minThreshold;

}

// **************************
//
float   NodeMaskPreFSEStep::GetAlpha                  () const
{
    auto val = GetState()->GetValueAt( 0 );

    return QueryTypedValue< ValueFloatPtr >( val )->GetValue();
}

// **************************
//
int     NodeMaskPreFSEStep::GetMaskIdx                 () const
{
    auto val = GetState()->GetValueAt( 1 );

    return QueryTypedValue< ValueIntPtr >( val )->GetValue();
}

// **************************
//
int     NodeMaskPreFSEStep::GetFgIdx                   () const
{
    auto val = GetState()->GetValueAt( 2 );

    return QueryTypedValue< ValueIntPtr >( val )->GetValue();
}

} // nrl
} // bv
