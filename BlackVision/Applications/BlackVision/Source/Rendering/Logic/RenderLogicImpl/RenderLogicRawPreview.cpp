#include "RenderLogicRawPreview.h"

#include "Engine/Graphics/Renderers/Renderer.h"


namespace bv {

// ***************************
//
RenderLogicRawPreview::RenderLogicRawPreview   ( bool videoCardEnabled, RenderTargetStack * renderTargetStack )
    : RenderLogicImpl( videoCardEnabled, renderTargetStack )
{
}


// ***************************
//
void    RenderLogicRawPreview::RenderFrame      ( Renderer * renderer, SceneNode * sceneRoot )
{
    PreRenderFrame( renderer );

//    EnableNextRenderTarget();

    { renderer; sceneRoot; }
}

} // bv


/*
EffectToPreview push to video card

DisplayLogic.AllocateTwoRenderTargets()
rt = DisplayLogic.GetCurRenderTarget()
RenderTargetLogic.Enable(rt)

RenderLogicContext [RenderTargetLogic, RenderTargetAllocator, Renderer] )
RenderScene(RenderTargetLogic, RenderTargetAllocator)

RenderTargetLogic.Dio
*/