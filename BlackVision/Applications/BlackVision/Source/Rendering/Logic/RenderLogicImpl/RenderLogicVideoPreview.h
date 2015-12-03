#pragma once

#include "Rendering/Logic/RenderLogicImpl/RenderLogicImpl.h"


namespace bv {

class RenderTargetStack;

class RenderLogicVideoPreview
{
public:

                    RenderLogicVideoPreview ( bool videoCardEnabled, RenderTargetStack * renderTargetStack );

    virtual void    RenderFrame             ( Renderer * renderer, SceneNode * sceneRoot );

};

} // bv
