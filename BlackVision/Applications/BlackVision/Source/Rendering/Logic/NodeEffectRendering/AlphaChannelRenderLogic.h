#pragma once

#include "Rendering/Logic/NodeEffectRendering/NodeEffectRenderLogic.h"

#include "Engine/Graphics/Effects/AlphaChannelEffect.h"

namespace bv {


class AlphaChannelRenderLogic : public NodeEffectRenderLogic
{
private:

    std::shared_ptr< AlphaChannelEffect >   m_effect;

public:

                    AlphaChannelRenderLogic     ( RenderLogic * renderLogic, OffscreenRenderLogic * offscreenRenderLogic );
    virtual         ~AlphaChannelRenderLogic    ();

    virtual void    RenderNode                  ( Renderer * renderer, SceneNode * node ) override;

	void            DrawNodeOnly				( Renderer * renderer, SceneNode * node );
    void            DrawChildren				( Renderer * renderer, SceneNode * node );
};

} // bv
