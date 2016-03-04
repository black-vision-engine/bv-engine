#include "stdafx.h"

#include "ShadowPreFullscreenEffectLogic.h"

#include "PreFullscreenEffectLogicUtils.h"

namespace bv {

// *********************************
//
ShadowPreFullscreenEffectLogic::ShadowPreFullscreenEffectLogic              ()
{
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
}

// *********************************
//
std::vector< IValuePtr >    ShadowPreFullscreenEffectLogic::GetValues     () const
{
    return std::vector< IValuePtr >();
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