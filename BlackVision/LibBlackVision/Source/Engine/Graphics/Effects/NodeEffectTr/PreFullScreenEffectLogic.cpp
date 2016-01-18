#include "PreFullScreenEffectLogic.h"

#include "Rendering/Utils/RenderLogicContext.h"


namespace bv {

// *********************************
//
PreFullscreenEffectLogic::~PreFullscreenEffectLogic     ()
{
}

// *********************************
//
void    PreFullscreenEffectLogic::RenderToRenderTarget  ( RenderLogicContext * ctx, RenderTarget * rt, SceneNode * node )
{
    enable( ctx, rt );
    clearBoundRT( ctx, glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );

    logic( ctx )->RenderNode( node, ctx ); 

    disableBoundRT( ctx );
}

} //bv
