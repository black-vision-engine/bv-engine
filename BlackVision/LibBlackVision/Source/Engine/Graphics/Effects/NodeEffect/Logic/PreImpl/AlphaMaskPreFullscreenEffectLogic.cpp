#include "stdafx.h"

#include "AlphaMaskPreFullscreenEffectLogic.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"

#include "Engine/Graphics/Effects/Utils/RenderLogicContext.h"

#include "Engine/Types/Values/ValuesFactory.h"




#include "Memory/MemoryLeaks.h"



namespace bv {

// *********************************
//
AlphaMaskPreFullscreenEffectLogic::AlphaMaskPreFullscreenEffectLogic    ( float minAlphaThreshold, float maxAlphaThreshold )
    : m_minAlphaThreshold( minAlphaThreshold )
    , m_maxAlphaThreshold( maxAlphaThreshold )
{
    assert( minAlphaThreshold >= 0.f && minAlphaThreshold < maxAlphaThreshold && minAlphaThreshold <= 1.f );
    assert( maxAlphaThreshold > 0.f && maxAlphaThreshold <= 1.f );

    m_alphaValue = ValuesFactory::CreateValueFloat( "alpha" );
    m_alphaValue->SetValue( 1.f );
}

// *********************************
//
void    AlphaMaskPreFullscreenEffectLogic::RenderImpl                   ( SceneNode * node, RenderLogicContext * ctx, std::vector< RenderTarget * > & outputs )
{
    assert( outputs.size() == 1 );

    auto alpha = m_alphaValue->GetValue();
    
    if( alpha > m_maxAlphaThreshold )       // No alpha mask in this case
    {
        logic( ctx )->DrawNode( node, ctx );
    }
    else if( alpha > m_minAlphaThreshold )  // Alpha mask rendering to a separate render target
    {
        auto mainRT = disableBoundRT( ctx );
        auto outRT  = outputs[ 0 ];

        enable( ctx, outRT );
        clearBoundRT( ctx, glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );

        logic( ctx )->DrawNode( node, ctx );

        disableBoundRT( ctx );

        enable( ctx, mainRT );
    }

    // No rendering in case of alpha too close to zero (treat it as a fully transparent object)
}

// *********************************
//
std::vector< IValuePtr >    AlphaMaskPreFullscreenEffectLogic::GetValues                () const
{
    std::vector< IValuePtr > res( 1 );
    res[ 0 ] = m_alphaValue;

    return res;
}

// *********************************
//
unsigned int                AlphaMaskPreFullscreenEffectLogic::GetPreferredNumOutputs   () const
{
    return 1;
}

// *********************************
//
bool                        AlphaMaskPreFullscreenEffectLogic::IsFSERequired            () const
{
    return IsApplicable();
}

// *********************************
//
bool                        AlphaMaskPreFullscreenEffectLogic::IsApplicable             () const
{
    auto v = m_alphaValue->GetValue();

    return  v > m_minAlphaThreshold && v <= m_maxAlphaThreshold;
}

} //bv
