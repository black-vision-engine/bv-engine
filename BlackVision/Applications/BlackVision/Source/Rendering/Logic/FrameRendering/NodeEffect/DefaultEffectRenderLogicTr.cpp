#include "DefaultEffectRenderLogicTr.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"

#include "Rendering/Utils/RenderLogicContext.h"


namespace bv {

// *********************************
//
DefaultEffectRenderLogicTr::DefaultEffectRenderLogicTr  ()
{
}

// *********************************
//
DefaultEffectRenderLogicTr::~DefaultEffectRenderLogicTr ()
{
}

// *********************************
//
void    DefaultEffectRenderLogicTr::RenderNode          ( SceneNode * node, RenderLogicContext * ctx )
{
    ctx->GetRenderLogic()->DrawNode( ctx->GetRenderer(), node );
}

} //bv
