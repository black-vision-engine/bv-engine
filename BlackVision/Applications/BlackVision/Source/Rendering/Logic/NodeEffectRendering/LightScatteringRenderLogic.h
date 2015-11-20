#pragma once

#include "Rendering/Logic/NodeEffectRendering/NodeEffectRenderLogic.h"


namespace bv {

class LightScatteringEffect;

class LightScatteringRenderLogic : public NodeEffectRenderLogic
{
    LightScatteringEffect     *   m_effect;

public:

                    LightScatteringRenderLogic  ( RenderLogic * renderLogic, OffscreenRenderLogic * offscreenRenderLogic );
    virtual         ~LightScatteringRenderLogic ();

    virtual void    RenderNode                  ( Renderer * renderer, SceneNode * node ) override;

};

} // bv
