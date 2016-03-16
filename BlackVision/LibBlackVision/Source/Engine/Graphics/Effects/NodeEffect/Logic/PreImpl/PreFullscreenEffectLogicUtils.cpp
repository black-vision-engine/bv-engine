#include "stdafx.h"

#include "PreFullscreenEffectLogicUtils.h"

#include "BlurPreFullscreenEffectLogic.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"

#include "Engine/Graphics/Effects/Utils/RenderLogicContext.h"

namespace bv
{

// *********************************
//
void        PFLogicUtils::RenderSceneNodeToRenderTarget( SceneNode * node, RenderLogicContext * ctx, RenderTarget * output )
{
    auto mainRT = disableBoundRT( ctx );

    enable( ctx, output );
    clearBoundRT( ctx, glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );

    logic( ctx )->DrawNode( node, ctx );

    disableBoundRT( ctx );

    enable( ctx, mainRT );
}

// *********************************
//
glm::vec2   PFLogicUtils::ScreenPosToFullScreenTexPos     ( const glm::vec3 & screenPos, SizeType screenWidth, SizeType screenHeight )
{
    auto x = screenPos.x / screenPos.z;
    auto y = screenPos.y / screenPos.z;

    auto screenAspectRatio = float( screenWidth ) / float( screenHeight );

    auto xd = 1.f;
    auto yd = 1.f;

    if( screenAspectRatio >= 1.f )
    {
        xd = screenAspectRatio;
    }
    else
    {
        yd = screenAspectRatio;
    }

    x = ( x + xd ) / ( 2 * xd );
    y = ( y + yd ) / ( 2 * yd );

    return glm::vec2( x, y );
}


}