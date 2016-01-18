#pragma once


#include "Engine/Graphics/Effects/Logic/FrameRendering/NodeEffect/NodeEffectRenderLogic.h"


namespace bv {

class MixChannelsEffect;
class Renderer;
class RenderTarget;

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

    MixChannelsEffect *  AccessMixChannelsEffect( RenderTarget * rt, int mixMask );

    void            BlitWithMixchannels         ( Renderer * renderer, RenderTarget * rt, int mixMask );

};


} //bv
