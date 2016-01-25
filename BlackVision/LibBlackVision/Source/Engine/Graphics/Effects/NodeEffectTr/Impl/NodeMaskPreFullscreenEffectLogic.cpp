#include "NodeMaskPreFullscreenEffectLogic.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"

#include "Engine/Graphics/Effects/Utils/RenderLogicContext.h"

#include "Engine/Types/Values/ValuesFactory.h"


namespace bv {

// *********************************
//
NodeMaskPreFullscreenEffectLogic::NodeMaskPreFullscreenEffectLogic          ( float minAlphaThreshold )
    : m_minAlphaThreshold( minAlphaThreshold )
{
    assert( minAlphaThreshold >= 0.f && minAlphaThreshold <= 1.f );

    m_alphaValue = ValuesFactory::CreateValueFloat( "alpha" );
    m_alphaValue->SetValue( 1.f );

    m_maskIndex = ValuesFactory::CreateValueInt( "maskIdx" );
    m_maskIndex->SetValue( 0 );

    m_foregroundIndex = ValuesFactory::CreateValueInt( "fgIdx" );
    m_foregroundIndex->SetValue( 1 );
}

// *********************************
//
void                        NodeMaskPreFullscreenEffectLogic::Render        ( SceneNode * node, RenderLogicContext * ctx, const std::vector< RenderTarget * > & outputs )
{
    assert( outputs.size() == 2 );

    if( node->NumChildNodes() < 2 )
    {
        logic( ctx )->DrawNode( node, ctx );
    }
    else 
    {
        auto alpha = m_alphaValue->GetValue();
               
        if( alpha < m_minAlphaThreshold )
        {
            logic( ctx )->DrawNodeOnly( renderer( ctx ), node );
            logic( ctx )->RenderChildren( node, ctx, 2 );
        }
        else
        {
            assert( ctx->GetBoundRenderTarget() != nullptr );

            // Render current parent
            logic( ctx )->DrawNodeOnly( renderer( ctx ), node );

            auto mainRT = disableBoundRT( ctx );

            // Get foreground and mask indices
            auto maskIdx = m_maskIndex->GetValue();
            auto fgIdx   = m_foregroundIndex->GetValue();

            assert( maskIdx == 0 || maskIdx == 1 );
            assert( fgIdx == 0 || fgIdx == 1 );
            assert( maskIdx != fgIdx );

            // Render outputs
            RenderToRenderTarget( ctx, outputs[ 0 ], node->GetChild( fgIdx ) );
            RenderToRenderTarget( ctx, outputs[ 1 ], node->GetChild( maskIdx ) );

            enable( ctx, mainRT );
        }
    }
}

// *********************************
//
std::vector< IValuePtr >    NodeMaskPreFullscreenEffectLogic::GetValues     () const
{
    std::vector< IValuePtr > res( 3 );
    res.push_back( m_alphaValue );
    res.push_back( m_maskIndex );
    res.push_back( m_foregroundIndex );

    return res;
}

// *********************************
//
unsigned int                NodeMaskPreFullscreenEffectLogic::GetPreferredNumOutputs              () const
{
    return 2;
}

// *********************************
//
bool                        NodeMaskPreFullscreenEffectLogic::IsFSERequired                       () const
{
    auto alpha = m_alphaValue->GetValue();

    return alpha >= m_minAlphaThreshold;
}

} //bv
