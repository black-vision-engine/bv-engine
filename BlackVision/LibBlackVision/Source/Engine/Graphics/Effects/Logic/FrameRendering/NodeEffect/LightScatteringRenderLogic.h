#pragma once

#include "Rendering/Logic/FrameRendering/NodeEffect/NodeEffectRenderLogic.h"
#include "Engine/Graphics/Resources/Textures/Texture2D.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"
#include "Engine/Graphics/Effects/NodeEffects/NodeEffect.h"

#include <memory>

namespace bv {


class RenderLogic;
class OffscreenRenderLogic;

class LightScatteringEffect;

class LightScatteringRenderLogic : public NodeEffectRenderLogic
{
private:

    std::shared_ptr< LightScatteringEffect >     m_effect;

    Texture2DPtr        DrawInput                   ( SceneNode * node, RenderLogicContext * ctx );
    RenderableEntity *  SetupEffect                 ( Texture2DPtr input, NodeEffectPtr effect );

public:

                        LightScatteringRenderLogic  ();
    virtual             ~LightScatteringRenderLogic ();

    virtual void        RenderNode                  ( SceneNode * node, RenderLogicContext * ctx ) override;

};

} // bv
