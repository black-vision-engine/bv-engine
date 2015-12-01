#pragma once

#include "Rendering/Logic/RenderLogicImpl/RenderLogicImpl.h"


namespace bv {

class RenderLogicRawPreview : public RenderLogicImpl
{
public:

                    RenderLogicRawPreview   ( bool videoCardEnabled, RenderTargetStack * renderTargetStack );

    virtual void    RenderFrame             ( Renderer * renderer, SceneNode * sceneRoot ) override;

};

} // bv
