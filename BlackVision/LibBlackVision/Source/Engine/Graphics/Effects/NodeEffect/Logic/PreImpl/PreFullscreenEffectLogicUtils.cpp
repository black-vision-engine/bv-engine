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
glm::vec2   PFLogicUtils::ScreenPosToFullScreenTexPos     ( const glm::vec3 & screenPos, bool cameraPerspective )
{
    auto x = cameraPerspective ? screenPos.x / screenPos.z : screenPos.x;
    auto y = cameraPerspective ? screenPos.y / screenPos.z : screenPos.y;

    //auto screenAspectRatio = float( screenWidth ) / float( screenHeight );

    //auto xd = 1.f;
    //auto yd = 1.f;

    //if( screenAspectRatio >= 1.f )
    //{
    //    xd = screenAspectRatio;
    //}
    //else
    //{
    //    yd = screenAspectRatio;
    //}

    x = ( x + 1.f ) / 2.f;
    y = ( y + 1.f ) / 2.f;
    //x = ( x + 1.f ) / 2.f;
    //y = ( y + 1.f ) / 2.f;
    //y = ( 1.f - y ) / 2.f;

    //x = ( x / ( 2 * xd ) ) + 0.5f;
    

    return glm::vec2( x, y );
}


}