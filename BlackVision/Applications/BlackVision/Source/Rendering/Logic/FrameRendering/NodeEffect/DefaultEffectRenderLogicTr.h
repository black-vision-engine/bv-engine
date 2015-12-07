#pragma once


#include "Rendering/Logic/FrameRendering/NodeEffect/NodeEffectRenderLogicTr.h"


namespace bv {

class DefaultEffectRenderLogicTr : public NodeEffectRenderLogicTr
{
public:

                    DefaultEffectRenderLogicTr  ();
    virtual         ~DefaultEffectRenderLogicTr ();

    virtual void    RenderNode                  ( SceneNode * node, RenderLogicContext * ctx ) override;

};

} //bv
