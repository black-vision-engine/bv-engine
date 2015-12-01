#include "RenderLogicVideoPreview.h"


namespace bv {

// ***************************
//
RenderLogicVideoPreview::RenderLogicVideoPreview( bool videoCardEnabled, RenderTargetStack * renderTargetStack )
    : RenderLogicImpl( videoCardEnabled, renderTargetStack )
{
}

// ***************************
//
void    RenderLogicVideoPreview::RenderFrame    ( Renderer * renderer, SceneNode * sceneRoot )
{
    { renderer; sceneRoot; }
}

} // bv
