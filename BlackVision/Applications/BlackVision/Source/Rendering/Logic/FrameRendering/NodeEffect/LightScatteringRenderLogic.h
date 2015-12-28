#pragma once

#include "Rendering/Logic/FrameRendering/NodeEffect/NodeEffectRenderLogic.h"

#include <memory>

namespace bv {


class RenderLogic;
class OffscreenRenderLogic;

class LightScatteringEffect;

class LightScatteringRenderLogic : public NodeEffectRenderLogic
{
    std::shared_ptr< LightScatteringEffect >     m_effect;

public:

                    LightScatteringRenderLogic  ();
    virtual         ~LightScatteringRenderLogic ();

    virtual void    RenderNode                  ( SceneNode * node, RenderLogicContext * ctx ) override;

};

} // bv
