#include "DefaultPreFullscreenEffectLogic.h"

#include "Engine/Graphics/Effects/Utils/RenderLogicContext.h"


namespace bv {

// *********************************
//
void                        DefaultPreFullscreenEffectLogic::Render                              ( SceneNode * node, RenderLogicContext * ctx, const std::vector< RenderTarget * > & outputs )
{
    { outputs; }
    assert( ctx->GetBoundRenderTarget() != nullptr );
    assert( outputs.size() == 0 );

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
