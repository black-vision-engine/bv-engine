#include "stdafx.h"

#include "LightScatteringPreFullscreenEffectLogic.h"

#include "Engine/Graphics/Effects/Utils/RenderLogicContext.h"

#include "Engine/Types/Values/ValuesFactory.h"

#include "PreFullscreenEffectLogicUtils.h"



#include "Memory/MemoryLeaks.h"



namespace bv {

// *********************************
//
LightScatteringPreFullscreenEffectLogic::LightScatteringPreFullscreenEffectLogic              ()
{
}

// *********************************
//
LightScatteringPreFullscreenEffectLogic::~LightScatteringPreFullscreenEffectLogic             ()
{
}

// *********************************
//
void                        LightScatteringPreFullscreenEffectLogic::RenderImpl    ( SceneNode * node, RenderLogicContext * ctx, std::vector< RenderTarget * > & outputs )
{
    assert( outputs.size() == 1 );
    
    PFLogicUtils::RenderSceneNodeToRenderTarget( node, ctx, outputs[ 0 ] );
}

// *********************************
//
std::vector< IValuePtr >    LightScatteringPreFullscreenEffectLogic::GetValues     () const
{
    return std::vector< IValuePtr >();
}

// *********************************
//
unsigned int                LightScatteringPreFullscreenEffectLogic::GetPreferredNumOutputs() const
{
    return 1;
}

// *********************************
//
bool                        LightScatteringPreFullscreenEffectLogic::IsFSERequired () const
{
    return true;
}

} // bv