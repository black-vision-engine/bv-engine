#include "stdafx.h"

#include "ShadowPreFullscreenEffectLogic.h"

#include "Engine/Types/Values/ValuesFactory.h"

#include "PreFullscreenEffectLogicUtils.h"

#include "Engine/Graphics/Resources/RenderTarget.h"

namespace bv {

// *********************************
//
ShadowPreFullscreenEffectLogic::ShadowPreFullscreenEffectLogic              ()
{
    m_textureSize   = ValuesFactory::CreateValueVec2( "textureSize" );
}

// *********************************
//
ShadowPreFullscreenEffectLogic::~ShadowPreFullscreenEffectLogic             ()
{
}

// *********************************
//
void                        ShadowPreFullscreenEffectLogic::RenderImpl    ( SceneNode * node, RenderLogicContext * ctx, std::vector< RenderTarget * > & outputs )
{
    assert( outputs.size() == 1 );
    
    PFLogicUtils::RenderSceneNodeToRenderTarget( node, ctx, outputs[ 0 ] );

    m_textureSize->SetValue( glm::vec2( outputs[ 0 ]->Width(), outputs[ 0 ]->Height() ) );
}

// *********************************
//
std::vector< IValuePtr >    ShadowPreFullscreenEffectLogic::GetValues     () const
{
    std::vector< IValuePtr > res( 1 );
    res[ 0 ] = m_textureSize;

    return res;
}

// *********************************
//
unsigned int                ShadowPreFullscreenEffectLogic::GetPreferredNumOutputs() const
{
    return 1;
}

// *********************************
//
bool                        ShadowPreFullscreenEffectLogic::IsFSERequired () const
{
    return true;
}

} // bv