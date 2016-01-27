#include "RenderOffscreenPreFullscreenEffectLogic.h"

#include "Engine/Graphics/Effects/Utils/RenderLogicContext.h"


namespace bv {

// *********************************
//
void                        RenderOffscreenPreFullscreenEffectLogic::Render     ( SceneNode * node, RenderLogicContext * ctx, const std::vector< RenderTarget * > & outputs )
{
    assert( ctx->GetBoundRenderTarget() != nullptr );
    assert( outputs.size() == 1 );

    auto mainTarget = disableBoundRT( ctx );
    enable( ctx, outputs[ 0 ] );
    clearBoundRT( ctx, glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );

    logic( ctx )->DrawNode( node, ctx );

    disableBoundRT( ctx );
    enable( ctx, mainTarget );
}

// *********************************
//
std::vector< IValuePtr >    RenderOffscreenPreFullscreenEffectLogic::GetValues  () const
{
    return std::vector< IValuePtr >();
}

// *********************************
//
unsigned int                RenderOffscreenPreFullscreenEffectLogic::GetPreferredNumOutputs     () const
{
    return 1;
}

// *********************************
//
bool                        RenderOffscreenPreFullscreenEffectLogic::IsFSERequired              () const
{
    return true;
}

} //bv
