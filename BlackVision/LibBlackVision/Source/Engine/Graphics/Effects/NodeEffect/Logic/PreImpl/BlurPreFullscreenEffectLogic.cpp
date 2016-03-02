#include "stdafx.h"

#include "BlurPreFullscreenEffectLogic.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"

#include "Engine/Graphics/Effects/Utils/RenderLogicContext.h"

#include "Engine/Types/Values/ValuesFactory.h"

namespace bv {

// *********************************
//
BlurPreFullscreenEffectLogic::BlurPreFullscreenEffectLogic              ()
{
    m_blurSize      = ValuesFactory::CreateValueFloat( "blurSize", 0.f );
    m_textureSize   = ValuesFactory::CreateValueVec2( "textureSize" );
}

// *********************************
//
BlurPreFullscreenEffectLogic::~BlurPreFullscreenEffectLogic             ()
{
}

// *********************************
//
void                        BlurPreFullscreenEffectLogic::RenderImpl    ( SceneNode * node, RenderLogicContext * ctx, std::vector< RenderTarget * > & outputs )
{
    assert( outputs.size() == 1 );
    
    auto mainRT = disableBoundRT( ctx );
    auto outRT  = outputs[ 0 ];

    enable( ctx, outRT );
    clearBoundRT( ctx, glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );

    logic( ctx )->DrawNode( node, ctx );

    m_textureSize->SetValue( glm::vec2( outRT->Width(), outRT->Height() ) );

    disableBoundRT( ctx );

    enable( ctx, mainRT );
}

// *********************************
//
std::vector< IValuePtr >    BlurPreFullscreenEffectLogic::GetValues     () const
{
    std::vector< IValuePtr > res( 2 );

    res[ 0 ] = m_blurSize;
    res[ 1 ] = m_textureSize;

    return res;
}

// *********************************
//
unsigned int                BlurPreFullscreenEffectLogic::GetPreferredNumOutputs() const
{
    return 1;
}

// *********************************
//
bool                        BlurPreFullscreenEffectLogic::IsFSERequired () const
{
    auto bs = m_blurSize->GetValue();

    return bs > 0;
}

} // bv