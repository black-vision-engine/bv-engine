#include "RenderLogicRawPreview.h"


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
    { renderer; sceneRoot; }
}

} // bv
