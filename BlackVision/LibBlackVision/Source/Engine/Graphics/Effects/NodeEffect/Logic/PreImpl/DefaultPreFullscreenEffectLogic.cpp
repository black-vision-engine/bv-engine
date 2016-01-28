#include "DefaultPreFullscreenEffectLogic.h"

#include "Engine/Graphics/Effects/Utils/RenderLogicContext.h"


namespace bv {

// *********************************
//
void                        DefaultPreFullscreenEffectLogic::RenderImpl                         ( SceneNode * node, RenderLogicContext * ctx, std::vector< RenderTarget * > & outputs )
{
    { outputs; }
    assert( outputs.size() == 0 );
    assert( ctx->GetBoundRenderTarget() != nullptr );

    logic( ctx )->DrawNode( node, ctx );
}

// *********************************
//
std::vector< IValuePtr >    DefaultPreFullscreenEffectLogic::GetValues                           () const
{
    return std::vector< IValuePtr >();
}

// *********************************
//
unsigned int                DefaultPreFullscreenEffectLogic::GetPreferredNumOutputs              () const
{
    return 0;
}

// *********************************
//
bool                        DefaultPreFullscreenEffectLogic::IsFSERequired                       () const
{
    return true;
}

} //bv
