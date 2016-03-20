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
void        PFLogicUtils::CalcCommonBoxForNode          ( SceneNode * node, mathematics::Box * box )
{
    if( auto b = node->GetBoundingBox() )
    {
        box->Include( *b );
    }

    for( SizeType i = 0; i < node->NumChildNodes(); ++i )
    {
        CalcCommonBoxForNode( node->GetChild( ( unsigned int )i ), box );
    }
}

}