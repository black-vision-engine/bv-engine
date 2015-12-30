#pragma once


#include "Rendering/Logic/FrameRendering/NodeEffect/NodeEffectRenderLogic.h"


namespace bv {

class MixChannelsEffect;
class Renderer;


class MixChannelsRenderLogic : public NodeEffectRenderLogic
{
private:

    MixChannelsEffect     *   m_effect;

public:

                    MixChannelsRenderLogic      ();
    virtual         ~MixChannelsRenderLogic     ();

    virtual void    RenderNode                  ( SceneNode * node, RenderLogicContext * ctx ) override;

private:

    void            EnableMixChannelsEffect     ( Renderer * renderer, SceneNode * node );

};


} //bv
