#pragma once

#include "Rendering/Logic/RenderLogicImpl/RenderLogicImpl.h"


namespace bv {

class RenderLogicVideoPreview : public RenderLogicImpl
{
public:

                    RenderLogicVideoPreview ( bool videoCardEnabled, RenderTargetStack * renderTargetStack );

    virtual void    RenderFrame             ( Renderer * renderer, SceneNode * sceneRoot ) override;

};

} // bv
