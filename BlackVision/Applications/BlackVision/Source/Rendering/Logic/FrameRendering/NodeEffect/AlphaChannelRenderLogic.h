#pragma once

#include "Rendering/Logic/FrameRendering/NodeEffect/NodeEffectRenderLogic.h"

#include "Engine/Graphics/Effects/AlphaChannelEffect.h"

namespace bv {

class Renderer;
class AlphaChannelRenderLogic : public NodeEffectRenderLogic
{
private:

    std::shared_ptr< AlphaChannelEffect >   m_effect;

public:

                    AlphaChannelRenderLogic     ();
    virtual         ~AlphaChannelRenderLogic    ();

    virtual void    RenderNode                  ( SceneNode * node, RenderLogicContext * ctx  ) override;

	void            DrawNodeOnly				( Renderer * renderer, SceneNode * node );
    void            DrawChildren				( Renderer * renderer, SceneNode * node );
};

} // bv
